#include "MyNtuple.h"

int main()
{

    // RooUtil::Cutflow is an object to facilitate cutflow and histogramming
    // N.B. This tutorial assumes user ran the ttreex_tutorial.exe and have the "dummy.root" that it creates

    // See bottom section of this file for a "manual" of Cutflow and Histogram
    // Now let's actually see this in action

    // The following code will produce a output.root with the following histograms
    // $ root -b -l output.root
    // .ls
    // root [0]
    // Attaching file output.root as _file0...
    // (TFile *) 0x4665160
    // root [1] .ls
    // TFile**         output.root
     // TFile*         output.root
      // KEY: TH1D     DiElChannelCutC_cutflow;1
      // KEY: TH1D     DiMuChannelCutC_cutflow;1
      // KEY: TH1D     DiElChannelCutC_rawcutflow;1
      // KEY: TH1D     DiMuChannelCutC_rawcutflow;1
      // KEY: TH1D     DiElChannel__Mll;1
      // KEY: TH1D     DiElChannelCutA__Mll;1
      // KEY: TH1D     DiElChannelCutB__Mll;1
      // KEY: TH1D     DiElChannelCutC__Mll;1
      // KEY: TH1D     DiMuChannel__Mll;1
      // KEY: TH1D     DiMuChannelCutA__Mll;1
      // KEY: TH1D     DiMuChannelCutB__Mll;1
      // KEY: TH1D     DiMuChannelCutC__Mll;1
      // KEY: TH1D     Root__Mll;1
    // root [2] .q


    // First create where the analysis histogram outputs will go to
    TFile* output_tfile = new TFile("output.root", "recreate");

    // Now create the RooUtil::Cutflow
    RooUtil::Cutflow cutflow;

    // Set the cutflow object output file
    cutflow.setTFile(output_tfile);

    cutflow.addCut("DiElChannel", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );
    cutflow.addCut("DiMuChannel", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );

    cutflow.getCut("DiElChannel");
    cutflow.addCutToLastActiveCut("DiElChannelCutA", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );
    cutflow.addCutToLastActiveCut("DiElChannelCutB", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );
    cutflow.addCutToLastActiveCut("DiElChannelCutC", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );

    cutflow.getCut("DiMuChannel");
    cutflow.addCutToLastActiveCut("DiMuChannelCutA", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );
    cutflow.addCutToLastActiveCut("DiMuChannelCutB", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );
    cutflow.addCutToLastActiveCut("DiMuChannelCutC", [&]() { return 1/*set your cut here*/; }, [&]() { return 1/*set your weight here*/; } );

    // Print cut structure
    cutflow.printCuts();

    // Histogram utility object that is used to define the histograms
    RooUtil::Histograms histograms;
    histograms.addHistogram("Mll", 180, 0, 250, [&]() { return 1/* set your variable here*/; } );

    // Book cutflows
    cutflow.bookCutflows();

    // Book Histograms
    cutflow.bookHistograms(histograms); // if just want to book everywhere

    // Let's create an instance with the template taken from the SNT style MakeClass class.
    RooUtil::Looper<MyNtuple> looper;

    // RooUtil::Looper accepts TChain not TTree
    TChain* ch = new TChain("t");
    ch->Add("dummy.root");

    // Now let's initialize the looper
    // first is TTree or TChain from the file, second is the SNT style MakeClass global instance address, last is the number of entries to loop. (set to -1 = loop all)
    looper.init(ch, &mynt, -1); 

    // Looping input file
    while (looper.nextEvent())
    {

        //Do what you need to do in for each event here
        //To save use the following function
        cutflow.fill();

    }

    // Writing output file
    cutflow.saveOutput();

    // The below can be sometimes crucial
    delete output_tfile;










    // The standard "manual" for Cutflow and Histogram is written here
    //
    // //********************************************************************************                                                                                                                                                ">>process.cc
    // //                                                                                                                                                                                                                                ">>process.cc
    // // Interlude... notes on RooUtil framework                                                                                                                                                                                        ">>process.cc
    // //                                                                                                                                                                                                                                ">>process.cc
    // //********************************************************************************                                                                                                                                                ">>process.cc
    //                                                                                                                                                                                                                                   ">>process.cc
    //     // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=                                                                                                                                                               ">>process.cc
    //     // Quick tutorial on RooUtil::Cutflow object cut tree formation                                                                                                                                                               ">>process.cc
    //     // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=                                                                                                                                                               ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // The RooUtil::Cutflow object facilitates creating a tree structure of cuts                                                                                                                                                  ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // To add cuts to each node of the tree with cuts defined, use \"addCut\" or \"addCutToLastActiveCut\"                                                                                                                        ">>process.cc
    //     // The \"addCut\" or \"addCutToLastActiveCut\" accepts three argument, <name>, and two lambda's that define the cut selection, and the weight to apply to that cut stage                                                      ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // e.g. To create following cut-tree structure one does                                                                                                                                                                       ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //             (Root) <--- Always exists as soon as RooUtil::Cutflow object is created. But this is basically hidden underneath and users do not have to care                                                                 ">>process.cc
    //     //                |                                                                                                                                                                                                           ">>process.cc
    //     //            CutWeight                                                                                                                                                                                                       ">>process.cc
    //     //            |       |                                                                                                                                                                                                       ">>process.cc
    //     //     CutPreSel1    CutPreSel2                                                                                                                                                                                               ">>process.cc
    //     //       |                  |                                                                                                                                                                                                 ">>process.cc
    //     //     CutSel1           CutSel2                                                                                                                                                                                              ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //   code:                                                                                                                                                                                                                    ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //      // Create the object (Root node is created as soon as the instance is created)                                                                                                                                        ">>process.cc
    //     //      RooUtil::Cutflow cutflow;                                                                                                                                                                                             ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //      cutflow.addCut(\"CutWeight\"                 , <lambda> , <lambda>); // CutWeight is added below \"Root\"-node                                                                                                        ">>process.cc
    //     //      cutflow.addCutToLastActiveCut(\"CutPresel1\" , <lambda> , <lambda>); // The last \"active\" cut is \"CutWeight\" since I just added that. So \"CutPresel1\" is added below \"CutWeight\"                              ">>process.cc
    //     //      cutflow.addCutToLastActiveCut(\"CutSel1\"    , <lambda> , <lambda>); // The last \"active\" cut is \"CutPresel1\" since I just added that. So \"CutSel1\" is added below \"CutPresel1\"                               ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //      cutflow.getCut(\"CutWeight\"); // By \"getting\" the cut object, this makes the \"CutWeight\" the last \"active\" cut.                                                                                                ">>process.cc
    //     //      cutflow.addCutToLastActiveCut(\"CutPresel2\" , <lambda> , <lambda>); // The last \"active\" cut is \"CutWeight\" since I \"getCut\" on it. So \"CutPresel2\" is added below \"CutWeight\"                             ">>process.cc
    //     //      cutflow.addCutToLastActiveCut(\"CutSel2\"    , <lambda> , <lambda>); // The last \"active\" cut is \"CutPresel2\" since I just added that. So \"CutSel2\" is added below \"CutPresel1\"                               ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // (Side note: \"UNITY\" lambda is defined in the framework to just return 1. This so that use don't have to type [&]() {return 1;} so many times.)                                                                           ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // Once the cutflow is formed, create cutflow histograms can be created by calling RooUtil::Cutflow::bookCutflows())                                                                                                          ">>process.cc
    //     // This function looks through the terminating nodes of the tree structured cut tree. and creates a histogram that will fill the yields                                                                                       ">>process.cc
    //     // For the example above, there are two terminationg nodes, \"CutSel1\", and \"CutSel2\"                                                                                                                                      ">>process.cc
    //     // So in this case Root::Cutflow::bookCutflows() will create two histograms. (Actually four histograms.)                                                                                                                      ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //  - TH1F* type object :  CutSel1_cutflow (4 bins, with first bin labeled \"Root\", second bin labeled \"CutWeight\", third bin labeled \"CutPreSel1\", fourth bin labeled \"CutSel1\")                                      ">>process.cc
    //     //  - TH1F* type object :  CutSel2_cutflow (...)                                                                                                                                                                              ">>process.cc
    //     //  - TH1F* type object :  CutSel1_rawcutflow (...)                                                                                                                                                                           ">>process.cc
    //     //  - TH1F* type object :  CutSel2_rawcutflow (...)                                                                                                                                                                           ">>process.cc
    //     //                                ^                                                                                                                                                                                           ">>process.cc
    //     //                                |                                                                                                                                                                                           ">>process.cc
    //     // NOTE: There is only one underscore \"_\" between <CutName>_cutflow or <CutName>_rawcutflow                                                                                                                                 ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // And later in the loop when RooUtil::Cutflow::fill() function is called, the tree structure will be traversed through and the appropriate yields will be filled into the histograms                                         ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // After running the loop check for the histograms in the output root file                                                                                                                                                    ">>process.cc
    //                                                                                                                                                                                                                                   ">>process.cc
    //     // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=                                                                                                                                                                               ">>process.cc
    //     // Quick tutorial on RooUtil::Histograms object                                                                                                                                                                               ">>process.cc
    //     // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=                                                                                                                                                                               ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // The RooUtil::Histograms object facilitates book keeping histogram definitions                                                                                                                                              ">>process.cc
    //     // And in conjunction with RooUtil::Cutflow object, one can book same histograms across different cut stages easily without copy pasting codes many times by hand.                                                            ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // The histogram addition happens in two steps.                                                                                                                                                                               ">>process.cc
    //     // 1. Defining histograms                                                                                                                                                                                                     ">>process.cc
    //     // 2. Booking histograms to cuts                                                                                                                                                                                              ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // Histograms are defined via following functions                                                                                                                                                                             ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //      RooUtil::Histograms::addHistogram       : Typical 1D histogram (TH1F*) \"Fill()\" called once per event                                                                                                               ">>process.cc
    //     //      RooUtil::Histograms::addVecHistogram    : Typical 1D histogram (TH1F*) \"Fill()\" called multiple times per event                                                                                                     ">>process.cc
    //     //      RooUtil::Histograms::add2DHistogram     : Typical 2D histogram (TH2F*) \"Fill()\" called once per event                                                                                                               ">>process.cc
    //     //      RooUtil::Histograms::add2DVecHistogram  : Typical 2D histogram (TH2F*) \"Fill()\" called multiple times per event                                                                                                     ">>process.cc
    //     // e.g.                                                                                                                                                                                                                       ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //    RooUtil::Histograms histograms;                                                                                                                                                                                         ">>process.cc
    //     //    histograms.addHistogram   (\"MllSS\"    , 180 , 0. , 300. , [&]() { return www.MllSS()  ; }); // The lambda returns float                                                                                               ">>process.cc
    //     //    histograms.addVecHistogram(\"AllLepPt\" , 180 , 0. , 300. , [&]() { return www.lep_pt() ; }); // The lambda returns vector<float>                                                                                       ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // The addVecHistogram will have lambda to return vector<float> and it will loop over the values and call TH1F::Fill() for each item                                                                                          ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // To book histograms to cuts one uses                                                                                                                                                                                        ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //      RooUtil::Cutflow::bookHistogramsForCut()                                                                                                                                                                              ">>process.cc
    //     //      RooUtil::Cutflow::bookHistogramsForCutAndBelow()                                                                                                                                                                      ">>process.cc
    //     //      RooUtil::Cutflow::bookHistogramsForCutAndAbove()                                                                                                                                                                      ">>process.cc
    //     //      RooUtil::Cutflow::bookHistogramsForEndCuts()                                                                                                                                                                          ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // e.g. Given a tree like the following, we can book histograms to various cuts as we want                                                                                                                                    ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //              Root                                                                                                                                                                                                          ">>process.cc
    //     //                |                                                                                                                                                                                                           ">>process.cc
    //     //            CutWeight                                                                                                                                                                                                       ">>process.cc
    //     //            |       |                                                                                                                                                                                                       ">>process.cc
    //     //     CutPreSel1    CutPreSel2                                                                                                                                                                                               ">>process.cc
    //     //       |                  |                                                                                                                                                                                                 ">>process.cc
    //     //     CutSel1           CutSel2                                                                                                                                                                                              ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // For example,                                                                                                                                                                                                               ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //    1. book a set of histograms to one cut:                                                                                                                                                                                 ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //       cutflow.bookHistogramsForCut(histograms, \"CutPreSel2\")                                                                                                                                                             ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //    2. book a set of histograms to a cut and below                                                                                                                                                                          ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //       cutflow.bookHistogramsForCutAndBelow(histograms, \"CutWeight\") // will book a set of histograms to CutWeight, CutPreSel1, CutPreSel2, CutSel1, and CutSel2                                                          ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //    3. book a set of histograms to a cut and above (... useless...?)                                                                                                                                                        ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //       cutflow.bookHistogramsForCutAndAbove(histograms, \"CutPreSel2\") // will book a set of histograms to CutPreSel2, CutWeight (nothing happens to Root node)                                                            ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //    4. book a set of histograms to a terminating nodes                                                                                                                                                                      ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //       cutflow.bookHistogramsForEndCuts(histograms) // will book a set of histograms to CutSel1 and CutSel2                                                                                                                 ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // The naming convention of the booked histograms are as follows                                                                                                                                                              ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //   cutflow.bookHistogramsForCut(histograms, \"CutSel1\");                                                                                                                                                                   ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     //  - TH1F* type object : CutSel1__MllSS;                                                                                                                                                                                     ">>process.cc
    //     //  - TH1F* type object : CutSel1__AllLepPt;                                                                                                                                                                                  ">>process.cc
    //     //                               ^^                                                                                                                                                                                           ">>process.cc
    //     //                               ||                                                                                                                                                                                           ">>process.cc
    //     // NOTE: There are two underscores \"__\" between <CutName>__<HistogramName>                                                                                                                                                  ">>process.cc
    //     //                                                                                                                                                                                                                            ">>process.cc
    //     // And later in the loop when RooUtil::CutName::fill() function is called, the tree structure will be traversed through and the appropriate histograms will be filled with appropriate variables                              ">>process.cc
    //     // After running the loop check for the histograms in the output root file                                                                                                                                                    ">>process.cc

}
