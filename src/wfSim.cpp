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

//void wfSim::gen_WF(TGraph *gr_wf, TH1D *h1_time_dist, Int_t n_signals, wfSimConfStr *wfConf){
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
  print_pe_vec(sig_pe_vec);
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
