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

double thresh = 17.0;

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

  TH1F *hist0  = new TH1F("", "", 100,0,150);
  TH1F *hist1  = new TH1F("", "", 100,0,150);
  TH1F *hist2  = new TH1F("", "", 1,0,1); // just to hold the runtime
  std::vector<TH1F*> vHist = {hist0, hist1, hist2};
  
  int entries = myTree->GetEntries();

  double start = -1;
  double end = -1;
  
  for(int i=0; i<entries; i++){
    myTree->GetEntry(i);

    if(i==0) start = timestamps_meta;
    else if(i==(entries-1)) end = timestamps_meta;


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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



    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if(
       (aboveThreshLaserStrip && !aboveThreshNeighbourStrip && !aboveThreshOtherStrip) ||
       (aboveThreshLaserStrip && aboveThreshNeighbourStrip && !aboveThreshOtherStrip) ||
       (aboveThreshLaserStrip && aboveThreshNeighbourStrip && aboveThreshOtherStrip)
       ){ 
      vHist[0]->Fill(-StripPeak[laserStrip]);
      vHist[1]->Fill(-StripPeak[laserStrip+30]);      
    }

    
  }// end loop over events
  
  double runtime = -1;
  if(start>0 && end > 0) runtime = end - start;
  if(inName == "../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root") runtime -= 1691; //awful hack
  else if(inName == "../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne20_nd4p0_2400V_s5/Analysis.root") runtime -= 90; //awful hack

  std::cout << "Runtime is " << runtime << "\n";
  std::cout << std::setprecision(12) << start << "\n";
  if(runtime < 0) abort();

  vHist[2]->SetBinContent(1, runtime);
  
  return vHist;
} // end makeHist()


std::vector<TGraphErrors*> makeGraph(TH1F* hist, std::vector<double> vCut, double runtime)
{
  int nCut = vCut.size();
  if(nCut > 100){
    std::cout << "Need to allocate more memory \n";
    abort();
  }
  Double_t x[100], y1[100], y2[100], ex[100], ey1[100], ey2[100];

  for(int i=0; i<nCut; i++){
    int iBinCut = hist->FindBin(vCut[i]);
    double integLo = hist->Integral(1, iBinCut);
    double integHi = hist->Integral(iBinCut, hist->GetNbinsX());

    x[i] = vCut[i];
    y1[i] = integLo / runtime;
    y2[i] = integHi / runtime;
    ex[i] = 0.;
    ey1[i] = sqrt(integLo) / runtime;
    ey2[i] = sqrt(integHi) / runtime;
  }
  TGraphErrors* g1 = new TGraphErrors(nCut, x,y1, ex,ey1);
  TGraphErrors* g2 = new TGraphErrors(nCut, x,y2, ex,ey2);
  return {g1,g2};
}

void macro()
{
  
  int sigStrip = 5;

  std::vector<int> vColor = {kRed, kGreen+2, kBlue, kMagenta};
    
  std::map<std::string, std::vector<TH1F*> > mHist;
  mHist["Forced trigger"] = makeHist("../files/reprocessed_240411_forcedTrigger_nd4p0_LAPPD58_Analysis.root", sigStrip);
  mHist["Self trigger, DAC1 28mV"] = makeHist("../files/reprocessed_240417_selfTrigger_dacZero17_dacOne28_nd4p0_LAPPD58_Analysis.root", sigStrip);
  mHist["Self trigger, DAC1 24mV"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne24_nd4p0_2400V_s5/Analysis.root", sigStrip);
  mHist["Self trigger, DAC1 20mV"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne20_nd4p0_2400V_s5/Analysis.root", sigStrip);
  mHist["Self trigger, DAC1 16mV"] = makeHist("../files/LAPPD58/2024-04-23/selfTrigger_dacZero17_dacOne16_nd4p0_2400V_s5/Analysis.root", sigStrip);
  
  std::vector<double> vCut = {1.7, 3.2, 5., 10., 15., 20., 25., 30., 35., 40.};
  
  TCanvas* c1 = new TCanvas("c1");
  TCanvas* c2 = new TCanvas("c2");
  TCanvas* c3 = new TCanvas("c3");
  TCanvas* c4 = new TCanvas("c4");

  TLegend* legend = new TLegend(0.5,0.6, 0.9,0.9);

  int iFile = 0;
  std::map<std::string, std::vector<TH1F*> >::iterator it;
  for (it = mHist.begin(); it != mHist.end(); it++){

    std::vector<TH1F*> vHist = it->second;
    double runtime = vHist[2]->GetBinContent(1);
    std::vector<TGraphErrors*> vGraph_side0 = makeGraph(vHist[0], vCut, runtime);
    std::vector<TGraphErrors*> vGraph_side1 = makeGraph(vHist[1], vCut, runtime);
    if(iFile == 0){
      legend->AddEntry(vGraph_side0[0], it->first.c_str(),"epl");
      
      c1->cd();
      vGraph_side0[0]->SetTitle("Side 0");
      vGraph_side0[0]->GetYaxis()->SetRangeUser(0., 15.);
      vGraph_side0[0]->GetYaxis()->SetTitle("Noise rate (Events / second)");
      vGraph_side0[0]->GetXaxis()->SetTitle("Cut value (mV)");
      vGraph_side0[0]->Draw("AEPL");

      c2->cd();
      vGraph_side0[1]->SetTitle("Side 0");
      vGraph_side0[1]->GetYaxis()->SetRangeUser(0., 15.);
      vGraph_side0[1]->GetYaxis()->SetTitle("Signal rate (Events / second)");
      vGraph_side0[1]->GetXaxis()->SetTitle("Cut value (mV)");
      vGraph_side0[1]->Draw("AEPL");      

      c3->cd();
      vGraph_side1[0]->SetTitle("Side 1");
      vGraph_side1[0]->GetYaxis()->SetRangeUser(0., 15.);
      vGraph_side1[0]->GetYaxis()->SetTitle("Noise rate (Events / second)");
      vGraph_side1[0]->GetXaxis()->SetTitle("Cut value (mV)");
      vGraph_side1[0]->Draw("AEPL");

      c4->cd();
      vGraph_side1[1]->SetTitle("Side 1");
      vGraph_side1[1]->GetYaxis()->SetRangeUser(0., 15.);
      vGraph_side1[1]->GetYaxis()->SetTitle("Signal rate (Events / second)");
      vGraph_side1[1]->GetXaxis()->SetTitle("Cut value (mV)");
      vGraph_side1[1]->Draw("AEPL");      
    }
    else{
      legend->AddEntry(vGraph_side0[0], it->first.c_str(),"epl");

      c1->cd();
      vGraph_side0[0]->SetLineColor(vColor[iFile-1]);
      vGraph_side0[0]->Draw("SAME EPL");

      c2->cd();
      vGraph_side0[1]->SetLineColor(vColor[iFile-1]);
      vGraph_side0[1]->Draw("SAME EPL");      

      c3->cd();
      vGraph_side1[0]->SetLineColor(vColor[iFile-1]);
      vGraph_side1[0]->Draw("SAME EPL");

      c4->cd();
      vGraph_side1[1]->SetLineColor(vColor[iFile-1]);
      vGraph_side1[1]->Draw("SAME EPL");      

    }

    iFile++;
  }
  c1->cd();
  legend->Draw("SAME.png");
  c1->SaveAs("c1.png");

  c2->cd();
  legend->Draw("SAME.png");
  c2->SaveAs("c2.png");

  c3->cd();
  legend->Draw("SAME.png");
  c3->SaveAs("c3.png");

  c4->cd();
  legend->Draw("SAME.png");
  c4->SaveAs("c4.png");

  //  //  c->SetLogy();
//
//
//  for(int i=0; i <2; i++){
//    std::string strip = "-StripPeak [mV] (strip "+std::to_string(sigStrip)+")";
//
//
//    mHist["ND 4.0 ST"][i+6]->SetFillColor(kBlue);
//    mHist["ND 4.0 ST"][i+4]->SetFillColor(kRed);
//    mHist["ND 4.0 ST"][i+2]->SetFillColor(kGreen+2);
//    mHist["ND 4.0 ST"][i+0]->SetFillColor(kMagenta);
//  
//    mHist["ND 4.0 ST"][i+6]->SetLineWidth(0);
//    mHist["ND 4.0 ST"][i+4]->SetLineWidth(0);
//    mHist["ND 4.0 ST"][i+2]->SetLineWidth(0);
//    mHist["ND 4.0 ST"][i+0]->SetLineWidth(0);
//  
//    THStack* hs = new THStack("hs",("Side " + std::to_string(i) + ";" + strip + ";Events").c_str());
//    hs->SetMinimum(1e-6);
//    //    hs->SetMaximum(10);
//    hs->SetMaximum(0.05);
//   
//    //  hs->GetYaxis()->SetRangeUser(1, 1e5);
//    hs->Add(mHist["ND 4.0 ST"][i+0]);
//    hs->Add(mHist["ND 4.0 ST"][i+2]);
//    hs->Add(mHist["ND 4.0 ST"][i+4]);
//    hs->Add(mHist["ND 4.0 ST"][i+6]);
//    hs->Draw("HIST");
//  
//  
//  
//    TH1F* hFT = (TH1F*) mHist["ND 4.0"][i+0]->Clone();
//    hFT->Add(mHist["ND 4.0"][i+2]);
//    hFT->Add(mHist["ND 4.0"][i+4]);
//    hFT->Add(mHist["ND 4.0"][i+6]);
//    hFT->Scale(vScale[i]);
//    hFT->Draw("SAME HIST");
//  
//    TLegend* legend = new TLegend(0.5,0.6, 0.9,0.9);
//    legend->AddEntry(mHist["ND 4.0 ST"][i+0], "Self trigger: laser","f");
//    legend->AddEntry(mHist["ND 4.0 ST"][i+2], "Self trigger: laser + neighbour","f");
//    legend->AddEntry(mHist["ND 4.0 ST"][i+4], "Self trigger: laser + neighbour + other","f");
//    legend->AddEntry(mHist["ND 4.0 ST"][i+6], "Self trigger: everything else","f");
//    legend->AddEntry(hFT, ( std::to_string(vScale[i])+" x Forced trigger").c_str(),"l");
//  
//    legend->Draw("SAME");
//    
//    c->SaveAs( (std::to_string(i)+"_stack.png").c_str() );
//
//  }
//  
//  //  for(int i=0; i<nHist; i++){  
////
////    std::string strip;
////    if(i%2 == 0) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 0)";
////    else if(i%2 == 1) strip = "StripPeak (strip "+std::to_string(sigStrip)+", side 1)";
////
////    //---------------------------
////    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-6, 10);
////    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Events / second (Hz)");
////    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
////    //---------------------------
////
////    //~~~~~~~~~~~~~~~~~~~~~~~
//////    mHist["ND 4.0"][i]->GetYaxis()->SetRangeUser(1e-4, 1e5);
//////    mHist["ND 4.0"][i]->GetYaxis()->SetTitle("Ratio to ND 4.0");
//////    mHist["ND 4.0"][i]->GetXaxis()->SetTitle( strip.c_str());
//////    mHist["ND 4.0 ST"][i] = DivideHist(mHist["ND 4.0 ST"][i], mHist["ND 4.0"][i]);
//////    mHist["ND 4.0"][i] = DivideHist(mHist["ND 4.0"][i], mHist["ND 4.0"][i]);    
////    //~~~~~~~~~~~~~~~~~~~~~~~
////    
////    
////    mHist["ND 4.0 ST"][i]->SetLineColor(kBlue);
////	
////    TLegend* legend = new TLegend(0.75,0.7, 0.9,0.9);
////    legend->AddEntry(mHist["ND 4.0"][i], "ND 4.0","l");
////    legend->AddEntry(mHist["ND 4.0 ST"][i], "ND 4.0 ST","l");
////				    
////    mHist["ND 4.0"][i]->Draw("HIST");
////    mHist["ND 4.0 ST"][i]->Draw("SAME HIST");
////    legend->Draw("SAME");
////    c->SaveAs(("laser_"+ std::to_string(i)+".png").c_str());
////  }
//  


}

