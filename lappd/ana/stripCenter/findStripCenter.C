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

int nHist = 6;
int noiseStrip = 10;

TH1F* DivideHist(TH1F* hNum, TH1F* hDenom)
{
  TH1F* hRet = (TH1F*) hNum->Clone();
  hRet->Clear();
  int nBins = hNum->GetNbinsX();
  for(int iBin=1; iBin<nBins; iBin++){
    double cNum = hNum->GetBinContent(iBin);
    double cDenom = hDenom->GetBinContent(iBin);
    double cRet = 1.0;
    if(cNum > 0 && cDenom > 0) cRet = cNum / cDenom;
    hRet->SetBinContent(iBin, cRet);
  }
  return hRet;
}

std::vector<TH1F*> makeHist(std::string inName, int laserStrip)
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
    TH1F *hist  = new TH1F("", "", 100,0,150); 
    vHist.push_back(hist);
  }
  
  int entries = myTree->GetEntries();
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);
    
    vHist[0]->Fill(-StripPeak[laserStrip]);
    vHist[1]->Fill(-StripPeak[laserStrip-1]);
    vHist[2]->Fill(-StripPeak[laserStrip+1]);

    vHist[3]->Fill(-StripPeak[laserStrip+30]);
    vHist[4]->Fill(-StripPeak[laserStrip+30-1]);
    vHist[5]->Fill(-StripPeak[laserStrip+30+1]);

  }// end loop over events
  
  return vHist;
} // end makeHist()


void findStripCenter()
{
  int sigStrip = 5;
    
 
  std::map<std::string, std::vector<TH1F*> > mHist;
  std::vector<TH1F*> vHist = makeHist("../files/LAPPD58/2024-04-26/selfTrigger_dacZero17_dacOne28_2400V_nd4p0_9hz_settings1to4/Analysis.root", sigStrip);


  
  TCanvas* c = new TCanvas();
  c->SetLogy();
  

  //---------------------------
  vHist[0]->GetYaxis()->SetRangeUser(1, 1e5);
  vHist[0]->GetYaxis()->SetTitle("Events");
  vHist[0]->GetXaxis()->SetTitle( "-StripPeak");

  vHist[1]->SetLineColor(kBlue);
  vHist[2]->SetLineColor(kRed);

  TLegend* leg0 = new TLegend(0.75,0.7, 0.9,0.9);
  leg0->AddEntry(vHist[1], ("Strip "+std::to_string(sigStrip-1)).c_str(),"l");
  leg0->AddEntry(vHist[0], ("Strip "+std::to_string(sigStrip)).c_str(), "l");
  leg0->AddEntry(vHist[2], ("Strip "+std::to_string(sigStrip+1)).c_str(),"l");

  vHist[0]->Draw("HIST");
  vHist[1]->Draw("SAME HIST");
  vHist[2]->Draw("SAME HIST");
  leg0->Draw("SAME");
  c->SaveAs("stripCenter_side0.png");
  
  //---------------------------


  vHist[3]->GetYaxis()->SetRangeUser(1, 1e5);
  vHist[3]->GetYaxis()->SetTitle("Events");
  vHist[3]->GetXaxis()->SetTitle( "-StripPeak");

  vHist[4]->SetLineColor(kBlue);
  vHist[5]->SetLineColor(kRed);

  TLegend* leg1 = new TLegend(0.75,0.7, 0.9,0.9);
  leg1->AddEntry(vHist[4], ("Strip "+std::to_string(sigStrip-1)).c_str(),"l");
  leg1->AddEntry(vHist[3], ("Strip "+std::to_string(sigStrip)).c_str(), "l");
  leg1->AddEntry(vHist[5], ("Strip "+std::to_string(sigStrip+1)).c_str(),"l");

  vHist[3]->Draw("HIST");
  vHist[4]->Draw("SAME HIST");
  vHist[5]->Draw("SAME HIST");
  leg1->Draw("SAME");
  c->SaveAs("stripCenter_side1.png");


}

