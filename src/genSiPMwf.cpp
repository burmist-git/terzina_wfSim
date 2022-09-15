//my
#include "genSiPMwf.hh"
#include "wfParametrisationStr.hh"

//root
#include "TRandom3.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TFile.h"
#include "TVirtualFFT.h"

//C, C++
#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include <iomanip>

Double_t lee(Double_t *x, Double_t *par);

genSiPMwf::genSiPMwf(){
  _gr_wf = new TGraph();
  _paramStr = new wfLEEStr();
  _parameterizationConf = "";
}

genSiPMwf::genSiPMwf(TString parameterizationConf){
  //
  _parameterizationConf = parameterizationConf;
  //
  _paramStr = new wfLEEStr();
  _paramStr->readFromFile(parameterizationConf);
  _paramStr->printInfo();  
  _gr_wf = new TGraph();
  _gr_wf->SetTitle(_paramStr->parameterizationName);
  _gr_wf->SetName(_paramStr->parameterizationName);
  //
  if(_paramStr->parameterizationType=="LEE"){
    LEE_function( _gr_wf, _paramStr);
    save_to_csv( _gr_wf, _paramStr->outputTemplate, false);
  }
  //std::cout<<"Integral : "<<_gr_wf->Integral(0,_gr_wf->GetN())<<std::endl;
  //_gr_wf->SaveAs("_gr_wf.C");
  //_paramStr->writeToFile("test.txt");

  //
  //if(_paramStr->parameterizationType=="LEE"){
  //  generate_line_exponent_exponent(_gr_wf, _paramStr->dt_ns, _paramStr->time_window_in_ns, _paramStr->a0, _paramStr->t0,
  //				    1.0/_paramStr->one_over_line_k, _paramStr->tau_decay_fast , _paramStr->tau_decay_slow, _paramStr->n_tau_slow);
  //  if(_paramStr->sigma_to_conv>0.0)
  //    convolut_with_gaus(_gr_wf,_paramStr->sigma_to_conv);
  //
  //  save_to_csv(_gr_wf, _paramStr->outputTemplate, false);
  //  _gr_wf->SaveAs("_gr_wf.C");
  //  _paramStr->writeToFile("test.txt");
  //}
}

genSiPMwf::~genSiPMwf(){

}

// error_calculation = 0 : xerr = 0.05 ns; yerr = 0.010
// error_calculation = 1 : xerr = 0.00 ns; yerr = 0.000
// error_calculation = 2 : xerr = 0.05 ns; yerr = 0.1*y 
void genSiPMwf::load_data( TString fileName, TGraphErrors *gr, Int_t error_calculation = 0, Int_t data_division=5){
  std::ifstream fileIn(fileName.Data());
  Double_t x;
  Double_t y;
  Int_t i = 0;
  Int_t k = 0;
  TString mot;
  if(fileIn.is_open()){
    //fileIn>>mot;
    while(fileIn>>x>>y){
      if(k%data_division==0){
        i=gr->GetN();
        gr->SetPoint(i,x,y);
	if(error_calculation == 0){
	  if(y<0.3)
	    gr->SetPointError(i,0.05,0.01);
	  else
	    gr->SetPointError(i,0.05,0.1);
	}
	else if(error_calculation == 1){
	  gr->SetPointError(i,0.0,0.0);
	}
	else if(error_calculation == 2){
	  gr->SetPointError(i,0.05,0.1*y);
	}
      }
      k++;
    }
    fileIn.close();
  }
  else
    std::cout<<"Unable to open file"<<std::endl;
}

void genSiPMwf::LEE_function(TGraph *gr, wfLEEStr *paramStr){
  Int_t npar = 9;
  Double_t time_window_start = 0.0;
  Double_t time_window_in_ns = paramStr->time_window_in_ns;
  TF1 *f_lee = new TF1("lee", lee, time_window_start, time_window_in_ns, npar);
  //
  f_lee->SetParName(0, "ampl");
  f_lee->SetParName(1, "line_k");
  f_lee->SetParName(2, "tau_decay_fast");
  f_lee->SetParName(3, "tau_decay_slow");
  f_lee->SetParName(4, "sigma_to_conv");
  //
  f_lee->SetParName(5, "t0");
  f_lee->SetParName(6, "a0");
  f_lee->SetParName(7, "n_tau_slow");
  f_lee->SetParName(8, "time_window_in_ns");
  //
  f_lee->FixParameter(0, paramStr->ampl);
  f_lee->FixParameter(1, 1.0/paramStr->one_over_line_k);
  f_lee->FixParameter(2, paramStr->tau_decay_fast);
  f_lee->FixParameter(3, paramStr->tau_decay_slow);
  f_lee->FixParameter(4, paramStr->sigma_to_conv);
  f_lee->FixParameter(5, paramStr->t0);
  f_lee->FixParameter(6, paramStr->a0);
  f_lee->FixParameter(7, paramStr->n_tau_slow);
  f_lee->FixParameter(8, paramStr->time_window_in_ns);
  //
  Int_t nPoint = (Int_t)time_window_in_ns/paramStr->dt_ns + 1;
  Double_t t;
  Double_t a;
  Double_t amax = 0.0;
  Double_t charge = 0.0;
  for(Int_t i = 0;i<nPoint;i++){
    t = paramStr->dt_ns*i;
    a = f_lee->Eval(t);
    charge += a*paramStr->dt_ns;
    gr->SetPoint(i,t,a);
    if(a>amax)
      amax = a;
  }
  if(paramStr->amplitudeChargeNorm == 1){
    for(Int_t i = 0;i<nPoint;i++){
      gr->GetPoint(i,t,a);
      if(amax != 0.0)
	gr->SetPoint(i,t,a/amax);
      else
	assert(0);
    }
  }
  if(paramStr->amplitudeChargeNorm == 2){
    for(Int_t i = 0;i<nPoint;i++){
      gr->GetPoint(i,t,a);
      if(charge != 0.0)
	gr->SetPoint(i,t,a/charge);
      else
	assert(0);
    }
  }
}

void genSiPMwf::get_FFT_spectrum( TGraph *gr, TGraph *gr_fft_re, TGraph *gr_fft_im, TGraph *gr_fft_abs){
  //
  TH1D *h1 = new TH1D();
  h1->SetTitle("h1");
  h1->SetName("h1");
  Double_t tmin;
  Double_t tmax;
  get_hist_from_graph(gr, h1, tmin, tmax);
  //
  // FFT
  TVirtualFFT::SetTransform(0);
  TH1 *h1_fft = 0;
  h1_fft = h1->FFT(h1_fft, "PH");
  TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
  Int_t n = h1->GetNbinsX();
  Double_t *fft_re = new Double_t[n];
  Double_t *fft_im = new Double_t[n];
  Double_t re;
  Double_t im;
  Double_t abs;
  Double_t frequency;
  fft->GetPointsComplex(fft_re,fft_im);
  for(Int_t i = 0;i<n;i++){
    frequency = i/(tmax - tmin)/1.0e+6/1.0e-9;
    abs = TMath::Sqrt(fft_re[i]*fft_re[i] + fft_im[i]*fft_im[i]);
    re = fft_re[i];
    im = fft_im[i];
    gr_fft_re->SetPoint(i,frequency,re);
    gr_fft_im->SetPoint(i,frequency,im);
    gr_fft_abs->SetPoint(i,frequency,abs);
  }
  delete h1;
  delete h1_fft;
  delete fft;
}

void genSiPMwf::get_hist_from_graph( TGraph *gr, TH1D *h1, Double_t &tmin, Double_t &tmax){
  Double_t t;
  Double_t a;
  Double_t a_tmin;
  Double_t a_tmax;
  Int_t n = gr->GetN();
  Double_t binW;
  gr->GetPoint(1,binW,a);
  gr->GetPoint(0,t,a);
  binW = binW - t;
  //
  gr->GetPoint(0,tmin,a_tmin);
  gr->GetPoint(n-1,tmax,a_tmax);
  //  
  h1->SetBins(n,tmin-binW/2.0,tmax+binW/2.0);
  for(Int_t i = 0;i<n;i++){
    gr->GetPoint(i,t,a);
    h1->SetBinContent(i+1,a);
  }
  //std::cout<<"gr->GetN()      "<<gr->GetN()<<std::endl
  //	   <<"h1->GetNbinsX() "<<h1->GetNbinsX()<<std::endl
  //	   <<"h1->GetNcells() "<<h1->GetNcells()<<std::endl;
  //std::cout<<"tmin      "<<tmin<<std::endl
  //	   <<"tmax      "<<tmax<<std::endl;
}

bool genSiPMwf::fit_LEE( TGraphErrors *gr, Bool_t fixAll = false){
  wfLEEStr *initialPar = new wfLEEStr();
  gr->SetName(initialPar->parameterizationName);
  gr->SetTitle(initialPar->parameterizationName);
  //
  wfLEEStr *param_initial_Str = _paramStr;
  Int_t npar = 9;
  Double_t time_window_start = 0.0;
  Double_t time_window_in_ns = param_initial_Str->time_window_in_ns;
  TF1 *f_lee = new TF1("lee", lee, time_window_start, time_window_in_ns, npar);
  //
  f_lee->SetParName(0, "ampl");
  f_lee->SetParName(1, "line_k");
  f_lee->SetParName(2, "tau_decay_fast");
  f_lee->SetParName(3, "tau_decay_slow");
  f_lee->SetParName(4, "sigma_to_conv");
  //
  f_lee->SetParName(5, "t0");
  f_lee->SetParName(6, "a0");
  f_lee->SetParName(7, "n_tau_slow");
  f_lee->SetParName(8, "time_window_in_ns");
  //
  f_lee->SetParameter(0, param_initial_Str->ampl);
  f_lee->SetParameter(1, 1.0/param_initial_Str->one_over_line_k);
  f_lee->SetParameter(2, param_initial_Str->tau_decay_fast);
  f_lee->SetParameter(3, param_initial_Str->tau_decay_slow);
  f_lee->SetParameter(4, param_initial_Str->sigma_to_conv);
  //
  f_lee->SetParameter(5, param_initial_Str->t0);
  f_lee->SetParameter(6, param_initial_Str->a0);
  f_lee->SetParameter(7, param_initial_Str->n_tau_slow);
  f_lee->SetParameter(8, param_initial_Str->time_window_in_ns);
  //
  //f_lee->FixParameter(0, param_initial_Str->ampl);
  //f_lee->FixParameter(1, 1.0/param_initial_Str->one_over_line_k);
  //f_lee->FixParameter(2, param_initial_Str->tau_decay_fast);
  //f_lee->FixParameter(3, param_initial_Str->tau_decay_slow);
  //f_lee->FixParameter(4, param_initial_Str->sigma_to_conv);
  //f_lee->FixParameter(5, param_initial_Str->t0);
  //
  f_lee->FixParameter(6, param_initial_Str->a0);
  f_lee->FixParameter(7, param_initial_Str->n_tau_slow);
  f_lee->FixParameter(8, param_initial_Str->time_window_in_ns);
  //
  if(fixAll){
    f_lee->FixParameter(0, param_initial_Str->ampl);
    f_lee->FixParameter(1, 1.0/param_initial_Str->one_over_line_k);
    f_lee->FixParameter(2, param_initial_Str->tau_decay_fast);
    f_lee->FixParameter(3, param_initial_Str->tau_decay_slow);
    f_lee->FixParameter(4, param_initial_Str->sigma_to_conv);
    f_lee->FixParameter(5, param_initial_Str->t0);
    f_lee->FixParameter(6, param_initial_Str->a0);
    f_lee->FixParameter(7, param_initial_Str->n_tau_slow);
    f_lee->FixParameter(8, param_initial_Str->time_window_in_ns);
  }
  if(!fixAll){
    gr->Fit(f_lee, "",  "", time_window_start, time_window_in_ns);
    param_initial_Str->t0 = f_lee->GetParameter(5);
    param_initial_Str->sigma_to_conv = f_lee->GetParameter(4);
    param_initial_Str->tau_decay_fast = f_lee->GetParameter(2);
    param_initial_Str->tau_decay_slow = f_lee->GetParameter(3);
    param_initial_Str->one_over_line_k = 1.0/f_lee->GetParameter(1);
    param_initial_Str->ampl = f_lee->GetParameter(0);
    param_initial_Str->writeToFile(_parameterizationConf);
  }
  return true;

}

void genSiPMwf::save_to_csv( TGraph *gr_wf, TString csv_file_out, bool csv_format = false){
  Double_t t;
  Double_t v_wf;
  std::ofstream csvfile;
  csvfile.open (csv_file_out.Data());
  if(csv_format){
    csvfile<<"time_ns,v_sig"<<std::endl;
    for(Int_t i = 0;i<gr_wf->GetN();i++){
      gr_wf->GetPoint(i,t,v_wf);
      csvfile<<t<<","<<v_wf<<std::endl;
    }
  }
  else{
    for(Int_t i = 0;i<gr_wf->GetN();i++){
      gr_wf->GetPoint(i,t,v_wf);
      csvfile<<t<<" "<<v_wf<<std::endl;
    }
  }
  csvfile.close();
}

void genSiPMwf::generate_line_exponent_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t time_window_in_ns, Double_t a0, Double_t t0,
						Double_t line_k, Double_t tau_decay_fast, Double_t tau_decay_slow, Double_t n_tau_slow){
  //
  Int_t nPoint = (Int_t)time_window_in_ns/dt_ns + 1;
  //
  Double_t b = 1.0 - line_k*t0;
  Double_t x0 = -b/line_k;
  //
  Double_t t;
  Double_t a;
  for(Int_t i = 0;i<nPoint;i++){
    t = dt_ns*i;
    a = a0;
    if(t <= x0)
      a = a0;
    if(t>x0 && t<=t0) 
      a = a0 + line_k*t + b;
    if((t>t0) && ((t-t0) <= n_tau_slow * tau_decay_slow)) 
      a = a0 + 0.5*TMath::Exp(-(t-t0)/tau_decay_fast) + 0.5*TMath::Exp(-(t-t0)/tau_decay_slow);
    else if((t-t0) > n_tau_slow * tau_decay_slow)
      a = a0;
    gr_wf->SetPoint(gr_wf->GetN(),t,a);
  }
}

void genSiPMwf::generate_gaus( TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t A0, Double_t t0, Double_t sigma){
  Double_t t;
  Double_t a;
  //
  for(Int_t i = 0;i<nPoint;i++){
    t = dt_ns*i;
    a=A0*TMath::Gaus(t,t0,sigma);
    gr_wf->SetPoint(gr_wf->GetN(),t,a);
  }
}

void genSiPMwf::generate_gaus_gaus_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t a0, Double_t t0_gauss, Double_t gauss01_start,
				 Double_t sigma_gauss_raise, Double_t sigma_gauss_fall, Double_t tau_decay01, Double_t n_tau, Double_t a_decay01_start){
  Double_t t_stitching = TMath::Sqrt(-TMath::Log(a_decay01_start)*2)*sigma_gauss_fall + t0_gauss;
  Double_t t;
  Double_t a;
  //
  for(Int_t i = 0;i<nPoint;i++){
    t = dt_ns*i;
    a = a0;
    if(t >= (t0_gauss - gauss01_start*sigma_gauss_raise) && t<t0_gauss)
      a = a0 + TMath::Gaus(t,t0_gauss,sigma_gauss_raise);
    if(t>=t0_gauss && t<t_stitching) 
      a = a0 + TMath::Gaus(t,t0_gauss,sigma_gauss_fall);
    if((t>=t_stitching) && ((t-t_stitching) < n_tau  * tau_decay01)) 
      a = a0 + a_decay01_start*TMath::Exp(-(t-t_stitching)/tau_decay01);
    else if((t-t_stitching) >= n_tau * tau_decay01)
      a = a0;
    gr_wf->SetPoint(gr_wf->GetN(),t,a);
  }
}

void genSiPMwf::convolut_with_gaus(TGraph *gr_wf_conv, Double_t sigma_to_conv){
  Double_t t;
  Double_t dt = 0.0;
  Double_t t_l = 0.0;
  Double_t t_r = 0.0;
  Double_t t_tmp = 0.0;
  Double_t a;
  Double_t a_max = 0;
  Double_t a_conv;
  Int_t n = gr_wf_conv->GetN();
  //
  TGraph *gr_wf = new TGraph();
  gr_wf->SetName("gr_wf");
  gr_wf->SetTitle("gr_wf");
  for(Int_t i = 0;i<n;i++){
    gr_wf_conv->GetPoint(i,t,a);
    gr_wf->SetPoint(i,t,a);
  }
  //
  gr_wf->GetPoint(0,t_l,a);
  gr_wf->GetPoint(1,t_r,a);
  dt = t_r - t_l;
  for(Int_t i = 0;i<n;i++){
    gr_wf->GetPoint(i,t,a);
    a_conv = 0;
    for(Int_t j = 0;j<n;j++){
      gr_wf->GetPoint(j,t_tmp,a);
      a_conv += a*TMath::Gaus(t_tmp,t,sigma_to_conv)*dt;
    }
    if(a_max<a_conv)
      a_max=a_conv;
    gr_wf_conv->SetPoint(i,t,a_conv);
  }
  if(a_max>0.0){
    for(Int_t i = 0;i<n;i++){
      gr_wf_conv->GetPoint(i,t,a);
      gr_wf_conv->SetPoint(i,t,a/a_max);
    }
  }
}

void genSiPMwf::get_gr_dt_tmin_tmax( TGraph *gr, Double_t &dt, Double_t &tmin, Double_t &tmax){
  Double_t a;
  if(gr->GetN()<2)
    assert(0);
  gr->GetPoint(0,tmin,a);
  gr->GetPoint(gr->GetN()-1,tmax,a);
  gr->GetPoint(1,dt,a);
  dt = dt - tmin;
}

void genSiPMwf::get_gr_charge_and_baseline( TGraph *gr, Double_t t_baseline_window_L, Double_t t_baseline_window_R, Double_t &charge, Double_t &baseline){
  charge = 0.0;
  baseline = 0.0;
  Int_t n_baseline = 0;
  Double_t t,a;
  Double_t dt;
  Double_t tmin;
  Double_t tmax;
  genSiPMwf::get_gr_dt_tmin_tmax( gr, dt, tmin, tmax);
  for(Int_t i = 0;i<gr->GetN();i++){
    gr->GetPoint(i,t,a);
    if(t>=t_baseline_window_L && t<=t_baseline_window_R){
      baseline += a;
      n_baseline++;
    }
    charge += a*dt;
  }
  if(n_baseline != 0)
    baseline = baseline/n_baseline;
  else
    assert(0);
}
 
void genSiPMwf::simulateFrequencyCutOffpreamp( TGraph *gr, Double_t cut_off_frequency){
  //
  Double_t dt;
  Double_t tmin;
  Double_t tmax;
  Double_t t;
  Double_t a;
  Double_t a_re;
  Double_t a_im;
  Double_t freq;
  Double_t areco;
  //
  Double_t charge_initial;
  Double_t baseline_initial;  
  //
  Double_t charge;
  Double_t baseline;
  //
  genSiPMwf::get_gr_charge_and_baseline( gr, 0.0, 100.0, charge_initial, baseline_initial);
  //
  TGraph *gr_fft_re = new TGraph(); gr_fft_re->SetNameTitle("gr_fft_re","gr_fft_re");
  TGraph *gr_fft_im = new TGraph(); gr_fft_im->SetNameTitle("gr_fft_im","gr_fft_im");
  TGraph *gr_fft_abs = new TGraph(); gr_fft_abs->SetNameTitle("gr_fft_abs","gr_fft_abs");
  get_FFT_spectrum( gr, gr_fft_re, gr_fft_im, gr_fft_abs);
  get_gr_dt_tmin_tmax( gr, dt, tmin, tmax);
  //
  //
  Int_t N = gr_fft_re->GetN()/2;
  Int_t nPoint = gr->GetN();
  //
  areco = 0.0;
  for(Int_t i = 0;i<nPoint;i++){
    gr->GetPoint(i,t,a);
    areco = 0.0;
    for(Int_t k = 0;k<N;k++){
      gr_fft_re->GetPoint(k,freq,a_re);
      gr_fft_im->GetPoint(k,freq,a_im);
      if(freq<cut_off_frequency){
	areco += a_re*TMath::Cos(2*TMath::Pi()*k*i/nPoint);
	areco += -a_im*TMath::Sin(2*TMath::Pi()*k*i/nPoint);
      }
    }
    gr->SetPoint(i,t,areco);
  }
  genSiPMwf::get_gr_charge_and_baseline( gr, 0.0, 100.0, charge, baseline);
  for(Int_t i = 0;i<nPoint;i++){
    gr->GetPoint(i,t,a);
    gr->SetPoint(i,t,(a-baseline));
  }
  genSiPMwf::get_gr_charge_and_baseline( gr, 0.0, 100.0, charge, baseline);
  for(Int_t i = 0;i<nPoint;i++){
    gr->GetPoint(i,t,a);
    gr->SetPoint(i,t,a/charge*charge_initial);
  }  
}

Double_t lee(Double_t *x, Double_t *par){
  //
  Double_t ampl = par[0];
  Double_t line_k = par[1];
  Double_t tau_decay_fast = par[2];
  Double_t tau_decay_slow = par[3];
  Double_t sigma_to_conv = par[4];
  //
  Double_t t0 = par[5];
  Double_t a0 = par[6];
  Double_t n_tau_slow = par[7];
  Double_t time_window_in_ns = par[8];
  Int_t convolutionOn = 1;
  if(sigma_to_conv == 0.0)
    convolutionOn = 0;
  //
  //cout<<"ampl              = "<<ampl<<endl
  //   <<"line_k            = "<<line_k<<endl
  //    <<"tau_decay_fast    = "<<tau_decay_fast<<endl
  //    <<"tau_decay_slow    = "<<tau_decay_slow<<endl
  //    <<"sigma_to_conv     = "<<sigma_to_conv<<endl
  //    <<"t0                = "<<t0<<endl
  //    <<"a0                = "<<a0<<endl
  //    <<"n_tau_slow        = "<<n_tau_slow<<endl
  //    <<"time_window_in_ns = "<<time_window_in_ns<<endl;
  //
  Double_t t = x[0];
  Double_t a;
  //
  Double_t b = 1.0 - line_k*t0;
  Double_t x0;
  //
  //
  //convolution
  //
  Int_t n = 10000;
  Double_t dt = time_window_in_ns/(n-1);
  Double_t a_conv = 0.0;
  //
  Double_t tt = 0.0;
  //
  if(convolutionOn == 1){
    for(Int_t i = 0;i<n;i++){
      tt = dt*i;
      if(line_k != 0.0)
	x0 = -b/line_k;
      else
	assert(0);
      //
      if( tt < 0.0 || t > time_window_in_ns){
	return a0;
      }
      //
      if(tt <= x0){
	a = a0;
      }
      else if (tt>x0 && tt<=t0){
	a = a0 + line_k*tt + b;
      }
      else if ((tt>t0) && ((tt-t0) <= n_tau_slow * tau_decay_slow)){
	a = a0 + 0.5*TMath::Exp(-(tt-t0)/tau_decay_fast) + 0.5*TMath::Exp(-(tt-t0)/tau_decay_slow);
      }
      else{
	a = a0;
      }
      a_conv += a*TMath::Gaus((t-dt*i),0.0,sigma_to_conv)*dt;
    }
    return a_conv*ampl;
  }
  if(convolutionOn == 0){
    tt = t;
    if(line_k != 0.0)
      x0 = -b/line_k;
    else
      assert(0);
    //
    if( tt < 0.0 || t > time_window_in_ns){
      return a0;
    }
    //
    if(tt <= x0){
      a = a0;
    }
    else if (tt>x0 && tt<=t0){
      a = a0 + line_k*tt + b;
    }
    else if ((tt>t0) && ((tt-t0) <= n_tau_slow * tau_decay_slow)){
      a = a0 + 0.5*TMath::Exp(-(tt-t0)/tau_decay_fast) + 0.5*TMath::Exp(-(tt-t0)/tau_decay_slow);
    }
    else{
      a = a0;
    }
  }
  return a*ampl;
}
