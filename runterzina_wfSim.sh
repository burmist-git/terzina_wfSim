#!/bin/bash

# Me 08 Jun 2022 14:02:24 CEST
# Autor: Leonid Burmistrov

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d       : default"
    echo " [0] -h       : print help"
}

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	make clean; make;
	input_configuration_file="wfSim_Terzina.conf"
	outputRootFileWith_wf="wfSim.root"
	numberOfWaveformsToSim=100
	rnd_seed=`date +%N`
	n_sig_pe=1
	./runterzina_wfSim 0 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe
	#./runterzina_wfSim 0 wfSim_Terzina.conf wfSim.root 10 `date +%N` 1
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi
