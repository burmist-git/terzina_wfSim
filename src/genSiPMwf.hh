#ifndef genSiPMwf_hh
#define genSiPMwf_hh

//C, C++
#include <iostream>
#include <vector>
#include "wfParametrisationStr.hh"

//root
#include "TROOT.h"

//my
struct wfLEEStr;

//root
class TString;
class TRandom3;
class TGraph;
class TH1D;
class TGraphErrors;

class genSiPMwf {

public :
  // parameterizationType == LEE (Linear Exponent Exponent)
  genSiPMwf();
  genSiPMwf(TString parameterizationConf);
  ~genSiPMwf();
  
private:

  void save_to_csv( TGraph *gr_wf,TString csv_file_out, bool csv_format);
  void generate_gaus_gaus_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t time_window_in_ns, Double_t a0, Double_t t0_gauss, Double_t gauss01_start,
				   Double_t sigma_gauss_raise, Double_t sigma_gauss_fall, Double_t tau_decay01, Double_t n_tau, Double_t a_decay01_start);
  void generate_line_exponent_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t a0, Double_t t0, Double_t line_k, Double_t tau_decay_fast, Double_t tau_decay_slow, Double_t n_tau_slow);
  void convolut_with_gaus(TGraph *gr_wf_conv, Double_t sigma_to_conv);
  void generate_gaus( TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t A0, Double_t t0, Double_t sigma);
  void LEE_function(TGraph *gr, wfLEEStr *paramStr);
  //
  void get_hist_from_graph( TGraph *gr, TH1D *h1, Double_t &tmin, Double_t &tmax);
  //
  TGraph *_gr_wf;
  wfLEEStr *_paramStr;
  TString _parameterizationConf;

public :
  
  const inline TGraph *getTemplate() {return _gr_wf;}
  const inline wfLEEStr *get_lee_Param_str() {return _paramStr;}
  //
  static void load_data( TString fileName, TGraphErrors *gr, Int_t error_calculation, Int_t data_division);
  static void get_gr_dt_tmin_tmax( TGraph *gr, Double_t &dt, Double_t &tmin, Double_t &tmax);
  static void get_gr_charge_and_baseline( TGraph *gr, Double_t t_baseline_window_L, Double_t t_baseline_window_R, Double_t &charge, Double_t &baseline);
  bool fit_LEE( TGraphErrors *gr, Bool_t fixAll);
  //
  void get_FFT_spectrum( TGraph *gr, TGraph *gr_fft_re, TGraph *gr_fft_im, TGraph *gr_fft_abs);
  void simulateFrequencyCutOffpreamp( TGraph *gr, Double_t cut_off_frequency);
  
};

#endif
