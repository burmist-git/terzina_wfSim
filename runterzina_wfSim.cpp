//my
#include "wfSim.hh"
#include "wfSimConfStr.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

int main(int argc, char *argv[]){
  //
  clock_t start, finish;
  start = clock();
  //
  if(argc == 7 && atoi(argv[1])==0){
    //
    TString input_configuration_file = argv[2];
    TString outputRootFileWith_wf = argv[3];
    Int_t numberOfWaveformsToSim = atoi(argv[4]);
    Int_t rnd_seed = atoi(argv[5]);
    Int_t n_sig_pe = atoi(argv[6]);
    //
    wfSimConfStr *wfConf = new wfSimConfStr();
    wfConf->readFromFile(input_configuration_file);
    wfConf->printInfo();
    //
    std::cout<<std::endl
	     <<"input_configuration_file "<<input_configuration_file<<std::endl
	     <<"outputRootFileWith_wf    "<<outputRootFileWith_wf<<std::endl
	     <<"numberOfWaveformsToSim   "<<numberOfWaveformsToSim<<std::endl
	     <<"rnd_seed                 "<<rnd_seed<<std::endl;
    //
    TRandom3 *rnd = new TRandom3(rnd_seed);
    wfSim *wf = new wfSim(rnd,wfConf);
    wf->getWF_tmpl(wfConf->Template);
    ////////////////
    TFile* rootFile = new TFile(outputRootFileWith_wf.Data(), "RECREATE", " Histograms", 1);
    rootFile->cd();
    if (rootFile->IsZombie()){
      std::cout<<"  ERROR ---> file "<<outputRootFileWith_wf.Data()<<" is zombi"<<std::endl;
      assert(0);
    }
    else {
      std::cout<<"  Output Histos file ---> "<<outputRootFileWith_wf.Data()<<std::endl;
    }
    ////////////////
    for(Int_t i = 0;i<numberOfWaveformsToSim;i++){
      std::cout<<i<<std::endl;
      TGraph *gr_wf = new TGraph();
      TGraph *gr_wf_sig = new TGraph();
      TGraph *gr_wf_sig_only = new TGraph();
      ////////////////
      char buffer [5];
      sprintf(buffer ,"%04d", i);
      TString wf_ID = buffer;
      TString gr_wf_name_title = "gr_wf_"; gr_wf_name_title += wf_ID;
      TString gr_wf_sig_name_title = "gr_wf_sig_"; gr_wf_sig_name_title += wf_ID;
      TString gr_wf_sig_only_name_title = "gr_wf_sig_only_"; gr_wf_sig_only_name_title += wf_ID;
      ////////////////
      gr_wf->SetTitle(gr_wf_name_title.Data());
      gr_wf->SetName(gr_wf_name_title.Data());
      gr_wf_sig->SetTitle(gr_wf_sig_name_title.Data());
      gr_wf_sig->SetName(gr_wf_sig_name_title.Data());
      gr_wf_sig_only->SetTitle(gr_wf_sig_only_name_title.Data());
      gr_wf_sig_only->SetName(gr_wf_sig_only_name_title.Data());
      wf->gen_WF( gr_wf, gr_wf_sig, gr_wf_sig_only, n_sig_pe);
      gr_wf->Write();
      gr_wf_sig->Write();
      gr_wf_sig_only->Write();
    }
    //
    wf->getTemplate()->Write();
    //
    rootFile->Close();
  }
  else{
    std::cout<<"  wavefrom (width ampl)                   "<<std::endl
	     <<"  runID [1] = 0                           "<<std::endl
 	     <<"        [2] - input configuration file    "<<std::endl
	     <<"        [3] - output root file with wf.   "<<std::endl
	     <<"        [4] - number of waveforms to sim. "<<std::endl
	     <<"        [5] - seed                        "<<std::endl
      	     <<"        [6] - number of signal p.e.       "<<std::endl;
  }
  //
  finish = clock();
  std::cout<<"-------------------------"<<std::endl
  	   <<"Working time : "<<((finish - start)/CLOCKS_PER_SEC)<<" (sec)"<<std::endl
  	   <<"-------------------------"<<std::endl;
  //
  return 0;
}
