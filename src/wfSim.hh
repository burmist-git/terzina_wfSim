#ifndef wfSim_hh
#define wfSim_hh

//C, C++
#include <iostream>
#include <vector>

//root
#include "TROOT.h"

//my
struct wfSimConfStr;

//root
class TString;
class TRandom3;
class TGraph;
class TH1D;

struct photoElectronInfo {
  Int_t photoelectronID;            // photo electron ID in the avalanche, first p.e. has ID = 0.
  Int_t photoelectronParentID;      // photo electron parent ID in the avalanche, first p.e. has parent ID = -1.
  Int_t photoelectronLocalID;       // photo electron local ID within its generation.
  Int_t photoelectronParentLocalID; // photo electron parent local ID within its generation.
  Int_t parentGeneration;
  Int_t generation;
  Int_t typeID;
  // 0 - initial photo electron
  // 1 - crosstalk induced signal
  // 2 - afterpulse induced signal
  Double_t time;
  Double_t ampl;
  photoElectronInfo(){
    //
    photoelectronID = -999;
    photoelectronParentID = -999;
    photoelectronLocalID = -999;
    photoelectronParentLocalID = -999;
    parentGeneration = -999;
    //
    generation = -999;
    typeID = -999;
    time = -999.0;
    ampl = -999.0;
    //
  }
  void printInfo () const {
    std::cout<<"generation                 "<<generation<<std::endl
	     <<"parentGeneration           "<<parentGeneration<<std::endl
	     <<"photoelectronID            "<<photoelectronID<<std::endl
	     <<"photoelectronParentID      "<<photoelectronParentID<<std::endl
	     <<"photoelectronLocalID       "<<photoelectronLocalID<<std::endl
	     <<"photoelectronParentLocalID "<<photoelectronParentLocalID<<std::endl
	     <<"typeID                     "<<typeID<<std::endl
	     <<"time                       "<<time<<std::endl
	     <<"ampl                       "<<ampl<<std::endl;
  }
};

class wfSim {

public :
  wfSim( TRandom3 *rnd, wfSimConfStr *wfConf);
  ~wfSim();

public :
  void getWF_tmpl(TString name);
  void gen_WF( TGraph *gr_wf, TGraph *gr_wf_sig, TGraph *gr_wf_sig_only, unsigned int n_signals);
  void gen_WF( TGraph *gr_wf, TGraph *gr_wf_sig, TGraph *gr_wf_sig_only, unsigned int n_signals, TH1D *h1_photon_time);
  void simPE(auto (&pe_vec), Double_t t, Double_t a, Int_t typeID, Int_t parentGenerationID, Int_t parentID, Double_t probabilityCorrectionFactor);
  const inline TGraph *getTemplate() {return _gr_wf_tmpl;}
  void save_to_csv( TGraph *gr_wf, TGraph *gr_wf_sig, TGraph *gr_wf_sig_only);
  static void get_Ampl_hist(TGraph *wf, TH1D *h1);
  static void get_AmplLocalMax_hist(TGraph *wf, TGraph *gr_max, TH1D *h1);
  static void get_count_threshold_vs_rate( TH1D *h1, TGraph *gr, Double_t totalTime_in_s);
  void testGenerateDistFromHist(TH1D *h1in,TH1D *h1out);
  
private:
  void crosstalk_sim(auto (&pe_vec), Double_t t, Double_t a, Int_t typeID, Int_t parentGenerationID, Int_t parentID, Double_t probabilityCorrectionFactor);
  void afterpulse_sim(auto (&pe_vec), Double_t t, Double_t a, Int_t typeID, Int_t parentGenerationID, Int_t parentID, Double_t probabilityCorrectionFactor);
  void print_pe_vec(const auto (&all_pe_vec));
  double generateDistFromHist(TH1D *h1);
  
  TRandom3 *_rnd;
  TGraph *_gr_wf_tmpl;
  Double_t _t_max_ampl_wf_tmpl;
  wfSimConfStr *_wfConf;

};

#endif
