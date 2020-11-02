#!/usr/bin/csh
starver SL20a
echo "Input arguments: $argv[*]"
set nevents = ${1}
if ( ${nevents} == "" ) then
    set nevents = -1
endif
echo "nevents ${nevents}"
root4star -q -b -l ./run.C\(${nevents}\)
