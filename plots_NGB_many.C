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

Int_t plots_NGB_many(){
  TString fileN;
  //fileN = "./wfSim_7ns.root";
  //fileN = "./wfSim_Terzina_25um_conv_fit_fullMHz_filter.root";
  //fileN = "./wfSim_25um_conv_fit_20MHz_filter.root";
  //fileN = "./wfSim_25um_conv_fit_40MHz_filter.root";
  //fileN = "wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root";
  fileN = "wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root";
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr_wf_01 = (TGraph*)f1->Get("gr_wf_0000");
  TGraph *gr_wf_locMax_01 = (TGraph*)f1->Get("gr_wf_locMax_0000");
  TGraph *gr_wf_02 = (TGraph*)f1->Get("gr_wf_0001");
  TGraph *gr_wf_locMax_02 = (TGraph*)f1->Get("gr_wf_locMax_0001");
  TGraph *gr_wf_03 = (TGraph*)f1->Get("gr_wf_0002");
  TGraph *gr_wf_locMax_03 = (TGraph*)f1->Get("gr_wf_locMax_0002");
  TGraph *gr_wf_04 = (TGraph*)f1->Get("gr_wf_0003");
  TGraph *gr_wf_locMax_04 = (TGraph*)f1->Get("gr_wf_locMax_0003");
  TGraph *gr_wf_05 = (TGraph*)f1->Get("gr_wf_0004");
  TGraph *gr_wf_locMax_05 = (TGraph*)f1->Get("gr_wf_locMax_0004");
  TGraph *gr_wf_06 = (TGraph*)f1->Get("gr_wf_0005");
  TGraph *gr_wf_locMax_06 = (TGraph*)f1->Get("gr_wf_locMax_0005");
  //
  gr_wf_01->SetMarkerStyle(1);
  gr_wf_02->SetMarkerStyle(1);
  gr_wf_03->SetMarkerStyle(1);
  gr_wf_04->SetMarkerStyle(1);
  gr_wf_05->SetMarkerStyle(1);
  gr_wf_06->SetMarkerStyle(1);
  //
  gr_wf_locMax_01->SetMarkerStyle(20);
  gr_wf_locMax_02->SetMarkerStyle(20);
  gr_wf_locMax_03->SetMarkerStyle(20);
  gr_wf_locMax_04->SetMarkerStyle(20);
  gr_wf_locMax_05->SetMarkerStyle(20);
  gr_wf_locMax_06->SetMarkerStyle(20);
  //
  gr_wf_01->SetMarkerColor(kBlack);
  gr_wf_01->SetLineColor(kBlack);
  gr_wf_02->SetMarkerColor(kBlack);
  gr_wf_02->SetLineColor(kBlack);
  gr_wf_03->SetMarkerColor(kBlack);
  gr_wf_03->SetLineColor(kBlack);
  gr_wf_04->SetMarkerColor(kBlack);
  gr_wf_04->SetLineColor(kBlack);
  //
  gr_wf_locMax_01->SetMarkerColor(kRed);
  gr_wf_locMax_02->SetMarkerColor(kRed);
  gr_wf_locMax_03->SetMarkerColor(kRed);
  gr_wf_locMax_04->SetMarkerColor(kRed);
  gr_wf_locMax_05->SetMarkerColor(kRed);
  gr_wf_locMax_06->SetMarkerColor(kRed);
  //
  //gr_wf_01->SetMarkerColor(kBlack);
  //gr_wf_02->SetMarkerColor(kRed);
  //gr_wf_03->SetMarkerColor(kBlue);
  //gr_wf_04->SetMarkerColor(kMagenta);
  //gr_wf_05->SetMarkerColor(kRed+2);
  //gr_wf_06->SetMarkerColor(kGreen+2);
  //
  //gr_wf_01->SetLineColor(kBlack);
  //gr_wf_02->SetLineColor(kRed);
  //gr_wf_03->SetLineColor(kBlue);
  //gr_wf_04->SetLineColor(kMagenta);
  //gr_wf_05->SetLineColor(kRed+2);
  //gr_wf_06->SetLineColor(kGreen+2);
  //
  gr_wf_01->SetLineWidth(2);
  gr_wf_02->SetLineWidth(2);
  gr_wf_03->SetLineWidth(2);
  gr_wf_04->SetLineWidth(2);
  gr_wf_05->SetLineWidth(2);
  gr_wf_06->SetLineWidth(2);
  //
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
  Double_t a_max = 8.0;
  //
  c1->cd(1);
  gPad->SetRightMargin(0.02);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.04);
  gPad->SetGridx();
  gPad->SetGridy();
  TMultiGraph *mg01 = new TMultiGraph();
  mg01->Add(gr_wf_01,"APL");
  mg01->Add(gr_wf_locMax_01,"AP");
  mg01->GetYaxis()->SetTitle("Amplitude, ADC count");
  mg01->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg01->GetYaxis()->SetRangeUser(0.0,a_max);
  //gr_wf_01->SetTitle("");
  //gr_wf_01->Draw("APL");
  //gr_wf_01->GetYaxis()->SetTitle("Amplitude, ADC count");
  //gr_wf_01->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg01->Draw("APL");
  c1->cd(2);
  gPad->SetRightMargin(0.02);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.04);
  gPad->SetGridx();
  gPad->SetGridy();
  gr_wf_02->SetTitle("");
  TMultiGraph *mg02 = new TMultiGraph();
  mg02->Add(gr_wf_02,"APL");
  mg02->Add(gr_wf_locMax_02,"AP");
  mg02->GetYaxis()->SetTitle("Amplitude, ADC count");
  //mg02->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg02->GetYaxis()->SetRangeUser(0.0,a_max);
  mg02->GetXaxis()->SetRangeUser(10000.0,30000.0);
  //gr_wf_02->Draw("APL");
  //gr_wf_02->GetYaxis()->SetTitle("Amplitude, ADC count");
  //gr_wf_02->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg02->Draw("APL");
  c1->cd(3);
  gPad->SetRightMargin(0.02);
  gPad->SetLeftMargin(0.02);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  gr_wf_03->SetTitle("");
  TMultiGraph *mg03 = new TMultiGraph();
  mg03->Add(gr_wf_03,"APL");
  mg03->Add(gr_wf_locMax_03,"AP");
  mg03->GetYaxis()->SetTitle("Amplitude, ADC count");
  //mg03->GetXaxis()->SetRangeUser(0.0,100000.0);
  mg03->GetYaxis()->SetRangeUser(0.0,a_max);
  mg03->GetXaxis()->SetRangeUser(10000.0,15000.0);
  mg03->GetXaxis()->SetTitle("time, ns");
  mg03->GetYaxis()->SetTitle("Amplitude, ADC count");
  mg03->Draw("APL");
  TLegend *leg03 = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  //leg03->AddEntry(gr_wf_03, "25um(64ns), BW: 20MHz", "pl");
  //leg03->AddEntry(gr_wf_03, "64 ns, BW: full", "pl");
  //leg03->AddEntry(gr_wf_03, "64 ns, BW: 20MHz", "pl");
  leg03->AddEntry(gr_wf_03, "64 ns, BW: 40MHz", "pl");
  leg03->Draw();  
  //gr_wf_03->Draw("APL");
  //gr_wf_03->GetXaxis()->SetTitle("time, ns");
  //gr_wf_03->GetYaxis()->SetTitle("Amplitude, ADC count");
  //gr_wf_03->GetXaxis()->SetRangeUser(0.0,100000.0);
  //
  //
  /*
  c1->cd(3);
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_wf_01);
  mg->Add(gr_wf_02);
  mg->Add(gr_wf_03);
  //mg->Add(gr_wf_sig_only_04);
  //mg->Add(gr_wf_sig_only_05);
  //mg->Add(gr_wf_sig_only_06);
  //mg->GetXaxis()->SetRangeUser(4950,6000);
  //mg->GetYaxis()->SetRangeUser(-0.2,6.0);
  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //c1->SaveAs("sig_wf_sim_7km_4pe.pdf");
  //c1->SaveAs("sig_wf_sim_15km_4pe.pdf");
  //c1->SaveAs("sig_wf_sim_25km_4pe.pdf");
  */
  return 0;
}
