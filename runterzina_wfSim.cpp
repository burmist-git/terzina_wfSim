//my
#include "wfSim.hh"
#include "wfSimConfStr.hh"
#include "sipmAvalancheTree.hh"
#include "genSiPMwf.hh"
#include "wfParametrisationStr.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TGraphErrors.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

void readDatTrkFile(TString fileName, Double_t &theta, Double_t &phi, 
                    Double_t &x_int, Double_t &y_int, Double_t &z_int, 
                    Double_t &xe0, Double_t &ye0, Double_t &ze0, 
                    Double_t &distToEarth, Double_t &distToTerzina, Double_t &angleTrzinaTrk, Double_t &nphotons_per_m2);
void readHistG4File( TString hist_fileName, TH1D *h1_photon_time, Double_t dist);
void copyHistogram(TH1D *h1, TH1D *h1_copy, TString h1_name, TString h1_title, bool ifBinsOnly, double norm);
void integrateHistogram(TH1D *h1, TH1D *h1_integral, bool normKey);

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
	     <<"rnd_seed                 "<<rnd_seed<<std::endl
      	     <<"n_sig_pe                 "<<n_sig_pe<<std::endl;
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
      //std::cout<<i<<std::endl;
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
  else if (argc == 9 && atoi(argv[1]) == 3) {
    //
    TString input_configuration_file = argv[2];
    TString outputRootFileWith_wf = argv[3];
    Int_t numberOfWaveformsToSim = atoi(argv[4]);
    Int_t rnd_seed = atoi(argv[5]);
    Int_t n_sig_pe = atoi(argv[6]);
    TString inRootFileWithShower = argv[7];
    TString trakInfoDescription = argv[8];
    //
    wfSimConfStr *wfConf = new wfSimConfStr();
    wfConf->readFromFile(input_configuration_file);
    wfConf->printInfo();
    Double_t totalTime_in_s = (wfConf->SimEndTime - wfConf->SimStartTime)*numberOfWaveformsToSim*1.0e-9;
    std::cout<<std::endl
	     <<"input_configuration_file "<<input_configuration_file<<std::endl
	     <<"outputRootFileWith_wf    "<<outputRootFileWith_wf<<std::endl
	     <<"numberOfWaveformsToSim   "<<numberOfWaveformsToSim<<std::endl
	     <<"rnd_seed                 "<<rnd_seed<<std::endl
      	     <<"n_sig_pe                 "<<n_sig_pe<<std::endl
    	     <<"inRootFileWithShower     "<<inRootFileWithShower<<std::endl
    	     <<"trakInfoDescription      "<<trakInfoDescription<<std::endl;
    //
    Double_t theta, phi;
    Double_t x_int, y_int, z_int;
    Double_t xe0, ye0, ze0; 
    Double_t distToEarth;
    Double_t distToTerzina;
    Double_t angleTrzinaTrk;
    Double_t nphotons_per_m2;
    //
    readDatTrkFile(trakInfoDescription, theta, phi, 
		   x_int, y_int, z_int, 
		   xe0, ye0, ze0, 
		   distToEarth, distToTerzina, angleTrzinaTrk, nphotons_per_m2);
    std::cout<<"distToTerzina = "<<distToTerzina<<std::endl;
    //
    TH1D *h1_photon_time = new TH1D();
    readHistG4File( inRootFileWithShower, h1_photon_time, distToTerzina);
    //h1_photon_time->SaveAs("h1_photon_time.C");
    //
    TRandom3 *rnd = new TRandom3(rnd_seed);
    wfSim *wf = new wfSim(rnd,wfConf);
    wf->getWF_tmpl(wfConf->Template);
    TH1D *h1_ampl = new TH1D("h1_ampl","h1_ampl",1000,0.0,20.0); 
    TH1D *h1_amplLocMax = new TH1D("h1_amplLocMax","h1_amplLocMax",1000,0.0,20.0); 
    TGraph *gr_threshold_counter = new TGraph();
    gr_threshold_counter->SetTitle("gr_threshold_counter");
    gr_threshold_counter->SetName("gr_threshold_counter");
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
      //std::cout<<i<<std::endl;
      TGraph *gr_wf = new TGraph();
      TGraph *gr_wf_locMax = new TGraph();
      TGraph *gr_wf_sig = new TGraph();
      TGraph *gr_wf_sig_only = new TGraph();
      ////////////////
      char buffer [5];
      sprintf(buffer ,"%04d", i);
      TString wf_ID = buffer;
      TString gr_wf_name_title = "gr_wf_"; gr_wf_name_title += wf_ID;
      TString gr_wf_locMax_name_title = "gr_wf_locMax_"; gr_wf_locMax_name_title += wf_ID;
      TString gr_wf_sig_name_title = "gr_wf_sig_"; gr_wf_sig_name_title += wf_ID;
      TString gr_wf_sig_only_name_title = "gr_wf_sig_only_"; gr_wf_sig_only_name_title += wf_ID;
      ////////////////
      gr_wf->SetTitle(gr_wf_name_title.Data());
      gr_wf->SetName(gr_wf_name_title.Data());
      gr_wf_locMax->SetTitle(gr_wf_locMax_name_title.Data());
      gr_wf_locMax->SetName(gr_wf_locMax_name_title.Data());
      gr_wf_sig->SetTitle(gr_wf_sig_name_title.Data());
      gr_wf_sig->SetName(gr_wf_sig_name_title.Data());
      gr_wf_sig_only->SetTitle(gr_wf_sig_only_name_title.Data());
      gr_wf_sig_only->SetName(gr_wf_sig_only_name_title.Data());
      wf->gen_WF( gr_wf, gr_wf_sig, gr_wf_sig_only, n_sig_pe, h1_photon_time);
      //wf->save_to_csv( gr_wf, gr_wf_sig, gr_wf_sig_only);
      wfSim::get_Ampl_hist( gr_wf, h1_ampl);
      wfSim::get_AmplLocalMax_hist( gr_wf, gr_wf_locMax, h1_amplLocMax);
      wfSim::get_count_threshold_vs_rate( h1_amplLocMax, gr_threshold_counter, totalTime_in_s);
      gr_wf->Write();
      gr_wf_locMax->Write();
      gr_wf_sig->Write();
      gr_wf_sig_only->Write();
    }
    //
    wf->getTemplate()->Write();
    h1_photon_time->Write();
    h1_ampl->Write();
    h1_amplLocMax->Write();
    //
    gr_threshold_counter->Write();
    //
    rootFile->Close();
  }
  else if (argc == 3 && atoi(argv[1]) == 4) {
    TString parameterizationConf = argv[2];
    std::cout<<"parameterizationConf "<<parameterizationConf<<std::endl;
    //genSiPMwf *lee_wf = new genSiPMwf(parameterizationConf);
    new genSiPMwf(parameterizationConf);
  }
  else if (argc == 4 && atoi(argv[1]) == 5) {
    TString parameterizationInitialConf = argv[2];
    TString templateFiles  = argv[3];
    std::cout<<"parameterizationInitialConf "<<parameterizationInitialConf<<std::endl
	     <<"templateFiles               "<<templateFiles<<std::endl;
    //
    genSiPMwf *lee_wf = new genSiPMwf(parameterizationInitialConf);
    TGraphErrors *grerr = new TGraphErrors();
    //
    genSiPMwf::load_data( templateFiles, grerr, 0, 5);
    lee_wf->fit_LEE( grerr, false);
    //grerr->SaveAs("grerr.C");
    //new genSiPMwf(parameterizationConf);
  }
  else if (argc == 5 && atoi(argv[1]) == 6) {
    TString templateFiles = argv[2];
    TString parameterizationInitialConf = argv[3];
    Double_t cut_off_frequency = atof(argv[4]);
    //
    std::cout<<"templateFiles               "<<templateFiles<<std::endl
	     <<"parameterizationInitialConf "<<parameterizationInitialConf<<std::endl
	     <<"cut_off_frequency           "<<cut_off_frequency<<std::endl;
    //
    genSiPMwf *wfsim_lee = new genSiPMwf();
    TGraphErrors *grerr = new TGraphErrors();
    genSiPMwf::load_data( templateFiles, grerr, 1, 1);
    //
    TGraph *gr = new TGraph();
    gr->SetNameTitle("gr","gr");
    TGraph *gr_filter = new TGraph();
    gr_filter->SetNameTitle("gr_filter","gr_filter");
    Double_t t,a;
    for(Int_t i = 0;i<grerr->GetN();i++){
      grerr->GetPoint(i,t,a);
      gr->SetPoint(i,t,a);
      gr_filter->SetPoint(i,t,a);
    }
    //
    TGraph *gr_fft_re = new TGraph(); gr_fft_re->SetNameTitle("gr_fft_re","gr_fft_re");
    TGraph *gr_fft_im = new TGraph(); gr_fft_im->SetNameTitle("gr_fft_im","gr_fft_im");
    TGraph *gr_fft_abs = new TGraph(); gr_fft_abs->SetNameTitle("gr_fft_abs","gr_fft_abs");
    wfsim_lee->get_FFT_spectrum( gr, gr_fft_re, gr_fft_im, gr_fft_abs);
    wfsim_lee->simulateFrequencyCutOffpreamp( gr_filter, cut_off_frequency);
    //
    genSiPMwf *lee_wf = new genSiPMwf(parameterizationInitialConf);
    TGraphErrors *grerr_filter = new TGraphErrors();
    Int_t ii = 0; 
    for(Int_t i = 0;i<gr_filter->GetN();i++){
      gr_filter->GetPoint(i,t,a);
      if(i%10 == 0){
	grerr_filter->SetPoint(ii,t,a);
	grerr_filter->SetPointError(ii,0.05,0.2);
	ii++;
      }
    }
    //
    lee_wf->fit_LEE( grerr_filter, true);
    grerr_filter->SetNameTitle("grerr_filter","grerr_filter");
    //
    TGraphErrors *grerr_filter_fit = new TGraphErrors();
    grerr_filter_fit->SetNameTitle("grerr_filter_fit","grerr_filter_fit");
    genSiPMwf::load_data(lee_wf->get_lee_Param_str()->outputTemplate, grerr_filter_fit, 1, 1);
    //
    TFile* rootFile = new TFile("fft.root", "RECREATE", " Histograms", 1);
    gr->Write();
    gr_filter->Write();
    grerr_filter->Write();
    grerr_filter_fit->Write();
    gr_fft_abs->Write();
    gr_fft_re->Write();
    gr_fft_im->Write();
    rootFile->Close();
  }
  else if (argc == 9 && atoi(argv[1]) == 7) {
    //
    TString input_configuration_file = argv[2];
    TString outputRootFileWith_wf = argv[3];
    Int_t numberOfWaveformsToSim = atoi(argv[4]);
    Int_t rnd_seed = atoi(argv[5]);
    Int_t n_sig_pe = atoi(argv[6]);
    TString inRootFileWithShower = argv[7];
    TString trakInfoDescription = argv[8];
    //
    wfSimConfStr *wfConf = new wfSimConfStr();
    wfConf->readFromFile(input_configuration_file);
    wfConf->printInfo();
    Double_t totalTime_in_s = (wfConf->SimEndTime - wfConf->SimStartTime)*numberOfWaveformsToSim*1.0e-9;
    std::cout<<std::endl
	     <<"input_configuration_file "<<input_configuration_file<<std::endl
	     <<"outputRootFileWith_wf    "<<outputRootFileWith_wf<<std::endl
	     <<"numberOfWaveformsToSim   "<<numberOfWaveformsToSim<<std::endl
	     <<"rnd_seed                 "<<rnd_seed<<std::endl
      	     <<"n_sig_pe                 "<<n_sig_pe<<std::endl
    	     <<"inRootFileWithShower     "<<inRootFileWithShower<<std::endl
    	     <<"trakInfoDescription      "<<trakInfoDescription<<std::endl;
    //
    Double_t theta, phi;
    Double_t x_int, y_int, z_int;
    Double_t xe0, ye0, ze0; 
    Double_t distToEarth;
    Double_t distToTerzina;
    Double_t angleTrzinaTrk;
    Double_t nphotons_per_m2;
    //
    readDatTrkFile(trakInfoDescription, theta, phi, 
		   x_int, y_int, z_int, 
		   xe0, ye0, ze0, 
		   distToEarth, distToTerzina, angleTrzinaTrk, nphotons_per_m2);
    std::cout<<"distToTerzina = "<<distToTerzina<<std::endl;
    //
    TH1D *h1_photon_time = new TH1D();
    TH1D *h1_photon_time_test = new TH1D();
    TH1D *h1_photon_time_integral = new TH1D();
    readHistG4File( inRootFileWithShower, h1_photon_time, distToTerzina);
    copyHistogram(h1_photon_time, h1_photon_time_test, "h1_photon_time_test", "h1_photon_time_test", true, 1.0);
    copyHistogram(h1_photon_time, h1_photon_time_integral, "h1_photon_time_integral", "h1_photon_time_integral", true, 1.0);
    integrateHistogram(h1_photon_time, h1_photon_time_integral, true);
    TRandom3 *rnd = new TRandom3(rnd_seed);
    wfSim *wf = new wfSim(rnd,wfConf);
    wf->testGenerateDistFromHist(h1_photon_time,h1_photon_time_test);
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
    h1_photon_time->Write();
    h1_photon_time_test->Write();
    h1_photon_time_integral->Write();
    rootFile->Close();
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
    std::cout<<"  runID [1] = 3 (Sig. + bkg.)             "<<std::endl
 	     <<"        [2] - input configuration file    "<<std::endl
	     <<"        [3] - output root file with wf.   "<<std::endl
	     <<"        [4] - number of waveforms to sim. "<<std::endl
	     <<"        [5] - seed                        "<<std::endl
      	     <<"        [6] - number of signal p.e.       "<<std::endl
	     <<"        [7] - in Root File With Shower    "<<std::endl
      	     <<"        [8] - trk Info File               "<<std::endl;
    std::cout<<"  runID [1] = 4 (generate LEE template (linear exp. exp. SiPM responce)) "<<std::endl
 	     <<"        [2] - input SiPM wf. configuration file "<<std::endl;
    std::cout<<"  runID [1] = 5 (fit with LEE function) "<<std::endl
 	     <<"        [2] - input initial SiPM wf. configuration file "<<std::endl
      	     <<"        [3] - file with waveform to fit "<<std::endl;
    std::cout<<"  runID [1] = 6 (simulate frequency cut off preamp.) "<<std::endl
 	     <<"        [2] - initial waveform"<<std::endl
       	     <<"        [3] - input initial SiPM wf. configuration file "<<std::endl
      	     <<"        [4] - cut off frequency in MHz"<<std::endl;
    std::cout<<"  runID [1] = 7 (test GenerateDistFromHist)"<<std::endl
 	     <<"        [2] - input configuration file    "<<std::endl
	     <<"        [3] - output root file with wf.   "<<std::endl
	     <<"        [4] - number of waveforms to sim. "<<std::endl
	     <<"        [5] - seed                        "<<std::endl
      	     <<"        [6] - number of signal p.e.       "<<std::endl
	     <<"        [7] - in Root File With Shower    "<<std::endl
      	     <<"        [8] - trk Info File               "<<std::endl;
  }  //
  finish = clock();
  std::cout<<"-------------------------"<<std::endl
  	   <<"Working time : "<<((finish - start)/CLOCKS_PER_SEC)<<" (sec)"<<std::endl
  	   <<"-------------------------"<<std::endl;
  //
  return 0;
}

void readDatTrkFile(TString fileName, Double_t &theta, Double_t &phi, 
                    Double_t &x_int, Double_t &y_int, Double_t &z_int, 
                    Double_t &xe0, Double_t &ye0, Double_t &ze0, 
                    Double_t &distToEarth, Double_t &distToTerzina, Double_t &angleTrzinaTrk, Double_t &nphotons_per_m2){
  x_int = -999.0;
  y_int = -999.0;
  z_int = -999.0;
  xe0 = -999.0;
  ye0 = -999.0;
  ze0 = -999.0;
  distToEarth = -999.0;
  distToTerzina = -999.0;
  angleTrzinaTrk = -999.0;
  nphotons_per_m2 = -999.0;
  //
  std::ifstream myfile(fileName.Data());
  std::string mot;
  if(myfile.is_open()){
    for(Int_t i = 0;i<12;i++)
      myfile>>mot;
    myfile>>theta>>phi>>x_int>>y_int>>z_int>>xe0>>ye0>>ze0>>distToEarth>>distToTerzina>>angleTrzinaTrk>>nphotons_per_m2;
    myfile.close();
  }
}

void readHistG4File( TString hist_fileName, TH1D *h1_photon_time, Double_t dist){
  //
  TFile *f1 = new TFile(hist_fileName.Data());
  //
  //
  TH1D *h1_1 = (TH1D*)f1->Get("r");
  Int_t binID = h1_1->FindBin(dist);
  TString h1_2_name = "t_dist_";
  h1_2_name += binID;
  TH1D *h1_2 = (TH1D*)f1->Get(h1_2_name.Data());
  double norm = h1_2->GetMaximum();
  copyHistogram(h1_2, h1_photon_time, "h1_photon_time", "h1_photon_time", false, norm);
  f1->Close();
}

void copyHistogram(TH1D *h1, TH1D *h1_copy, TString h1_name, TString h1_title, bool ifBinsOnly, double norm){
  h1_copy->SetName(h1_name.Data());
  h1_copy->SetTitle(h1_title.Data());
  const int n_bins_max = 1000;
  double bins_low_edge[n_bins_max];
  int nBins = h1->GetNbinsX();
  if((nBins+1)>n_bins_max)
    std::cout<<"ERROR --> (nBins+1)  > n_bins_max"<<std::endl
	     <<"          nBins      = "<<nBins<<std::endl
	     <<"          n_bins_max = "<<n_bins_max<<std::endl;
  for(int i = 0;i<nBins;i++)
    bins_low_edge[i] = h1->GetBinLowEdge(i+1);
  bins_low_edge[nBins] = h1->GetBinLowEdge(nBins) + h1->GetBinWidth(nBins);
  h1_copy->SetBins(nBins,bins_low_edge);
  if(!ifBinsOnly && norm>0.0)
    for(int i = 1;i<=nBins;i++)
      h1_copy->SetBinContent(i,h1->GetBinContent(i)/norm);
}

void integrateHistogram(TH1D *h1, TH1D *h1_integral, bool normKey){
  int nBins = h1->GetNbinsX();
  Double_t integralhist = 0; 
  for(int i = 1;i<nBins;i++){
    integralhist = h1->Integral(1,i+1);
    h1_integral->SetBinContent(i,integralhist);
  }
  Double_t norm = h1_integral->GetMaximum();
  if(normKey)
    for(int i = 1;i<=nBins;i++)
      h1_integral->SetBinContent(i,h1_integral->GetBinContent(i)/norm);
}
