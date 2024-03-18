#include "TGraph2D.h"
#include "TAxis.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

void plot()
{

  TFile *inFile = new TFile("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");
  
  TTree *myTree;
  myTree = (TTree*)inFile->Get("ffmytree");

  int entries = myTree->GetEntries();

  int NHits;
  double xperp[1];
  double xpar[1];
  double StripPeak[60];

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


  
  TH2F *myHisto  = new TH2F("", "", 30,0,30, 30,-100,100);

  int nPass = 0;
  
  for(int i=0; i<entries; i++){
  //  for(int i=0; i<10000; i++){    
    myTree->GetEntry(i);
    //    if(NHits < 1) continue;

    //    if(StripPeak[40] > -50.) continue;
    
    int iMin = 0;
    double minVal = 0.;
    for(int i=1; i<60; i++){
      if(i==30 || i==59) continue;
      
      if(StripPeak[i] < minVal){
	minVal = StripPeak[i];
	iMin = i;
      }
    }

    //    if(minVal > -7) continue;
    
    myHisto->Fill(xperp[0],xpar[0]);
    //    std::cout << iMin << " " << xperp[0] << " " << xpar[0] << "\n";
    nPass++;
  }
  
  myHisto->Draw("COLZ");
  
}

