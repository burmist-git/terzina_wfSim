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

void load_data( TString fileName, TGraph *gr);

Int_t plots_template_comp(){
  //
  TString fileN01;
  TString fileN02;
  //
  fileN01 = "./wfData/linearExpExp_tau_260ns.txt";
  fileN02 = "./wfData/linearExpExp_tau_260ns_fit_40MHz_filter.txt";
  //
  TGraph *gr01 = new TGraph();
  gr01->SetTitle("gr01");
  gr01->SetName("gr01");
  //
  TGraph *gr02 = new TGraph();
  gr02->SetTitle("gr02");
  gr02->SetName("gr02");
  //
  load_data(fileN01, gr01);
  load_data(fileN02, gr02);
  //
  gr01->SetMarkerStyle(1);
  gr01->SetMarkerColor(kBlack);
  gr01->SetLineColor(kBlack);
  gr01->SetLineWidth(2);
  //
  gr02->SetMarkerStyle(1);
  gr02->SetMarkerColor(kRed);
  gr02->SetLineColor(kRed);
  gr02->SetLineWidth(2);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,800,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c1->SetRightMargin(0.01);
  c1->SetLeftMargin(0.1);
  c1->SetTopMargin(0.02);
  c1->SetBottomMargin(0.08);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg01 = new TMultiGraph();
  mg01->Add(gr01);
  mg01->Add(gr02);
  mg01->Draw("APL");
  mg01->GetXaxis()->SetRangeUser(400,1500);
  //mg01->SetMinimum(0.0);
  //mg01->SetMaximum(4.0);
  //mg01->SetMaximum(6.0);
  //mg01->SetMaximum(5.0);
  //
  mg01->GetXaxis()->SetTitle("time, ns");
  mg01->GetYaxis()->SetTitle("Amplitude, a.u.");
  //  
  TLegend *leg01 = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg01->AddEntry(gr01, "25 um", "pl");
  leg01->AddEntry(gr02, "35 um", "pl");
  leg01->Draw();
  ///////////////////////
  TCanvas *c2 = new TCanvas("c2","c2",10,10,800,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c2->SetRightMargin(0.01);
  c2->SetLeftMargin(0.1);
  c2->SetTopMargin(0.02);
  c2->SetBottomMargin(0.08);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg02 = new TMultiGraph();
  mg02->Add(gr01);
  mg02->Add(gr02);
  mg02->Draw("APL");
  mg02->GetXaxis()->SetRangeUser(450,650);
  //mg02->SetMinimum(0.0);
  //mg02->SetMaximum(4.0);
  //mg02->SetMaximum(6.0);
  //mg02->SetMaximum(5.0);
  //
  mg02->GetXaxis()->SetTitle("time, ns");
  mg02->GetYaxis()->SetTitle("Amplitude, a.u.");
  //
  TLegend *leg02 = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg02->AddEntry(gr01, "25 um", "pl");
  leg02->AddEntry(gr02, "35 um", "pl");
  leg02->Draw();
  ///////////////////////
  return 0;
}

void load_data( TString fileName, TGraph *gr){
  ifstream fileIn(fileName.Data());
  Double_t x;
  Double_t y;
  Int_t i = 0;
  TString mot;
  if(fileIn.is_open()){
    //fileIn>>mot;
    while(fileIn>>x>>y){
      i=gr->GetN();
      gr->SetPoint(i,x,y);
    }
    fileIn.close();
  }
  else
    cout<<"Unable to open file"<<endl;
}
  
