#include <iostream>
#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>

struct  threshStruct {  
  double           dac0;       
  double           dac1;
  std::vector<int> vTrig; // Number of triggers in 60 seconds.
                          // Each elem is a separate data-taking
                          // (in case we do multiple for a given setting)
};    


// Data taken on Feb 28th 2024, unknown oscope trigger level
//std::vector<threshStruct> vData = {
//  {1.228, 1.224, {127}},
//  {1.226, 1.224, {144}},
//  {1.224, 1.224, {109}},
//
//  {1.228, 1.226, {3066}},
//  {1.228, 1.228, {7912}},
//
//  {1.224, 1.223, {29}},
//  {1.223, 1.223, {45}},
//};


// Data taken on Mar 1st 2024, oscope trigger level -30 mV
//std::vector<threshStruct> vData = {
//
//  {1.228, 1.228, {5315}},
//  {1.228, 1.227, {1329}},
//  {1.228, 1.226, {466}},
//  {1.228, 1.225, {204}},
//  {1.228, 1.224, {16}},
//  {1.228, 1.223, {10}},
//
//  {1.227, 1.223, {7}},
//  {1.227, 1.224, {9}},
//  {1.227, 1.225, {114}},
//  {1.227, 1.226, {318}},
//
//  {1.226, 1.223, {1}},
//  {1.226, 1.224, {12}},
//  {1.226, 1.225, {89}},
//
//  {1.225, 1.223, {2}},
//  {1.225, 1.224, {2}},
//
//  {1.224, 1.223, {0}},
//  {1.224, 1.224, {6}},
//
//  {1.228, 1.222, {5}},
//  {1.227, 1.222, {0}},
//  {1.226, 1.222, {0}},
//
//  {1.228, 1.221, {0}},
//  {1.228, 1.220, {0}},
//
//  {1.225, 1.222, {0}},
//  {1.224, 1.222, {0}},
//  {1.223, 1.222, {0}},
//  {1.222, 1.222, {0}},
//
//  {1.223, 1.223, {1}},
//
//  {1.229, 1.222, {2}},
//  {1.229, 1.223, {5}},
//  {1.229, 1.224, {29}},
//  {1.229, 1.225, {296}},
//
//  {1.230, 1.222, {1}},
//  {1.230, 1.223, {10}},
//  {1.230, 1.224, {36}},
//  {1.230, 1.225, {400}},
//};


// Data taken on Mar 12th 2024, HV on, no laser
// LAPPD 39, now with "spare" trigger board
// Likely garbage due to 10s timeout
//std::vector<threshStruct> vData = {
//  {1.225, 1.170, {9}},
//  {1.225, 1.200, {142}},
//  {1.225, 1.220, {161}},
//
//  {1.230, 1.170, {13}},
//  {1.230, 1.200, {147}},
//  {1.230, 1.220, {60}},
//
//  {1.235, 1.170, {15}},
//  {1.235, 1.200, {116}},
//  {1.235, 1.220, {139}},
//  
//  {1.235, 1.230, {118}},
//  
//};

// Data taken on Mar 12th 2024, HV on, no laser
// LAPPD 39, now with "spare" trigger board
// 1s timeout
std::vector<threshStruct> vData = {
  {1.225, 1.170, {3}},
  {1.225, 1.200, {320}},
  {1.225, 1.220, {355}},

  {1.230, 1.170, {8}},
  {1.230, 1.200, {339}},
  {1.230, 1.220, {345}},

  {1.235, 1.170, {12}},
  {1.235, 1.200, {334}},
  {1.235, 1.220, {332}},
  
  {1.235, 1.230, {312}},
  
};



void createAndSaveHistogram() {

  double period = 30;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // This block is to determine the number of bins and axis ranges
  double max0 = 0.;
  double min0 = 1.25;

  double max1 = 0.;
  double min1 = 1.25;

  int maxCount = 0;

  for(threshStruct data : vData){
    if(data.dac0 > max0) max0 = data.dac0;
    if(data.dac0 < min0) min0 = data.dac0;

    if(data.dac1 > max1) max1 = data.dac1;
    if(data.dac1 < min1) min1 = data.dac1;

    std::vector<int> vTrig = data.vTrig;
    double count = accumulate(vTrig.begin(), vTrig.end(), 0.0) / vTrig.size();  
    if(count > maxCount) maxCount = count;    
  }

  double maxRate = maxCount / period;

  double diff0 = max0-min0;
  if( fmod(diff0,0.001) >  1e-6 ){ // fmod() has precision issues
    std::cout << "We accept only data at grid points of size 0.001 \n";
    exit(1);
  }
  
  double diff1 = max1-min1;
  if( fmod(diff1,0.001) >  1e-6 ){ // fmod() has precision issues
    std::cout << "We accept only data at grid points of size 0.001 \n";
    exit(1);
  }

  int nBin0 = std::round(diff0/0.001) + 1;
  int nBin1 = std::round(diff1/0.001) + 1;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  // Create a canvas
  TCanvas *canvas = new TCanvas("canvas", "2D Histogram", 800, 600);
  canvas->SetLeftMargin(0.12);
  canvas->SetRightMargin(0.18);
  canvas->SetBottomMargin(0.12);
   
  // Create a 2D histogram
  // Define binning such that data point lives in the centre of the bin rather than at the boundary
  TH2F *h2 = new TH2F("histogram", "LAPPD 39 trigger board noise;DAC0 (V);DAC1 (V); Trigger rate (Hz)",
		      nBin0, min0-0.0005, max0+0.0005,
		      //		      9, 1.2195, 1.2285,
		      nBin1, min1-0.0005, max1+0.0005);

  h2->GetXaxis()->CenterTitle(true);
  h2->GetXaxis()->SetTitleOffset(1.03);

  h2->GetYaxis()->CenterTitle(true);
  h2->GetYaxis()->SetTitleOffset(1.03);

    h2->GetZaxis()->SetRangeUser(5e-4,maxRate*1.2);
  
  for(threshStruct data : vData){
    //    std::cout << data.dac0 << " " << data.dac1 << " " << data.vTrig[0] << "\n";

    std::vector vTrig = data.vTrig;
    int nElem = vTrig.size();
    double sum = accumulate(vTrig.begin(), vTrig.end(), 0.0);  
    if(sum == 0) sum = 1e-1; // just so log scale will work

    double mean = sum / nElem;

    
    double rate = mean / period; // divide by number of seconds of data taking  
    double err = rate / std::sqrt(sum);

    int iBinX = h2->GetXaxis()->FindBin(data.dac0);
    int iBinY = h2->GetYaxis()->FindBin(data.dac1);

//    if(rate > 80){
//      std::cout  << data.dac0 << " " << iBinX << "\n";
//      std::cout  << data.dac1 << " " << iBinY << "\n";
//    }
    double val = h2->GetBinContent(iBinX, iBinY);
    if(val){
      std::cout << "accidental overwrite \n";
      std::cout << data.dac0 << " " << data.dac1 << " " << data.vTrig[0] << "\n";
      exit(1);
    }
    
    h2->SetBinContent(iBinX, iBinY, rate);
    h2->SetBinError(iBinX, iBinY, err);
  }

  
  
//  // Fill the histogram with some random data
//  for (int i = 0; i < 10000; ++i) {
//    double x = gRandom->Gaus(0, 1); // Gaussian random number for x
//    double y = gRandom->Gaus(0, 1); // Gaussian random number for y
//        h2->Fill(x, y);
//  }


  // Draw the histogram
  h2->Draw("colz");

  //canvas->SetLogz(true);
  
  // Save the histogram as a PNG file
  canvas->SaveAs("histogram.png");
  
  // Clean up
  delete canvas;
  delete h2;
}

void trigBoardNoiseRate() {
  createAndSaveHistogram();
}
