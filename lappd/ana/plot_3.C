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
#include "TLatex.h"


TH1F* normalise(TH1F* hist, int integral=0)
{

  
  if(!integral) integral = hist->Integral();
  hist->Scale(1.0/integral);
  return hist;
}

TH1F* makeHist(std::string inName, double minHeight)
{

  TFile *inFile = new TFile(inName.c_str());
    
  TTree *myTree;
  myTree = (TTree*)inFile->Get("ffmytree");
  int nChannels = 60;
   
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

  TH1F *hist  = new TH1F("", "", 60,0,60); 
  
  int entries = myTree->GetEntries();
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);
          
    for(int j=0; j<nChannels; j++){
      if(StripPeak[j] <  minHeight){
	hist->Fill(j);
      }
    }// end for(channels)    

  }// end loop over events

  //  hist->Scale(1.0/entries);

  return hist;
} // end makeHist()


void doIt(double minHeight)
{
  
  TH1F* hist1 = makeHist("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root", minHeight);
  //hist1 = normalise(hist1, 22800);
  
  TH1F* hist2 = makeHist("240318_s6_selfTrigger_dacZero18_dacOne130_nd3p0_laserOn_Analysis.root", minHeight);
  //hist2 = normalise(hist2, 10860);

//  TH1F* hist3 = makeHist("240318_s6_selfTrigger_dacZero8_dacOne130_nd3p0_laserOn_Analysis.root");
//  //hist3 = normalise(hist3, 3180);
//
//  TH1F* hist4 = makeHist("240318_s6_selfTrigger_dacZero2_dacOne130_nd3p0_laserOn_Analysis.root");
//  //hist4 = normalise(hist4, 360);


  TH1F* hist3 = makeHist("240319_selfTrigger_dacZero8_dacOne130_laserOn_hvOn_nd2p5_Analysis.root", minHeight);  
  hist3 = normalise(hist3, 369);
  
  TH1F* hist4 = makeHist("240319_selfTrigger_dacZero8_dacOne130_laserOff_hvOn_nd2p5_Analysis.root", minHeight);
  hist4 = normalise(hist4, 5792);


  
  double ymax = hist3->GetBinContent(hist3->GetMaximumBin());
  
  hist1->SetLineColor(kBlue);
  hist3->SetLineColor(kGreen+2);
  hist4->SetLineColor(kRed);
  //  hist4->GetYaxis()->SetRangeUser(1e-12, 100*ymax);
  hist4->GetYaxis()->SetRangeUser(1e-6, 1e3);
  hist4->GetYaxis()->SetTitle("nPeaks / second");
  hist4->GetXaxis()->SetTitle("Channel number");
  hist4->SetTitle(("#splitline{\"Peak\" defined as any time StripPeak < " +std::to_string(minHeight)+"mV}{24/03/19, selfTrigger, DAC0 8mV, DAC1 130mV, HV on, ND 2.5}").c_str());

  TLegend* legend = new TLegend(0.7,0.7, 0.9,0.9);
  //  legend->AddEntry(hist1, "Laser in strip 10, 18mV thresh","l");
  //  legend->AddEntry(hist2, "Laser in strip 6, 18mV thresh","l");
  legend->AddEntry(hist3, "Laser on","l");
  legend->AddEntry(hist4, "Laser off","l");  

  TCanvas* c = new TCanvas();
  c->SetLogy();
  
  //  hist1->Draw("HIST");
  hist4->Draw("HIST");
  //  hist2->Draw("SAME HIST");
  hist3->Draw("SAME HIST");
  legend->Draw("SAME");

  std::string name = "hist_minHeight" + std::to_string(-int(minHeight)) + ".png";
  
  c->SaveAs(name.c_str());		    
}

void plot_3()
{
  for(int i=0; i<50; i++){
  doIt(-i);
  }
}
  
