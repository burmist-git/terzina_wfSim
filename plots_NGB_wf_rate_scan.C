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

Int_t plots_NGB_wf_rate_scan(){

  TGraph *gr_wf_01 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root", "gr_wf_0000", kBlack, 1);
  TGraph *gr_wf_locMax_01 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root", "gr_wf_locMax_0000", kRed, 20);
  TGraph *gr_wf_02 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.root", "gr_wf_0000", kBlack, 1);
  TGraph *gr_wf_locMax_02 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.root", "gr_wf_locMax_0000", kRed, 20);
  TGraph *gr_wf_03 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root", "gr_wf_0000", kBlack, 1);
  TGraph *gr_wf_locMax_03 = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root", "gr_wf_locMax_0000", kRed, 20);

  TCanvas *c1 = new TCanvas("c1","c1",10,10,1500,1000);
  c1->Divide(1,3);
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
  Double_t a_max = 10.0;
  //
  //
  c1->cd(1);
  gPad->SetRightMargin(0.005);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.04);
  gPad->SetGridx();
  gPad->SetGridy();
  TMultiGraph *mg01 = new TMultiGraph();
  mg01->Add(gr_wf_01,"APL");
  mg01->Add(gr_wf_locMax_01,"AP");
  mg01->GetYaxis()->SetTitle("Amplitude, single p.e.");
  mg01->GetXaxis()->SetRangeUser(10000.0,13000.0);
  mg01->GetYaxis()->SetRangeUser(0.0,a_max);
  mg01->Draw("APL");
  TLegend *leg01 = new TLegend(0.5,0.85,0.95,0.95,"","brNDC");
  leg01->AddEntry(gr_wf_01, "NUVHD 25um, BW: 40MHz, DCR+NGB = 10 MHz", "pl");
  leg01->Draw();
  c1->cd(2);
  gPad->SetRightMargin(0.005);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.04);
  gPad->SetGridx();
  gPad->SetGridy();
  gr_wf_02->SetTitle("");
  TMultiGraph *mg02 = new TMultiGraph();
  mg02->Add(gr_wf_02,"APL");
  mg02->Add(gr_wf_locMax_02,"AP");
  mg02->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //mg02->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg02->GetYaxis()->SetRangeUser(0.0,a_max);
  mg02->GetXaxis()->SetRangeUser(10000.0,13000.0);
  //gr_wf_02->Draw("APL");
  //gr_wf_02->GetYaxis()->SetTitle("Amplitude, ADC count");
  //gr_wf_02->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg02->Draw("APL");
  TLegend *leg02 = new TLegend(0.5,0.85,0.95,0.95,"","brNDC");
  leg02->AddEntry(gr_wf_01, "NUVHD 25um, BW: 40MHz, DCR+NGB = 20 MHz", "pl");
  leg02->Draw();
  c1->cd(3);
  gPad->SetRightMargin(0.005);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  gr_wf_03->SetTitle("");
  TMultiGraph *mg03 = new TMultiGraph();
  mg03->Add(gr_wf_03,"APL");
  mg03->Add(gr_wf_locMax_03,"AP");
  mg03->GetYaxis()->SetRangeUser(0.0,a_max);
  mg03->GetXaxis()->SetRangeUser(10000.0,13000.0);
  mg03->GetXaxis()->SetTitle("time, ns");
  mg03->GetYaxis()->SetTitle("Amplitude, single p.e.");
  mg03->Draw("APL");
  TLegend *leg03 = new TLegend(0.5,0.85,0.95,0.95,"","brNDC");
  leg03->AddEntry(gr_wf_01, "NUVHD 25um, BW: 40MHz, DCR+NGB = 30 MHz", "pl");
  leg03->Draw();
  return 0;
}

TGraph* get_tgraph(TString fileN, TString grName1, Int_t colorID, Int_t markerStyleID){
  //
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr = (TGraph*)f1->Get(grName1.Data());  //
  //
  gr->SetMarkerStyle(markerStyleID);
  gr->SetMarkerColor(colorID);
  gr->SetLineColor(colorID);
  gr->SetLineWidth(2);
  gr->SetTitle("");
  return gr;
}
