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

Int_t plots_NGB_single(){
  TString fileN;
  fileN = "./wfSim_25um_conv_fit_20MHz_filter.root";
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr_wf = (TGraph*)f1->Get("gr_wf_0001");  //
  TGraph *gr_wf_sig = (TGraph*)f1->Get("gr_wf_sig_0001");  //
  TGraph *gr_wf_sig_only = (TGraph*)f1->Get("gr_wf_sig_only_0001");  //
  //TGraph *gr_wf = (TGraph*)f1->Get("_gr_wf_tmpl");  //
  gr_wf->SetMarkerStyle(1);
  gr_wf->SetMarkerColor(kBlack);
  gr_wf->SetLineColor(kBlack);
  gr_wf->SetLineWidth(2);
  gr_wf_sig->SetMarkerStyle(1);
  gr_wf_sig->SetMarkerColor(kRed);
  gr_wf_sig->SetLineColor(kRed);
  gr_wf_sig->SetLineWidth(2);
  gr_wf_sig_only->SetMarkerStyle(1);
  gr_wf_sig_only->SetMarkerColor(kBlue);
  gr_wf_sig_only->SetLineColor(kBlue);
  gr_wf_sig_only->SetLineWidth(2);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c1->SetRightMargin(0.03);
  c1->SetLeftMargin(0.1);
  c1->SetTopMargin(0.01);
  c1->SetBottomMargin(0.08);  
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_wf_sig);
  mg->Add(gr_wf_sig_only);
  mg->Add(gr_wf);
  mg->Draw("APL");
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //  
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr_wf, "NGB", "pl");
  leg->AddEntry(gr_wf_sig_only, "sig.", "pl");
  leg->AddEntry(gr_wf_sig, "NGB + sig.", "pl");
  leg->Draw();  
  ///////////////////////
  return 0;
}
