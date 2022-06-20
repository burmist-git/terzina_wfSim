//my
#include "wfSim.hh"
#include "wfSimConfStr.hh"
#include "sipmAvalancheTree.hh"

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
  else if (argc == 6 && atoi(argv[1])==1) {
    //
    sipmAvalancheTree *theTree = new sipmAvalancheTree("theTree","theTree");
    //theTree->test();
    //assert(0);
    //    
    TString input_configuration_file = argv[2];
    TString outputRootFileWith_wf = argv[3];
    Int_t numberOfWaveformsToSim = atoi(argv[4]);
    Int_t rnd_seed = atoi(argv[5]);
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
    Int_t parentID;
    Int_t typeID;
    Int_t parentGenerationID;
    Double_t probabilityCorrectionFactor = 1.0;
    Double_t first_pe_time;
    Double_t first_pe_ampl;
    Int_t binID;
    theTree->SetMinimum(1.0);
    theTree->SetMaximum(1000.0);
    for(Int_t i = 0;i<numberOfWaveformsToSim;i++){
      std::vector<photoElectronInfo> pe_vec;
      first_pe_time = wfConf->signal_t0;
      first_pe_ampl = wfConf->single_p_e_ampl;
      parentGenerationID = -1;
      parentID = -1;
      typeID = 0;
      wf->simPE(pe_vec, first_pe_time, first_pe_ampl, typeID, parentGenerationID, parentID, probabilityCorrectionFactor);
      for(unsigned int j = 0;j<pe_vec.size();j++){
	//pe_vec.at(j).printInfo();
	binID = pe_vec.at(j).photoelectronID+1;
	theTree->SetBinContent(binID,(theTree->GetBinContent(binID)+1.0));
      }
    }
    //
    wf->getTemplate()->Write();
    theTree->Write();
    //
    theTree->DrawHisto();
    //
    rootFile->Close();
  }
  else if (argc == 2 && atoi(argv[1])==2) {
    //
    sipmAvalancheTree *theTree01 = new sipmAvalancheTree("theTree01","theTree01");
    sipmAvalancheTree *theTree02 = new sipmAvalancheTree("theTree02","theTree02");
    sipmAvalancheTree *theTree03 = new sipmAvalancheTree("theTree03","theTree03");
    theTree01->test();
    theTree02->test2();
    theTree03->test3();
  }
  else{
    std::cout<<"  runID [1] = 0 (Sig. + bkg.)             "<<std::endl
 	     <<"        [2] - input configuration file    "<<std::endl
	     <<"        [3] - output root file with wf.   "<<std::endl
	     <<"        [4] - number of waveforms to sim. "<<std::endl
	     <<"        [5] - seed                        "<<std::endl
      	     <<"        [6] - number of signal p.e.       "<<std::endl;
    std::cout<<"  runID [1] = 1 (signal tree)             "<<std::endl
 	     <<"        [2] - input configuration file    "<<std::endl
	     <<"        [3] - output root file with wf.   "<<std::endl
	     <<"        [4] - number of waveforms to sim. "<<std::endl
	     <<"        [5] - seed                        "<<std::endl;
    std::cout<<"  runID [1] = 2 (tree test)               "<<std::endl
	     <<"        [5] - seed                        "<<std::endl;
  }
  //
  finish = clock();
  std::cout<<"-------------------------"<<std::endl
  	   <<"Working time : "<<((finish - start)/CLOCKS_PER_SEC)<<" (sec)"<<std::endl
  	   <<"-------------------------"<<std::endl;
  //
  return 0;
}
