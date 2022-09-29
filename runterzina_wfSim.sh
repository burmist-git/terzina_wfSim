#!/bin/bash

# Me 08 Jun 2022 14:02:24 CEST
# Autor: Leonid Burmistrov

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d                     : default"
    echo " [0] -photTime              : simulating realistic photon time"
    echo " [0] -photTimescreen        : simulating realistic photon time (with screen)"
    echo " [0] -photTimescreenNUV     : simulating realistic photon time (with screen) NUVHD SiPM"
    echo " [0] -photTimescreenNUVfull : simulating realistic photon time (with screen) NUVHD SiPM FULL BW."
    echo " [0] -photTimescreenNUV_DCR : scanning different levels of DCR"
    echo " [0] -s                     : single p.e. simulator"
    echo " [0] -t                     : test SiPM tree"
    echo " [0] -LEE                   : generate LEE template (linear exp. exp. SiPM responce)"
    echo " [0] -LEEfit                : fit with LEE function"
    echo " [0] -FREQCUTOFF            : simulate frequency cut off preamp."
    echo " [0] -h                     : print help"
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
	#input_configuration_file="wfSim_Terzina_25um_conv_fit_fullMHz_filter.conf"
	#outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_fullMHz_filter.root"
	#input_configuration_file="wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.conf"
	#outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root"
	#input_configuration_file="wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.conf"
	#outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.root"
	input_configuration_file="wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.conf"
	outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root"
	numberOfWaveformsToSim=10
	rnd_seed=`date +%N`
	#
	inRootFileWithShower="./wfData_root/EASCherSim_0022360.ini.npz.root" # 7  km 100 PeV
	#inRootFileWithShower="./wfData_root/EASCherSim_0000235.ini.npz.root" # 15 km 350 PeV
	#inRootFileWithShower="./wfData_root/EASCherSim_0000701.ini.npz.root"  # 25 km 750 PeV
	#
	trakInfoDescription="trkInfo_0022360.dat" # 7  km 100 PeV
	#trakInfoDescription="trkInfo_0000235.dat" # 15 km 350 PeV
	#trakInfoDescription="trkInfo_0000701.dat"  # 25 km 750 PeV
	#
	n_sig_pe=0
	./runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
    elif [ "$1" = "-photTimescreenNUVfull" ]; then
	make clean; make;
	#
	simpath="/home/dpncguest/home2/work/POEMMA/geant4/terzina_wfSim/"
	#
	#
	screenName='lee01'
	sufix="_7ns"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=10
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
    elif [ "$1" = "-photTimescreenNUV_DCR" ]; then
	make clean; make;
	simpath="/home/dpncguest/home2/work/POEMMA/geant4/terzina_wfSim/"
	inRootFileWithShower="./wfData_root/EASCherSim_0022360.ini.npz.root" # 7  km 100 PeV
	trakInfoDescription="trkInfo_0022360.dat" # 7  km 100 PeV
	numberOfWaveformsToSim=100
	n_sig_pe=0
	#
	input_configuration_file="wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.conf"
	outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root"
	rnd_seed=`date +%N`
	screenName='lee01'
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	#
	input_configuration_file="wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.conf"
	outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.root"
	rnd_seed=`date +%N`
	screenName='lee02'
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	#
	input_configuration_file="wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.conf"
	outputRootFileWith_wf="wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root"
	rnd_seed=`date +%N`
	screenName='lee03'
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
    elif [ "$1" = "-photTimescreenNUV" ]; then
	make clean; make;
	#
	simpath="/home/dpncguest/home2/work/POEMMA/geant4/terzina_wfSim/"
	numberOfWaveformsToSim_all=100

	#
	#
	screenName='lee01'
	sufix="_25um_conv_fit_20MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee02'
	sufix="_25um_conv_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee03'
	sufix="_35um_conv_fit_20MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee04'
	sufix="_35um_conv_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee05'
	sufix="_25um_conv_fit_fullMHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee06'
	sufix="_35um_conv_fit_fullMHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
    elif [ "$1" = "-photTimescreen" ]; then
	make clean; make;
	#
	simpath="/home/dpncguest/home2/work/POEMMA/geant4/terzina_wfSim/"
	numberOfWaveformsToSim_all=100
	#
	#
	screenName='lee01'
	sufix="_30ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee02'
	sufix="_90ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee03'
	sufix="_120ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee04'
	sufix="_150ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee05'
	sufix="_180ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee06'
	sufix="_210ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee07'
	sufix="_230ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	echo "$screenName"
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
	#
	#
	screenName='lee08'
	sufix="_260ns_fit_40MHz_filter"
	input_configuration_file=$simpath"wfSim_Terzina"$sufix".conf"
	outputRootFileWith_wf=$simpath"wfSim"$sufix".root"
	cp $simpath"./wfData_root/EASCherSim_0022360.ini.npz.root" $simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	inRootFileWithShower=$simpath"./wfData_root/EASCherSim_0022360.ini.npz"$sufix".root"
	cp $simpath"./trkInfo_0022360.dat" $simpath"./trkInfo_0022360"$sufix".dat"
	trakInfoDescription=$simpath"./trkInfo_0022360"$sufix".dat"
	numberOfWaveformsToSim=$numberOfWaveformsToSim_all
	n_sig_pe=0
	rnd_seed=`date +%N`
	screen -S $screenName -L -d -m $simpath/runterzina_wfSim 3 $input_configuration_file $outputRootFileWith_wf $numberOfWaveformsToSim $rnd_seed $n_sig_pe $inRootFileWithShower $trakInfoDescription
	sleep 1
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
	#./runterzina_wfSim 4 linearExpExp_NUVHD_25um_conv.conf
	#./runterzina_wfSim 4 linearExpExp_NUVHD_35um_conv.conf
	#./runterzina_wfSim 4 linearExpExp_NUVHD_25um_conv_fit_20MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_NUVHD_25um_conv_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_NUVHD_35um_conv_fit_20MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_NUVHD_35um_conv_fit_40MHz_filter.conf	
	./runterzina_wfSim 4 linearExpExp_tau_7ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_30ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_90ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_120ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_150ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_180ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_210ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_230ns.conf	
	#./runterzina_wfSim 4 linearExpExp_tau_260ns.conf
	#./runterzina_wfSim 4 linearExpExp_tau_30ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_90ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_120ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_150ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_180ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_210ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_230ns_fit_40MHz_filter.conf
	#./runterzina_wfSim 4 linearExpExp_tau_260ns_fit_40MHz_filter.conf
    elif [ "$1" = "-LEEfit" ]; then
	make clean; make;
	./runterzina_wfSim 5 linearExpExp_NUVHD_25um_conv_fit.conf ./wfData/linearExpExp_NUVHD_25um_conv.txt
    elif [ "$1" = "-FREQCUTOFF" ]; then
	make clean; make;
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_25um_conv.txt linearExpExp_NUVHD_25um_conv_fit.conf 20
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_25um_conv.txt linearExpExp_NUVHD_25um_conv_fit.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_35um_conv.txt linearExpExp_NUVHD_35um_conv_fit.conf 20
	#./runterzina_wfSim 6 ./wfData/linearExpExp_NUVHD_35um_conv.txt linearExpExp_NUVHD_35um_conv_fit.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_30ns.txt linearExpExp_tau_30ns_fit_40MHz_filter.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_90ns.txt linearExpExp_tau_90ns_fit_40MHz_filter.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_120ns.txt linearExpExp_tau_120ns_fit_40MHz_filter.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_150ns.txt linearExpExp_tau_150ns_fit_40MHz_filter.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_180ns.txt linearExpExp_tau_180ns_fit_40MHz_filter.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_210ns.txt linearExpExp_tau_210ns_fit_40MHz_filter.conf 40
	#./runterzina_wfSim 6 ./wfData/linearExpExp_tau_230ns.txt linearExpExp_tau_230ns_fit_40MHz_filter.conf 40
	./runterzina_wfSim 6 ./wfData/linearExpExp_tau_260ns.txt linearExpExp_tau_260ns_fit_40MHz_filter.conf 40
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi
