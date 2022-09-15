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

Int_t plots_fft(){
  TString fileN;
  fileN = "./fft.root";
  TFile *f1 = new TFile(fileN.Data());
  TGraph *gr = (TGraph*)f1->Get("gr");
  TGraph *gr_filter = (TGraph*)f1->Get("gr_filter");
  TGraphErrors *grerr_filter = (TGraphErrors*)f1->Get("grerr_filter");
  TGraphErrors *grerr_filter_fit = (TGraphErrors*)f1->Get("grerr_filter_fit");
  //
  gr->SetMarkerStyle(1);
  gr_filter->SetMarkerStyle(1);
  grerr_filter_fit->SetMarkerStyle(1);
  grerr_filter->SetMarkerStyle(1);
  //
  gr->SetMarkerColor(kBlack);
  gr_filter->SetMarkerColor(kRed);
  grerr_filter_fit->SetMarkerColor(kBlue);
  grerr_filter->SetMarkerColor(kRed+2);
  //
  gr->SetLineColor(kBlack);
  gr_filter->SetLineColor(kRed);
  grerr_filter_fit->SetLineColor(kBlue);
  grerr_filter->SetLineColor(kRed+2);
  //
  gr->SetLineWidth(3);
  gr_filter->SetLineWidth(3);
  grerr_filter_fit->SetLineWidth(3);
  grerr_filter->SetLineWidth(3);
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
  mg->Add(gr);
  mg->Add(gr_filter);
  mg->Add(grerr_filter_fit);
  //mg->Add(grerr_filter);
  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //  
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr, "Initial", "pl");
  leg->AddEntry(gr_filter, "Frequency cut off", "pl");
  leg->Draw();
  ///////////////////////
  return 0;
}
