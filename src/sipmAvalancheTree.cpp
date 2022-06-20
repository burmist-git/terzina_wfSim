/*******************************
* Thu Jun 16 11:16:49 CET 2022 *
* Autor: Leonid Burmistrov     *
********************************/

//my
#include <sipmAvalancheTree.hh>

//c, c++
#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <vector>

//root
#include <TVector2.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TText.h>
#include <TMath.h>
#include <TH2F.h>
#include <TLegend.h>
#include <TCrown.h>
#include <TArc.h>
#include <TROOT.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TPad.h>
#include <TString.h>
#include <TFile.h>
#include <TAxis.h>
#include <TVector2.h>
#include <TPaletteAxis.h>

using namespace std;

sipmAvalancheTree::sipmAvalancheTree(const char* name, const char* title) : TH2Poly()
{
  m_histName = name;
  m_histTitle = title;
  SetName(m_histName.Data());
  SetTitle(m_histTitle.Data());
  //
  m_verboseLevel = 0;
  m_nCircularPoints = 100;
  m_window_H = 1090.0;
  m_window_W = 810.0;
  m_window_x0 = 0.0;
  m_window_y0 = 0.0;
  //
  SetUpVerticesMap();
  //
  if (m_verboseLevel > 0) {
    std::cout<<"m_verboseLevel      "<< m_verboseLevel << std::endl
	     <<"  m_nCircularPoints "<<  m_nCircularPoints << std::endl
	     <<"  m_window_H        "<< m_window_H << std::endl
	     <<"  m_window_W        "<< m_window_W << std::endl
	     <<"  m_window_x0       "<< m_window_x0 << std::endl
	     <<"  m_window_y0       "<< m_window_y0 << std::endl;
    dumpVerticesMap();
  }
  //
  unsigned int n;
  double* x;
  double* y;
  //
  for (auto& m : m_verticesMap) {
    n = m.second.size();
    x = new double [n];
    y = new double [n];
    //Loop over polygonal points which defines bins.
    for (unsigned int i = 0; i < n; i++) {
      x[i] = m.second[i].X();
      y[i] = m.second[i].Y();
    }
    AddBin(n, x, y);
    delete []x;
    delete []y;
  }
}

void sipmAvalancheTree::DrawHisto(TString opt, TString outTreeFileName){
  //
  TCanvas* c1 = new TCanvas("c1", "c1", m_window_H, m_window_W);
  c1->SetTitle(m_histTitle.Data());
  c1->SetName(m_histName.Data());
  //c1->SetRightMargin(0.17);
  //c1->SetLeftMargin(0.12);
  //c1->SetTopMargin(0.15);
  //c1->SetBottomMargin(0.15);
  c1->SetRightMargin(0.0);
  c1->SetLeftMargin(0.0);
  c1->SetTopMargin(0.0);
  c1->SetBottomMargin(0.0);
  //
  //gStyle->SetOptStat(0);
  //gStyle->SetPalette(10);
  //
  double frame_x_min = m_window_x0 - m_window_W/2.0;
  double frame_x_max = m_window_x0 + m_window_W/2.0;;
  double frame_y_min = m_window_y0 - m_window_H/2.0;
  double frame_y_max = m_window_y0 + m_window_H/2.0;
  //  
  TH2F* frame = new TH2F("h2", "h2", 40, frame_x_min, frame_x_max, 40, frame_y_min, frame_y_max);
  //TH2F *frame = new TH2F("h2","h2",40, 400, 650, 40, -50, 200);
  //frame->GetXaxis()->SetTitle("x (mm)");
  //frame->GetYaxis()->SetTitle("y (mm)");
  //frame->GetXaxis()->CenterTitle();
  //frame->GetYaxis()->CenterTitle();
  //frame->GetYaxis()->SetTitleOffset(1.5);
  frame->SetStats(kFALSE);
  frame->SetTitle("");
  frame->Draw();
  //  
  frame->GetXaxis()->SetLabelSize(0);
  frame->GetYaxis()->SetLabelSize(0);
  frame->GetXaxis()->SetTickLength(0);
  frame->GetYaxis()->SetTickLength(0);
  //
  SetMarkerSize(0.3);
  //
  Draw(opt.Data());
  //////////
  //
  for (unsigned int i = 0; i < m_centerVec.size(); i++) {
    //std::cout<<"m_centerVec.size() "<<m_centerVec.size()<<std::endl;
    int nodeID = i+1;
    if(nodeID>1){
      int type_pe;
      if ( nodeID % 2 == 0)
	type_pe = 1;
      else
	type_pe = 2;
      int parrentID = (nodeID - type_pe)/2+1;
      //std::cout<<"nodeID    "<<nodeID<<std::endl
      //       <<"parrentID "<<parrentID<<std::endl;
      drawLine(m_centerVec.at(nodeID-1),m_centerVec.at(parrentID-1), m_rVec.at(nodeID-1), m_rVec.at(parrentID-1),type_pe);
    }
  }
  //////////
  c1->Modified();
  c1->Update();
  if (outTreeFileName.Length() > 0) {
    std::cout << "outTreeFileName.Length() " << outTreeFileName.Length() << std::endl;
    TString outnamePDF = outTreeFileName;
    //TString outnameEPS = outDirName; outnameEPS += m_histName; outnameEPS += ".eps";
    c1->SaveAs(outnamePDF.Data());
    //c1->SaveAs(outnameEPS.Data());
  }
}

void sipmAvalancheTree::drawLine(TVector2 nodeC,TVector2 parrentC, double rnode, double rparrent, Int_t type_pe){
  TVector2 d_r = parrentC - nodeC;
  TVector2 r_b = nodeC + d_r*(rnode/d_r.Mod());
  TVector2 r_e = parrentC - d_r*(rparrent/d_r.Mod());
  TLine *ln = new TLine( r_b.X(), r_b.Y(), r_e.X(), r_e.Y());
  //ln->SetLineWidth(3.0);
  ln->SetLineWidth(2.0);
  if(type_pe == 2)
    ln->SetLineColor(kRed);
  ln->Draw("same");
}

void sipmAvalancheTree::dumpVerticesMap(){
  unsigned int i;
  for (auto& m : m_verticesMap) {
    std::cout << " --> Tree vertex : " << m.first << '\n';
    for (i = 0; i < m.second.size(); i++) {
      std::cout << "                    " << setw(15) << m.second[i].X() << setw(15) << m.second[i].Y() << std::endl;
    }
  }
}

void sipmAvalancheTree::SetUpVerticesMap(){
  double phi_min = 0.0*TMath::Pi()/180.0;
  double phi_max = 360.0*TMath::Pi()/180.0;
  double dphi = (phi_max - phi_min)/(m_nCircularPoints - 1);
  double phi;
  const int nG = 7;
  double r[nG] = {60, 40, 30, 20, 15, 10, 5};
  int nNodes[nG] = {(int)TMath::Power(2,0),
		    (int)TMath::Power(2,1),
		    (int)TMath::Power(2,2),
		    (int)TMath::Power(2,3),
		    (int)TMath::Power(2,4),
		    (int)TMath::Power(2,5),
		    (int)TMath::Power(2,6)};
  double pitch_x[nG] = {0, 200, 120, 80, 45, 25, 12.5};
  double pitch_y[nG] = { 0,
			 0 + 200*1.3,
			 0 + 200*1.3 + 150*1.3,
			 0 + 200*1.3 + 150*1.3 + 130*1.3,
			 0 + 200*1.3 + 150*1.3 + 130*1.3 + 120*1.3,
			 0 + 200*1.3 + 150*1.3 + 130*1.3 + 120*1.3 + 90*1.3,
  			 0 + 200*1.3 + 150*1.3 + 130*1.3 + 120*1.3 + 90*1.3 + 55};
  TVector2 r0_global(0.0,450);
  int nodesID = 0;
  for (int i = 0; i < nG; i++) {
    for (int k = 0; k < nNodes[i]; k++) {
      TVector2 r0(((-nNodes[i]/2)*pitch_x[i] + pitch_x[i]/2.0 + pitch_x[i]*k),(-pitch_y[i]));
      TVector2 r_center = r0 + r0_global;
      m_centerVec.push_back(r_center);
      m_rVec.push_back(r[i]);
      std::vector<TVector2> vecTvec;
      for (int j = 0; j < m_nCircularPoints; j++) {
	phi = phi_min + dphi*j;
	TVector2 v0;
	v0.SetMagPhi(r[i], phi);
	TVector2 v;
	v = v0 + r0 + r0_global;
	vecTvec.push_back(v);
      }  
      m_verticesMap[nodesID] = vecTvec;
      nodesID++;
    }
  }
  std::cout<<"nodesID = "<<nodesID<<std::endl;
}

void sipmAvalancheTree::test(){
  std::cout<<"GetNcells() = "<<GetNcells()<<std::endl;
  for(int i = 0;i<GetNcells();i++)
    SetBinContent(i+1,i+1);
  DrawHisto("ZCOLOT text same", "./theTree_test_01.pdf");
}

void sipmAvalancheTree::test2(){
  TRandom3 *rnd = new TRandom3(131231);
  unsigned int cellID; 
  for(int i = 0;i<10000;i++){
    cellID = (Int_t)rnd->Uniform(0.0,130.0);
    if(cellID>0 && cellID<128)
      SetBinContent(cellID,(GetBinContent(cellID)+1));
  }
  DrawHisto("ZCOLOT text same", "./theTree_test_02.pdf");
}

void sipmAvalancheTree::test3(){
  SetMinimum(1);
  for(int i = 0;i<GetNcells();i++)
    SetBinContent(i+1,0);
  DrawHisto("ZCOLOT text same", "./theTree_test_03.pdf");
}
