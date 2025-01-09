#include "TGraph.h"
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

int nLAPPD=3;

double x_low = 0.;
double x_high = 20000.;
int nBins = 15;

std::vector<TH1L*> makeHists(std::string inName)
{  
  TFile *inFile = new TFile(inName.c_str());
    
  TTree *myTree;
  myTree = (TTree*)inFile->Get("Waveform");
   
  int LAPPD_ID;
  ULong64_t LAPPDDataTimeStampUL;
  ULong64_t LAPPDDataBeamgateUL;
  
  myTree->SetBranchAddress("LAPPD_ID",&LAPPD_ID);
  myTree->SetBranchAddress("LAPPDDataTimeStampUL",&LAPPDDataTimeStampUL);
  myTree->SetBranchAddress("LAPPDDataBeamgateUL",&LAPPDDataBeamgateUL);
  
  // vHist[ID][stripNo][side]
  std::vector<TH1L*>  vHist;

  for(int iLAPPD=0; iLAPPD<nLAPPD; iLAPPD++){
      TH1L *hist  = new TH1L("", "", nBins, x_low, x_high);
      vHist.push_back(hist);
  }
  
  int entries = myTree->GetEntries();
  
  for(int i=0; i<entries; i++){
    myTree->GetEntry(i);
    vHist[LAPPD_ID]->Fill((LAPPDDataTimeStampUL - LAPPDDataBeamgateUL)*3.125);
  }// end loop over events
  
  return vHist;
} // end makeHist()


void macro()
{  

  std::vector<std::string> vName = {
    "../files/tank/5140/LAPPDTree.root",
    "../files/tank/5154/LAPPDTree.root",
    "../files/tank/5174/LAPPDTree.root",
    "../files/tank/5240/LAPPDTree.root",
  };
  std::vector<double> vDuration = {19.4, 22.1, 17.6, 36.7};
  
  std::vector<std::vector<TH1L*>> vHist_file;

  int nFile = vName.size();
  for(int i=0; i<nFile; i++){
    std::vector<TH1L*>  vHist = makeHists(vName[i]);
    for(TH1L* hist : vHist){
      for(int iBin=1; iBin<=nBins; iBin++){	
	double val = hist->GetBinContent(iBin);
	double new_val = val / vDuration[i];
	double bin_err = std::sqrt(val) / vDuration[i];
	hist->SetBinContent(iBin, new_val);
	hist->SetBinError(iBin, bin_err);
      }
    }
    
    vHist_file.push_back(vHist);

  }
  

  std::vector<double> vYLim = {500., 14000., 10000.};
  std::vector<int> vColor = {kBlue, kRed, kGreen+2, kMagenta};
  
  TCanvas* c = new TCanvas();
  //  c->SetLogy();

  for(int iLAPPD=0; iLAPPD<nLAPPD; iLAPPD++){

    vHist_file[0][iLAPPD]->SetLineColor(kBlue);
    vHist_file[0][iLAPPD]->GetYaxis()->SetRangeUser(0., vYLim[iLAPPD]);
    vHist_file[0][iLAPPD]->GetYaxis()->SetTitle("Events per hour");
    vHist_file[0][iLAPPD]->GetXaxis()->SetTitle("T_{data} - T_{beamgate} [ns]");
    vHist_file[0][iLAPPD]->Draw("HIST");
    vHist_file[0][iLAPPD]->Draw("E1 SAME");


    for(int iFile=1; iFile<nFile; iFile++){
      vHist_file[iFile][iLAPPD]->SetLineColor(vColor[iFile]);
      vHist_file[iFile][iLAPPD]->Draw("SAME HIST");
      vHist_file[iFile][iLAPPD]->Draw("SAME E1");
    }


    
    c->SaveAs((std::to_string(iLAPPD)+".png").c_str());
    
//    vHist[0][iStrip][iSide]->GetYaxis()->SetRangeUser(1e-3, 1e5);
//    vHist[0][iStrip][iSide]->GetYaxis()->SetTitle("Events / hour");
//    vHist[0][iStrip][iSide]->GetXaxis()->SetTitle("Max waveform amplitude [mV]");
//    //vHist[0][iStrip][iSide]->SetTitle(title.c_str());
//
//      
//      vHist[0][iStrip][iSide]->SetLineColor(kBlack);
//      vHist[0][iStrip][iSide]->Draw("HIST");
//
//      vHist[1][iStrip][iSide]->SetLineColor(kRed);
//      vHist[1][iStrip][iSide]->Draw("SAME HIST");
//
//      vHist[2][iStrip][iSide]->SetLineColor(kBlue);
//      vHist[2][iStrip][iSide]->Draw("SAME HIST");
//
//      TLegend* leg = new TLegend(0.7, 0.6, 0.90, 0.85);
//      leg->AddEntry(vHist[0][iStrip][iSide], "LAPPD 64", "l");
//      leg->AddEntry(vHist[1][iStrip][iSide], "LAPPD 58", "l");
//      leg->AddEntry(vHist[2][iStrip][iSide], "LAPPD 39", "l");
//      leg->Draw("SAME");
//
//      TLatex *latex = new TLatex();
//      latex->SetTextSize(0.08);           // Set text size
//      //latex->DrawLatex(60, 1e3, ("Channel " + std::to_string(iStrip)).c_str() );
//      latex->DrawLatex(60, 1e3, ("Side " + std::to_string(iSide)).c_str() ); 
//      
//      std::stringstream ss;
//      ss << iStrip << "_" << iSide << ".png";
//      c->SaveAs(ss.str().c_str());
  }
}
