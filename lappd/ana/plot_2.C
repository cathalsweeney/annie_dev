#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

void plot_2()
{

  //TFile *inFile = new TFile("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");
  //TFile *inFile = new TFile("240313_ForceTriggering_20mVThreshold_80k_LaserON_Analysis.root");
  TFile *inFile = new TFile("first402_Analysis.root");

  //double t_max = 21600;
  //int nHist = 20;
    
  double t_max = 5955;
  int nHist = 6;



  
  
  TTree *myTree;
  myTree = (TTree*)inFile->Get("ffmytree");

  //  myTree->Print();
 
  int  Npulses_simp;
  int pulsestrip_simp[100];
  int pulseside_simp[100];
  double EventTime;

  
  myTree->SetBranchAddress("Npulses_simp",&Npulses_simp);
  myTree->SetBranchAddress("EventTime",&EventTime);
  myTree->SetBranchAddress("pulsestrip_simp",pulsestrip_simp);
  myTree->SetBranchAddress("pulseside_simp",pulseside_simp);


  double period = (double) t_max / nHist;    

  std::vector<TH1F*> vHist;
  for(int iHist=0; iHist<nHist; iHist++){
    std::string name = "hist" + std::to_string(iHist);
    TH1F *hist  = new TH1F(name.c_str(), name.c_str(), 30,0,30);
    vHist.push_back(hist);
  }

  
//  TH1F *myHisto_1  = new TH1F("hist1", "hist1", 30,0,30);
//  TH1F *myHisto_2  = new TH1F("hist2", "hist2", 30,0,30);
//  TH1F *myHisto_3  = new TH1F("hist3", "hist3", 30,0,30);
//  TH1F *myHisto_4  = new TH1F("hist4", "hist4", 30,0,30);

  int entries = myTree->GetEntries();
  
  for(int i=0; i<entries; i++){

    if(i < 189) continue;
    
    myTree->GetEntry(i);
    
    if(Npulses_simp < 1) continue;    

    int iHist = std::floor(EventTime / period);
    if(iHist >= nHist) continue;    
//    if(i % 100 == 0 ){
//      std::cout << period << " " << EventTime << " " << iHist << "\n";
//    }
      

    for(int j=0; j<Npulses_simp; j++){
      if(pulseside_simp[j] != 0) continue;
      //if(pulseside_simp[j] != 1) continue;
      vHist[iHist]->Fill(pulsestrip_simp[j]);
    }
    

  }



  TCanvas* c = new TCanvas();

  std::vector<int> vStrip = {4, 11, 13, 16};
  std::map<int,TGraphErrors*> gMap;

  for(int strip : vStrip){
    TGraphErrors* g = new TGraphErrors();
    gMap[strip] = g;
  }
    
  for(int iHist=0; iHist<nHist; iHist++){
    vHist[iHist]->GetYaxis()->SetRangeUser(0, 500);
    std::string name = "hist" + std::to_string(iHist) + ".png";
    vHist[iHist]->Draw("HIST");
    c->SaveAs(name.c_str());


    for(int strip : vStrip){    
      double val = vHist[iHist]->GetBinContent(strip); 
      double rate = val / period;
      double rate_err = rate / sqrt(val);
      double t = (iHist + 0.5)*period;
      double t_err = 0.5*period;
      gMap[strip]->SetPoint(iHist, t, rate);
      gMap[strip]->SetPointError(iHist, t_err, rate_err);    

      //      if(strip == 11) std::cout << rate << "\n";
    }
    
  }

  //  gMap[4]->GetYaxis()->SetRangeUser(0.01, 15.0);
  gMap[4]->GetYaxis()->SetRangeUser(0., 0.1);
  gMap[4]->GetYaxis()->SetTitle("Pulse rate (Hz)");
  gMap[4]->GetXaxis()->SetTitle("Time (s)");
  gMap[4]->SetTitle("Pulse rate in side 0");
  gMap[4]->GetYaxis()->CenterTitle(true);
  gMap[4]->GetXaxis()->CenterTitle(true);
  
  gMap[4]->SetLineColor(kRed);
  gMap[11]->SetLineColor(kBlue);
  gMap[13]->SetLineColor(kBlack);
  gMap[16]->SetLineColor(kGreen+2);
			 
  gMap[4]->Draw("APL");
  gMap[11]->Draw("PL SAME");
  gMap[13]->Draw("PL SAME");
  gMap[16]->Draw("PL SAME");  
  //  c->SetLogy();
  TLegend* legend = new TLegend(0.7,0.7, 0.9,0.9);
  legend->AddEntry(gMap[4],"Strip 4","l");
  legend->AddEntry(gMap[11],"Strip 11","l");
  legend->AddEntry(gMap[13],"Strip 13","l");
  legend->AddEntry(gMap[16],"Strip 16","l");

  legend->Draw("SAME");
  c->SaveAs("graph.png");


  
//  myHisto_1->GetYaxis()->SetRangeUser(0, 2000);
//  myHisto_2->GetYaxis()->SetRangeUser(0, 2000);
//
//  
//  //  myHisto_1->GetYaxis()->SetRangeUser(0, 900);
//  //  myHisto_2->GetYaxis()->SetRangeUser(0, 900);
//  myHisto_3->GetYaxis()->SetRangeUser(0, 900);
//  myHisto_4->GetYaxis()->SetRangeUser(0, 900);
//  
//  myHisto_1->Draw("HIST");
//  c->SaveAs("hist1.png");
//  
//  myHisto_2->Draw("HIST");
//  c->SaveAs("hist2.png");
//  
//  myHisto_3->Draw("HIST");
//  c->SaveAs("hist3.png");
//  
//  myHisto_4->Draw("HIST");
//  c->SaveAs("hist4.png");
}

