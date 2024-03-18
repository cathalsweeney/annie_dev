#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

void plot_3()
{

  TFile *inFile = new TFile("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");
  //TFile *inFile = new TFile("240313_ForceTriggering_20mVThreshold_80k_LaserON_Analysis.root");

  double t_max = 21600;
  //double t_max = 4000;
  int nHist = 20;
  double minHeight = -20.0;
  
  
  TTree *myTree;
  myTree = (TTree*)inFile->Get("ffmytree");
  int nChannels = 60;
  
  //  myTree->Print();
 
  int NHits;
  double xperp[1];
  double xpar[1];
  double StripPeak[nChannels];

  int  Npulses_simp;
  int pulsestrip_simp[100];
  int pulseside_simp[100];
  double EventTime;


  myTree->SetBranchAddress("NHits",&NHits);
  myTree->SetBranchAddress("xpar",xpar);
  myTree->SetBranchAddress("xperp",xperp);
  myTree->SetBranchAddress("StripPeak",StripPeak);

  
  myTree->SetBranchAddress("Npulses_simp",&Npulses_simp);
  myTree->SetBranchAddress("EventTime",&EventTime);
  myTree->SetBranchAddress("pulsestrip_simp",pulsestrip_simp);
  myTree->SetBranchAddress("pulseside_simp",pulseside_simp);


  double period = (double) t_max / nHist;    

  std::vector<TH1F*> vHist;
  for(int iHist=0; iHist<nHist; iHist++){
    std::string name = "hist" + std::to_string(iHist);
    TH1F *hist  = new TH1F(name.c_str(), name.c_str(), 60,0,60);
    vHist.push_back(hist);
  }

  int entries = myTree->GetEntries();
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);

    
    int iHist = std::floor(EventTime / period);
    if(iHist >= nHist) continue;    
      

    for(int j=0; j<nChannels; j++){
      if(StripPeak[j] <  minHeight){
	vHist[iHist]->Fill(j);
      }
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
    gMap[4]->GetYaxis()->SetRangeUser(0., 0.4);
    
  gMap[4]->SetLineColor(kRed);
  gMap[11]->SetLineColor(kBlue);
  gMap[13]->SetLineColor(kBlack);
  gMap[16]->SetLineColor(kGreen+2);
			 
  gMap[4]->Draw("APL");
  gMap[11]->Draw("PL SAME");
  gMap[13]->Draw("PL SAME");
  gMap[16]->Draw("PL SAME");  
  //  c->SetLogy();
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

