// Plots the difference in clock ticks between successive PPS events

#include "TAxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TLine.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

int nLAPPD = 3;

void PlotAll(std::vector<std::pair<TH1F*, TH1F*>> vHist, std::string outname)
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

  double textSize = 0.06;
  // Plot in each segment
  c->cd(1);
  gPad->SetLogy(true);
  vHist[0].first->Draw(); 
  TLatex* text1 = new TLatex(-3e8,800,"foo");
  text1->SetTextSize(textSize);
  text1->Draw("SAME");
  TLatex* text2 = new TLatex(-3e8,200,"bar");
  text2->SetTextSize(textSize);
  text2->Draw("SAME");
  TLatex* text3 = new TLatex(-3e8,50,"bar");
  text3->SetTextSize(textSize);
  text3->Draw("SAME");
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

  c->SaveAs(outname.c_str() );
  
  for(int i=0; i<3; i++){
    std::cout << vHist[i].first->Integral(0,101) << "\n";
    std::cout << vHist[i].second->Integral(0,101) << "\n";
  }
}

void plot_missing()
{
  
  //TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/4765/LAPPDTree.root");
  TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/5007/LAPPDTree.root");
  //TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/5008/LAPPDTree.root");

  TTree *myTree;
  myTree = (TTree*)inFile->Get("TimeStamp");

  int entries = myTree->GetEntries();
  
  ULong64_t ppsTime0;
  ULong64_t ppsTime1;
  int LAPPD_ID;
  
  
  myTree->SetBranchAddress("ppsTime0",&ppsTime0);
  myTree->SetBranchAddress("ppsTime1",&ppsTime1);
  myTree->SetBranchAddress("LAPPD_ID",&LAPPD_ID);

  std::string xLabel = "#Delta t_{pps} (clock ticks)";
  std::string yLabel = "Events";
  Long64_t xLo = -4e8;
  Long64_t xHi = 7e8;
  int nBins = 100;
  double yLo = 5e-1;
  double yHi = 1e6;


  std::string xLabel_zoom = "#Delta t_{pps} - 3.2e8 (clock ticks)";
  Long64_t xLo_zoom = -15;
  Long64_t xHi_zoom = 15;
  int nBins_zoom = xHi_zoom - xLo_zoom;
  double yLo_zoom = 5e-1;
  double yHi_zoom = 1e6;
  
  std::vector<std::pair<TH1F*,TH1F*>> hDiff_vec; // pair of hists {acdc0, acdc1} for each LAPPD ID
  std::vector<std::pair<TH1F*,TH1F*>> hDiff_vec_zoom; // same thing, but we subtract 3.2e8 (= 1s) from entries
  for(int i=0; i<nLAPPD; i++){
    TH1F *hACDC0  = new TH1F("", "", nBins, xLo, xHi);
    hACDC0->GetXaxis()->CenterTitle(true);
    hACDC0->GetYaxis()->CenterTitle(true);
    hACDC0->GetXaxis()->SetTitle(xLabel.c_str());
    hACDC0->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC0->GetYaxis()->SetRangeUser(yLo, yHi);    
    hACDC0->SetTitle("ACDC 0");
    TH1F* hACDC1 = (TH1F*) hACDC0->Clone();
    hACDC1->SetTitle("ACDC 1");    
    hDiff_vec.push_back({hACDC0, hACDC1});

    TH1F *hACDC0_zoom  = new TH1F("", "", nBins_zoom, xLo_zoom, xHi_zoom);
    hACDC0_zoom->GetXaxis()->CenterTitle(true);
    hACDC0_zoom->GetYaxis()->CenterTitle(true);
    hACDC0_zoom->GetXaxis()->SetTitle(xLabel_zoom.c_str());
    hACDC0_zoom->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC0_zoom->GetYaxis()->SetRangeUser(yLo_zoom, yHi_zoom);    
    hACDC0_zoom->SetTitle("ACDC 0");
    TH1F* hACDC1_zoom = (TH1F*) hACDC0_zoom->Clone();
    hACDC1_zoom->SetTitle("ACDC 1");    
    hDiff_vec_zoom.push_back({hACDC0_zoom, hACDC1_zoom});


  }


  std::vector<std::pair<std::vector<int>,std::vector<int> > > vCounts; // count number of events where delta_t is 0, 3.2e8, or other
  for(int i=0; i<nLAPPD; i++){
    std::pair<std::vector<int>,std::vector<int> > pair = { {0,0,0} , {0,0,0} };
    vCounts.push_back(pair);
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

    hDiff_vec_zoom[LAPPD_ID].first->Fill(diff.first - 3.2e8);
    hDiff_vec_zoom[LAPPD_ID].second->Fill(diff.second - 3.2e8);

    if(diff.first == 0) vCounts[LAPPD_ID].first.at(0)++;
    else if(diff.first == 3.2e8) vCounts[LAPPD_ID].first.at(1)++;
    else vCounts[LAPPD_ID].first.at(2)++;

    if(diff.second == 0) vCounts[LAPPD_ID].second.at(0)++;
    else if(diff.second == 3.2e8) vCounts[LAPPD_ID].second.at(1)++;
    else vCounts[LAPPD_ID].second.at(2)++;


    
    //
//    if(diff.first > xHi || diff.first < xLo){
//      std::cout << pps_prev.first << " : " << pps_current.first << " : " << diff.first <<  "\n";
//    }
//    if(diff.second > xHi || diff.second < xLo){
//      std::cout << pps_prev.second << " : " << pps_current.second << " : " << diff.second <<  "\n";
//    }

    
  }


  for(int i=0; i<nLAPPD; i++){
    for(int j : vCounts[i].first) std::cout << j << ", ";
    std::cout << "\n";
    for(int j : vCounts[i].second) std::cout << j << ", ";
    std::cout << "\n";
  }

  
  PlotAll(hDiff_vec, "all.png");
  std::cout << "------------------- \n";
  PlotAll(hDiff_vec_zoom, "all_zoom.png");
}

