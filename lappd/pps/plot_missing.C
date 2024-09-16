// Plots the difference in clock ticks between successive PPS events

#include "TAxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TLine.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

void PlotAll(std::vector<std::pair<TH1F*, TH1F*>> vHist)
{

  TCanvas* c = new TCanvas("", "", 1300,600);
  //  c->SetLogy(true);

  c->Range(0.,0., 1.,1.);

  TLine	*line1 = new TLine(1./3., 0, 1./3., 1);
  TLine	*line2 = new TLine(2./3., 0, 2./3., 1);
  line1->SetLineColor(kBlack);
  line1->SetLineWidth(2);
  line1->Draw();
  line2->Draw();


  // Divide the canvas into three vertical segments
  c->Divide(3, 2);  // 3 columns, 2 rows
  
  // Plot in each segment
  c->cd(1);
  gPad->SetLogy(true);
  vHist[0].first->Draw(); 
  c->cd(4);
  gPad->SetLogy(true);
  vHist[0].second->Draw();
  
  c->cd(2);
  gPad->SetLogy(true);
  vHist[1].first->Draw(); 
  c->cd(5);
  gPad->SetLogy(true);
  vHist[1].second->Draw(); 
  
  c->cd(3);
  gPad->SetLogy(true);
  vHist[2].first->Draw(); 
  c->cd(6);
  gPad->SetLogy(true);
  vHist[2].second->Draw(); 
 
  
  // Update the canvas
  c->Update();

  c->SaveAs("all.png");
}

void plot_missing()
{
  
  //TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/4765/LAPPDTree.root");
  TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/5007/LAPPDTree.root");

  TTree *myTree;
  myTree = (TTree*)inFile->Get("TimeStamp");

  int entries = myTree->GetEntries();
  
  ULong64_t ppsTime0;
  ULong64_t ppsTime1;
  int LAPPD_ID;
  
  Long64_t xLo = -4e8;
  Long64_t xHi = 7e8;
  int nBins = 100;
  
//  Long64_t xLo = 320000000 - 15;
//  Long64_t xHi = 320000000 + 15;
//  int nBins = 30;
  
  myTree->SetBranchAddress("ppsTime0",&ppsTime0);
  myTree->SetBranchAddress("ppsTime1",&ppsTime1);
  myTree->SetBranchAddress("LAPPD_ID",&LAPPD_ID);

  std::string xLabel = "#Delta t_{pps} (clock ticks)";
  std::string yLabel = "Events";
  double yLo = 5e-1;
  double yHi = 1e6;
  
  std::vector<std::pair<TH1F*,TH1F*>> hDiff_vec; // pair of hists {acdc0, acdc1} for each LAPPD ID
  int nLAPPD = 3;
  for(int i=0; i<nLAPPD; i++){
    TH1F *hACDC0  = new TH1F("", "", nBins, xLo, xHi);
    hACDC0->GetXaxis()->CenterTitle(true);
    hACDC0->GetYaxis()->CenterTitle(true);
    hACDC0->GetXaxis()->SetTitle(xLabel.c_str());
    hACDC0->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC0->GetYaxis()->SetRangeUser(yLo, yHi);
    hACDC0->SetTitle("ACDC 0");

    TH1F *hACDC1  = new TH1F("", "", nBins, xLo, xHi);
    hACDC1->GetXaxis()->CenterTitle(true);
    hACDC1->GetYaxis()->CenterTitle(true);
    hACDC1->GetXaxis()->SetTitle(xLabel.c_str());
    hACDC1->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC1->GetYaxis()->SetRangeUser(yLo, yHi);
    hACDC1->SetTitle("ACDC 1");


    hDiff_vec.push_back({hACDC0, hACDC1});
  }

  
  std::pair<Long64_t, Long64_t> pps_prev = {0, 0}; // pps timestamp from the previous event {ACDC0, ACDC1}
  std::pair<Long64_t, Long64_t> pps_current = {0, 0}; // pps timestamp from the current event {ACDC0, ACDC1}

  std::pair<Long64_t, Long64_t> diff = {0, 0};
  for(int i=0; i<entries; i++){
    diff = {0,0};
    pps_prev = pps_current; 
    myTree->GetEntry(i);
    if(ppsTime0 == 0 || ppsTime1 == 0) std::cout << "FOO A \n";

    pps_current = {ppsTime0, ppsTime1};
    diff.first = pps_current.first - pps_prev.first;
    diff.second = pps_current.second - pps_prev.second;    

    hDiff_vec[LAPPD_ID].first->Fill(diff.first);
    hDiff_vec[LAPPD_ID].second->Fill(diff.second);
//
//    if(diff.first > xHi || diff.first < xLo){
//      std::cout << pps_prev.first << " : " << pps_current.first << " : " << diff.first <<  "\n";
//    }
//    if(diff.second > xHi || diff.second < xLo){
//      std::cout << pps_prev.second << " : " << pps_current.second << " : " << diff.second <<  "\n";
//    }


    
  }

  PlotAll(hDiff_vec);
  
//  TCanvas* c = new TCanvas();
//  c->SetLogy(true);
//
//  std::string xLabel = "#Delta t_{pps} (clock ticks)";
//  std::string yLabel = "Events";
//  double yLo = 5e-1;
//  double yHi = 1e6;
//  for(int i=0; i<nLAPPD; i++){
//    hDiff_vec[i].first->GetXaxis()->CenterTitle(true);
//    hDiff_vec[i].first->GetYaxis()->CenterTitle(true);
//    hDiff_vec[i].first->GetXaxis()->SetTitle(xLabel.c_str());
//    hDiff_vec[i].first->GetYaxis()->SetTitle(yLabel.c_str());
//    hDiff_vec[i].first->GetYaxis()->SetRangeUser(yLo, yHi);
//    hDiff_vec[i].first->SetTitle("ACDC 0");
//    hDiff_vec[i].first->Draw("HIST");
//    c->SaveAs( (std::to_string(i)+"_0.png").c_str() );
//
//    hDiff_vec[i].second->GetXaxis()->CenterTitle(true);
//    hDiff_vec[i].second->GetYaxis()->CenterTitle(true);
//    hDiff_vec[i].second->GetXaxis()->SetTitle(xLabel.c_str());
//    hDiff_vec[i].second->GetYaxis()->SetTitle(yLabel.c_str());
//    hDiff_vec[i].second->GetYaxis()->SetRangeUser(yLo, yHi);
//    hDiff_vec[i].second->SetTitle("ACDC 1");
//    hDiff_vec[i].second->Draw("HIST");
//    c->SaveAs( (std::to_string(i)+"_1.png").c_str() );
//  }
}

