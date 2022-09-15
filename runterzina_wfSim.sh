#!/bin/bash

# Me 08 Jun 2022 14:02:24 CEST
# Autor: Leonid Burmistrov

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d          : default"
    echo " [0] -photTime   : simulating realistic photon time"
    echo " [0] -s          : single p.e. simulator"
    echo " [0] -t          : test SiPM tree"
    echo " [0] -LEE        : generate LEE template (linear exp. exp. SiPM responce)"
    echo " [0] -LEEfit     : fit with LEE function"
    echo " [0] -FREQCUTOFF : simulate frequency cut off preamp."
    echo " [0] -h          : print help"
}

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	make clean; make;
	input_configuration_file="wfSim_Terzina.conf"
	outputRootFileWith_wf="wfSim.root"
	numberOfWaveformsToSim=10
	rnd_seed=`date +%N`
	n_sig_pe=0
	./runterzina_wfSim 0 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe
	#./runterzina_wfSim 0 wfSim_Terzina.conf wfSim.root 10 `date +%N` 1
    elif [ "$1" = "-photTime" ]; then
	make clean; make;
	input_configuration_file="wfSim_Terzina.conf"
	outputRootFileWith_wf="wfSim.root"
	numberOfWaveformsToSim=10
	rnd_seed=`date +%N`
	#
	inRootFileWithShower="EASCherSim_0022360.ini.npz.root" # 7  km 100 PeV
	#inRootFileWithShower="EASCherSim_0000235.ini.npz.root" # 15 km 350 PeV
	#inRootFileWithShower="EASCherSim_0000701.ini.npz.root"  # 25 km 750 PeV
	#
	trakInfoDescription="trkInfo_0022360.dat" # 7  km 100 PeV
	#trakInfoDescription="trkInfo_0000235.dat" # 15 km 350 PeV
	#trakInfoDescription="trkInfo_0000701.dat"  # 25 km 750 PeV
	#
	n_sig_pe=7
	./runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
    elif [ "$1" = "-s" ]; then
	make clean; make;
	input_configuration_file="wfSim_Terzina.conf"
	outputRootFileWith_wf="wfSim.root"
	numberOfWaveformsToSim=100000
	rnd_seed=`date +%N`
	./runterzina_wfSim 1 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed
    elif [ "$1" = "-t" ]; then
	make clean; make;
	./runterzina_wfSim 2
    elif [ "$1" = "-LEE" ]; then
	make clean; make;
	./runterzina_wfSim 4 linearExpExp_NUVHD_25um_conv.conf
	./runterzina_wfSim 4 linearExpExp_NUVHD_35um_conv.conf
	./runterzina_wfSim 4 linearExpExp_NUVHD_25um_conv_fit_20MHz_filter.conf
	./runterzina_wfSim 4 linearExpExp_NUVHD_25um_conv_fit_40MHz_filter.conf
	./runterzina_wfSim 4 linearExpExp_NUVHD_35um_conv_fit_20MHz_filter.conf
	./runterzina_wfSim 4 linearExpExp_NUVHD_35um_conv_fit_40MHz_filter.conf
    elif [ "$1" = "-LEEfit" ]; then
	make clean; make;
	./runterzina_wfSim 5 linearExpExp_NUVHD_25um_conv_fit.conf ./wfData/linearExpExp_NUVHD_25um_conv.txt
    elif [ "$1" = "-FREQCUTOFF" ]; then
	make clean; make;
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_25um_conv.txt linearExpExp_NUVHD_25um_conv_fit.conf 20
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_25um_conv.txt linearExpExp_NUVHD_25um_conv_fit.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_35um_conv.txt linearExpExp_NUVHD_35um_conv_fit.conf 20
	./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_35um_conv.txt linearExpExp_NUVHD_35um_conv_fit.conf 40
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi
