#ifndef wfSimConfStr_hh
#define wfSimConfStr_hh

//root
#include "TROOT.h"

//C, C++
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

struct wfSimConfStr {
  TString DetectorName;
  TString Template;
  TString amplDistFile;
  Int_t Ncells;
  Double_t Vbd_V;
  Double_t Vbias;
  Double_t AmpliSlope;
  Double_t Gfadc;
  Double_t Cucell_fF;
  Double_t Rbias_K;
  Double_t Rquenching_K;
  Double_t ElectronicBaseine;
  Double_t ElectronicNoiseSigm;
  Double_t ElectNoiseFreq;
  Double_t ElectNoiseAmpli;
  Double_t single_p_e_ampl;
  Double_t SigmaGain;
  Double_t SensorArea;
  Double_t signal_t0;
  Double_t signalUniDistWindow;
  Double_t SimStartTime;
  Double_t SimEndTime;
  Double_t SamplingTime;
  Double_t DCRrate;
  Double_t PxtProb;
  Double_t PapProb;
  Double_t PapTau;
  wfSimConfStr(){
    DetectorName = "";
    Template = "";
    amplDistFile = "NONE";
    Ncells = -999;
    Vbd_V = -999.0;
    Vbias = -999.0;
    AmpliSlope = -999.0;
    Gfadc = -999.0;
    Cucell_fF = -999.0;
    Rbias_K = -999.0;
    Rquenching_K = -999.0;
    ElectronicBaseine = -999.0;
    ElectronicNoiseSigm = -999.0;
    ElectNoiseFreq = -999.0;
    ElectNoiseAmpli = -999.0;
    single_p_e_ampl = -999.0;
    SigmaGain = -999.0;
    SensorArea = -999.0;
    signal_t0 = -999.0;
    signalUniDistWindow = -999.0;
    SimStartTime = -999.0;
    SimEndTime = -999.0;
    SamplingTime = -999.0;
    DCRrate = -999.0;
    PxtProb = -999.0;
    PapProb = -999.0;
    PapTau = -999.0;
  }
  void printInfo(){
    std::cout<<"DetectorName        "<<DetectorName<<std::endl
	     <<"Template            "<<Template<<std::endl
      	     <<"amplDistFile        "<<amplDistFile<<std::endl
	     <<"Ncells              "<<Ncells<<std::endl
	     <<"Vbd_V               "<<Vbd_V<<std::endl
	     <<"Vbias               "<<Vbias<<std::endl
	     <<"AmpliSlope          "<<AmpliSlope<<std::endl
	     <<"Gfadc               "<<Gfadc<<std::endl
	     <<"Cucell_fF           "<<Cucell_fF<<std::endl
	     <<"Rbias_K             "<<Rbias_K<<std::endl
	     <<"Rquenching_K        "<<Rquenching_K<<std::endl
	     <<"ElectronicBaseine   "<<ElectronicBaseine<<std::endl
	     <<"ElectronicNoiseSigm "<<ElectronicNoiseSigm<<std::endl
	     <<"ElectNoiseFreq      "<<ElectNoiseFreq<<std::endl
	     <<"ElectNoiseAmpli     "<<ElectNoiseAmpli<<std::endl
      	     <<"single_p_e_ampl     "<<single_p_e_ampl<<std::endl
	     <<"SigmaGain           "<<SigmaGain<<std::endl
	     <<"SensorArea          "<<SensorArea<<std::endl
	     <<"signal_t0           "<<signal_t0<<std::endl
	     <<"signalUniDistWindow "<<signalUniDistWindow<<std::endl
	     <<"SimStartTime        "<<SimStartTime<<std::endl
	     <<"SimEndTime          "<<SimEndTime<<std::endl
	     <<"SamplingTime        "<<SamplingTime<<std::endl
	     <<"DCRrate             "<<DCRrate<<std::endl
	     <<"PxtProb             "<<PxtProb<<std::endl
	     <<"PapProb             "<<PapProb<<std::endl
      	     <<"PapTau              "<<PapTau<<std::endl;
  }  
  void readFromFile(TString name){
    std::ifstream confFile(name.Data());
    if (confFile.is_open()) {
      std::string mot;
      while(confFile>>mot){
	if(mot == "DetectorName:")
	  confFile>>DetectorName;
	if(mot == "Template:")
	  confFile>>Template;
	if(mot == "amplDistFile:")
	  confFile>>amplDistFile;
	if(mot == "Ncells:")
	  confFile>>Ncells;
	if(mot == "Vbd_V:")
	  confFile>>Vbd_V;
	if(mot == "Vbias:")
	  confFile>>Vbias;
	if(mot == "AmpliSlope:")
	  confFile>>AmpliSlope;
	if(mot == "Gfadc:")
	  confFile>>Gfadc;
	if(mot == "Cucell_fF:")
	  confFile>>Cucell_fF;
	if(mot == "Rbias_K:")
	  confFile>>Rbias_K;
	if(mot == "Rquenching_K:")
	  confFile>>Rquenching_K;
	if(mot == "ElectronicBaseine:")
	  confFile>>ElectronicBaseine;
	if(mot == "ElectronicNoiseSigm:")
	  confFile>>ElectronicNoiseSigm;
	if(mot == "ElectNoiseFreq:")
	  confFile>>ElectNoiseFreq;
	if(mot == "single_p_e_ampl:")
	  confFile>>single_p_e_ampl;
	if(mot == "SigmaGain:")
	  confFile>>SigmaGain;
	if(mot == "SensorArea:")
	  confFile>>SensorArea;
	if(mot == "signal_t0:")
	  confFile>>signal_t0;
	if(mot == "signalUniDistWindow:")
	  confFile>>signalUniDistWindow;
	if(mot == "SimStartTime:")
	  confFile>>SimStartTime;
	if(mot == "SimEndTime:")
	  confFile>>SimEndTime;
	if(mot == "SamplingTime:")
	  confFile>>SamplingTime;
	if(mot == "DCRrate:")
	  confFile>>DCRrate;
	if(mot == "PxtProb:")
	  confFile>>PxtProb;
	if(mot == "PapProb:")
	  confFile>>PapProb;
	if(mot == "PapTau:")
	  confFile>>PapTau;
      }
      confFile.close();
    }
    else {
      std::cout << "Unable to open file"<<std::endl; 
      assert(0);
    }
  }
};

#endif
