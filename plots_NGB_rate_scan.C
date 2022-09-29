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

TGraph* get_tgraph(TString fileN, TString grName1, Int_t colorID);

Int_t plots_NGB_rate_scan(){
  TGraph* gr_threshold_counter_10MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root","gr_threshold_counter",kBlack);
  TGraph* gr_threshold_counter_20MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.root","gr_threshold_counter",kBlue);
  TGraph* gr_threshold_counter_30MHz = get_tgraph("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root","gr_threshold_counter",kRed);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,600,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c1->SetRightMargin(0.03);
  c1->SetLeftMargin(0.1);
  c1->SetTopMargin(0.03);
  c1->SetBottomMargin(0.08);  
  //
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_threshold_counter_10MHz);
  mg->Add(gr_threshold_counter_20MHz);
  mg->Add(gr_threshold_counter_30MHz);
  mg->GetXaxis()->SetRangeUser(0,15);
  mg->GetYaxis()->SetRangeUser(1.0,1.0e8);
  mg->Draw("APL");
  //
  //
  mg->GetYaxis()->SetTitle("Rate, Hz");
  mg->GetXaxis()->SetTitle("Threshold, single p.e.");
  //
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr_threshold_counter_10MHz, "DCR + NGB = 10 MHz", "pl");
  leg->AddEntry(gr_threshold_counter_20MHz, "DCR + NGB = 20 MHz", "pl");
  leg->AddEntry(gr_threshold_counter_30MHz, "DCR + NGB = 30 MHz", "pl");
  leg->Draw();
  //
  // mg02->GetXaxis()->SetRangeUser(450,650);
  // mg02->GetXaxis()->SetTitle("time, ns");
  //mg02->GetYaxis()->SetTitle("Amplitude, a.u.");
  
  
  //gr_threshold_counter_30MHz->SetTitle("");
  //gr_threshold_counter_30MHz->Draw("APL");
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  
  return 0;
}

TGraph* get_tgraph(TString fileN, TString grName1, Int_t colorID){
  //
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr = (TGraph*)f1->Get(grName1.Data());  //
  //
  gr->SetMarkerStyle(1);
  gr->SetMarkerColor(colorID);
  gr->SetLineColor(colorID);
  gr->SetLineWidth(2);
  gr->SetTitle("");
  return gr;
}
