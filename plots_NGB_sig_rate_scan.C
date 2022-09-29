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

Int_t plots_NGB_sig_rate_scan(){
  //
  TGraph *gr_wf_0000_10MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz_signal.root", "gr_wf_0000", kBlack, 1);
  TGraph *gr_wf_sig_0000_10MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz_signal.root", "gr_wf_sig_0000", kRed, 1);
  TGraph *gr_wf_sig_only_0000_10MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz_signal.root", "gr_wf_sig_only_0000", kBlue, 1);
  //
  TGraph *gr_wf_0000_20MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz_signal.root", "gr_wf_0000", kBlack, 1);
  TGraph *gr_wf_sig_0000_20MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz_signal.root", "gr_wf_sig_0000", kRed, 1);
  TGraph *gr_wf_sig_only_0000_20MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz_signal.root", "gr_wf_sig_only_0000", kBlue, 1);
  //
  TGraph *gr_wf_0000_30MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz_signal.root", "gr_wf_0000", kBlack, 1);
  TGraph *gr_wf_sig_0000_30MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz_signal.root", "gr_wf_sig_0000", kRed, 1);
  TGraph *gr_wf_sig_only_0000_30MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz_signal.root", "gr_wf_sig_only_0000", kBlue, 1);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1500,1000);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  c1->Divide(1,3);
  //
  Double_t a_min = -0.5;
  Double_t a_max = 12.0;
  Double_t t_sig_window_L = 1000.0;
  Double_t t_sig_window_R = 9000.0;
  //
  c1->cd(1);
  gPad->SetRightMargin(0.005);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.04);
  gPad->SetGridx();
  gPad->SetGridy();
  TMultiGraph *mg01 = new TMultiGraph();
  mg01->Add(gr_wf_sig_0000_10MHz);
  mg01->Add(gr_wf_sig_only_0000_10MHz);
  mg01->Add(gr_wf_0000_10MHz);
  mg01->GetYaxis()->SetTitle("Amplitude, single p.e.");
  mg01->GetXaxis()->SetTitle("time, ns");
  mg01->GetXaxis()->SetRangeUser(t_sig_window_L,t_sig_window_R);
  mg01->GetYaxis()->SetRangeUser(a_min,a_max);
  mg01->Draw("APL");
  TLegend *leg01 = new TLegend(0.7,0.7,0.95,0.95,"","brNDC");
  leg01->AddEntry(gr_wf_0000_10MHz, "NGB+DCR (10MHz)", "pl");
  leg01->AddEntry(gr_wf_sig_only_0000_10MHz, "signal (7 p.e.)", "pl");
  leg01->AddEntry(gr_wf_sig_0000_10MHz, "NGB+DCR+signal", "pl");
  leg01->Draw();
  //
  c1->cd(2);
  gPad->SetRightMargin(0.005);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.04);
  gPad->SetGridx();
  gPad->SetGridy();
  TMultiGraph *mg02 = new TMultiGraph();
  mg02->Add(gr_wf_sig_0000_20MHz);
  mg02->Add(gr_wf_sig_only_0000_20MHz);
  mg02->Add(gr_wf_0000_20MHz);
  mg02->GetYaxis()->SetTitle("Amplitude, single p.e.");
  mg02->GetXaxis()->SetTitle("time, ns");
  mg02->GetXaxis()->SetRangeUser(t_sig_window_L,t_sig_window_R);
  mg02->GetYaxis()->SetRangeUser(a_min,a_max);
  mg02->Draw("APL");
  TLegend *leg02 = new TLegend(0.7,0.7,0.95,0.95,"","brNDC");
  leg02->AddEntry(gr_wf_0000_10MHz, "NGB+DCR (20MHz)", "pl");
  leg02->AddEntry(gr_wf_sig_only_0000_10MHz, "signal (7 p.e.)", "pl");
  leg02->AddEntry(gr_wf_sig_0000_10MHz, "NGB+DCR+signal", "pl");
  leg02->Draw();
  //
  c1->cd(3);
  gPad->SetRightMargin(0.005);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  TMultiGraph *mg03 = new TMultiGraph();
  mg03->Add(gr_wf_sig_0000_30MHz);
  mg03->Add(gr_wf_sig_only_0000_30MHz);
  mg03->Add(gr_wf_0000_30MHz);
  mg03->GetYaxis()->SetTitle("Amplitude, single p.e.");
  mg03->GetXaxis()->SetTitle("time, ns");
  mg03->GetXaxis()->SetRangeUser(t_sig_window_L,t_sig_window_R);
  mg03->GetYaxis()->SetRangeUser(a_min,a_max);
  mg03->Draw("APL");
  TLegend *leg03 = new TLegend(0.7,0.7,0.95,0.95,"","brNDC");
  leg03->AddEntry(gr_wf_0000_10MHz, "NGB+DCR (30MHz)", "pl");
  leg03->AddEntry(gr_wf_sig_only_0000_10MHz, "signal (7 p.e.)", "pl");
  leg03->AddEntry(gr_wf_sig_0000_10MHz, "NGB+DCR+signal", "pl");
  leg03->Draw();
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
