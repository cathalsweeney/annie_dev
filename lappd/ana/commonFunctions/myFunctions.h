#include "TAxis.h"
#include "TMath.h"
#include "TFile.h"
#include "THStack.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>
#include <chrono>

double headroom = 0.3;

double getRunTime(TTree* myTree)
{    
  int nBins = 50;
  
  double EventTime;  
  myTree->SetBranchAddress("EventTime",&EventTime);
  int entries = myTree->GetEntries();


  double endTime = 0.;
  for(int i=0; i<entries; i++){
    myTree->GetEntry(i);    
    if(EventTime > endTime) endTime = EventTime;
  }

  
  TH1F *hist  = new TH1F("", "", nBins, 0., 1.1*endTime);  
 
  for(int i=0; i<entries; i++){
    myTree->GetEntry(i);    
    hist->Fill(EventTime);    
  }// end loop over events

  TCanvas* c = new TCanvas();
  hist->Draw("HIST");
  auto now = std::chrono::system_clock::now();
  auto UTC = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
  c->SaveAs((std::to_string(UTC) + ".png").c_str());

  
  bool atEdge = false;
  bool weird = false;
  double sum = 0.;
  int nCountedBins = 0.;
  int nZero = 0.;
  std::vector<int> vBinIdx;
  
  for(int iBin=nBins; iBin>0; iBin--){

    int cBin = hist->GetBinContent(iBin);
    //    std::cout << iBin << ", " << cBin << "\n";    

    if(!atEdge){
      if(cBin == 0){
	continue;
      }
      else{
	atEdge = true;
	vBinIdx.push_back(iBin);
      }
    }// end if(!atEdge)
    else{
      if( (vBinIdx.at(0) - iBin) < 4){
	sum += cBin;
	nCountedBins++;
      }
      else{ //more than 4 from edge
	double avg = sum / (double) nCountedBins;
	if(cBin/avg < (1+headroom) && cBin/avg > (1-headroom)){
	  sum += cBin;
	  nCountedBins++;
	  weird = false;
	}
      	else if(cBin/avg < (2+headroom) && cBin/avg > (2-headroom)){
	  if(vBinIdx.size() == 1){
	    vBinIdx.push_back(iBin);
	  }
	  if(vBinIdx.size() != 2){
	    std::cout << "Something wrong here A \n";
	    abort();
	  }
	  sum += (cBin/2.0);
	  nCountedBins++;
	  weird = false;
	}
      	else if(cBin/avg < (3+headroom) && cBin/avg > (3-headroom)){
	  if(vBinIdx.size() == 2){
	    vBinIdx.push_back(iBin);
	  }
	  if(vBinIdx.size() != 3){
	    std::cout << "Something wrong here B \n";
	    abort();
	  }
	  sum += (cBin/3.0);
	  nCountedBins++;
	  weird = false;
	}
      	else if(cBin/avg < (4+headroom) && cBin/avg > (4-headroom)){
	  if(vBinIdx.size() == 3){
	    vBinIdx.push_back(iBin);
	  }
	  if(vBinIdx.size() != 4){
	    std::cout << "Something wrong here C \n";
	    abort();
	  }
	  sum += (cBin/4.0);
	  nCountedBins++;
	  weird = false;
	}
      	else if(cBin/avg < 0.4){
	  nZero++;
	  if(nZero > 4){
	    std::cout << "Too many empty bins \n"; 
	    abort();
	  }
	  weird = false;
	}
	else{
	  if(weird){
	   std::cout << "Something wrong here D \n";
	   std::cout << iBin << ", " << cBin << ", " << avg << "\n";
	   return -1;
	   //	   abort();	  
	  }
	  weird = true;
	}
	
      }// end more than 4 from edge

    }// end else(atEdge)
    
  }// end for(iBin)

  double runTime = 0.;
  for(int binIdx : vBinIdx){
    double val = hist->GetXaxis()->GetBinLowEdge(binIdx+1);
    runTime += val;
  }
  runTime -= nZero*hist->GetXaxis()->GetBinWidth(1);;
  
  return runTime;
} // end getRunTime()

