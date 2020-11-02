#include <TSystem>
#include <ctime>
#include <string>

class StMaker;
class StChain;
class StPicoDstMaker;

StChain *chain;
void run(
        const Int_t nEv_input=-1,
        const Char_t *picoDstList="test.list",
        const Char_t *out_name   ="test"
){
    //------------------------------------------------------
    // Load required libraries
    //------------------------------------------------------
    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();

    gSystem->Load("StMuDSTMaker");  
    gSystem->Load("StPicoEvent");
    gSystem->Load("StPicoDstMaker");
    gSystem->Load("StChain");
    gSystem->Load("StDbBroker");
    gSystem->Load("St_db_Maker.so");
    gSystem->Load("StEmcUtil");
    gSystem->Load("$FASTJET/lib/libfastjet");
    gSystem->Load("$FASTJET/lib/libfastjettools");
    gSystem->Load("RunData");

    //-----------------------------------------
    //  output file
    //-----------------------------------------
    ifstream file;
    file.open(picoDstList);
    if (!file.is_open()){
        cout << "Fatal error: Could not open " << picoDstList << endl;
        return kFatal;
    }
    string str;
    ifstream file;
    file.close();

    //-----------------------------------------
    // process the input file for xrootd files
    //-----------------------------------------
    // process in the input file to get rid of the run numbers (if present)
    // for when the input is like: 
    // /tmp/djs232/D96D05E45C742BAAF8F88D9560575DEE_0/INPUTFILES/st_ssdmb_16124017_raw_2500003.picoDst.root 8586
    // but the picoDst wants the line without the 8586 appended
    file.open(picoDstList);
    ofstream fout;
    fout.open("linted_picos.list");
    while(getline(file,str)){
        TString tstr = str;
        cout << "Line before: " << tstr << endl;
        TPRegexp("(\\S+) (\\d+)").Substitute(tstr,("$1"));
        fout << tstr << endl;
        cout << "Line after " << tstr << endl;
    };
    fout.close();

    //----------------------------------------
    // Get the bemc maker
    //----------------------------------------
    chain = new StChain();

    //-----------------------------------
    // Make chain and run main macro
    //-----------------------------------
    StPicoDstMaker *picoMaker =0X0; 
    StPicoDstMaker::PicoIoMode IoMode = 2;
    cout << "picoDstList: " << picoDstList << endl;
    picoMaker = new StPicoDstMaker(IoMode,"linted_picos.list","picoDst");
    /* picoMaker->SetStatus("*",1); */
    picoMaker->SetStatus("Event",1);
    picoMaker->SetStatus("Track",1);
    picoMaker->SetStatus("EmcTrigger",0); //
    picoMaker->SetStatus("MtdTrigger",0);
    picoMaker->SetStatus("BTowHit",1);
    picoMaker->SetStatus("BTofHit",0);
    picoMaker->SetStatus("MtdHit",0);
    picoMaker->SetStatus("BbcHit",0); //
    picoMaker->SetStatus("EpdHit",0);
    picoMaker->SetStatus("FmsHit",0);
    picoMaker->SetStatus("EmcPidTraits",0);
    picoMaker->SetStatus("BTofPidTraits",0);
    picoMaker->SetStatus("MtdPidTraits",0);
    picoMaker->SetStatus("TrackCovMatrix",0);
    picoMaker->SetStatus("BEmcSmdEHit",0);
    picoMaker->SetStatus("BEmcSmdPHit",0);
    picoMaker->SetStatus("ETofHit",0);
    picoMaker->SetStatus("ETofPidTraits",0);
    picoMaker->SetStatus("BEmcSmdPHit",0);

    cout << "picoMaker1: " << picoMaker << endl;

    RunData *myrunQA = new RunData(
        "pf",
        out_name,
        picoMaker
    );

    cout << "chain->Init()" <<endl;
    chain->Init();

    int total = picoMaker->chain()->GetEntries();
    cout << " Total entries = " << total << endl;
    int nEvents = nEv_input;
    if(nEvents>total||nEvents<0) nEvents = total;
    cout<<"Number of Events = "<<nEvents<<endl;

    int usage0 = 0;
    for (Int_t i=0; i<nEvents; i++){   
        if(i%5000==0) {
            cout << "- Finished " << i << " events." << endl;

        }
        chain->Clear();
        int iret = chain->Make(i);
        if (iret) { cout << "Bad return code!" << iret << endl; break;}
        total++;
    } 

    current_time = time(NULL);
    loc = localtime(&current_time);

	chain->Finish();

	delete chain;
}
