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

Int_t plots_fit_LEE(){
  //
  TString fileN;
  //fileN = "fit_LEE_25um_lee_conv_cut_off_frequency_40MHz.root";
  fileN = "fit_LEE_fast_25um_lee.root";
  TFile *f1 = new TFile(fileN.Data());
  //
  TGraphErrors *gr = (TGraphErrors*)f1->Get("gr");
  TGraphErrors *gr_nofit = (TGraphErrors*)f1->Get("gr_nofit");
  TGraphErrors *gr_initial = (TGraphErrors*)f1->Get("gr_initial");
  TGraphErrors *gr_fit = (TGraphErrors*)f1->Get("gr_fit");
  //
  //Ploting
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,1200);
  c1->SetRightMargin(0.03);
  c1->SetLeftMargin(0.09);
  c1->SetTopMargin(0.09);
  c1->SetBottomMargin(0.09);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  gPad->SetGridx();
  gPad->SetGridy();
  //
  //gr->Draw("APL");
  //
  TMultiGraph *mg = new TMultiGraph();
  //mg->Add(gr);
  mg->Add(gr_nofit);
  //mg->Add(gr_initial);
  mg->Add(gr_fit);

  gr->SetLineColor(kBlack);
  gr_nofit->SetLineColor(kBlack);
  gr_initial->SetLineColor(kBlue);
  gr_fit->SetLineColor(kRed);
  gr->SetMarkerColor(kBlack);
  gr_nofit->SetMarkerColor(kBlack);
  gr_initial->SetMarkerColor(kBlue);
  gr_fit->SetMarkerColor(kRed);

  mg->Draw("APL");
  //mg->SetMinimum(1.0e-5);
  //mg->SetMaximum(yMax);

  return 0;
}
