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

int nHist = 8;
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

    bool aboveThreshLaserStrip = false;
    bool aboveThreshNeighbourStrip = false;
    bool aboveThreshOtherStrip = false;
    for(int iChan=0; iChan<60; iChan++){
      if(iChan == 30 || iChan == 59) continue;

      
      bool inLaserStrip = false;
      bool inNeighbourStrip = false;
      if( iChan == laserStrip    ||
	  iChan == (laserStrip+30)  ){
	inLaserStrip = true;
      }
      else if( iChan == (laserStrip-1)  ||
	       iChan == (laserStrip+1)  ||
	       iChan == (laserStrip+29) ||
	       iChan == (laserStrip+31) ){
	inNeighbourStrip = true;
      }

      
      if(-StripPeak[iChan] > thresh){
	if(inLaserStrip) aboveThreshLaserStrip = true;
	else if(inNeighbourStrip) aboveThreshNeighbourStrip = true;
	else{
	  aboveThreshOtherStrip = true;
	  //	  std::cout << iChan << "\n";
	}
      }// end if(above thresh)      
    }// end for(iChan)

    if(aboveThreshLaserStrip && !aboveThreshNeighbourStrip && !aboveThreshOtherStrip){   
      vHist[0]->Fill(-StripPeak[laserStrip]);
      vHist[1]->Fill(-StripPeak[laserStrip+30]);   
    }
    else if(aboveThreshLaserStrip && aboveThreshNeighbourStrip && !aboveThreshOtherStrip){  
      vHist[2]->Fill(-StripPeak[laserStrip]);
      vHist[3]->Fill(-StripPeak[laserStrip+30]);   
    }
    else if(aboveThreshLaserStrip && aboveThreshNeighbourStrip && aboveThreshOtherStrip){  
      vHist[4]->Fill(-StripPeak[laserStrip]);
      vHist[5]->Fill(-StripPeak[laserStrip+30]);   
    }
    else{
      vHist[6]->Fill(-StripPeak[laserStrip]);
      vHist[7]->Fill(-StripPeak[laserStrip+30]);   
    }

    
  }// end loop over events
  
//  double runtime = -1;
//  if(start>0 && end > 0) runtime = end - start;
//  if(inName == "../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root") runtime -= 1691; //awful hack
//
//  std::cout << "Runtime is " << runtime << "\n";
//  std::cout << std::setprecision(12) << start << "\n";
//  if(runtime < 0) abort();
//  
//  for(TH1F* hist : vHist) hist->Scale(1.0/runtime);
  
  return vHist;
} // end makeHist()


void macro()
{
  //  gStyle->SetHistTopMargin(0);
  
  int sigStrip = 5;
  //int sigStrip = 10;
    
  std::map<std::string, std::vector<TH1F*> > mHist;
  mHist["ND 4.0"] = makeHist("../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root", sigStrip);
    mHist["ND 4.0 ST"] = makeHist("../files/reprocessed_240417_selfTrigger_dacZero17_dacOne28_nd4p0_LAPPD58_Analysis.root", sigStrip);
  //    mHist["ND 4.0 ST"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne24_nd4p0_2400V_s5/Analysis.root", sigStrip);
    

//  for(int i=0; i<4; i++){
//    int maxBin_0 = mHist["ND 4.0 ST"][2*i]->GetMaximumBin();
//    double scale_0 = mHist["ND 4.0 ST"][2*i]->GetBinContent(maxBin_0) / mHist["ND 4.0"][2*i]->GetBinContent(maxBin_0);
//    int maxBin_1 = mHist["ND 4.0 ST"][2*i+1]->GetMaximumBin();
//    double scale_1 = mHist["ND 4.0 ST"][2*i+1]->GetBinContent(maxBin_1) / mHist["ND 4.0"][2*i+1]->GetBinContent(maxBin_1);
//    std::cout << scale_0 << ", " << scale_1 << "\n";
//  }

  std::vector<double> vScale;
  int maxBin_0 = mHist["ND 4.0 ST"][0]->GetMaximumBin();
  double scale_0 = mHist["ND 4.0 ST"][0]->GetBinContent(maxBin_0) / mHist["ND 4.0"][0]->GetBinContent(maxBin_0);
  int maxBin_1 = mHist["ND 4.0 ST"][1]->GetMaximumBin();
  double scale_1 = mHist["ND 4.0 ST"][1]->GetBinContent(maxBin_1) / mHist["ND 4.0"][1]->GetBinContent(maxBin_1);
  std::cout << scale_0 << ", " << scale_1 << "\n";
  vScale.push_back(scale_0);
  vScale.push_back(scale_1);

  
  TCanvas* c = new TCanvas();
  c->SetLogy();


  for(int i=0; i <2; i++){
    std::string strip = "-StripPeak [mV] (strip "+std::to_string(sigStrip)+")";


    mHist["ND 4.0 ST"][i+6]->SetFillColor(kBlue);
    mHist["ND 4.0 ST"][i+4]->SetFillColor(kRed);
    mHist["ND 4.0 ST"][i+2]->SetFillColor(kGreen+2);
    mHist["ND 4.0 ST"][i+0]->SetFillColor(kMagenta);
  
    mHist["ND 4.0 ST"][i+6]->SetLineWidth(0);
    mHist["ND 4.0 ST"][i+4]->SetLineWidth(0);
    mHist["ND 4.0 ST"][i+2]->SetLineWidth(0);
    mHist["ND 4.0 ST"][i+0]->SetLineWidth(0);
  
    THStack* hs = new THStack("hs",("Side " + std::to_string(i) + ";" + strip + ";Events").c_str());
    hs->SetMinimum(1);
    hs->SetMaximum(1e5);
    //  hs->GetYaxis()->SetRangeUser(1, 1e5);
    hs->Add(mHist["ND 4.0 ST"][i+0]);
    hs->Add(mHist["ND 4.0 ST"][i+2]);
    hs->Add(mHist["ND 4.0 ST"][i+4]);
    hs->Add(mHist["ND 4.0 ST"][i+6]);
    hs->Draw("HIST");
  
  
  
    TH1F* hFT = (TH1F*) mHist["ND 4.0"][i+0]->Clone();
    hFT->Add(mHist["ND 4.0"][i+2]);
    hFT->Add(mHist["ND 4.0"][i+4]);
    hFT->Add(mHist["ND 4.0"][i+6]);
    hFT->Scale(vScale[i]);
    hFT->Draw("SAME HIST");
  
    TLegend* legend = new TLegend(0.5,0.6, 0.9,0.9);
    legend->AddEntry(mHist["ND 4.0 ST"][i+0], "Self trigger: laser","f");
    legend->AddEntry(mHist["ND 4.0 ST"][i+2], "Self trigger: laser + neighbour","f");
    legend->AddEntry(mHist["ND 4.0 ST"][i+4], "Self trigger: laser + neighbour + other","f");
    legend->AddEntry(mHist["ND 4.0 ST"][i+6], "Self trigger: everything else","f");
    legend->AddEntry(hFT, ( std::to_string(vScale[i])+" x Forced trigger").c_str(),"l");
  
    legend->Draw("SAME");
    
    c->SaveAs( (std::to_string(i)+"_stack.png").c_str() );

  }
  
  //  for(int i=0; i<nHist; i++){  
//
//    std::string strip;
//    if(i%2 == 0) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 0)";
//    else if(i%2 == 1) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 1)";
//
//    //---------------------------
//    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-6, 10);
//    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events / second (Hz)");
//    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
//    //---------------------------
//
//    //~~~~~~~~~~~~~~~~~~~~~~~
////    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 1e5);
////    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Ratio to ND 4.0");
////    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
////    mHist["ND 4.0 ST"][i] = DivideHist(mHist["ND 4.0 ST"][i], mHist["ND 4.0"][i]);
////    mHist["ND 4.0"][i] = DivideHist(mHist["ND 4.0"][i], mHist["ND 4.0"][i]);    
//    //~~~~~~~~~~~~~~~~~~~~~~~
//    
//    
//    mHist["ND 4.0 ST"][i]->SetLineColor(kBlue);
//	
//    TLegend* legend = new TLegend(0.75,0.7, 0.9,0.9);
//    legend->AddEntry(mHist["ND 4.0"][i], "ND 4.0","l");
//    legend->AddEntry(mHist["ND 4.0 ST"][i], "ND 4.0 ST","l");
//				    
//    mHist["ND 4.0"][i]->Draw("HIST");
//    mHist["ND 4.0 ST"][i]->Draw("SAME HIST");
//    legend->Draw("SAME");
//    c->SaveAs(("laser_"+ std::to_string(i)+".png").c_str());
//  }
  


}

