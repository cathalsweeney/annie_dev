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
int noiseStrip = 16;

std::vector<int> vColour = {kBlack, kBlue};

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
    
    vHist[0]->Fill(-StripPeak[laserStrip]);
    vHist[1]->Fill(-StripPeak[laserStrip+30]);

    vHist[2]->Fill(-StripPeak[noiseStrip]);
    vHist[3]->Fill(-StripPeak[noiseStrip+30]);

  }// end loop over events
  std::cout << vHist[0]->Integral() / (double) vHist[1]->Integral() << ", " <<  vHist[2]->Integral() / (double) vHist[3]->Integral() << "\n";

  
//  double runtime = -1;
//  if(start>0 && end > 0) runtime = end - start;
//  
//  std::cout << "Runtime is " << runtime << "\n";
//  std::cout << std::setprecision(12) << start << "\n";
//  if(runtime < 0) abort();
//  
//  for(TH1F* hist : vHist) hist->Scale(1.0/runtime);
  for(TH1F* hist : vHist){
    int integral = hist->Integral();
    hist->Scale(1.0/integral);
  }
  
  return vHist;
} // end makeHist()

void plot_assort()
{
  int sigStrip = 14;
 
  std::map<std::string, std::vector<TH1F*> > mHist;


  mHist["ND 4.0"] = makeHist("../files/LAPPD39/2024-07-24/forcedTrigger_strip14_nd4p0/Analysis.root", sigStrip);
  mHist["ND 4.5"] = makeHist("../files/LAPPD39/2024-07-24/forcedTrigger_strip14_nd4p5/Analysis.root", sigStrip);
  //  mHist["ND 3.8 (Feb 2024)"] = makeHist("../files/LAPPD39/2024-02-21/s20_2550_nd3p8/Analysis.root", 20);
  mHist["ND 4.0 (Feb 2024)"] = makeHist("../files/LAPPD39/2024-02-21/s20_2550_nd4p0/Analysis.root", 20);
    
  TCanvas* c = new TCanvas();
  c->SetLogy();

  for(int i=0; i<nHist; i++){  

    std::string strip;
    //    if(i == 0) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 0)";
    //    else if(i == 1) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 1)";
    if(i == 0) strip = "StripPeak (Laser strip), side 0)";
    else if(i == 1) strip = "StripPeak (Laser strip), side 1)";
    else if(i == 2) strip = ("StripPeak (strip "+std::to_string(noiseStrip)+", side 0)").c_str();
    else if(i == 3) strip = ("StripPeak (strip "+std::to_string(noiseStrip)+", side 1)").c_str();


    mHist["ND 4.5"][i]->SetLineColor(kRed);
    //    mHist["ND 3.8 (Feb 2024)"][i]->SetLineColor(kGreen+2);
    mHist["ND 4.0 (Feb 2024)"][i]->SetLineColor(kBlue);
    
    //---------------------------
    //    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1, 10e6);
    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-5, 1);
    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events (area normalised)");
    //    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events / second (Hz)");
    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
    //---------------------------
	
    TLegend* legend = new TLegend(0.6,0.6, 0.9,0.9);
    legend->AddEntry(mHist["ND 4.0"][i], "ND 4.0","l");
    legend->AddEntry(mHist["ND 4.5"][i], "ND 4.5","l");
    //    legend->AddEntry(mHist["ND 3.8 (Feb 2024)"][i], "ND 3.8 (Feb 2024)","l");
    legend->AddEntry(mHist["ND 4.0 (Feb 2024)"][i], "ND 4.0 (Feb 2024)","l");
    
    mHist["ND 4.0"][i]->Draw("HIST");
    mHist["ND 4.5"][i]->Draw("SAME HIST");
    //    mHist["ND 3.8 (Feb 2024)"][i]->Draw("SAME HIST");
    mHist["ND 4.0 (Feb 2024)"][i]->Draw("SAME HIST");
    legend->Draw("SAME");
    c->SaveAs(("laser_"+ std::to_string(i)+".png").c_str());

    std::cout <<  mHist["ND 4.0"][i]->Integral() /  mHist["ND 4.5"][i]->Integral() << "\n";
  }
  


}

