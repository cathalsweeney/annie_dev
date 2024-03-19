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

  TFile *inFile = new TFile("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");
  //TFile *inFile = new TFile("240313_ForceTriggering_20mVThreshold_80k_LaserON_Analysis.root");
  //TFile *inFile = new TFile("first402_Analysis.root");

  int nBins = 3;
  std::vector<int> vStrip = {4, 11, 13, 16};  
  
  TTree *myTree;
  myTree = (TTree*)inFile->Get("ffmytree");
  int entries = myTree->GetEntries();
  
  //  myTree->Print();
 
  int  Npulses_simp;
  int pulsestrip_simp[100];
  int pulseside_simp[100];
  double EventTime;

  
  myTree->SetBranchAddress("Npulses_simp",&Npulses_simp);
  myTree->SetBranchAddress("EventTime",&EventTime);
  myTree->SetBranchAddress("pulsestrip_simp",pulsestrip_simp);
  myTree->SetBranchAddress("pulseside_simp",pulseside_simp);



  std::map<int, TH1F*> vHist;
  for(int strip : vStrip){
    std::string name = "hist_" + std::to_string(strip);
    TH1F *hist  = new TH1F(name.c_str(), name.c_str(), nBins,0, entries);
    vHist[strip] = hist;
  }
  
  for(int i=0; i<entries; i++){

    //    if(i < 189) continue;
    
    myTree->GetEntry(i);
    
    //    if(Npulses_simp < 1) continue;          
    
    for(int j=0; j<Npulses_simp; j++){
      //if(pulseside_simp[j] != 0) continue;
      //if(pulseside_simp[j] != 1) continue;

      int stripNum = pulsestrip_simp[j];
      int cnt = std::count(vStrip.begin(), vStrip.end(), stripNum); 
      if(cnt != 1 ) continue; 
      vHist[stripNum]->Fill(i);
    }
    
  }// end loop over events



  TCanvas* c = new TCanvas();

  int nHist = vHist.size();
  
//  for(int iHist=0; iHist<nHist; iHist++){
//    //    vHist[iHist]->GetYaxis()->SetRangeUser(0, 500);
//    std::string name = "hist" + std::to_string(iHist) + ".png";
//    vHist[iHist]->Draw("HIST");
//    c->SaveAs(name.c_str());
//    
//  }

  double ymax = 3000 / nBins; 
  
  vHist[4]->GetYaxis()->SetRangeUser(0., ymax);
  vHist[4]->GetYaxis()->SetTitle("nPulses");
  vHist[4]->GetXaxis()->SetTitle("Event number");
  //vHist[4]->SetTitle("Pulse rate in side 0");
  vHist[4]->GetYaxis()->CenterTitle(true);
  vHist[4]->GetXaxis()->CenterTitle(true);
  
  vHist[4]->SetLineColor(kRed);
  vHist[11]->SetLineColor(kBlue);
  vHist[13]->SetLineColor(kBlack);
  vHist[16]->SetLineColor(kGreen+2);
			 
  vHist[4]->Draw("HIST");
  vHist[11]->Draw("HIST SAME");
  vHist[13]->Draw("HIST SAME");
  vHist[16]->Draw("HIST SAME");  
  //  c->SetLogy();
  TLegend* legend = new TLegend(0.7,0.7, 0.9,0.9);
  legend->AddEntry(vHist[4],"Strip 4","l");
  legend->AddEntry(vHist[11],"Strip 11","l");
  legend->AddEntry(vHist[13],"Strip 13","l");
  legend->AddEntry(vHist[16],"Strip 16","l");

  legend->Draw("SAME");
  c->SaveAs("hist.png");


  
}

