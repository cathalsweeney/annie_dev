#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

int nBins = 110;
double xHi = nBins*100.;
TH1F* makeHist(std::string inName)
{
  std::cout << inName << "\n";
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

  TH1F *hist  = new TH1F("", "", nBins,0,xHi);
  
  int entries = myTree->GetEntries();

  double start = -1;

  
  for(int i=0; i<entries; i++){
    myTree->GetEntry(i);    
    if(i==0) start = timestamps_meta;
    double time_val = timestamps_meta - start;
    hist->Fill(time_val);    

    if(i%1000 == 0 && time_val < 0) std::cout << i << ", " << time_val << "\n";
  }// end loop over events
  
  return hist;
} // end makeHist()


void macro()
{
  

  std::vector<int> vColor = {kRed, kGreen+2, kBlue, kMagenta};
    
  std::map<std::string, TH1F* > mHist;
  mHist["Forced trigger"] = makeHist("../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root");
  mHist["Self trigger, DAC1 28mV"] = makeHist("../files/reprocessed_240417_selfTrigger_dacZero17_dacOne28_nd4p0_LAPPD58_Analysis.root");
  mHist["Self trigger, DAC1 24mV"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne24_nd4p0_2400V_s5/Analysis.root");
  mHist["Self trigger, DAC1 20mV"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne20_nd4p0_2400V_s5/Analysis.root");
  mHist["Self trigger, DAC1 16mV"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne16_nd4p0_2400V_s5/Analysis.root");
  
  
  TCanvas* c1 = new TCanvas("c1");

  TLegend* legend = new TLegend(0.5,0.6, 0.9,0.9);

  int iFile = 0;
  std::map<std::string, TH1F* >::iterator it;
  for (it = mHist.begin(); it != mHist.end(); it++){

    TH1F* hist = it->second;
    std::cout << it->first << ", " << hist->Integral(0,nBins+1) << "\n";
    if(iFile == 0){
      legend->AddEntry(hist, it->first.c_str(),"l");
      
      hist->SetTitle("");
      hist->GetYaxis()->SetRangeUser(0., 2000.);
      hist->GetYaxis()->SetTitle("Events / 100 seconds");
      hist->GetXaxis()->SetTitle("Time since starting DAQ (seconds)");
      hist->GetYaxis()->CenterTitle(true);
      hist->GetXaxis()->CenterTitle(true);
      hist->Draw("HIST");

    }
    else{
      legend->AddEntry(hist, it->first.c_str(),"l");

      hist->SetLineColor(vColor[iFile-1]);
      hist->Draw("SAME HIST");
    }

    iFile++;
  }
  legend->Draw("SAME.png");
  c1->SaveAs("c1.png");


}

