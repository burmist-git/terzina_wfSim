#!/bin/bash

function nm_sh {

    liblist=$1
    
    for filename in $liblist ; do
	echo " "
	echo "----> $filename"
	nm -CD --defined-only $filename | awk '{if($2=="T"){{print }}}' | grep -v _fini | grep -v _init | nl
	echo " "
    done
    
}

nm_sh "./obj/*.so"
