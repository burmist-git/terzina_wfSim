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
  TGraph *gr_wf_sig_only_02 = (TGraph*)f1->Get("gr_wf_sig_only_0001");
  TGraph *gr_wf_sig_only_03 = (TGraph*)f1->Get("gr_wf_sig_only_0002");
  TGraph *gr_wf_sig_only_04 = (TGraph*)f1->Get("gr_wf_sig_only_0003");
  TGraph *gr_wf_sig_only_05 = (TGraph*)f1->Get("gr_wf_sig_only_0004");
  TGraph *gr_wf_sig_only_06 = (TGraph*)f1->Get("gr_wf_sig_only_0005");  
  //
  gr_wf_sig_only_01->SetMarkerStyle(1);
  gr_wf_sig_only_02->SetMarkerStyle(1);
  gr_wf_sig_only_03->SetMarkerStyle(1);
  gr_wf_sig_only_04->SetMarkerStyle(1);
  gr_wf_sig_only_05->SetMarkerStyle(1);
  gr_wf_sig_only_06->SetMarkerStyle(1);
  //
  gr_wf_sig_only_01->SetMarkerColor(kBlack);
  gr_wf_sig_only_02->SetMarkerColor(kRed);
  gr_wf_sig_only_03->SetMarkerColor(kBlue);
  gr_wf_sig_only_04->SetMarkerColor(kMagenta);
  gr_wf_sig_only_05->SetMarkerColor(kRed+2);
  gr_wf_sig_only_06->SetMarkerColor(kGreen+2);
  //
  gr_wf_sig_only_01->SetLineColor(kBlack);
  gr_wf_sig_only_02->SetLineColor(kRed);
  gr_wf_sig_only_03->SetLineColor(kBlue);
  gr_wf_sig_only_04->SetLineColor(kMagenta);
  gr_wf_sig_only_05->SetLineColor(kRed+2);
  gr_wf_sig_only_06->SetLineColor(kGreen+2);
  //
  gr_wf_sig_only_01->SetLineWidth(2);
  gr_wf_sig_only_02->SetLineWidth(2);
  gr_wf_sig_only_03->SetLineWidth(2);
  gr_wf_sig_only_04->SetLineWidth(2);
  gr_wf_sig_only_05->SetLineWidth(2);
  gr_wf_sig_only_06->SetLineWidth(2);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,400);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //  
  c1->SetRightMargin(0.01);
  c1->SetLeftMargin(0.08);
  c1->SetTopMargin(0.02);
  c1->SetBottomMargin(0.08);
  //
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_wf_sig_only_01);
  mg->Add(gr_wf_sig_only_02);
  mg->Add(gr_wf_sig_only_03);
  //mg->Add(gr_wf_sig_only_04);
  //mg->Add(gr_wf_sig_only_05);
  //mg->Add(gr_wf_sig_only_06);
  mg->GetXaxis()->SetRangeUser(4950,6000);
  mg->GetYaxis()->SetRangeUser(-0.2,6.0);
  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, single p.e.");
  c1->SaveAs("sig_wf_sim_7km_4pe.pdf");
  //c1->SaveAs("sig_wf_sim_15km_4pe.pdf");
  //c1->SaveAs("sig_wf_sim_25km_4pe.pdf");
  return 0;
}
