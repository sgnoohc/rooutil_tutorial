# rooutil_tutorial

To compile all the tutorials

    source rooutil/thisrooutil.sh
    source rooutil/root.sh
    make -j

## Tutorial 1: TTreeX

    make ttreex_tutorial.exe
    ./ttreex_tutorial.exe

Then read ```ttreex_tutorial.cc```

## Tutorial 2: Looper

This requires Tutorial 1 to be finished.

Then one has to generate a source file in the SNT "MakeClass" style.

    makeclass.sh dummy.root t MyNtuple blah mynt

This will create ```MyNtuple.cc/h```. Afterwards, run the following command

    make looper_tutorial.exe
    ./looper_tutorial.exe

Then read ```looper_tutorial.cc```

## Tutorial 3: Cutflow / Histogram

    make cutflow_tutorial.exe
    ./cutflow_tutorial.exe

Then read ```cutflow_tutorial.cc```
