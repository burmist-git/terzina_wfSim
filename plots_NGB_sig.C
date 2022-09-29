//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>

#include <time.h>

using namespace std;

TGraph* get_tgraph(TString fileN, TString grName1, Int_t colorID, Int_t markerStyleID);

Int_t plots_NGB_sig(){
  TGraph *gr_wf_sig_only_0000 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_signal.root", "gr_wf_sig_only_0000", kBlack, 1);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1500,1000);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  Double_t a_min = -0.5;
  Double_t a_max = 12.0;
  Double_t t_sig_window_L = 1000.0;
  Double_t t_sig_window_R = 9000.0;
  gr_wf_sig_only_0000->Draw("APL");
  return 0;
}

TGraph* get_tgraph(TString fileN, TString grName1, Int_t colorID, Int_t markerStyleID){
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr = (TGraph*)f1->Get(grName1.Data());
  gr->SetMarkerStyle(markerStyleID);
  gr->SetMarkerColor(colorID);
  gr->SetLineColor(colorID);
  gr->SetLineWidth(2);
  gr->SetTitle("");
  return gr;
}
