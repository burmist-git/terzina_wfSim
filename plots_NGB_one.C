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

TGraph* plot(TString fileN);
Int_t plots_NGB_slow();

Int_t plots_NGB_one(){
  //plots_NGB_slow();
  //plot("wfSim_Terzina_25um_conv_fit_40MHz_filter_10MHz.root");
  //plot("wfSim_Terzina_25um_conv_fit_40MHz_filter_20MHz.root");
  plot("wfSim_Terzina_25um_conv_fit_40MHz_filter_30MHz.root");
  return 0;
}

Int_t plots_NGB_slow(){
  TString fileN;
  //fileN = "./wfSim_7ns.root";
  //fileN = "./wfSim_Terzina_25um_conv_fit_fullMHz_filter.root";
  //fileN = "./wfSim_25um_conv_fit_fullMHz_filter.root";
  //fileN = "./wfSim_25um_conv_fit_20MHz_filter.root";
  //fileN = "./wfSim_25um_conv_fit_40MHz_filter.root";
  const Int_t n = 9;
  Double_t x[n] = {30, 64, 90, 120, 150, 180, 210, 230, 260};
  Double_t y[n];
  fileN = "./wfSim_30ns_fit_40MHz_filter.root";
  y[0] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_25um_conv_fit_40MHz_filter.root";
  y[1] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_90ns_fit_40MHz_filter.root";
  y[2] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_120ns_fit_40MHz_filter.root";
  y[3] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_150ns_fit_40MHz_filter.root";
  y[4] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_180ns_fit_40MHz_filter.root";
  y[5] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_210ns_fit_40MHz_filter.root";
  y[6] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_230ns_fit_40MHz_filter.root";
  y[7] = plot(fileN)->Eval(7.0);
  fileN = "./wfSim_260ns_fit_40MHz_filter.root";
  y[8] = plot(fileN)->Eval(7.0);
  //
  TGraph *gr = new TGraph(n,x,y);
  //
  TCanvas *c5 = new TCanvas("c5","c5",10,10,800,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c5->SetRightMargin(0.03);
  c5->SetLeftMargin(0.1);
  c5->SetTopMargin(0.02);
  c5->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gr->SetMarkerStyle(1);
  gr->SetMarkerColor(kBlack);
  gr->SetLineColor(kBlack);
  gr->SetLineWidth(2);
  gr->SetTitle("");
  gr->GetXaxis()->SetTitle("Slow component, ns");
  gr->GetYaxis()->SetTitle("Rate, Hz");
  gr->GetXaxis()->SetRangeUser(0,270);
  gr->GetYaxis()->SetRangeUser(100,100000);
  gr->Draw("APL");
  //
  for(Int_t i = 0;i<9;i++)
    cout<<y[i]<<endl;
  //
  return 0;
}

TGraph* plot(TString fileN){
  //
  TFile *f1 = new TFile(fileN.Data());
  TGraph *_gr_wf_tmpl = (TGraph*)f1->Get("_gr_wf_tmpl");  //
  TH1D *h1_amplLocMax = (TH1D*)f1->Get("h1_amplLocMax");  //
  TGraph *gr_threshold_counter = (TGraph*)f1->Get("gr_threshold_counter");  //
  _gr_wf_tmpl->SetMarkerStyle(1);
  _gr_wf_tmpl->SetMarkerColor(kBlack);
  _gr_wf_tmpl->SetLineColor(kBlack);
  _gr_wf_tmpl->SetLineWidth(2);
  //
  h1_amplLocMax->SetMarkerStyle(1);
  h1_amplLocMax->SetMarkerColor(kBlack);
  h1_amplLocMax->SetLineColor(kBlack);
  h1_amplLocMax->SetLineWidth(2);
  //
  gr_threshold_counter->SetMarkerStyle(1);
  gr_threshold_counter->SetMarkerColor(kBlack);
  gr_threshold_counter->SetLineColor(kBlack);
  gr_threshold_counter->SetLineWidth(2);
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,800,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c1->SetRightMargin(0.03);
  c1->SetLeftMargin(0.1);
  c1->SetTopMargin(0.01);
  c1->SetBottomMargin(0.08);  
  //
  gPad->SetGridx();
  gPad->SetGridy();
  _gr_wf_tmpl->SetTitle("");
  _gr_wf_tmpl->Draw("APL");
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  _gr_wf_tmpl->GetXaxis()->SetTitle("time, ns");
  _gr_wf_tmpl->GetYaxis()->SetTitle("Amplitude, single p.e.");
  //

  TCanvas *c2 = new TCanvas("c2","c2",10,10,800,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c2->SetRightMargin(0.03);
  c2->SetLeftMargin(0.1);
  c2->SetTopMargin(0.01);
  c2->SetBottomMargin(0.08);  
  //
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  //
  h1_amplLocMax->SetTitle("");
  h1_amplLocMax->Draw();
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  //h1_amplLocMax->GetXaxis()->SetTitle("time, ns");
  h1_amplLocMax->GetXaxis()->SetTitle("Amplitude, single p.e.");
  h1_amplLocMax->GetXaxis()->SetRangeUser(0,15);

  TCanvas *c3 = new TCanvas("c3","c3",10,10,800,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c3->SetRightMargin(0.03);
  c3->SetLeftMargin(0.1);
  c3->SetTopMargin(0.02);
  c3->SetBottomMargin(0.08);  
  //
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gr_threshold_counter->SetTitle("");
  gr_threshold_counter->Draw("APL");
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(4.0);
  //mg->SetMaximum(6.0);
  //mg->SetMaximum(5.0);
  gr_threshold_counter->GetYaxis()->SetTitle("Rate, Hz");
  gr_threshold_counter->GetXaxis()->SetTitle("Threshold, single p.e.");
  gr_threshold_counter->GetXaxis()->SetRangeUser(0,15);
  gr_threshold_counter->GetYaxis()->SetRangeUser(1.0,1.0e8);
  //
  //
  //
  //
  TCanvas *c4 = new TCanvas("c4","c4",10,10,1500,950);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  c4->Divide(2,2);
  //
  c4->cd(1);
  gPad->SetRightMargin(0.03);
  gPad->SetLeftMargin(0.1);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  _gr_wf_tmpl->Draw("APL");
  c4->cd(2);
  //
  gPad->SetRightMargin(0.03);
  gPad->SetLeftMargin(0.1);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  h1_amplLocMax->Draw();
  c4->cd(3);
  TLegend *leg = new TLegend(0.1,0.4,0.9,0.7,"","brNDC");
  leg->AddEntry(h1_amplLocMax, fileN.Data(), "p");
  leg->Draw();    
  c4->cd(4);
  gPad->SetRightMargin(0.03);
  gPad->SetLeftMargin(0.1);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.08);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gr_threshold_counter->Draw("APL");
  TString pdfOut = fileN;
  pdfOut += ".pdf";
  c4->SaveAs(pdfOut.Data());
  //
  //
  ///////////////////////
  return gr_threshold_counter;
}
