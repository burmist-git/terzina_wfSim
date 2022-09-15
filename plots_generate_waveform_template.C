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

Int_t plots_generate_waveform_template(){
  //
  TString fileN;
  fileN = "./wfData/waveform_template_fast_35um.root";
  TFile *f1 = new TFile(fileN.Data());
  //
  TGraph *gr_wf_gge = (TGraph*)f1->Get("gr_wf_gge");
  TGraph *gr_wf_lee = (TGraph*)f1->Get("gr_wf_lee");
  TGraph *gr_wf_gge_conv = (TGraph*)f1->Get("gr_wf_gge_conv");
  TGraph *gr_wf_lee_conv = (TGraph*)f1->Get("gr_wf_lee_conv");
  //
  gr_wf_gge->SetMarkerStyle(1);
  gr_wf_lee->SetMarkerStyle(1);
  gr_wf_gge_conv->SetMarkerStyle(1);
  gr_wf_lee_conv->SetMarkerStyle(1);
  //
  gr_wf_gge->SetLineColor(kBlack);
  gr_wf_lee->SetLineColor(kRed);
  gr_wf_gge_conv->SetLineColor(kBlack);
  gr_wf_lee_conv->SetLineColor(kRed);
  //
  //gr_wf_gge->SetMarkerColor(kBlack);
  //gr_wf_lee->SetMarkerColor(kRed);
  //gr_wf_gge_conv->SetMarkerColor(kBlack);
  //gr_wf_lee_conv->SetMarkerColor(kRed);
  //
  //gr_wf_gge->SetLineColor(kBlack);
  //gr_wf_lee->SetLineColor(kBlack);
  //gr_wf_gge_conv->SetLineColor(kRed);
  //gr_wf_lee_conv->SetLineColor(kRed);
  //
  gr_wf_gge->SetLineWidth(2);
  gr_wf_lee->SetLineWidth(2);
  gr_wf_gge_conv->SetLineWidth(2);
  gr_wf_lee_conv->SetLineWidth(2);
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
  //mg->Add(gr_wf_gge);
  //mg->Add(gr_wf_lee);
  mg->Add(gr_wf_gge_conv);
  mg->Add(gr_wf_lee_conv);
  mg->Draw("APL");
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, a.u.");
  //  
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  //leg->AddEntry(gr_wf_gge, "Gaus, gaus, exp. (GGE)", "pl");
  //leg->AddEntry(gr_wf_lee, "Linear, exp., exp. (LEE)", "pl");
  leg->AddEntry(gr_wf_gge_conv, "GGE convolution with gaus", "pl");
  leg->AddEntry(gr_wf_lee_conv, "LEE convolution with gaus", "pl");
  leg->Draw();
  ///////////////////////
  return 0;
}
