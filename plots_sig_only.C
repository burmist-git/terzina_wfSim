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

Int_t plots_sig_only(){
  TString fileN;
  fileN = "./wfSim.root";
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr_wf_sig_only_01 = (TGraph*)f1->Get("gr_wf_sig_only_0000");
  TGraph *gr_wf_sig_only_02 = (TGraph*)f1->Get("gr_wf_sig_only_0007");
  TGraph *gr_wf_sig_only_03 = (TGraph*)f1->Get("gr_wf_sig_only_0046");
  
  //
  gr_wf_sig_only_01->SetMarkerStyle(1);
  gr_wf_sig_only_02->SetMarkerStyle(1);
  gr_wf_sig_only_03->SetMarkerStyle(1);
  //
  gr_wf_sig_only_01->SetMarkerColor(kBlack);
  gr_wf_sig_only_02->SetMarkerColor(kRed);
  gr_wf_sig_only_03->SetMarkerColor(kBlue);
  //
  gr_wf_sig_only_01->SetLineColor(kBlack);
  gr_wf_sig_only_02->SetLineColor(kRed);
  gr_wf_sig_only_03->SetLineColor(kBlue);
  //
  //
  gr_wf_sig_only_01->SetLineWidth(2);
  gr_wf_sig_only_02->SetLineWidth(2);
  gr_wf_sig_only_03->SetLineWidth(2);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_wf_sig_only_01);
  mg->Add(gr_wf_sig_only_02);
  mg->Add(gr_wf_sig_only_03);
  mg->GetXaxis()->SetRangeUser(4950,5500);
  mg->GetYaxis()->SetRangeUser(-0.2,1.3);
  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, single p.e.");
  return 0;
}
