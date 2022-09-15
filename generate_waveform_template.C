//root
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TMath.h"
#include "TFree.h"
#include "TBranch.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

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

void save_to_csv( TGraph *gr_wf,TString csv_file_out, bool for_gen);
void generate_gaus_gaus_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t a0, Double_t t0_gauss, Double_t gauss01_start,
				 Double_t sigma_gauss_raise, Double_t sigma_gauss_fall, Double_t tau_decay01, Double_t n_tau, Double_t a_decay01_start);
void generate_line_exponent_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t a0, Double_t t0, Double_t line_k, Double_t tau_decay_fast, Double_t tau_decay_slow, Double_t n_tau_slow);
void convolut_with_gaus(TGraph *gr_wf_conv, TGraph *gr_wf, Double_t sigma_to_conv);
void generate_gaus( TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t A0, Double_t t0, Double_t sigma);

Int_t main(int argc, char *argv[]){
  //
  TString fileOut_root = "./wfData/waveform_template_fast_25um.root";
  Double_t dt_ns = 0.25;               //ns
  Double_t time_window_in_ns = 2000.0; //ns
  Double_t sigma_to_conv = 0.5;        //ns
  Double_t a0 =   0.0;                 //V
  Double_t dT_prefix = 500.0;          //ns
  //
  if(argc == 8 && atoi(argv[1])==0){
    fileOut_root = argv[2];
    dt_ns = atof(argv[3]);
    time_window_in_ns = atof(argv[4]);
    sigma_to_conv = atof(argv[5]);
    a0 = atof(argv[6]);
    dT_prefix = atof(argv[7]);
  
  
    //

  Int_t nPoint = (Int_t)time_window_in_ns/dt_ns + 1;
  ////////////////////////
  // gaus gaus exponent
  //slow
  //Double_t gauss01_start = 4.0;      //in sigma
  //Double_t sigma_gauss_raise = 3.0;  //ns
  //Double_t sigma_gauss_fall  = 6.5;  //ns
  //Double_t tau_decay01       = 170;  //ns
  //Double_t n_tau             = 10;   //in decay times
  //Double_t a_decay01_start   = 0.4;  //V  
  //Double_t t_stitching = TMath::Sqrt(-TMath::Log(a_decay01_start)*2)*sigma_gauss_fall + t0_gauss;
  ////////////////////////
  // line exponent exponent 25um
  // 25 um 20 MHz cut off
  //Double_t t0 = dT_prefix;           //ns
  //Double_t line_k = 1/34.0;          //ns^-1
  //Double_t tau_decay_fast = 6.1;     //ns
  //Double_t tau_decay_slow = 64.0;    //ns NUVHD 25um
  //Double_t n_tau_slow = n_tau;
  //sigma_to_conv = 8.0;               //ns
  ////////////////////////
  // 35 um 20 MHz cut off
  //Double_t t0 = dT_prefix;           //ns
  //Double_t line_k = 1/34.0;          //ns^-1
  //Double_t tau_decay_fast = 12.0;    //ns
  //Double_t tau_decay_slow = 150.0;   //ns NUVHD 25um
  //Double_t n_tau_slow = n_tau;
  //sigma_to_conv = 8.0;               //ns
  //
  //gr_wf->SetMarkerStyle(1);
  //gr_wf->SetMarkerColor(kBlack);
  //gr_wf->SetLineColor(kBlack);
  //gr_wf->SetLineWidth(2);
  //
  ///////////////////////
  // 25 um
  Double_t t0_gauss = dT_prefix;       //ns
  Double_t gauss01_start     = 4.0;    //in sigma
  Double_t sigma_gauss_raise = 1.0;    //ns
  Double_t sigma_gauss_fall  = 2.5;    //ns
  Double_t tau_decay01       = 64.0;   //ns
  Double_t n_tau             = 10;     //in decay times
  Double_t a_decay01_start   = 0.49;   //V
  sigma_to_conv = 0.5;                 //ns
  ///////////////////////
  // 25 um
  Double_t t0 = dT_prefix;             //ns
  Double_t line_k = 1/3.0;             //ns^-1
  Double_t tau_decay_fast = 2.0;       //ns
  Double_t tau_decay_slow = 64.0;      //ns NUVHD 25um
  Double_t n_tau_slow = n_tau;
  sigma_to_conv = 0.5;                 //ns
  ////////////////////////
  Double_t A0_gaus = 1.0;
  Double_t t0_gaus = dT_prefix;
  Double_t sigma_gaus_short = 0.1; //ns
  Double_t sigma_gaus_long = 10.0; //ns
  //
}



  ////////////////////////
  //
  TGraph *gr_wf_gge = new TGraph();
  TGraph *gr_wf_lee = new TGraph();
  TGraph *gr_wf_gge_conv = new TGraph();
  TGraph *gr_wf_lee_conv = new TGraph();
  TGraph *gr_wf_gaus_short = new TGraph();
  TGraph *gr_wf_gaus_long = new TGraph();
  gr_wf_gge->SetTitle("gr_wf_gge");
  gr_wf_gge->SetName("gr_wf_gge");
  gr_wf_lee->SetTitle("gr_wf_lee");
  gr_wf_lee->SetName("gr_wf_lee");
  gr_wf_lee_conv->SetTitle("gr_wf_lee_conv");
  gr_wf_lee_conv->SetName("gr_wf_lee_conv");
  gr_wf_gge_conv->SetTitle("gr_wf_gge_conv");
  gr_wf_gge_conv->SetName("gr_wf_gge_conv");
  gr_wf_gaus_short->SetTitle("gr_wf_gaus_short");
  gr_wf_gaus_short->SetName("gr_wf_gaus_short");
  gr_wf_gaus_long->SetTitle("gr_wf_gaus_long");
  gr_wf_gaus_long->SetName("gr_wf_gaus_long");
  //
  generate_gaus_gaus_exponent( gr_wf_gge, dt_ns, nPoint, a0, t0_gauss, gauss01_start, sigma_gauss_raise, sigma_gauss_fall, tau_decay01, n_tau, a_decay01_start);  
  convolut_with_gaus( gr_wf_gge_conv, gr_wf_gge, sigma_to_conv);
  generate_line_exponent_exponent( gr_wf_lee, dt_ns, nPoint, a0, t0, line_k, tau_decay_fast, tau_decay_slow,  n_tau_slow);
  convolut_with_gaus( gr_wf_lee_conv, gr_wf_lee, sigma_to_conv);
  generate_gaus( gr_wf_gaus_short, dt_ns, nPoint, A0_gaus, t0_gaus, sigma_gaus_short);
  generate_gaus( gr_wf_gaus_long, dt_ns, nPoint, A0_gaus, t0_gaus, sigma_gaus_long);


  TFile* rootFile = new TFile(fileOut_root.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    std::cout<<"  ERROR ---> file "<<fileOut_root.Data()<<" is zombi"<<std::endl;
    assert(0);
  }
  else {
    std::cout<<"  Output Histos file ---> "<<fileOut_root.Data()<<std::endl;
  }
  
  //
  gr_wf_gge->Write();
  gr_wf_lee->Write();
  gr_wf_gge_conv->Write();
  gr_wf_lee_conv->Write();
  gr_wf_gaus_short->Write();
  gr_wf_gaus_long->Write();
  rootFile->Close();
  //
  return 0;
}

void save_to_csv( TGraph *gr_wf,TString csv_file_out, bool for_gen = false){
  Double_t t;
  Double_t v_wf;
  std::ofstream csvfile;
  csvfile.open (csv_file_out.Data());
  if(!for_gen){
    csvfile<<"time_ns,v_sig"<<std::endl;
    for(Int_t i = 0;i<gr_wf->GetN();i++){
      gr_wf->GetPoint(i,t,v_wf);
      csvfile<<t<<","<<v_wf<<std::endl;
    }
  }
  else{
    for(Int_t i = 0;i<gr_wf->GetN();i++){
      gr_wf->GetPoint(i,t,v_wf);
      csvfile<<t<<" "<<v_wf<<std::endl;
    }
  }
  csvfile.close();
}

void generate_line_exponent_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t a0, Double_t t0,
				     Double_t line_k, Double_t tau_decay_fast, Double_t tau_decay_slow, Double_t n_tau_slow){
  Double_t b = 1.0 - line_k*t0;
  Double_t x0 = -b/line_k;
  //
  Double_t t;
  Double_t a;
  for(Int_t i = 0;i<nPoint;i++){
    t = dt_ns*i;
    a = a0;
    if(t <= x0)
      a = a0;
    if(t>x0 && t<=t0) 
      a = a0 + line_k*t + b;
    if((t>t0) && ((t-t0) <= n_tau_slow * tau_decay_slow)) 
      a = a0 + 0.5*TMath::Exp(-(t-t0)/tau_decay_fast) + 0.5*TMath::Exp(-(t-t0)/tau_decay_slow);
    else if((t-t0) > n_tau_slow * tau_decay_slow)
      a = a0;
    gr_wf->SetPoint(gr_wf->GetN(),t,a);
  }
}

void generate_gaus( TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t A0, Double_t t0, Double_t sigma){
  Double_t t;
  Double_t a;
  //
  for(Int_t i = 0;i<nPoint;i++){
    t = dt_ns*i;
    a=A0*TMath::Gaus(t,t0,sigma);
    gr_wf->SetPoint(gr_wf->GetN(),t,a);
  }
}

void generate_gaus_gaus_exponent(TGraph *gr_wf, Double_t dt_ns, Int_t nPoint, Double_t a0, Double_t t0_gauss, Double_t gauss01_start,
				 Double_t sigma_gauss_raise, Double_t sigma_gauss_fall, Double_t tau_decay01, Double_t n_tau, Double_t a_decay01_start){
  Double_t t_stitching = TMath::Sqrt(-TMath::Log(a_decay01_start)*2)*sigma_gauss_fall + t0_gauss;
  Double_t t;
  Double_t a;
  //
  for(Int_t i = 0;i<nPoint;i++){
    t = dt_ns*i;
    a = a0;
    if(t >= (t0_gauss - gauss01_start*sigma_gauss_raise) && t<t0_gauss)
      a = a0 + TMath::Gaus(t,t0_gauss,sigma_gauss_raise);
    if(t>=t0_gauss && t<t_stitching) 
      a = a0 + TMath::Gaus(t,t0_gauss,sigma_gauss_fall);
    if((t>=t_stitching) && ((t-t_stitching) < n_tau  * tau_decay01)) 
      a = a0 + a_decay01_start*TMath::Exp(-(t-t_stitching)/tau_decay01);
    else if((t-t_stitching) >= n_tau * tau_decay01)
      a = a0;
    gr_wf->SetPoint(gr_wf->GetN(),t,a);
  }
}

void convolut_with_gaus(TGraph *gr_wf_conv, TGraph *gr_wf, Double_t sigma_to_conv){
  Int_t n = gr_wf->GetN();
  Double_t t;
  Double_t dt = 0.0;
  Double_t t_l = 0.0;
  Double_t t_r = 0.0;
  Double_t t_tmp = 0.0;
  Double_t a;
  Double_t a_max = 0;
  Double_t a_conv;
  gr_wf->GetPoint(0,t_l,a);
  gr_wf->GetPoint(1,t_r,a);
  dt = t_r - t_l;
  for(Int_t i = 0;i<n;i++){
    gr_wf->GetPoint(i,t,a);
    a_conv = 0;
    for(Int_t j = 0;j<n;j++){
      gr_wf->GetPoint(j,t_tmp,a);
      a_conv += a*TMath::Gaus(t_tmp,t,sigma_to_conv)*dt;
    }
    if(a_max<a_conv)
      a_max=a_conv;
    gr_wf_conv->SetPoint(gr_wf_conv->GetN(),t,a_conv);
  }
  if(a_max>0.0){
    for(Int_t i = 0;i<n;i++){
      gr_wf_conv->GetPoint(i,t,a);
      gr_wf_conv->SetPoint(i,t,a/a_max);
    }
  }
}
