#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TFile.h"
#include "THStack.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

int nHist = 10;
int nBins = 100;
double thresh = 17.0;

std::vector<TH1F*> makeHist(std::string inName, int laserStrip,  double runtime_byHand)
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
    TH1F *hist  = new TH1F("", "", nBins,0,150); 
    vHist.push_back(hist);
  }
  
  int entries = myTree->GetEntries();

  double start = -1;
  double end = -1;
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);

    if(i==0) start = timestamps_meta;
    else if(i==(entries-1)) end = timestamps_meta;

    bool aboveThreshLaserStrip = false;
    bool aboveThreshNeighbourStrip = false;
    bool aboveThreshOtherStrip = false;
    for(int iChan=0; iChan<30; iChan++){
      
      bool inLaserStrip = false;
      bool inNeighbourStrip = false;
      if( iChan == laserStrip){
	inLaserStrip = true;
      }
      else if( iChan == (laserStrip-1) ||
	       iChan == (laserStrip+1) ){
	inNeighbourStrip = true;
      }
      
      if(-StripPeak[iChan] > thresh){
	if(inLaserStrip) aboveThreshLaserStrip = true;
	else if(inNeighbourStrip) aboveThreshNeighbourStrip = true;
	else{
	  aboveThreshOtherStrip = true;
	}
      }// end if(above thresh)      
    }// end for(iChan)


    bool onlyLaser = aboveThreshLaserStrip && !aboveThreshNeighbourStrip && !aboveThreshOtherStrip;
    bool onlyLaserAndNeighbour = aboveThreshLaserStrip && aboveThreshNeighbourStrip && !aboveThreshOtherStrip;
    bool allOtherLaser = aboveThreshLaserStrip && !onlyLaser && !onlyLaserAndNeighbour;
    
    if(onlyLaser){   
      vHist[0]->Fill(-StripPeak[laserStrip]);
      vHist[1]->Fill(-StripPeak[laserStrip+30]);   
    }
    else if(onlyLaserAndNeighbour){  
      vHist[2]->Fill(-StripPeak[laserStrip]);
      vHist[3]->Fill(-StripPeak[laserStrip+30]);   
    }
    else if(allOtherLaser){  
      vHist[4]->Fill(-StripPeak[laserStrip]);
      vHist[5]->Fill(-StripPeak[laserStrip+30]);   
    }
    else{// notLaser
      vHist[6]->Fill(-StripPeak[laserStrip]);
      vHist[7]->Fill(-StripPeak[laserStrip+30]);   
    }

    // These two are just a cross-check on the integral
    vHist[8]->Fill(-StripPeak[laserStrip]);
    vHist[9]->Fill(-StripPeak[laserStrip+30]);
  }// end loop over events
  
  double runtime = -1;
  if(start>0 && end > 0) runtime = end - start;
  //  std::cout << end << " - " << start << " = " << runtime << "\n";
  std::cout << runtime << "/" << runtime_byHand << " = " << runtime / runtime_byHand << "\n"; 

//
//  std::cout << "Runtime is " << runtime << "\n";
//  std::cout << std::setprecision(12) << start << "\n";
//  if(runtime < 0) abort();
//  
  for(TH1F* hist : vHist) hist->Scale(1.0/runtime_byHand);

  
  return vHist;
} // end makeHist()

std::vector<THStack*> MakeStacks(std::vector<TH1F*> vHist)
{
  std::vector<THStack*> vStack;
  
  for(int iSide=0; iSide <2; iSide++){
    std::string strip = "-StripPeak [mV] (laser strip)";

    vHist[iSide+6]->SetFillColor(kBlue);
    vHist[iSide+4]->SetFillColor(kOrange-3);
    vHist[iSide+2]->SetFillColor(kGreen+2);
    vHist[iSide+0]->SetFillColor(kMagenta);
  
    vHist[iSide+6]->SetLineWidth(0);
    vHist[iSide+4]->SetLineWidth(0);
    vHist[iSide+2]->SetLineWidth(0);
    vHist[iSide+0]->SetLineWidth(0);
  
    THStack* hs = new THStack("",("Side " + std::to_string(iSide) + ";" + strip + ";Events / second [Hz]").c_str());
    hs->SetMinimum(1e-3);
    hs->SetMaximum(2);
    //hs->SetMaximum(50.0);
   
    hs->Add(vHist[iSide+6]);
    hs->Add(vHist[iSide+2]);
    hs->Add(vHist[iSide+4]);
    hs->Add(vHist[iSide+0]);
  
    //    vHist[iSide+8]->Draw("SAME HIST");

    double integral_1 = vHist[iSide+8]->Integral(0, nBins+1);
    double integral_2 = 0;
    integral_2 += vHist[iSide+0]->Integral(0, nBins+1);
    integral_2 += vHist[iSide+2]->Integral(0, nBins+1);
    integral_2 += vHist[iSide+4]->Integral(0, nBins+1);
    integral_2 += vHist[iSide+6]->Integral(0, nBins+1);
	  
    std::cout << "integral: " << integral_1 << "/" << integral_2 << " = " << integral_1 / integral_2 << "\n";
    

    vStack.push_back(hs);
  }
  return vStack;
}


void macro()
{  
  int sigStrip = 5;
   
  std::map<std::string, std::vector<TH1F*> > mHist;
  mHist["ND 4.0"] = makeHist("../files/LAPPD58/2024-05-03/forcedTrigger_2400V_nd4p0/Analysis.root", sigStrip, 3200);
  //  mHist["ND 4.0 ST"] = makeHist("../files/LAPPD58/2024-05-06/selfTrigger_dacZero17_dacOne28_2400V_nd4p0_9hz/Analysis.root", sigStrip, 16700);
  //mHist["ND 4.0 ST"] = makeHist("../files/LAPPD58/2024-05-06/selfTrigger_dacZero17_dacOne24_2400V_nd4p0_9hz/Analysis.root", sigStrip, 4700);
  mHist["ND 4.0 ST"] = makeHist("../files/LAPPD58/2024-05-06/selfTrigger_dacZero17_dacOne20_2400V_nd4p0_9hz/Analysis.root", sigStrip, 6200);
//  std::vector<double> vScale;
//  int maxBin_0 = mHist["ND 4.0 ST"][0]->GetMaximumBin();
//  double scale_0 = mHist["ND 4.0 ST"][0]->GetBinContent(maxBin_0) / mHist["ND 4.0"][0]->GetBinContent(maxBin_0);
//  int maxBin_1 = mHist["ND 4.0 ST"][1]->GetMaximumBin();
//  double scale_1 = mHist["ND 4.0 ST"][1]->GetBinContent(maxBin_1) / mHist["ND 4.0"][1]->GetBinContent(maxBin_1);
//  std::cout << scale_0 << ", " << scale_1 << "\n";
//  vScale.push_back(scale_0);
//  vScale.push_back(scale_1);

  
  TCanvas* c = new TCanvas();
  c->SetLogy();


  std::vector<THStack*> vStack_st = MakeStacks(mHist["ND 4.0 ST"]);
  std::vector<THStack*> vStack_ft = MakeStacks(mHist["ND 4.0"]);

  vStack_st[0]->Draw("HIST");
  c->SaveAs("foo_0.png");
  vStack_st[1]->Draw("HIST");
  c->SaveAs("foo_1.png");

  vStack_ft[0]->Draw("HIST");
  c->SaveAs("bar_0.png");
  vStack_ft[1]->Draw("HIST");
  c->SaveAs("bar_1.png");

//    TLegend* legend = new TLegend(0.5,0.6, 0.9,0.9);
//    legend->AddEntry(mHist["ND 4.0 ST"][iSide+0], "Self trigger: only laser strip above trigger thresh","f");
//    legend->AddEntry(mHist["ND 4.0 ST"][iSide+2], "Self trigger: only laser strip and neighbour above trigger thresh","f");
//    legend->AddEntry(mHist["ND 4.0 ST"][iSide+4], "Self trigger: all other event with laser strip above thresh","f");
//    legend->AddEntry(mHist["ND 4.0 ST"][iSide+6], "Self trigger: laser strip below trigger thresh","f");
//    //    legend->AddEntry(hFT, ( std::to_string(vScale[iSide])+" x Forced trigger").c_str(),"l");
//  
//    legend->Draw("SAME");

  
  

}
