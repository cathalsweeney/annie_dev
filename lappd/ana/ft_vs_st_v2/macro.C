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

    // Here we decide if LAPPD was likely triggered  by laser. We look at strippeak
    // in side0 (I'm 95% sure that is where the trigger board is).
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
    // There are no cuts applied for these hists
    vHist[8]->Fill(-StripPeak[laserStrip]);
    vHist[9]->Fill(-StripPeak[laserStrip+30]);
  }// end loop over events


  // In an ideal world this would reliably calculate the runtime,
  // but I don't really trust it. So I'm estimating the runtime visually (by hand)
  // by looking at a plot of EventTime
  double runtime = -1;
  if(start>0 && end > 0) runtime = end - start;
  std::cout << runtime << "/" << runtime_byHand << " = " << runtime / runtime_byHand << "\n"; 

  for(TH1F* hist : vHist) hist->Scale(1.0/runtime_byHand);
  
  return vHist;
} // end makeHist()

//---------------------------------------------------------------------

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
    
    THStack* hs = new THStack();
    hs->SetMinimum(1e-4);
    hs->SetMaximum(1e-1);
   
    hs->Add(vHist[iSide+6]);
    hs->Add(vHist[iSide+2]);
    hs->Add(vHist[iSide+4]);
    hs->Add(vHist[iSide+0]);
      
    hs->Draw();
    hs->GetYaxis()->CenterTitle(true);
    hs->GetXaxis()->CenterTitle(true);
    hs->GetYaxis()->SetTitle("Events (Area normalised)");
    hs->GetXaxis()->SetTitle(strip.c_str());

    vStack.push_back(hs);
  }
  return vStack;
}

//---------------------------------------------------------------------
void plot(std::vector<TH1F*> vHist_st, std::vector<TH1F*> vHist_ft,
	  std::string longTitle, std::string shortTitle)
// Make a nice plot with two THStacks side by side on one canvas
{
  
  std::vector<std::string> vTitle = {
    longTitle,
    "Forced Trigger"
  };


  // Make copies of the histograms which will be area-normalised
  // (original hists are time-normalised)
  std::vector<TH1F*> vHist_st_norm;
  for(TH1F* hist : vHist_st){
    TH1F* hist_norm = (TH1F*) hist->Clone();
    vHist_st_norm.push_back(hist_norm);
  }

  std::vector<TH1F*> vHist_ft_norm;
  for(TH1F* hist : vHist_ft){
    TH1F* hist_norm = (TH1F*) hist->Clone();
    vHist_ft_norm.push_back(hist_norm);
  }
  
  for(int iSide=0; iSide<2; iSide++){
    double integral_st = vHist_st[iSide+8]->Integral(0, nBins+1);  
    vHist_st_norm[iSide+0]->Scale(1./integral_st);
    vHist_st_norm[iSide+2]->Scale(1./integral_st);
    vHist_st_norm[iSide+4]->Scale(1./integral_st);
    vHist_st_norm[iSide+6]->Scale(1./integral_st);

    double integral_ft = vHist_ft[iSide+8]->Integral(0, nBins+1);  
    vHist_ft_norm[iSide+0]->Scale(1./integral_ft);
    vHist_ft_norm[iSide+2]->Scale(1./integral_ft);
    vHist_ft_norm[iSide+4]->Scale(1./integral_ft);
    vHist_ft_norm[iSide+6]->Scale(1./integral_ft);
    
  }
      
  std::vector<THStack*> vStack_st = MakeStacks(vHist_st_norm);
  std::vector<THStack*> vStack_ft = MakeStacks(vHist_ft_norm);

  
  double y_pad = 0.35;
  double top_margin = 0.2;
  double bottom_margin = 0.1;
  
  for(int iSide=0; iSide<2; iSide++){
    TCanvas* c = new TCanvas("", "", 800, 500);
    c->Draw();  
  
    TPad *p1 = new TPad("p1","p1", 0.,y_pad, 1.,1.);
    p1->Draw();
    p1->Divide(2,1);
    
    TPad *p11 = (TPad*)p1->cd(1);
    p11->SetLogy();
    p11->SetRightMargin(0.05);
    p11->SetLeftMargin(0.10);
    p11->SetTopMargin(top_margin);
    p11->SetBottomMargin(bottom_margin);
    vStack_st[iSide]->Draw("HIST");
    TLatex *   t1 = new TLatex(0.15,0.83,(vTitle[0]).c_str());
    t1->SetNDC();
    t1->SetTextSize(0.06);
    t1->Draw();
      
    TPad* p12 = (TPad*) p1->cd(2);
    p12->SetLogy();
    p12->SetRightMargin(0.05);
    p12->SetLeftMargin(0.10);
    p12->SetTopMargin(top_margin);
    p12->SetBottomMargin(bottom_margin);
    vStack_ft[iSide]->SetTitle("");
    vStack_ft[iSide]->Draw("HIST");
    TLatex *   t2 = new TLatex(0.35,0.83,(vTitle[1]).c_str());
    t2->SetNDC();
    t2->SetTextSize(0.06);
    t2->Draw();
  
    c->cd(0);
    TPad* p2 = new TPad("p2","p2", 0.,0., 1.,y_pad);
    p2->Draw();
    p2->cd();
    p2->SetTopMargin(0.02);
    
    std::vector<int> vSpace = {39, 15, 25, 46};
    TLegend* legend = new TLegend(0.0,0.0, 0.9,0.85);
    for(int iCat=0; iCat<4; iCat++){
      std::string label = "placeholder";
      if(iCat == 0) label = "Only laser strip above trigger thresh";
      else if(iCat == 1) label = "Only laser strip and neighbour above trigger thresh";
      else if(iCat == 2) label = "All other events with laser strip above thresh";
      else if(iCat == 3) label = "Laser strip below trigger thresh";
        
      int hist_idx = iSide + 2*iCat;
      double integral_ft = vHist_ft[hist_idx]->Integral();
      double integral_st = vHist_st[hist_idx]->Integral();
      std::stringstream ss;
      
      ss << std::fixed << std::setprecision(2) << label;
      // Very crude way of aligning text in legend
      for(int i=0; i<vSpace[iCat]; i++) ss << " ";
      ss << "ST / FT = " << integral_st << " / " << integral_ft << " = " << integral_st/integral_ft;
  
      legend->AddEntry(vHist_st_norm[hist_idx], ss.str().c_str(), "f");
    }  
    legend->SetTextSize(0.10);
    legend->Draw();
    TLatex *   tRatio = new TLatex(0.71,0.85,"#bf{Ratio of event rates in Hz}");
    tRatio->SetNDC();
    tRatio->SetTextSize(0.11);
    tRatio->Draw();
    
    c->cd(0);
    TLatex *   tex = new TLatex(0.45,0.93,("Side "+ std::to_string(iSide)).c_str());
    tex->SetNDC();
    tex->SetTextSize(0.06);
    tex->Draw();
      
    c->SaveAs((std::to_string(iSide)+"_"+shortTitle+".png").c_str());
  }//end for (iSide)

}

//---------------------------------------------------------------------

void macro()
{  
  int sigStrip = 5;
   
  std::map<std::string, std::vector<TH1F*> > mHist;

  // You may need to change these paths. The final argument is the runtime, which I have determined by hand in a TBrowser
  mHist["ND 4.0"] = makeHist("../files/LAPPD58/2024-05-03/forcedTrigger_2400V_nd4p0_reprocessed/Analysis.root", sigStrip, 3200);

  mHist["ND 4.0 ST 17_28"] = makeHist("../files/LAPPD58/2024-05-06/selfTrigger_dacZero17_dacOne28_2400V_nd4p0_9hz_reprocessed/Analysis.root", sigStrip, 16400);
  mHist["ND 4.0 ST 17_24"] = makeHist("../files/LAPPD58/2024-05-06/selfTrigger_dacZero17_dacOne24_2400V_nd4p0_9hz_reprocessed/Analysis.root", sigStrip, 4700);
  mHist["ND 4.0 ST 17_20"] = makeHist("../files/LAPPD58/2024-05-06/selfTrigger_dacZero17_dacOne20_2400V_nd4p0_9hz_reprocessed/Analysis.root", sigStrip, 6200);

  mHist["ND 4.0 ST 12_28"] = makeHist("../files/LAPPD58/2024-05-07/selfTrigger_dacZero12_dacOne28_2400V_nd4p0_9hz/Analysis.root", sigStrip, 9200);
  mHist["ND 4.0 ST 22_28"] = makeHist("../files/LAPPD58/2024-05-07/selfTrigger_dacZero22_dacOne28_2400V_nd4p0_9hz/Analysis.root", sigStrip, 5350);

  plot(mHist["ND 4.0 ST 17_28"], mHist["ND 4.0"],
       "Self-trigger; DAC0 17mV, DAC1 28mV",
       "dZero17_dOne28");

  plot(mHist["ND 4.0 ST 17_24"], mHist["ND 4.0"],
       "Self-trigger; DAC0 17mV, DAC1 24mV",
       "dZero17_dOne24");
  
  plot(mHist["ND 4.0 ST 17_20"], mHist["ND 4.0"],
       "Self-trigger; DAC0 17mV, DAC1 20mV",
       "dZero17_dOne20");

  plot(mHist["ND 4.0 ST 12_28"], mHist["ND 4.0"],
       "Self-trigger; DAC0 12mV, DAC1 28mV",
       "dZero12_dOne28");
   
  plot(mHist["ND 4.0 ST 22_28"], mHist["ND 4.0"],
       "Self-trigger; DAC0 22mV, DAC1 28mV",
       "dZero22_dOne28");
  

  // Here we plot data from different datasets overlayed on one canvas  
  std::vector<int> vColor = {kBlack, kRed, kBlue};
  
  std::vector<std::vector<string>> vFiles = {
    {"ND 4.0 ST 22_28", "DAC0 = 22"},
    {"ND 4.0 ST 17_28", "DAC0 = 17"},
    {"ND 4.0 ST 12_28", "DAC0 = 12"}
  };

  TCanvas* c1 = new TCanvas();
  c1->SetLogy();
  for(int iSide=0; iSide<2; iSide++){
    int counter = 0;
    TLegend* leg = new TLegend(0.7,0.7, 0.9,0.9);
    for(std::vector<string> myFile : vFiles){
      TH1F* hist = mHist[myFile[0]][iSide+8];
      hist->Scale(1.0/hist->Integral(0, nBins+1) );
      
      hist->SetLineColor(vColor[counter]);
      if(!counter){
	hist->SetTitle(("Side "+std::to_string(iSide)).c_str());
	hist->GetYaxis()->SetTitle("Events (Area normalised)");
	hist->GetXaxis()->SetTitle("-StripPeak [mV] (laser strip)");
	hist->GetYaxis()->SetRangeUser(1e-4, 1e-1);
	hist->Draw("HIST");
      }
      else{
	hist->Draw("SAME HIST");
      }
      leg->AddEntry(hist, myFile[1].c_str(), "l");
      counter++;
    }// end loop over vFiles
    leg->Draw("SAME");
    c1->SaveAs((std::to_string(iSide)+".png").c_str());
  }//end for(side)

}
