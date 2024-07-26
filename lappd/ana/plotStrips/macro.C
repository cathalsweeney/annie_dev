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

int nStrip = 30;
int nHist= 30*2;

std::vector<TH1F*> makeHist(std::string inName)
{  
  TFile *inFile = new TFile(inName.c_str());
    
  TTree *myTree;
  myTree = (TTree*)inFile->Get("ffmytree");
  int nChannels = 60;
   
  int NHits;
  int timestamps_meta;
  double xperp[1];
  double xpar[1];
  double StripPeak[nChannels];

  int  Npulses_simp;
  int pulsestrip_simp[100];
  int pulseside_simp[100];
  double EventTime;

  myTree->SetBranchAddress("timestamps_meta",&timestamps_meta);
  myTree->SetBranchAddress("NHits",&NHits);
  myTree->SetBranchAddress("xpar",xpar);
  myTree->SetBranchAddress("xperp",xperp);
  myTree->SetBranchAddress("StripPeak",StripPeak);

  
  myTree->SetBranchAddress("Npulses_simp",&Npulses_simp);
  myTree->SetBranchAddress("EventTime",&EventTime);
  myTree->SetBranchAddress("pulsestrip_simp",pulsestrip_simp);
  myTree->SetBranchAddress("pulseside_simp",pulseside_simp);

  std::vector<TH1F*> vHist;
  for(int i=0; i <nHist; i++){
    //    TH1F *hist  = new TH1F("", "", 100,0,100);
    TH1F *hist  = new TH1F("", "", 100,0,500); 
    vHist.push_back(hist);
  }
  
  int entries = myTree->GetEntries();
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);

    for(int iStrip=0; iStrip<nStrip; iStrip++){
      vHist[2*iStrip]->Fill(-StripPeak[iStrip]);
      vHist[2*iStrip +1]->Fill(-StripPeak[iStrip+30]);
    }

  }// end loop over events
  
  return vHist;
} // end makeHist()


void macro()
{  
  std::vector<TH1F*> vHist = makeHist("../files/LAPPD58/2024-04-26/selfTrigger_dacZero17_dacOne28_2400V_nd4p0_9hz_settings1to4/Analysis.root");


  
  TCanvas* c = new TCanvas();
  c->SetLogy();
  

  //---------------------------
  for(int iStrip=0; iStrip<nStrip; iStrip++){
    
    vHist[2*iStrip]->GetYaxis()->SetRangeUser(1, 1e5);
    vHist[2*iStrip]->GetYaxis()->SetTitle("Events");
    vHist[2*iStrip]->GetXaxis()->SetTitle( "-StripPeak");
  
    vHist[2*iStrip +1]->SetLineColor(kBlue);

  
    TLegend* leg = new TLegend(0.75,0.7, 0.9,0.9);
    leg->AddEntry(vHist[2*iStrip], "Side 0", "l");
    leg->AddEntry(vHist[2*iStrip +1], "Side 1","l");
  
    vHist[2*iStrip]->Draw("HIST");
    vHist[2*iStrip +1]->Draw("SAME HIST");
    leg->Draw("SAME");
    c->SaveAs( (std::to_string(iStrip)+".png").c_str() );
  }  


}

