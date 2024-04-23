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
double thresh = 17.0;


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

  double start = -1;
  double end = -1;
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);

    if(i==0) start = timestamps_meta;
    else if(i==(entries-1)) end = timestamps_meta;

    bool aboveThreshNearLaser = false;
    bool aboveThreshFarFromLaser = false;
    for(int iChan=0; iChan<60; iChan++){
      if(iChan == 30 || iChan == 59) continue;

      bool nearLaserStrip = false;
      if( iChan == laserStrip    ||
	  iChan == laserStrip-1  ||
	  iChan == laserStrip+1  ||
	  iChan == laserStrip+30 ||
	  iChan == laserStrip+29 ||
	  iChan == laserStrip+31 
	  ){
	nearLaserStrip = true;
      }

      if(-StripPeak[iChan] > thresh){
	if(nearLaserStrip) aboveThreshNearLaser = true;
	else aboveThreshFarFromLaser = true;
      }
      
    }

    if(aboveThreshNearLaser && !aboveThreshFarFromLaser){   
      vHist[0]->Fill(-StripPeak[laserStrip]);
      vHist[1]->Fill(-StripPeak[laserStrip+30]);   
    }
    else if(!aboveThreshNearLaser && aboveThreshFarFromLaser){   
      vHist[2]->Fill(-StripPeak[laserStrip]);
      vHist[3]->Fill(-StripPeak[laserStrip+30]);   
    }
    else{ // Both above or both below
      vHist[4]->Fill(-StripPeak[laserStrip]);
      vHist[5]->Fill(-StripPeak[laserStrip+30]);   
    }

  }// end loop over events
  
  double runtime = -1;
  if(start>0 && end > 0) runtime = end - start;
  if(inName == "../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root") runtime -= 1691; //awful hack

  std::cout << "Runtime is " << runtime << "\n";
  std::cout << std::setprecision(12) << start << "\n";
  if(runtime < 0) abort();
  
  for(TH1F* hist : vHist) hist->Scale(1.0/runtime);
  
  return vHist;
} // end makeHist()


void macro()
{
  int sigStrip = 5;
  //int sigStrip = 10;
    
  std::map<std::string, std::vector<TH1F*> > mHist;
  mHist["ND 4.0"] = makeHist("../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root", sigStrip);
  mHist["ND 4.0 ST"] = makeHist("../files/reprocessed_240417_selfTrigger_dacZero17_dacOne28_nd4p0_LAPPD58_Analysis.root", sigStrip);
    
  TCanvas* c = new TCanvas();
  c->SetLogy();
  
  for(int i=0; i<nHist; i++){  

    std::string strip;
    if(i%2 == 0) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 0)";
    else if(i%2 == 1) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 1)";

    //---------------------------
//    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 10);
//    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events / second (Hz)");
//    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
    //---------------------------

    //~~~~~~~~~~~~~~~~~~~~~~~
    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 1e5);
    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Ratio to ND 4.0");
    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
    mHist["ND 4.0 ST"][i] = DivideHist(mHist["ND 4.0 ST"][i], mHist["ND 4.0"][i]);
    mHist["ND 4.0"][i] = DivideHist(mHist["ND 4.0"][i], mHist["ND 4.0"][i]);    
    //~~~~~~~~~~~~~~~~~~~~~~~
    
    
    mHist["ND 4.0 ST"][i]->SetLineColor(kBlue);
	
    TLegend* legend = new TLegend(0.75,0.7, 0.9,0.9);
    legend->AddEntry(mHist["ND 4.0"][i], "ND 4.0","l");
    legend->AddEntry(mHist["ND 4.0 ST"][i], "ND 4.0 ST","l");
				    
    mHist["ND 4.0"][i]->Draw("HIST");
    mHist["ND 4.0 ST"][i]->Draw("SAME HIST");
    legend->Draw("SAME");
    c->SaveAs(("laser_"+ std::to_string(i)+".png").c_str());
  }
  


}

