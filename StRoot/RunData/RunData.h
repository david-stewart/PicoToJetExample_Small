#ifndef RUNQA_H
#define RUNQA_H

#include "TTree.h"
/* #include "St_db_Maker/St_db_Maker.h" */
/* #include "StEmcUtil/database/StBemcTables.h" */
/* #include "StRoot/StPicoDstMaker/StPicoDst.h" */
#include "StRoot/StPicoEvent/StPicoDst.h"
#include "StRoot/StPicoEvent/StPicoTrack.h"
#include "StRoot/StPicoEvent/StPicoEvent.h"
#include "StRoot/StPicoDstMaker/StPicoDstMaker.h"
#include "StRoot/StPicoEvent/StPicoEmcTrigger.h"
#include "StRoot/StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoBEmcPidTraits.h"
#include "StMaker.h"
#include "StThreeVectorF.hh"
#include "TH1F.h"

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class TString;
class TH1D;

class RunData : public StMaker {

    private:
    StPicoDstMaker* picoDstMaker;
    TFile*          fout_root;

    public:
    RunData(
        const char*     _name,
        const char*     _outName,
        StPicoDstMaker* _picoMaker

    );
    virtual ~RunData();

    TH1D* jet_pt;
    TH1D* jet_area;
    TH1D* n_events;
    
    virtual Int_t Init();
    virtual Int_t Make();
    virtual void  Clear(Option_t *opt="");
    virtual Int_t Finish();

    ClassDef(RunData, 1)
};

#endif
