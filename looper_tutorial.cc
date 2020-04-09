#include "MyNtuple.h"

int main()
{
    // RooUtil::Looper facilitates looping over TTree
    // But it relies on SNT style "makeclass" function

    // This tutorial assumes that the ttreex_tutorial.exe was exected and "dummy.root" exists in the workdir.

    // then run the following shell command to generate SNT style "makeclass" source files
    //
    // > source rooutil/thisrooutil.sh
    // > source rooutil/root.sh (if root was not set)
    // > makeclass.sh dummy.root t MyNtuple blah mynt
    //
    // If one takes a look at the output MyNtuple.cc/h one will notice that there is a global variable called "mynt".
    // This instance facilitates reading of the ntuples.
    // This is equivalent to "Nano.cc/h" or the "CMS3.cc/h"

    // Let's create an instance with the template taken from the SNT style MakeClass class.
    RooUtil::Looper<MyNtuple> looper;

    // RooUtil::Looper accepts TChain not TTree
    TChain* ch = new TChain("t");
    ch->Add("dummy.root");

    // Now let's initialize the looper
    // first is TTree or TChain from the file, second is the SNT style MakeClass global instance address, last is the number of entries to loop. (set to -1 = loop all)
    looper.init(ch, &mynt, -1); 

    // Now loop over
    while (looper.nextEvent())
    {
        // Now access event content using the SNT style global instance.
        std::cout <<  " mynt.my_int(): " << mynt.my_int() <<  std::endl;
    }

}
