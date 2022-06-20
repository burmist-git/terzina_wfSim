#pragma once

//root
#include <TObject.h>
#include <TH2Poly.h>
#include <TGraph.h>
#include <TVector2.h>

//c, c++
#include <string>
#include <vector>
#include <map>

class sipmAvalancheTree: public TH2Poly {
 public:

  ~sipmAvalancheTree() {};
  sipmAvalancheTree(const char* name, const char* title);
  void DrawHisto(TString opt = "ZCOLOT text same", TString outTreeFileName = "./theTree.pdf");
  void test();
  void test2();  
  void test3();
  
protected:

  void SetUpVerticesMap();
  void dumpVerticesMap();
  void drawLine(TVector2 nodeC,TVector2 parrentC, double rnode, double rparrent, Int_t type_pe);
  
  int m_verboseLevel;
  int m_nCircularPoints;
  double m_window_H;
  double m_window_W;
  double m_window_x0;
  double m_window_y0;

  std::map<Int_t, std::vector<TVector2>> m_verticesMap;
  std::vector<TVector2> m_centerVec;
  std::vector<double> m_rVec;
  
  TString m_histName;
  TString m_histTitle;

};
