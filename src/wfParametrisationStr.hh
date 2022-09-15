#ifndef wfParametrisationStr_hh
#define wfParametrisationStr_hh

//root
#include "TROOT.h"

//C, C++
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

struct wfLEEStr {
  TString parameterizationType;
  TString parameterizationName;
  TString  outputTemplate;
  Double_t dt_ns;
  Double_t time_window_in_ns;
  Double_t a0;
  Double_t t0;
  Double_t one_over_line_k;
  Double_t tau_decay_fast;
  Double_t tau_decay_slow;
  Double_t n_tau_slow;
  Double_t sigma_to_conv;
  Double_t ampl;
  Int_t amplitudeChargeNorm; 
  wfLEEStr(){
    parameterizationType = "";
    parameterizationName = "";
    outputTemplate = "";
    dt_ns = -999.0;
    time_window_in_ns = -999.0;
    a0 = -999.0;
    t0 = -999.0;
    one_over_line_k = -999.0;
    tau_decay_fast = -999.0;
    tau_decay_slow = -999.0;
    n_tau_slow = -999.0;
    sigma_to_conv = -999.0;
    ampl = -999.0;
    amplitudeChargeNorm = -999;
  }
  void printInfo(){
    std::cout<<"parameterizationType "<<parameterizationType<<std::endl
	     <<"parameterizationName "<<parameterizationName<<std::endl
	     <<"outputTemplate       "<<outputTemplate<<std::endl
	     <<"dt_ns                "<<dt_ns<<std::endl
	     <<"time_window_in_ns    "<<time_window_in_ns<<std::endl
	     <<"a0                   "<<a0<<std::endl
	     <<"t0                   "<<t0<<std::endl
	     <<"one_over_line_k      "<<one_over_line_k<<std::endl
	     <<"tau_decay_fast       "<<tau_decay_fast<<std::endl
	     <<"tau_decay_slow       "<<tau_decay_slow<<std::endl
	     <<"n_tau_slow           "<<n_tau_slow<<std::endl
	     <<"sigma_to_conv        "<<sigma_to_conv<<std::endl
    	     <<"ampl                 "<<ampl<<std::endl
	     <<"amplitudeChargeNorm  "<<amplitudeChargeNorm<<std::endl;
  }  
  void readFromFile(TString name){
    std::ifstream confFile(name.Data());
    if (confFile.is_open()) {
      std::string mot;
      while(confFile>>mot){
	if(mot == "parameterizationType:")
	  confFile>>parameterizationType;
	if(mot == "parameterizationName:")
	  confFile>>parameterizationName;
	if(mot == "outputTemplate:")
	  confFile>>outputTemplate;
	if(mot == "dt_ns:")
	  confFile>>dt_ns;
	if(mot == "time_window_in_ns:")
	  confFile>>time_window_in_ns;
	if(mot == "a0:")
	  confFile>>a0;
	if(mot == "t0:")
	  confFile>>t0;
	if(mot == "one_over_line_k:")
	  confFile>>one_over_line_k;
	if(mot == "tau_decay_fast:")
	  confFile>>tau_decay_fast;
	if(mot == "tau_decay_slow:")
	  confFile>>tau_decay_slow;
	if(mot == "n_tau_slow:")
	  confFile>>n_tau_slow;
	if(mot == "sigma_to_conv:")
	  confFile>>sigma_to_conv;
	if(mot == "ampl:")
	  confFile>>ampl;
	if(mot == "amplitudeChargeNorm:")
	  confFile>>amplitudeChargeNorm;
      }
      confFile.close();
    }
    else {
      std::cout << "Unable to open file"<<std::endl; 
      assert(0);
    }
  }
  //
  void writeToFile(TString name){
    std::ofstream outConfFile;
    outConfFile.open(name.Data());
    outConfFile<<"parameterizationType: "<<parameterizationType<<std::endl
	       <<"parameterizationName: "<<parameterizationName<<std::endl
	       <<"outputTemplate:       "<<outputTemplate<<std::endl
	       <<"dt_ns:                "<<dt_ns<<std::endl
	       <<"time_window_in_ns:    "<<time_window_in_ns<<std::endl
	       <<"a0:                   "<<a0<<std::endl
	       <<"t0:                   "<<t0<<std::endl
	       <<"one_over_line_k:      "<<one_over_line_k<<std::endl
	       <<"tau_decay_fast:       "<<tau_decay_fast<<std::endl
	       <<"tau_decay_slow:       "<<tau_decay_slow<<std::endl
	       <<"n_tau_slow:           "<<n_tau_slow<<std::endl
	       <<"sigma_to_conv:        "<<sigma_to_conv<<std::endl
    	       <<"ampl:                 "<<ampl<<std::endl
	       <<"amplitudeChargeNorm:  "<<amplitudeChargeNorm<<std::endl;
    outConfFile.close();
  }
};

#endif
