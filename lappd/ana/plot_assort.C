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

int nHist = 4;
int noiseStrip = 10;

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
    TH1F *hist  = new TH1F("", "", 100,0,100); 
    vHist.push_back(hist);
  }
  
  int entries = myTree->GetEntries();

  double start = -1;
  double end = -1;
  
  for(int i=0; i<entries; i++){

    myTree->GetEntry(i);

    if(i==0) start = timestamps_meta;
    else if(i==(entries-1)) end = timestamps_meta;
    
    vHist[0]->Fill(-StripPeak[laserStrip]);
    vHist[1]->Fill(-StripPeak[laserStrip+30]);

    vHist[2]->Fill(-StripPeak[noiseStrip]);
    vHist[3]->Fill(-StripPeak[noiseStrip+30]);

  }// end loop over events

  double runtime = -1;
  if(start>0 && end > 0) runtime = end - start;
  if(inName == "files/240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root") runtime -= 1691; //awful hack

  std::cout << "Runtime is " << runtime << "\n";
  std::cout << std::setprecision(12) << start << "\n";
  if(runtime < 0) abort();
  
  for(TH1F* hist : vHist) hist->Scale(1.0/runtime);
  
  return vHist;
} // end makeHist()


void plot_assort()
{

  //TH1F* hist1 = makeHist("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");


  int sigStrip = 5;
  
  //TH1F* hist1 = makeHist("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");
  

  
  std::map<std::string, std::vector<TH1F*> > mHist;
  mHist["ND 4.0"] = makeHist("files/240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root", sigStrip);
  mHist["ND 4.5"] = makeHist("files/240411_forcedTrigger_nd4p5_LAPPD58_Analysis.root", sigStrip);
  mHist["ND 4.7"] = makeHist("files/240412_forcedTrigger_nd4p7_LAPPD58_Analysis.root", sigStrip);
  
//  std::vector<TH1F*> vHist2 = makeHist("240318_s6_selfTrigger_dacZero18_dacOne130_nd3p0_laserOn_Analysis.root", 6, 10860);
//  std::vector<TH1F*> vHist3 = makeHist("240318_s6_selfTrigger_dacZero8_dacOne130_nd3p0_laserOn_Analysis.root", 6, 3180);
//  std::vector<TH1F*> vHist4 = makeHist("240318_s6_selfTrigger_dacZero2_dacOne130_nd3p0_laserOn_Analysis.root", 6, 360);
//
//  std::vector<TH1F*> vHist5 = makeHist("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root", 10, 23940);
//
//
//  std::vector<TH1F*> vHist6 = makeHist("240319_selfTrigger_dacZero8_dacOne130_laserOn_hvOn_nd2p5_Analysis.root", 6, 369);
//  std::vector<TH1F*> vHist7 = makeHist("240319_selfTrigger_dacZero8_dacOne130_laserOff_hvOn_nd2p5_Analysis.root", 6, 5792);


  
  TCanvas* c = new TCanvas();
  c->SetLogy();
  
  for(int i=0; i<nHist; i++){  
    //    vHist6[i]->SetLineColor(kBlue);
    //    vHist7[i]->SetLineColor(kRed);

    std::string strip;
    if(i == 0) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 0)";
    else if(i == 1) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 1)";
    else if(i == 2) strip = ("StripPeak (strip "+std::to_string(noiseStrip)+", side 0)").c_str();
    else if(i == 3) strip = ("StripPeak (strip "+std::to_string(noiseStrip)+", side 1)").c_str();


    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 10);
    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events / second (Hz)");    
    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());


    mHist["ND 4.5"][i]->SetLineColor(kBlue);
    mHist["ND 4.7"][i]->SetLineColor(kRed);
	
    TLegend* legend = new TLegend(0.6,0.6, 0.9,0.9);
    legend->AddEntry(mHist["ND 4.0"][i], "ND 4.0","l");
    legend->AddEntry(mHist["ND 4.5"][i], "ND 4.5","l");
    legend->AddEntry(mHist["ND 4.7"][i], "ND 4.7","l");
				    
    mHist["ND 4.0"][i]->Draw("HIST");
    mHist["ND 4.5"][i]->Draw("SAME HIST");
    mHist["ND 4.7"][i]->Draw("SAME HIST");
    legend->Draw("SAME");
    c->SaveAs(("laser_"+ std::to_string(i)+".png").c_str());
  }
  






//  for(int i=0; i<nHist; i++){  
//    vHist2[i]->SetLineColor(kBlue);
//    vHist5[i]->SetLineColor(kRed);
//
//    std::string strip;
//    if(i == 0) strip = "StripPeak (laser strip, side 0)";
//    else if(i == 1) strip = "StripPeak (laser strip, side 1)";
//    else if(i == 2) strip = "StripPeak (noise strip, side 0)";
//    else if(i == 3) strip = "StripPeak (noise strip, side 1)";
//
//
//    vHist2[i]->GetYaxis()->SetRangeUser(1e-4, 10);
//    vHist2[i]->GetYaxis()->SetTitle("Events / second (Hz)");    
//    vHist2[i]->GetXaxis()->SetTitle( strip.c_str());
//
//    TLegend* legend = new TLegend(0.7,0.7, 0.9,0.9);
//    legend->AddEntry(vHist2[i], "laser in strip 6","l");
//    legend->AddEntry(vHist5[i], "laser in strip 10","l");
//				    
//    vHist2[i]->Draw("HIST");
//    vHist5[i]->Draw("SAME HIST");
//    legend->Draw("SAME");
//    c->SaveAs(("strip_"+ std::to_string(i)+".png").c_str());
//  }
//
//
//
//  for(int i=0; i<nHist; i++){  
//    vHist2[i]->SetLineColor(kBlue);
//    vHist3[i]->SetLineColor(kGreen+2);
//    vHist4[i]->SetLineColor(kRed);
//
//    int strip;
//    if(i == 0) strip = 6;
//    else if(i == 1) strip = 36;
//    else if(i == 2) strip = noiseStrip;
//    else if(i == 3) strip = noiseStrip+30;
//
//
//    vHist2[i]->GetYaxis()->SetRangeUser(1e-4, 10);
//    vHist2[i]->GetYaxis()->SetTitle("Events / second (Hz)");    
//    vHist2[i]->GetXaxis()->SetTitle( ("-StripPeak["+std::to_string(strip)+"]").c_str());
//
//    TLegend* legend = new TLegend(0.7,0.7, 0.9,0.9);
//    legend->AddEntry(vHist2[i], "DAC0 18mV","l");
//    legend->AddEntry(vHist3[i], "DAC0 8mV","l");
//    legend->AddEntry(vHist4[i], "DAC0 2mV","l");
//				    
//    vHist2[i]->Draw("HIST");
//    vHist3[i]->Draw("SAME HIST");
//    vHist4[i]->Draw("SAME HIST");
//    legend->Draw("SAME");
//    c->SaveAs(("thresh_"+ std::to_string(i)+".png").c_str());
//  }
  





				       

//  double ymax = hist4->GetBinContent(hist4->GetMaximumBin());
//  
//  hist1->SetLineColor(kBlue);
//  hist3->SetLineColor(kGreen+2);
//  hist4->SetLineColor(kRed);
//  hist4->GetYaxis()->SetRangeUser(0., 1.2*ymax);
//  hist4->GetYaxis()->SetTitle("nPeaks / Event");
//  hist4->GetXaxis()->SetTitle("Channel number");
//  hist4->SetTitle((" \"Peak\" defined as any time StripPeak < " +std::to_string(minHeight)+"mV").c_str());
//
//  TLegend* legend = new TLegend(0.7,0.7, 0.9,0.9);
//  //  legend->AddEntry(hist1, "Laser in strip 10, 18mV thresh","l");
//  legend->AddEntry(hist2, "Laser in strip 6, 18mV thresh","l");
//  legend->AddEntry(hist3, "Laser in strip 6, 8mV thresh","l");
//  legend->AddEntry(hist4, "Laser in strip 6, 2mV thresh","l");
//
//    
//  TCanvas* c = new TCanvas();
//
//  //  hist1->Draw("HIST");
//  hist4->Draw("HIST");
//  hist2->Draw("SAME HIST");
//  hist3->Draw("SAME HIST");
//  legend->Draw("SAME");
//  
//  c->SaveAs("hist.png");

}

