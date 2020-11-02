//  What this does:
//      * Reads PicoDst files and generates charged jets and fills a jet area and pT histogram.
//  What this does NOT do (to do these, see code under /star/u/djs232/PicoToJetsExample) :
//      * Read the BEMC hits (neutral energy mostly). So, it's just charged jets.
//      * fill a tree with the results.
//      * read a bad run list
//      * read a bad tower list
//      * output to a log file
//      * do jet background subtraction
//

#include "RunData.h"
#include "TChain.h"
#include <fstream>
#include "fastjet/config.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/Subtractor.hh"

using namespace fastjet;

ClassImp(RunData)
//-----------------------------------------------------------------------------
RunData::RunData(
        const char*     _name,
        const char*     _outName,
        StPicoDstMaker* _picoMaker
    )
  : StMaker         (_name), 
    picoDstMaker    {_picoMaker}
{
    fout_root = new TFile(Form("%s.root",_outName),"RECREATE");
    // Initialize the histograms
    //
    jet_pt = new TH1D("jet_pt", "Charged jets;p_{T};N_{jets}", 100, 0., 50.);
    jet_area = new TH1D("jet_area", "Charged jets;Area (in #phi-#eta space);N_{jets}", 100, 0., 1.);
    n_events = new TH1D("n_events", "N_events;;N_{events}", 1, -0.5, 0.5);
};

//----------------------------------------------------------------------------- 
RunData::~RunData()
{
};

//----------------------------------------------------------------------------- 
Int_t RunData::Init() {
    return kStOK;
};

//----------------------------------------------------------------------------- 
Int_t RunData::Finish() {
    fout_root->Write();
    fout_root->Close();
    return kStOK;
};
    
//----------------------------------------------------------------------------- 
void RunData::Clear(Option_t *opt) { };
//----------------------------------------------------------------------------- 
Int_t RunData::Make() {
    if(!picoDstMaker) {
        return kStWarn;
    }

    StPicoDst* picoDst = picoDstMaker->picoDst();
    if(!picoDst) {
        return kStWarn;
    }

    StPicoEvent* picoEvent = picoDst->event();

    // Make some kind of trigger requirement
    if (!( 
        picoEvent->isTrigger(500004)
     || picoEvent->isTrigger(500008)
     || picoEvent->isTrigger(500018)
     || picoEvent->isTrigger(500205)
     || picoEvent->isTrigger(500206)
     || picoEvent->isTrigger(500215)
     || picoEvent->isTrigger(490202))) return kStOk;


    // cut on vz
    float vz = picoEvent->primaryVertex().z();
    float vzVpd = picoEvent->vzVpd();
    if (TMath::Abs(vz-vzVpd) > 6.) return kStOk;

    vector<PseudoJet> pseudo_track;
    for (unsigned int i = 0; i < picoDst->numberOfTracks(); ++i) {
        StPicoTrack* track = picoDst->track(i);
        if (!track || !(track->isPrimary()))   continue;
        TVector3 Ptrack = track->pMom();

        //apply track cuts
        float eta  { static_cast<float>(Ptrack.PseudoRapidity()) };
        if ( 
               TMath::Abs(eta) >= 1.0
            || track->nHitsFit() < 15) continue;
        float nhit_ratio = ((float)track->nHitsFit()) / (float)track->nHitsMax();
        if (nhit_ratio <= 0.52) continue;

        float pt { static_cast<float>(Ptrack.Perp()) };
        if (pt > 30.)  return kStOK;
        if (pt < 0.2) continue;

        TVector3 dca = track->origin() - picoEvent->primaryVertex(); 
        float dcaXYZ { static_cast<float>(dca.Mag()) };
        if (dcaXYZ > 3.) continue;

        const float px { static_cast<float>(Ptrack.X())};
        const float py { static_cast<float>(Ptrack.Y())};
        const float pz { static_cast<float>(Ptrack.Z())};
        const float E  { static_cast<float>(TMath::Sqrt(Ptrack.Mag2()))};

        pseudo_track.push_back( {px, py, pz, E} );
    }
   
    // Make and fill in the charged jets
    fastjet::JetDefinition jetdef (fastjet::antikt_algorithm, 0.4);

    // only measure jets out to |\eta|<=0.6
    fastjet::Selector      jetrap  = fastjet::SelectorAbsEtaMax(0.6)  ;

    fastjet::Selector      not_pure_ghost = !SelectorIsPureGhost();
    fastjet::Selector      selection      = jetrap && not_pure_ghost;

    const double pTMin {0.2};
    const double jet_R {0.4};

    // cluster the jets with ghosts to get the jet areas as well
    AreaDefinition area_def(active_area_explicit_ghosts,GhostedAreaSpec(4.0,1,0.01));
    JetDefinition jet_def(antikt_algorithm, jet_R);
    fastjet::ClusterSequenceArea ch_clustSeq(pseudo_track, jetdef, area_def);
    vector<PseudoJet> ch_jets = sorted_by_pt( selection( ch_clustSeq.inclusive_jets(pTMin)) );

    // fill the histograms with the jet values
    for (auto& jet : ch_jets) {
        jet_pt->Fill(jet.perp());
        jet_area->Fill(jet.area());
    }
    n_events->Fill(0.);

    return kStOk;
};
