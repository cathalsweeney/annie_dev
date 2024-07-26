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
  double StripMinAmp[nChannels];

  int  Npulses_simp;
  int pulsestrip_simp[100];
  int pulseside_simp[100];
  double EventTime;

  myTree->SetBranchAddress("timestamps_meta",&timestamps_meta);
  myTree->SetBranchAddress("NHits",&NHits);
  myTree->SetBranchAddress("xpar",xpar);
  myTree->SetBranchAddress("xperp",xperp);
  myTree->SetBranchAddress("StripMinAmp",StripMinAmp);

  
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
    
    vHist[0]->Fill(-StripMinAmp[laserStrip]);
    vHist[1]->Fill(-StripMinAmp[laserStrip+30]);

    vHist[2]->Fill(-StripMinAmp[noiseStrip]);
    vHist[3]->Fill(-StripMinAmp[noiseStrip+30]);

  }// end loop over events
  std::cout << vHist[0]->Integral() / (double) vHist[1]->Integral() << ", " <<  vHist[2]->Integral() / (double) vHist[3]->Integral() << "\n";

  
  double runtime = -1;
  if(start>0 && end > 0) runtime = end - start;
  if(inName == "../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root") runtime -= 1691; //awful hack
  else if(inName == "../files/240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root") runtime -= 1691; //awful hack
  
  std::cout << "Runtime is " << runtime << "\n";
  std::cout << std::setprecision(12) << start << "\n";
  if(runtime < 0) abort();
  
  for(TH1F* hist : vHist) hist->Scale(1.0/runtime);
  
  return vHist;
} // end makeHist()


void stripminamp_plot_assort()
{

  //TH1F* hist1 = makeHist("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");


  int sigStrip = 5;
  
  //TH1F* hist1 = makeHist("240314_lappd39_forceTrigger_nd3p0_daczero18_dacone130_5k_Analysis.root");
  

  
  std::map<std::string, std::vector<TH1F*> > mHist;

//  mHist["ND 4.5"] = makeHist("files/240411_forcedTrigger_nd4p5_LAPPD58_Analysis.root", sigStrip);
//  mHist["ND 4.7"] = makeHist("files/240412_forcedTrigger_nd4p7_LAPPD58_Analysis.root", sigStrip);
//  mHist["ND 3.7"] = makeHist("files/240415_forcedTrigger_nd3p7_LAPPD58_Analysis.root", sigStrip);
  mHist["ND 4.0"] = makeHist("../files/240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root", sigStrip);
  mHist["ND 4.0 ST"] = makeHist("../files/240417_selfTrigger_dacZero17_dacOne28_nd4p0_LAPPD58_Analysis.root", sigStrip);


  
  //  mHist["ND 4.0"] = makeHist("../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root", sigStrip);
  //  mHist["ND 4.0 ST"] = makeHist("../files/reprocessed_240417_selfTrigger_dacZero17_dacOne28_nd4p0_LAPPD58_Analysis.root", sigStrip);
  
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
    if(i == 0) strip = "StripMinAmp (strip "+std::to_string(sigStrip)+", side 0)";
    else if(i == 1) strip = "StripMinAmp (strip "+std::to_string(sigStrip)+", side 1)";
    else if(i == 2) strip = ("StripMinAmp (strip "+std::to_string(noiseStrip)+", side 0)").c_str();
    else if(i == 3) strip = ("StripMinAmp (strip "+std::to_string(noiseStrip)+", side 1)").c_str();


    //~~~~~~~~~~~~~~~~~~~~~~~~
//    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 1e5);
//    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Ratio to ND 4.0");
//    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
//    mHist["ND 4.0 ST"][i] = DivideHist(mHist["ND 4.0 ST"][i], mHist["ND 4.0"][i]);
//    mHist["ND 4.0"][i] = DivideHist(mHist["ND 4.0"][i], mHist["ND 4.0"][i]);
    //~~~~~~~~~~~~~~~~~~~~~~~~

    //---------------------------
    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 10);
    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events / second (Hz)");
    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
    //---------------------------

    //<<<<<<<<<<<<<
//    if(i==1 || i==3) continue;
//    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-2, 1e2);
//    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Side 0 / Side 1");
//    mHist["ND 4.0"][i] = DivideHist(mHist["ND 4.0"][i], mHist["ND 4.0"][i+1]);
//    mHist["ND 4.7"][i] = DivideHist(mHist["ND 4.7"][i], mHist["ND 4.7"][i+1]);
//    mHist["ND 3.7"][i] = DivideHist(mHist["ND 3.7"][i], mHist["ND 3.7"][i+1]);
//    mHist["ND 4.0 ST"][i] = DivideHist(mHist["ND 4.0 ST"][i], mHist["ND 4.0 ST"][i+1]);
//    mHist["ND 4.5"][i] = DivideHist(mHist["ND 4.5"][i], mHist["ND 4.5"][i+1]);
    //<<<<<<<<<<<<<

//    mHist["ND 4.5"][i]->SetLineColor(kBlue);
//    mHist["ND 4.7"][i]->SetLineColor(kRed);
//    mHist["ND 3.7"][i]->SetLineColor(kGreen+2);
    mHist["ND 4.0 ST"][i]->SetLineColor(kBlue+2);
	
    TLegend* legend = new TLegend(0.75,0.7, 0.9,0.9);
    //    legend->AddEntry(mHist["ND 3.7"][i], "ND 3.7","l");
    legend->AddEntry(mHist["ND 4.0"][i], "ND 4.0","l");
    legend->AddEntry(mHist["ND 4.0 ST"][i], "ND 4.0 ST","l");
//    legend->AddEntry(mHist["ND 4.5"][i], "ND 4.5","l");
//    legend->AddEntry(mHist["ND 4.7"][i], "ND 4.7","l");
				    
    mHist["ND 4.0"][i]->Draw("HIST");
//    mHist["ND 4.5"][i]->Draw("SAME HIST");
//    mHist["ND 4.7"][i]->Draw("SAME HIST");
//    mHist["ND 3.7"][i]->Draw("SAME HIST");
    mHist["ND 4.0 ST"][i]->Draw("SAME HIST");
    legend->Draw("SAME");
    c->SaveAs(("laser_"+ std::to_string(i)+".png").c_str());

    std::cout <<  mHist["ND 4.0"][i]->Integral() /  mHist["ND 4.0 ST"][i]->Integral() << "\n";
  }
  


}

