//root
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TMath.h"
#include "TFree.h"
#include "TBranch.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <time.h>

using namespace std;

void load_data(TString fileName, TGraphErrors *gr);

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

Int_t main(){
  ///////////////////
  //
  //TString wf_to_fit = "./wfData/waveform_template_fast_25um_lee_conv_cut_off_frequency_20MHz.csv";
  //TString wf_to_fit = "./wfData/waveform_template_fast_25um_lee_conv_cut_off_frequency_40MHz.csv";
  //TString wf_to_fit = "./wfData/waveform_template_fast_35um_lee_conv_cut_off_frequency_20MHz.csv";
  TString wf_to_fit = "./wfData/waveform_template_fast_25um_lee.txt";
  //TString fileOut_hist_root = "fit_LEE_25um_lee_conv_cut_off_frequency_20MHz.root";
  //TString fileOut_hist_root = "fit_LEE_25um_lee_conv_cut_off_frequency_40MHz.root";
  TString fileOut_hist_root = "fit_LEE_fast_25um_lee.root";
  //
  Double_t dt_ns = 0.25;//ns
  Double_t time_window_start = 0.0;
  Double_t time_window_in_ns = 2000.0; //ns 
  const Int_t nPoint = (Int_t)time_window_in_ns/dt_ns + 1;
  //
  ////////////////////////
  //25um_lee_conv_cut_off_frequency_20MHz
  //Double_t ampl = 0.0305;
  //Double_t line_k = 1/34.0;           //ns^-1
  //Double_t tau_decay_fast = 6.1;      //ns
  //Double_t tau_decay_slow = 64.0;     //ns NUVHD 25um
  //Double_t sigma_to_conv = 8.0;       //ns
  //
  //Double_t t0 = 518;
  //Double_t a0 = 0.0;
  //Double_t n_tau_slow = 10.0;
  //
  //
  //25um_lee
  Double_t ampl = 0.7;
  Double_t line_k = 1/3.0;           //ns^-1
  Double_t tau_decay_fast = 4.0;      //ns
  Double_t tau_decay_slow = 64.0;     //ns NUVHD 25um
  Double_t sigma_to_conv = 0.5;       //ns
  //
  Double_t t0 = 500;
  Double_t a0 = 0.0;
  Double_t n_tau_slow = 10.0;
  //
  //25um_lee_conv_cut_off_frequency_40MHz
  //Double_t ampl = 0.065;
  //Double_t line_k = 1/17.0;           //ns^-1
  //Double_t tau_decay_fast = 6.0;      //ns
  //Double_t tau_decay_slow = 64.0;     //ns NUVHD 25um
  //Double_t sigma_to_conv = 5.0;       //ns
  //
  //Double_t t0 = 507;
  //Double_t a0 = 0.0;
  //Double_t n_tau_slow = 10.0;
  //
  //
  //35um_lee_conv_cut_off_frequency_20MHz
  //Double_t ampl = 0.036;
  //Double_t line_k = 1/34.0;            //ns^-1
  //Double_t tau_decay_fast = 12.0;      //ns
  //Double_t tau_decay_slow = 150.0;     //ns NUVHD 25um
  //Double_t sigma_to_conv = 8.0;        //ns
  //
  //Double_t t0 = 519;
  //Double_t a0 = 0.0;
  //Double_t n_tau_slow = 10.0;
  //
  Int_t npar = 9;
  TF1 *f_lee = new TF1("lee", lee, time_window_start, time_window_in_ns, npar);
  //
  ////////////////////////
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
  f_lee->SetParameter(0, ampl);
  f_lee->SetParameter(1, line_k);
  f_lee->SetParameter(2, tau_decay_fast);
  f_lee->SetParameter(3, tau_decay_slow);
  f_lee->SetParameter(4, sigma_to_conv);
  //
  f_lee->SetParameter(5, t0);
  f_lee->SetParameter(6, a0);
  f_lee->SetParameter(7, n_tau_slow);
  f_lee->SetParameter(8, time_window_in_ns);
  //
  //f_lee->FixParameter(0, ampl);
  //f_lee->FixParameter(1, line_k);
  //f_lee->FixParameter(2, tau_decay_fast);
  //f_lee->FixParameter(3, tau_decay_slow);
  f_lee->FixParameter(4, sigma_to_conv);
  //f_lee->FixParameter(5, t0);
  f_lee->FixParameter(6, a0);
  f_lee->FixParameter(7, n_tau_slow);
  f_lee->FixParameter(8, time_window_in_ns);

  //
  //f_lee->Eval(0);
  //
  TGraphErrors *gr_initial = new TGraphErrors();
  gr_initial->SetName("gr_initial");
  gr_initial->SetTitle("gr_initial");
  for(Int_t i = 0;i<nPoint; i++)
    gr_initial->SetPoint(gr_initial->GetN(),dt_ns*i,f_lee->Eval(dt_ns*i));
  //
  ////////////////////////
  //
  //
  TGraphErrors *gr = new TGraphErrors();
  gr->SetName("gr");
  gr->SetTitle("gr");
  TGraphErrors *gr_nofit = new TGraphErrors();
  gr_nofit->SetName("gr_nofit");
  gr_nofit->SetTitle("gr_nofit");
  load_data(wf_to_fit, gr);
  load_data(wf_to_fit, gr_nofit);
  gr->Fit(f_lee, "",  "", time_window_start, time_window_in_ns);
  TGraphErrors *gr_fit = new TGraphErrors();
  gr_fit->SetName("gr_fit");
  gr_fit->SetTitle("gr_fit");
  for(Int_t i = 0;i<nPoint; i++)
    gr_fit->SetPoint(gr_fit->GetN(),dt_ns*i,f_lee->Eval(dt_ns*i));
  //  
  //
  //Ploting
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,1200);
  c1->SetRightMargin(0.03);
  c1->SetLeftMargin(0.09);
  c1->SetTopMargin(0.09);
  c1->SetBottomMargin(0.09);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  gPad->SetGridx();
  gPad->SetGridy();
  //
  //gr->Draw("APL");
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr);
  mg->Add(gr_fit);
  gr->SetLineColor(kBlack);
  gr_initial->SetLineColor(kBlue);
  gr_fit->SetLineColor(kRed);
  gr->SetMarkerColor(kBlack);
  gr_initial->SetMarkerColor(kBlue);
  gr_fit->SetMarkerColor(kRed);
  mg->Draw("APL");
  //mg->SetMinimum(1.0e-5);
  //mg->SetMaximum(yMax);

  TFile* rootFile = new TFile(fileOut_hist_root.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    std::cout<<"  ERROR ---> file "<<fileOut_hist_root.Data()<<" is zombi"<<std::endl;
    assert(0);
  }
  else {
    std::cout<<"  Output Histos file ---> "<<fileOut_hist_root.Data()<<std::endl;
  }
  ////////////////////////
  //
  c1->Write();
  gr->Write();
  gr_nofit->Write();
  gr_initial->Write();
  gr_fit->Write();
  rootFile->Close();
  ///////////////////
  return 0;
}

void load_data(TString fileName, TGraphErrors *gr){
  ifstream fileIn(fileName.Data());
  Double_t x;
  Double_t y;
  Int_t i = 0;
  Int_t k = 0;
  TString mot;
  if(fileIn.is_open()){
    //fileIn>>mot;
    while(fileIn>>x>>y){
      if(k%5==0){
	i=gr->GetN();
	gr->SetPoint(i,x,y);
	//gr->SetPointError(i,0.0,0.0);
	if(y<0.3)
	  gr->SetPointError(i,0.1,0.01);
	else
	  gr->SetPointError(i,0.1,0.01);
      }
      k++;
    }
    fileIn.close();
  }
  else
    cout<<"Unable to open file"<<endl;
}
