# Introduction
This whole file is located locally on RCAS under /star/u/djs232/AN/PicoToJetsExample_Small

This directory is meant as a short example to:
    * read PicoDst files from the server
    * make charged jets
    * fill histograms and write them to an output root file

This already requires a lot of detail, but it is also missing a lot more.

Here are some notes.

# Files present:
1. Makefile:
    Super simple file that really just runs the cons "./cons"
    cons is RCAS's local facility to make you code with the c++ compiler.
    It is nice because it will do all the linking to the StRoot code

    So you can compile you'r code with `make` which will run `./cons`
    or you can just use `cons` directly. It is the same thing.

    I have it locally because it plays nice with the :Make command
    from inside of vim (which locally populates Vim's clist with 
    all the compile errors -- but if Vim's not your thing, then 
    this doesn't help at all...)
2. (not saved on github.com): _x.cxx and _h.h:
    These are just sybolic links pointing to the actual code 
    under StRoot/RunData/RunData{.h,.cxx}
3. run.C:
    This is the script that actually runs the code
4. test.csh:
    This is the command line script that uses run.C to run a test example
    of the code and output the results locally
5. sums.xml:
    This is the driver to run the code under the STAR Utility Management 
    System. Use it like: `star-submit sums.xml`

    Note that it includes an output location for your code (which by default 
    is to David Stewart's space under /star/pwd location). You should
    *You should change the output file location* 

    *You should also adjust the nFiles=all to nFiles=<number you pick>*
6. (not saved on github.com) fastjet:
    This is a link to a copy of fastjet's library. You will probably have to 
    regenerate this link. The command:
    `ln -s /direct/star+u/djs232/software/fastjet-install/include/fastjet fastjet`
    if run in your local directory works 
7. StRoot:
    This contains the actual code (see above under _x.cxx and _h.h)
8. linted_picos.list:
    You can always delete this file. It is generated locally each time the run.C 
    script is run. It deals with formating this picoDst list passed from sums.xml.
    It is possible that it is no longer needed
9. test.list:
    This file contains a short list of picoDst's that can be used when testing
    you're file when running the test.csh script.
10. sums/{input,err,out}:
    When you run the `star-submit sums.xml` command, many required files will
    be generated in these directories

# How to use this code:
 Compile the code with `./cons` or `./make` 
 Run `./test.csh` 
 If you get an output `test.root` file, it all works.

 Play with the code under `./StRoot/RunData/RunData.{h,cxx}` to get
 different output.  When you want to run something larger, modify sums.xml
 to output the larger output to your preferred directory, and to git the
 desired number of input files, and run `star-submit sums.xml`


