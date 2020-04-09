# rooutil_tutorial

## Setting up Tutorial

    git clone --recursive git@github.com:sgnoohc/rooutil_tutorial.git
    cd rooutil_tutorial

The following sets up ```rooutil``` environements and the recommended ROOT version.

    source rooutil/thisrooutil.sh
    source rooutil/root.sh

## Tutorial 1: TTreeX

    make ttreex_tutorial.exe
    ./ttreex_tutorial.exe

Then read ```ttreex_tutorial.cc```

## Tutorial 2: Looper

This requires Tutorial 1 to be finished.

Then one has to generate a source file in the SNT "MakeClass" style.

This is done via ```makeclass.sh``` which is ```rooutil```.  The ```makeclass.sh``` command should exists assuming ```source rooutil/thisrooutil.sh``` was already called.

    makeclass.sh dummy.root t MyNtuple blah mynt

This will create ```MyNtuple.cc/h```. Afterwards, run the following command

    make looper_tutorial.exe
    ./looper_tutorial.exe

Then read ```looper_tutorial.cc```

## Tutorial 3: Cutflow / Histogram

This requires Tutorial 2 to be finished.

    make cutflow_tutorial.exe
    ./cutflow_tutorial.exe

Then read ```cutflow_tutorial.cc```
