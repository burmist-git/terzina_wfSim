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

Int_t plots_template_single(){
  //
  TString fileN01;
  //
  //fileN01 = "./wfData/NUV_HD_template.txt";
  //fileN01 = "./wfData/nuv_6x6_75nc.out";
  //fileN01 = "./wfData/linearExpExp_NUVHD_25um_conv.txt";
  //fileN01 = "./wfData/linearExpExp_tau_30ns.txt";
  //fileN01 = "./wfData/linearExpExp_tau_90ns.txt";  
  //fileN01 = "./wfData/linearExpExp_tau_120ns.txt";
  //fileN01 = "./wfData/linearExpExp_tau_150ns.txt";
  //fileN01 = "./wfData/linearExpExp_tau_180ns.txt";
  //fileN01 = "./wfData/linearExpExp_tau_210ns.txt";
  //fileN01 = "./wfData/linearExpExp_tau_230ns.txt";
  fileN01 = "./wfData/linearExpExp_tau_260ns.txt";
  //
  TGraph *gr01 = new TGraph();
  gr01->SetTitle("gr01");
  gr01->SetName("gr01");
  load_data(fileN01, gr01);
  //
  gr01->SetMarkerStyle(1);
  gr01->SetMarkerColor(kBlack);
  gr01->SetLineColor(kBlack);
  gr01->SetLineWidth(2);
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
  gr01->Draw("APL");
  //gr01->GetXaxis()->SetRangeUser(400,1500);
  //
  gr01->GetXaxis()->SetTitle("time, ns");
  gr01->GetYaxis()->SetTitle("Amplitude, a.u.");
  //  
  TLegend *leg01 = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg01->AddEntry(gr01, fileN01.Data(), "pl");
  leg01->Draw();
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
  
