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



Int_t plots(){
  TString fileN;
  //fileN = "./wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz_signal.root";
  //fileN = "./wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz_signal.root";
  fileN = "./wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz_signal.root";
  TFile *f1 = new TFile(fileN.Data());
  TGraph *_gr_wf_tmpl = (TGraph*)f1->Get("_gr_wf_tmpl");
  TGraph *gr_wf = (TGraph*)f1->Get("gr_wf_0001");
  TGraph *gr_wf_sig = (TGraph*)f1->Get("gr_wf_sig_0001");
  TGraph *gr_wf_sig_only = (TGraph*)f1->Get("gr_wf_sig_only_0001");
  //TGraph *gr_wf_sig_only_01 = (TGraph*)f1->Get("gr_wf_sig_only_0094");
  //TGraph *gr_wf_sig_only_02 = (TGraph*)f1->Get("gr_wf_sig_only_0073");
  
  TGraph *gr_wf_01 = (TGraph*)f1->Get("gr_wf_0001");
  TGraph *gr_wf_02 = (TGraph*)f1->Get("gr_wf_0002");
  //TGraph *gr_wf_03 = (TGraph*)f1->Get("gr_wf_0003");


  TGraph *gr_wf_sig_01 = (TGraph*)f1->Get("gr_wf_sig_0001");
  TGraph *gr_wf_sig_only_01 = (TGraph*)f1->Get("gr_wf_sig_only_0001");
  //
  gr_wf->SetMarkerStyle(1);
  gr_wf_sig->SetMarkerStyle(1);
  gr_wf_sig_only->SetMarkerStyle(1);
  gr_wf_sig_01->SetMarkerStyle(1);
  gr_wf_sig_only_01->SetMarkerStyle(1);
  //
  gr_wf->SetMarkerColor(kBlack);
  gr_wf_sig->SetMarkerColor(kRed);
  gr_wf_sig_only->SetMarkerColor(kBlue);
  gr_wf_sig_01->SetMarkerColor(kRed);
  gr_wf_sig_only_01->SetMarkerColor(kBlue);
  //
  gr_wf->SetLineColor(kBlack);
  gr_wf_sig->SetLineColor(kRed);
  gr_wf_sig_only->SetLineColor(kBlue);
  gr_wf_sig_01->SetLineColor(kRed);
  gr_wf_sig_only_01->SetLineColor(kBlue);
  //
  //gr_wf_01->SetMarkerColor(kBlue);
  //gr_wf_01->SetLineColor(kBlue);
  //gr_wf_01->SetLineWidth(2);
  //
  //gr_wf_02->SetMarkerColor(kRed);
  //gr_wf_02->SetLineColor(kRed);
  //gr_wf_02->SetLineWidth(2);
  //
  //gr_wf_03->SetMarkerColor(kGreen+2);
  //gr_wf_03->SetLineColor(kGreen+2);
  //gr_wf_03->SetLineWidth(2);  
  //
  gr_wf->SetLineWidth(2);
  gr_wf_sig->SetLineWidth(2);
  gr_wf_sig_only->SetLineWidth(2);  
  //
  gr_wf_01->SetLineWidth(2);
  gr_wf_sig_01->SetLineWidth(2);
  gr_wf_sig_only_01->SetLineWidth(2);  
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
  //mg->Add(gr_wf_sig);
  //mg->Add(gr_wf);
  //mg->Add(gr_wf_sig_only);
  mg->Add(gr_wf_01);
  //mg->Add(gr_wf_02);
  //mg->Add(gr_wf_03);
  //mg->GetXaxis()->SetLimits(xMin,xMax);
  mg->Draw("APL");
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //  
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr_wf, "NGB @ 50 MHz", "pl");
  leg->AddEntry(gr_wf_sig, "NGB @ 50 MHz + signal ", "pl");
  leg->AddEntry(gr_wf_sig_only, "signal only", "pl");
  leg->Draw();
  ///////////////////////
  TCanvas *c2 = new TCanvas("c2","c2",10,10,1200,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  _gr_wf_tmpl->SetTitle("");
  _gr_wf_tmpl->SetMarkerColor(kBlack);
  _gr_wf_tmpl->SetLineColor(kBlack);
  _gr_wf_tmpl->SetLineWidth(2);
  //
  _gr_wf_tmpl->Draw("APL");
  //
  _gr_wf_tmpl->GetXaxis()->SetTitle("time, ns");
  _gr_wf_tmpl->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //
  ////////////////////////
  TCanvas *c3 = new TCanvas("c3","c3",10,10,1200,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c3->Divide(1,2);
  //
  c3->cd(1);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg3 = new TMultiGraph();
  mg3->Add(gr_wf_sig);
  mg3->Add(gr_wf);
  mg3->Add(gr_wf_sig_only);
  mg3->Draw("APL");
  mg3->GetXaxis()->SetTitle("time, ns");
  mg3->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //  
  c3->cd(2);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg4 = new TMultiGraph();
  mg4->Add(gr_wf_sig_01);
  mg4->Add(gr_wf_01);
  mg4->Add(gr_wf_sig_only_01);
  mg4->Draw("APL");
  mg4->GetXaxis()->SetTitle("time, ns");
  mg4->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //  

  
  ///////////////////////
  return 0;
}
