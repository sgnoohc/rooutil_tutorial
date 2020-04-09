#include "rooutil.h"

int main()
{

    // RooUtil::TTreeX is a wrapper around ttree to facilitate writing without creating gazillion number of hard coded variables.
    // It can also access the variables that are set that are about to be written into.
    // TTreeX is not useful for reading an external TTree, that was not what this was inteded for.
    // It is true when this is done at a massive scale it is potentially not as fast as hardcoded version. But when used for a handful of numbers should not affect the performance.

    // Let's create a ttree we want to work with
    TFile* file = new TFile("dummy.root", "recreate");
    TTree* tree = new TTree("t", "t");

    // Create an instance of TTreeX
    RooUtil::TTreeX treex;

    // Now set the TTree pointer
    treex.setTree(tree);

    // Now let's create some branches
    // Supported formats are the following
    // Int_t
    // Bool_t
    // Float_t
    // TString
    // LV
    // TBits
    // unsigned long long
    // std::vector<Int_t>
    // std::vector<Bool_t>
    // std::vector<Float_t>
    // std::vector<TString>
    // std::vector<LV>
    //
    // Here LV is typedef of ROOT::Math::LorentzVector with float
    // Exact type depends on the compilation option (i.e. whether it had -DLorentzVectorPtEtaPhiM4D or not)
    //
    // #ifdef LorentzVectorPtEtaPhiM4D
    // typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > LV;
    // #else
    // typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LV;
    // #endif
    //

    treex.createBranch<int>("my_int");
    treex.createBranch<float>("my_float");
    treex.createBranch<std::vector<float>>("my_vecfloat");
    treex.createBranch<std::vector<LV>>("my_vecLV");

    // Let's fill some events
    for (unsigned int ievent = 0; ievent < 1000; ++ievent)
    {
        // For flat variables it is easy to set them
        treex.setBranch<int>("my_int", ievent);
        treex.setBranch<float>("my_float", 3.1416 * ievent);

        // For vector type of branches one can do a "pushback"
        for (unsigned int iobj = 0; iobj < 10; ++iobj)
            treex.pushbackToBranch<float>("my_vecfloat", 3.1416 * iobj);

        // For vector type of barnches one can do a "pushback"
        for (unsigned int ilv = 0; ilv < 5; ++ilv)
        {
            LV lv = RooUtil::Calc::getLV(10. * ilv, -5 + 0.1 * ilv, 1 * ilv, 0); // RooUtil::Calc is a namespace with math related tools
            treex.pushbackToBranch<LV>("my_vecLV", lv);
        }

        // Accessing the variable values on the fly
        // The branch values that are set can be accessed on the fly as long as RooUtil::TTreeX::clear() is not called.

        std::cout << "my_int : " << treex.getBranch<int>("my_int") << std::endl;
        std::cout << "my_float : " << treex.getBranch<float>("my_float") << std::endl;
        std::cout << "Pt of the first LV of my_vecLV : " << treex.getBranch<std::vector<LV>>("my_vecLV")[0].pt() << std::endl;

        // Actually fill the TTree object that is wrapped by the RooUtil::TTreeX
        treex.fill();

        // Clear all the variables
        treex.clear();

        // Now everything is reset, before setting the branch, if you call the "getBranch" then it will throw an error and quit the program
        // This is to protect the user from using dummy values like -999 in the analysis by accident

        // However, one can bypass by calling "getBranchLazy." It does not rigorously check, hence the name "Lazy"
        std::cout << treex.getBranchLazy<int>("my_int") << std::endl;

        // The following line will crash and quit the program
        // User can uncomment this to test it out
        // std::cout << treex.getBranch<int>("my_int") << std::endl;
    }

    treex.write(); // Equivalent to tree->Write();

    file->Close();

    // Once you open the dummy.root file you will see the following content
    //
    // $ root -b -l dummy.root
    // .ls
    // root [0]
    // Attaching file dummy.root as _file0...
    // (TFile *) 0x40815c0
    // root [1] .ls
    // TFile**         dummy.root
    //  TFile*         dummy.root
    //   KEY: TTree    t;1     t
    // root [2] t->GetEntries()
    // (long long) 1000
    // root [3] t->Scan()
    // ***********************************************************************************************************
    // *    Row   * Instance *    my_int *  my_float * my_vecflo * my_vecLV. * my_vecLV. * my_vecLV. * my_vecLV. *
    // ***********************************************************************************************************
    // *        0 *        0 *         0 *         0 *         0 *         0 *         0 *         0 *         0 *
    // *        0 *        1 *         0 *         0 * 3.1415998 *        10 * -4.899944 *         1 *         0 *
    // *        0 *        2 *         0 *         0 * 6.2831997 *        20 * -4.799932 *         2 * -0.353553 *
    // *        0 *        3 *         0 *         0 * 9.4247999 *        30 * -4.699917 *         3 *       0.5 *
    // *        0 *        4 *         0 *         0 * 12.566399 *        40 * -4.600039 * -2.283185 *         0 *
    // *        0 *        5 *         0 *         0 * 15.708000 *           *           *           *           *
    // *        0 *        6 *         0 *         0 * 18.849599 *           *           *           *           *
    // *        0 *        7 *         0 *         0 * 21.991199 *           *           *           *           *
    // *        0 *        8 *         0 *         0 * 25.132799 *           *           *           *           *
    // *        0 *        9 *         0 *         0 * 28.274400 *           *           *           *           *
    // *        1 *        0 *         1 * 3.1415998 *         0 *         0 *         0 *         0 *         0 *
    // *        1 *        1 *         1 * 3.1415998 * 3.1415998 *        10 * -4.899944 *         1 *         0 *
    // *        1 *        2 *         1 * 3.1415998 * 6.2831997 *        20 * -4.799932 *         2 * -0.353553 *
    // *        1 *        3 *         1 * 3.1415998 * 9.4247999 *        30 * -4.699917 *         3 *       0.5 *
    // *        1 *        4 *         1 * 3.1415998 * 12.566399 *        40 * -4.600039 * -2.283185 *         0 *
    // *        1 *        5 *         1 * 3.1415998 * 15.708000 *           *           *           *           *
    // *        1 *        6 *         1 * 3.1415998 * 18.849599 *           *           *           *           *
    // *        1 *        7 *         1 * 3.1415998 * 21.991199 *           *           *           *           *
    // *        1 *        8 *         1 * 3.1415998 * 25.132799 *           *           *           *           *
    // *        1 *        9 *         1 * 3.1415998 * 28.274400 *           *           *           *           *
    // *        2 *        0 *         2 * 6.2831997 *         0 *         0 *         0 *         0 *         0 *
    // *        2 *        1 *         2 * 6.2831997 * 3.1415998 *        10 * -4.899944 *         1 *         0 *
    // *        2 *        2 *         2 * 6.2831997 * 6.2831997 *        20 * -4.799932 *         2 * -0.353553 *
    // *        2 *        3 *         2 * 6.2831997 * 9.4247999 *        30 * -4.699917 *         3 *       0.5 *
    // *        2 *        4 *         2 * 6.2831997 * 12.566399 *        40 * -4.600039 * -2.283185 *         0 *
    // Type <CR> to continue or q to quit ==>

}
