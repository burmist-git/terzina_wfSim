//my
#include "wfSim.hh"
#include "wfSimConfStr.hh"

//root
#include "TRandom3.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1D.h"

//C, C++
#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include <iomanip>

wfSim::wfSim(TRandom3 *rnd, wfSimConfStr *wfConf){
  _rnd = rnd;
  _gr_wf_tmpl = new TGraph();
  _wfConf = wfConf;
  _t_max_ampl_wf_tmpl = -999.0;
}

wfSim::~wfSim(){
}

void wfSim::get_Ampl_hist(TGraph *wf, TH1D *h1){
  Double_t t;
  Double_t a;
  for(Int_t i = 0;i<wf->GetN();i++){
    wf->GetPoint(i,t,a);
    h1->Fill(a);
  }
}

void wfSim::get_AmplLocalMax_hist(TGraph *wf,  TGraph *gr_max, TH1D *h1){
  Double_t t_l;
  Double_t a_l;
  Double_t t;
  Double_t a;
  Double_t t_r;
  Double_t a_r;
  Double_t delta_a_l;
  Double_t delta_a_r;
  for(Int_t i = 1;i<(wf->GetN()-1);i++){
    //
    wf->GetPoint(i-1,t_l,a_l);
    wf->GetPoint(i,t,a);
    wf->GetPoint(i+1,t_r,a_r);
    //
    delta_a_l = a - a_l;
    delta_a_r = a - a_r;
    //
    if(delta_a_l>=0.0 && delta_a_r>=0.0){
      if(a>0.0){
	h1->Fill(a);
	gr_max->SetPoint(gr_max->GetN(),t,a);
      }
    }
  }
}

void wfSim::getWF_tmpl(TString name){
  std::ifstream fileIn(name.Data());
  double x;
  double y;
  double ymax = -999.0;
  //_gr_wf_tmpl->SetName(name.Data());
  //_gr_wf_tmpl->SetTitle(name.Data());
  _gr_wf_tmpl->SetName("_gr_wf_tmpl");
  _gr_wf_tmpl->SetTitle("_gr_wf_tmpl");
  if (fileIn.is_open()){
    while(fileIn>>x>>y){
      _gr_wf_tmpl->SetPoint(_gr_wf_tmpl->GetN(),x,y);
      if(ymax<y){
	ymax = y;
	_t_max_ampl_wf_tmpl = x;
      }
    }
    fileIn.close();
  }
  else {
    std::cout<<"Unable to open file"<<std::endl;
    assert(0);
  }
}

void wfSim::simPE(auto (&pe_vec), Double_t t, Double_t a, Int_t typeID, Int_t parentGenerationID, Int_t parentID, Double_t probabilityCorrectionFactor){
  photoElectronInfo pe_info;
  Int_t generation = -999;
  if(parentID>-1 && parentGenerationID>-1){
    generation = parentGenerationID + 1;
    pe_info.photoelectronID = 2*parentID + typeID;
    pe_info.photoelectronParentID = parentID;
    pe_info.photoelectronParentLocalID = parentID - TMath::Power(2,parentGenerationID) + 1;  
    pe_info.photoelectronLocalID = pe_info.photoelectronID - TMath::Power(2,generation) + 1;  
    pe_info.parentGeneration = parentGenerationID;
    pe_info.generation = generation;
    pe_info.typeID = typeID;
    pe_info.time = t;
    pe_info.ampl = a;
  }
  else if ((parentID == -1) && (parentGenerationID == -1)){
    generation = parentGenerationID + 1;
    pe_info.photoelectronID = 0;
    pe_info.photoelectronParentID = parentID;
    pe_info.photoelectronParentLocalID = -1;  
    pe_info.photoelectronLocalID = 0;  
    pe_info.parentGeneration = parentGenerationID;
    pe_info.generation = generation;
    pe_info.typeID = typeID;
    pe_info.time = t;
    pe_info.ampl = a;
  }
  pe_vec.push_back(pe_info);
  crosstalk_sim(pe_vec, t, a, typeID, generation, pe_info.photoelectronID, probabilityCorrectionFactor);
  afterpulse_sim(pe_vec, t, a, typeID, generation, pe_info.photoelectronID, probabilityCorrectionFactor);
}

void wfSim::crosstalk_sim(auto (&pe_vec), Double_t t, Double_t a, Int_t typeID, Int_t parentGenerationID, Int_t parentID, Double_t probabilityCorrectionFactor){
  if(_rnd->Uniform()<=_wfConf->PxtProb*probabilityCorrectionFactor){
    Double_t ampl = _rnd->Gaus(_wfConf->single_p_e_ampl,_wfConf->SigmaGain);
    Double_t probCorr = 1.0;
    typeID = 1;
    simPE( pe_vec, t, ampl, typeID, parentGenerationID, parentID, probCorr);
  }
}

void wfSim::afterpulse_sim(auto (&pe_vec), Double_t t, Double_t a, Int_t typeID, Int_t parentGenerationID, Int_t parentID, Double_t probabilityCorrectionFactor){
  if(_rnd->Uniform()<=_wfConf->PapProb*probabilityCorrectionFactor){
    Double_t dT = _rnd->Exp(_wfConf->PapTau);
    Double_t tap = t + dT;
    Double_t a_net = (_wfConf->single_p_e_ampl - _gr_wf_tmpl->Eval(dT+_t_max_ampl_wf_tmpl));
    //Double_t a_net = _wfConf->single_p_e_ampl;
    Double_t probCorr = a_net/_wfConf->single_p_e_ampl;
    if(a_net<0.0){
      std::cout<<"ERROR -->  "<<std::endl
	       <<"           a_net    = "<<a_net<<std::endl;
      assert(0);
    }
    if(probCorr<0.0 || probCorr>1.0){
      std::cout<<"ERROR -->  "<<std::endl
	       <<"           probCorr = "<<probCorr<<std::endl;
      assert(0);
    }
    Double_t ampl = _rnd->Gaus( a_net, _wfConf->SigmaGain*probCorr);    
    //std::cout<<"dT                  "<<dT<<std::endl
    //	     <<"_t_max_ampl_wf_tmpl "<<_t_max_ampl_wf_tmpl<<std::endl
    //	     <<"tap                 "<<tap<<std::endl
    //	     <<"a_net               "<<a_net<<std::endl
    //	     <<"ampl                "<<ampl<<std::endl;
    typeID = 2;
    //probCorr = 1.0;
    simPE( pe_vec, tap, ampl, typeID, parentGenerationID, parentID, probCorr);
  }
}

void wfSim::print_pe_vec(const auto (&all_pe_vec)){
  for(unsigned int i = 0;i<all_pe_vec.size();i++){
    std::cout<<std::endl;
    for(unsigned int j = 0;j<all_pe_vec.at(i).size();j++){
      all_pe_vec.at(i).at(j).printInfo();
    }
    std::cout<<std::endl;
  }
}

void wfSim::get_count_threshold_vs_rate( TH1D *h1, TGraph *gr, Double_t totalTime_in_s){
  Double_t threshold;
  Double_t rate;
  for(Int_t i = 1; i<=h1->GetNbinsX();i++){
    threshold = h1->GetBinCenter(i);
    rate = h1->Integral(i,(h1->GetNbinsX()+1))/totalTime_in_s;
    gr->SetPoint(i-1,threshold,rate);
  }
}

void wfSim::gen_WF( TGraph *gr_wf, TGraph *gr_wf_sig, TGraph *gr_wf_sig_only, unsigned int n_signals, TH1D *h1_photon_time){
  //
  Double_t dT = (_wfConf->SimEndTime - _wfConf->SimStartTime);
  Double_t dT_s = dT*1.0e-9; 
  //unsigned int n_noise = (Int_t)(TMath::Floor(dT_s*_wfConf->DCRrate)+1);
  //Int_t n = (Int_t)(TMath::Floor(dT/_wfConf->SamplingTime)+1);
  unsigned int n_noise = (Int_t)(TMath::Floor(dT_s*_wfConf->DCRrate));
  Int_t n = (Int_t)(TMath::Floor(dT/_wfConf->SamplingTime));
  //
  Double_t first_pe_time;
  Double_t first_pe_ampl;
  Int_t parentID;
  Int_t typeID;
  Int_t parentGenerationID;
  Double_t probabilityCorrectionFactor = 1.0;
  //noise
  std::vector<std::vector<photoElectronInfo>> all_pe_vec;
  for(unsigned int i = 0; i<n_noise; i++){
    std::vector<photoElectronInfo> pe_vec;
    first_pe_time = _rnd->Uniform(_wfConf->SimEndTime,_wfConf->SimStartTime);
    first_pe_ampl = _rnd->Gaus(_wfConf->single_p_e_ampl,_wfConf->SigmaGain);
    parentGenerationID = -1;
    parentID = -1;
    typeID = 0;
    simPE(pe_vec, first_pe_time, first_pe_ampl, typeID, parentGenerationID, parentID, probabilityCorrectionFactor);
    all_pe_vec.push_back(pe_vec);
  }
  //print_pe_vec(all_pe_vec);
  //signal
  std::vector<std::vector<photoElectronInfo>> sig_pe_vec;
  Double_t dT_shower;
  //
  for(unsigned int i = 0; i<n_signals; i++){
    std::vector<photoElectronInfo> pe_vec;
    dT_shower = generateDistFromHist(h1_photon_time);
    //std::cout<<"dT_shower = "<<dT_shower<<std::endl;
    first_pe_time = _wfConf->signal_t0 + dT_shower;
    first_pe_ampl = _rnd->Gaus(_wfConf->single_p_e_ampl,_wfConf->SigmaGain);
    parentGenerationID = -1;
    parentID = -1;
    typeID = 0;
    simPE(pe_vec, first_pe_time, first_pe_ampl, typeID, parentGenerationID, parentID, probabilityCorrectionFactor);
    sig_pe_vec.push_back(pe_vec);
  }
  //
  //print_pe_vec(sig_pe_vec);
  //  
  Double_t t;
  Double_t ampl;
  Double_t ampl_p_signal;
  Double_t ampl_signal;
  Double_t electricNoise;
  for(Int_t i = 0;i<n;i++){
    //if(i%10000==0)
    //std::cout<<i<<std::endl;
    t = _wfConf->SimStartTime + _wfConf->SamplingTime*i;
    ampl = 0.0;
    ampl_p_signal = 0.0;
    ampl_signal = 0.0;
    //p.e. noise
    for(unsigned int j = 0;j<all_pe_vec.size();j++){
      for(unsigned int k = 0;k<all_pe_vec.at(j).size();k++){
	if((t - all_pe_vec.at(j).at(k).time )>=0 && (t - all_pe_vec.at(j).at(k).time)<1000){
    	  ampl += all_pe_vec.at(j).at(k).ampl*_gr_wf_tmpl->Eval(t - all_pe_vec.at(j).at(k).time);
	}
      }
    }
    ampl_p_signal = ampl;
    //signal
    for(unsigned int j = 0;j<sig_pe_vec.size();j++){
      for(unsigned int k = 0;k<sig_pe_vec.at(j).size();k++){
	if((t - sig_pe_vec.at(j).at(k).time )>=0 && (t - sig_pe_vec.at(j).at(k).time)<1000){   
	  ampl_p_signal += sig_pe_vec.at(j).at(k).ampl*_gr_wf_tmpl->Eval(t - sig_pe_vec.at(j).at(k).time);
	  ampl_signal += sig_pe_vec.at(j).at(k).ampl*_gr_wf_tmpl->Eval(t - sig_pe_vec.at(j).at(k).time);
	}
      }
    }
    //electric noise
    electricNoise = _rnd->Gaus(_wfConf->ElectronicBaseine,_wfConf->ElectronicNoiseSigm);
    ampl += electricNoise;
    ampl_p_signal += electricNoise;
    ampl_signal += electricNoise;
    gr_wf->SetPoint(gr_wf->GetN(),t,ampl);
    gr_wf_sig->SetPoint(gr_wf_sig->GetN(),t,ampl_p_signal);
    gr_wf_sig_only->SetPoint(gr_wf_sig_only->GetN(),t,ampl_signal);
  }
}

void wfSim::testGenerateDistFromHist(TH1D *h1in, TH1D *h1out){
  Int_t nn = 1000000;
  for(Int_t i = 0;i<nn;i++){
    h1out->Fill(generateDistFromHist(h1in));
  }
  Double_t norm = h1out->GetMaximum();
  for(int i = 1;i<=h1out->GetNbinsX();i++)
    h1out->SetBinContent(i,h1out->GetBinContent(i)/norm);
}

double wfSim::generateDistFromHist(TH1D *h1){
  int nn = h1->GetNbinsX()+1;
  int binI;
  double val;
  bool go = false;
  double binL;
  double binR;
  double t_min = h1->GetBinLowEdge(1);
  double t_max = h1->GetBinLowEdge(nn);
  while( go == false ){
    binI = (int)_rnd->Uniform(1,nn);
    //binI = h1->FindBin(_rnd->Uniform(t_min,t_max));
    val  = h1->GetBinContent(binI);
    if(val>=_rnd->Uniform()){
      binL = h1->GetBinLowEdge(binI);
      binR = binL + h1->GetBinWidth(binI);
      go = true;
      return _rnd->Uniform(binL,binR);
    }
  }
  return -999.0;
}

void wfSim::gen_WF( TGraph *gr_wf, TGraph *gr_wf_sig, TGraph *gr_wf_sig_only, unsigned int n_signals){
  //
  Double_t dT = (_wfConf->SimEndTime - _wfConf->SimStartTime);
  Double_t dT_s = dT*1.0e-9; 
  //unsigned int n_noise = (Int_t)(TMath::Floor(dT_s*_wfConf->DCRrate)+1);
  //Int_t n = (Int_t)(TMath::Floor(dT/_wfConf->SamplingTime)+1);
  unsigned int n_noise = (Int_t)(TMath::Floor(dT_s*_wfConf->DCRrate));
  Int_t n = (Int_t)(TMath::Floor(dT/_wfConf->SamplingTime));
  //
  Double_t first_pe_time;
  Double_t first_pe_ampl;
  Int_t parentID;
  Int_t typeID;
  Int_t parentGenerationID;
  Double_t probabilityCorrectionFactor = 1.0;
  //noise
  std::vector<std::vector<photoElectronInfo>> all_pe_vec;
  for(unsigned int i = 0; i<n_noise; i++){
    std::vector<photoElectronInfo> pe_vec;
    first_pe_time = _rnd->Uniform(_wfConf->SimEndTime,_wfConf->SimStartTime);
    first_pe_ampl = _rnd->Gaus(_wfConf->single_p_e_ampl,_wfConf->SigmaGain);
    parentGenerationID = -1;
    parentID = -1;
    typeID = 0;
    simPE(pe_vec, first_pe_time, first_pe_ampl, typeID, parentGenerationID, parentID, probabilityCorrectionFactor);
    all_pe_vec.push_back(pe_vec);
  }
  //print_pe_vec(all_pe_vec);
  //signal
  std::vector<std::vector<photoElectronInfo>> sig_pe_vec;
  for(unsigned int i = 0; i<n_signals; i++){
    std::vector<photoElectronInfo> pe_vec;
    first_pe_time = _wfConf->signal_t0;
    first_pe_ampl = _rnd->Gaus(_wfConf->single_p_e_ampl,_wfConf->SigmaGain);
    parentGenerationID = -1;
    parentID = -1;
    typeID = 0;
    simPE(pe_vec, first_pe_time, first_pe_ampl, typeID, parentGenerationID, parentID, probabilityCorrectionFactor);
    sig_pe_vec.push_back(pe_vec);
  }
  //
  //print_pe_vec(sig_pe_vec);
  //  
  Double_t t;
  Double_t ampl;
  Double_t ampl_p_signal;
  Double_t ampl_signal;
  Double_t electricNoise;
  for(Int_t i = 0;i<n;i++){
    //if(i%10000==0)
    //std::cout<<i<<std::endl;
    t = _wfConf->SimStartTime + _wfConf->SamplingTime*i;
    ampl = 0.0;
    ampl_p_signal = 0.0;
    ampl_signal = 0.0;
    //p.e. noise
    for(unsigned int j = 0;j<all_pe_vec.size();j++){
      for(unsigned int k = 0;k<all_pe_vec.at(j).size();k++){
	if((t - all_pe_vec.at(j).at(k).time )>=0 && (t - all_pe_vec.at(j).at(k).time)<1000){
    	  ampl += all_pe_vec.at(j).at(k).ampl*_gr_wf_tmpl->Eval(t - all_pe_vec.at(j).at(k).time);
	}
      }
    }
    ampl_p_signal = ampl;
    //signal
    for(unsigned int j = 0;j<sig_pe_vec.size();j++){
      for(unsigned int k = 0;k<sig_pe_vec.at(j).size();k++){
	if((t - sig_pe_vec.at(j).at(k).time )>=0 && (t - sig_pe_vec.at(j).at(k).time)<1000){   
	  ampl_p_signal += sig_pe_vec.at(j).at(k).ampl*_gr_wf_tmpl->Eval(t - sig_pe_vec.at(j).at(k).time);
	  ampl_signal += sig_pe_vec.at(j).at(k).ampl*_gr_wf_tmpl->Eval(t - sig_pe_vec.at(j).at(k).time);
	}
      }
    }
    //electric noise
    electricNoise = _rnd->Gaus(_wfConf->ElectronicBaseine,_wfConf->ElectronicNoiseSigm);
    ampl += electricNoise;
    ampl_p_signal += electricNoise;
    ampl_signal += electricNoise;
    gr_wf->SetPoint(gr_wf->GetN(),t,ampl);
    gr_wf_sig->SetPoint(gr_wf_sig->GetN(),t,ampl_p_signal);
    gr_wf_sig_only->SetPoint(gr_wf_sig_only->GetN(),t,ampl_signal);
  }  
}

void wfSim::save_to_csv( TGraph *gr_wf, TGraph *gr_wf_sig, TGraph *gr_wf_sig_only){
  Double_t t;
  Double_t v_wf;
  Double_t v_wf_all;
  Double_t v_wf_sig;
  TString csv_file_out=gr_wf->GetTitle();
  csv_file_out += ".csv";
  std::ofstream csvfile;
  csvfile.open (csv_file_out.Data());
  csvfile<<"time_ns,v_bkg,v_tot,v_sig"<<std::endl;
  for(Int_t i = 0;i<gr_wf->GetN();i++){
    gr_wf->GetPoint(i,t,v_wf);
    gr_wf_sig->GetPoint(i,t,v_wf_all);
    gr_wf_sig_only->GetPoint(i,t,v_wf_sig);
    csvfile<<t
	   <<","<<v_wf
	   <<","<<v_wf_all
	   <<","<<v_wf_sig<<std::endl;
  }
  csvfile.close();
}
