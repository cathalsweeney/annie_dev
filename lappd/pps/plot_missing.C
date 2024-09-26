// Plots the difference in clock ticks between successive PPS events

#include "TAxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TLine.h"
#include "TDatime.h"
#include "TCanvas.h"
#include <iostream>

int nLAPPD = 3;
int nBins = 100;
int prec = 3;
double textX = -5.5e8;
double textSize = 0.05;

void PlotAll(std::vector<std::pair<TH1F*, TH1F*>> vHist, std::string outname,
	     std::vector<std::pair<std::vector<int>,std::vector<int> > > vCounts )
{
  TCanvas* c = new TCanvas("", "", 1300,600);
  //  c->SetLogy(true);


  std::vector<std::pair<std::vector<Color_t>,std::vector<Color_t> > > vColor;
  Color_t g = kGreen+2;
  Color_t r = kRed;
  Color_t b = kBlack;
  vColor.push_back({ {r,r,g}, {r,g,r} });
  vColor.push_back({ {r,r,g}, {r,r,g} });
  vColor.push_back({ {r,g,r}, {r,g,r} });

//  vColor.push_back({ {b,b,b}, {b,b,b} });
//  vColor.push_back({ {b,b,b}, {b,b,b} });
//  vColor.push_back({ {b,b,b}, {b,b,b} });
  
  
  c->Range(0.,0., 1.,1.);

  TLine	*line1 = new TLine(1./3., 0, 1./3., 1);
  TLine	*line2 = new TLine(2./3., 0, 2./3., 1);
  line1->SetLineColor(kBlack);
  line1->SetLineWidth(2);
  line1->Draw();
  line2->Draw();

  // Divide the canvas into three vertical segments
  c->Divide(3, 2);  // 3 columns, 2 rows


  // Plot in each segment

  for(int i=0; i<nLAPPD; i++){
    c->cd(i+1);
    gPad->SetLogy(true);
    int int_1 = vCounts.at(i).first.at(0) + vCounts.at(i).first.at(1) + vCounts.at(i).first.at(2);
    double frac1_1 = vCounts.at(i).first.at(0) / (double) int_1;
    double frac2_1 = vCounts.at(i).first.at(1) / (double) int_1;
    double frac3_1 = vCounts.at(i).first.at(2) / (double) int_1;
    std::ostringstream out0_1;
    out0_1 << "nEvents = " << int_1 ;
    std::ostringstream out1_1;
    out1_1 << "(#Delta t = 0) :       " << setprecision(prec) << (frac1_1*100) << "%";
    std::ostringstream out2_1;
    out2_1 << "(#Delta t = 3.2e8) : " << setprecision(prec) << (frac2_1*100) << "%";
    std::ostringstream out3_1;
    out3_1 << "Other :            " << setprecision(prec) << (frac3_1*100) << "%";
    vHist[i].first->Draw("HIST"); 
    TLatex* text0_1 = new TLatex(textX, 64e-4, out0_1.str().c_str() );
    text0_1->SetTextSize(textSize);
    text0_1->Draw("SAME");
    TLatex* text1_1 = new TLatex(textX,16e-4, out1_1.str().c_str() );
    text1_1->SetTextSize(textSize);
    text1_1->SetTextColor(vColor.at(i).first.at(0));
    text1_1->Draw("SAME");
    TLatex* text2_1 = new TLatex(textX,4e-4, out2_1.str().c_str() );
    text2_1->SetTextSize(textSize);
    text2_1->SetTextColor(vColor.at(i).first.at(1));
    text2_1->Draw("SAME");
    TLatex* text3_1 = new TLatex(textX,1e-4, out3_1.str().c_str() );
    text3_1->SetTextSize(textSize);
    text3_1->SetTextColor(vColor.at(i).first.at(2));
    text3_1->Draw("SAME");
    gPad->Update();
    TPaveStats *st1 = (TPaveStats*)vHist[i].first->FindObject("stats");
    st1->SetX1NDC(2.); //new x start position
    st1->SetX2NDC(2.); //new x end position
    st1->SetY1NDC(2.); 
    st1->SetY2NDC(2.); 


    
    c->cd(i+1+3);
    gPad->SetLogy(true);
    int int_2 = vCounts.at(i).second.at(0) + vCounts.at(i).second.at(1) + vCounts.at(i).second.at(2);
    double frac1_2 = vCounts.at(i).second.at(0) / (double) int_2;
    double frac2_2 = vCounts.at(i).second.at(1) / (double) int_2;
    double frac3_2 = vCounts.at(i).second.at(2) / (double) int_2;
    std::ostringstream out0_2;
    out0_2 << "nEvents = " << int_2 ;
    std::ostringstream out1_2;
    out1_2 << "(#Delta t = 0) :       " << setprecision(prec) << (frac1_2*100) << "%";
    std::ostringstream out2_2;
    out2_2 << "(#Delta t = 3.2e8) : " << setprecision(prec) << (frac2_2*100) << "%";
    std::ostringstream out3_2;
    out3_2 << "Other :            " << setprecision(prec) << (frac3_2*100) << "%";
    vHist[i].second->Draw("HIST"); 
    TLatex* text0_2 = new TLatex(textX,64e-4, out0_2.str().c_str() );
    text0_2->SetTextSize(textSize);
    text0_2->Draw("SAME");
    TLatex* text1_2 = new TLatex(textX,16e-4, out1_2.str().c_str() );
    text1_2->SetTextSize(textSize);
    text1_2->SetTextColor(vColor.at(i).second.at(0));
    text1_2->Draw("SAME");
    TLatex* text2_2 = new TLatex(textX,4e-4, out2_2.str().c_str() );
    text2_2->SetTextSize(textSize);
    text2_2->SetTextColor(vColor.at(i).second.at(1));
    text2_2->Draw("SAME");
    TLatex* text3_2 = new TLatex(textX,1e-4, out3_2.str().c_str() );
    text3_2->SetTextSize(textSize);
    text3_2->SetTextColor(vColor.at(i).second.at(2));
    text3_2->Draw("SAME");
    gPad->Update();
    TPaveStats *st2 = (TPaveStats*)vHist[i].second->FindObject("stats");
    st2->SetX1NDC(2.); //new x start position
    st2->SetX2NDC(2.); //new x end position
    st2->SetY1NDC(2.); 
    st2->SetY2NDC(2.); 

    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

//    TPad *pad = new TPad("pad", "pad", 0.2, 0.4, 0.4, 0.8);
//    pad->Draw();
//    pad->cd();
//    // Create the table size (2 rows x 4 columns)
//    const int nRows = 4;
//    const int nCols = 2;    
//    // Create a table and fill in the cells
//    double cellWidth = 1.0 / nCols;
//    double cellHeight = 1.0 / nRows;    
//    for (int row = 0; row < nRows; ++row) {
//        for (int col = 0; col < nCols; ++col) {
//            double x1 = col * cellWidth;
//            double y1 = 1 - (row + 1) * cellHeight;
//            double x2 = (col + 1) * cellWidth;
//            double y2 = 1 - row * cellHeight;            
//            // Draw the box for the cell
//            pad->cd();
//            TBox *box = new TBox(x1, y1, x2, y2);
//            box->SetFillStyle(0); // No fill
//            box->Draw();
//            // Add some text in the cell
//            TString cellText = TString::Format("R%dC%d", row+1, col+1);
//            TText *text = new TText((x1 + x2) / 2, (y1 + y2) / 2, cellText);
//            text->SetTextAlign(22); // Center alignment
//	    text->SetTextSize(textSize);
//            text->Draw();
//        }
//    }
//
//    // Update the canvas to show the table
//    c->Update();
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    
//    c->cd(i+1+3);
//    gPad->SetLogy(true);
//    vHist[i].second->Draw(); 
//    TLatex* text1_2 = new TLatex(-3e8,800,"foo");
//    text1_2->SetTextSize(textSize);
//    text1_2->Draw("SAME");
//    TLatex* text2_2 = new TLatex(-3e8,200,"bar");
//    text2_2->SetTextSize(textSize);
//    text2_2->Draw("SAME");
//    TLatex* text3_2 = new TLatex(-3e8,50,"#align{bar ; bar ; foobar}");
//    text3_2->SetTextSize(textSize);
//    text3_2->Draw("SAME");
  }

   
  // Update the canvas
  c->Update();

  c->SaveAs(outname.c_str() );
  
  for(int i=0; i<3; i++){
    std::cout << vHist[i].first->Integral(0,101) << "\n";
    std::cout << vHist[i].second->Integral(0,101) << "\n";
  }
}


void PlotZoom(std::vector<std::pair<TH1F*, TH1F*>> vHist, std::string outname)
{
  TCanvas* c = new TCanvas("", "", 1300,600);
  //  c->SetLogy(true);
  
  c->Range(0.,0., 1.,1.);

  TLine	*line1 = new TLine(1./3., 0, 1./3., 1);
  TLine	*line2 = new TLine(2./3., 0, 2./3., 1);
  line1->SetLineColor(kBlack);
  line1->SetLineWidth(2);
  line1->Draw();
  line2->Draw();

  // Divide the canvas into three vertical segments
  c->Divide(3, 2);  // 3 columns, 2 rows


  // Plot in each segment
  for(int i=0; i<nLAPPD; i++){
    c->cd(i+1);
    gPad->SetLogy(true);
    vHist[i].first->Draw("HIST"); 
    gPad->Update();
    TPaveStats *st1 = (TPaveStats*)vHist[i].first->FindObject("stats");
    st1->SetX1NDC(0.55); //new x start position
    st1->SetX2NDC(0.9); //new x end position
    st1->SetY1NDC(0.7); 
    st1->SetY2NDC(0.9); 

    c->cd(i+1+3);
    gPad->SetLogy(true);
    vHist[i].second->Draw("HIST"); 
    gPad->Update();
    TPaveStats *st2 = (TPaveStats*)vHist[i].second->FindObject("stats");
    st2->SetX1NDC(0.55); //new x start position
    st2->SetX2NDC(0.9); //new x end position
    st2->SetY1NDC(0.7); 
    st2->SetY2NDC(0.9); 

  }
   
  // Update the canvas
  c->Update();
  c->SaveAs(outname.c_str() );
}




void plot_missing()
{
  //  gStyle->SetOptStat(111110);
  gStyle->SetOptStat(1110);
  //TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/4765/LAPPDTree.root");
  TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/5007/LAPPDTree.root");
  //TFile *inFile = new TFile("~/mount/from_yue/ToolAnalysis/processed/5008/LAPPDTree.root");

  TTree *myTree;
  myTree = (TTree*)inFile->Get("TimeStamp");


  //--------------------------------------

    std::string xLabel = "#Delta t_{pps} (clock ticks)";
    std::string yLabel = "Events (normalised)";
    Long64_t xLo = -6e8;
    Long64_t xHi = 7e8;
    //Long64_t xLo = -6e7;
    //Long64_t xHi = 5e7;
    //  double yLo = 5e-1;
    //  double yHi = 1e6;
    double yLo = 1e-6;
    double yHi = 10;


    std::string xLabel_zoom = "#Delta t_{pps} - 3.2e8 (clock ticks)";
    Long64_t xLo_zoom = -15;
    Long64_t xHi_zoom = 15;
    int nBins_zoom = xHi_zoom - xLo_zoom;
    //  double yLo_zoom = 5e-1;
    //  double yHi_zoom = 1e6;
    double yLo_zoom = 1e-7;
    double yHi_zoom = 1;
  
//    Long64_t xLo_zoom2 = -50;
//    Long64_t xHi_zoom2 = 50;
//    Long64_t xLo_zoom2 = -1e3;
//    Long64_t xHi_zoom2 = 1e3;  
    Long64_t xLo_zoom2 = -1e8;
    Long64_t xHi_zoom2 = 1e8;  

    int nBins_zoom2 = 100;
  
  std::vector<std::pair<TH1F*,TH1F*>> hDiff_vec; // pair of hists {acdc0, acdc1} for each LAPPD ID
  std::vector<std::pair<TH1F*,TH1F*>> hDiff_vec_zoom; // same thing, but we subtract 3.2e8 (= 1s) from entries
  std::vector<std::pair<TH1F*,TH1F*>> hDiff_vec_zoom2; // zoom in on x=0 
  for(int i=0; i<nLAPPD; i++){
    TH1F *hACDC0  = new TH1F("", "", nBins, xLo, xHi);
    hACDC0->GetXaxis()->CenterTitle(true);
    hACDC0->GetYaxis()->CenterTitle(true);
    hACDC0->GetXaxis()->SetTitle(xLabel.c_str());
    hACDC0->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC0->GetYaxis()->SetRangeUser(yLo, yHi);    
    hACDC0->SetTitle("ACDC 0");
    TH1F* hACDC1 = (TH1F*) hACDC0->Clone();
    hACDC1->SetTitle("ACDC 1");    
    hDiff_vec.push_back({hACDC0, hACDC1});

    TH1F *hACDC0_zoom  = new TH1F("", "", nBins_zoom, xLo_zoom, xHi_zoom);
    hACDC0_zoom->GetXaxis()->CenterTitle(true);
    hACDC0_zoom->GetYaxis()->CenterTitle(true);
    hACDC0_zoom->GetXaxis()->SetTitle(xLabel_zoom.c_str());
    hACDC0_zoom->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC0_zoom->GetYaxis()->SetRangeUser(yLo_zoom, yHi_zoom);    
    hACDC0_zoom->SetTitle("ACDC 0");
    TH1F* hACDC1_zoom = (TH1F*) hACDC0_zoom->Clone();
    hACDC1_zoom->SetTitle("ACDC 1");    
    hDiff_vec_zoom.push_back({hACDC0_zoom, hACDC1_zoom});


    TH1F *hACDC0_zoom2  = new TH1F("", "", nBins_zoom2, xLo_zoom2, xHi_zoom2);
    hACDC0_zoom2->GetXaxis()->CenterTitle(true);
    hACDC0_zoom2->GetYaxis()->CenterTitle(true);
    hACDC0_zoom2->GetXaxis()->SetTitle(xLabel.c_str());
    hACDC0_zoom2->GetYaxis()->SetTitle(yLabel.c_str());
    hACDC0_zoom2->GetYaxis()->SetRangeUser(yLo_zoom, yHi_zoom);    
    hACDC0_zoom2->SetTitle("ACDC 0");
    TH1F* hACDC1_zoom2 = (TH1F*) hACDC0_zoom2->Clone();
    hACDC1_zoom2->SetTitle("ACDC 1");    
    hDiff_vec_zoom2.push_back({hACDC0_zoom2, hACDC1_zoom2});

  }
  std::vector<std::pair<std::vector<int>,std::vector<int> > > vCounts; // count number of events where delta_t is 0, 3.2e8, or other
  for(int i=0; i<nLAPPD; i++){
    std::pair<std::vector<int>,std::vector<int> > pair = { {0,0,0} , {0,0,0} };
    vCounts.push_back(pair);
  }
  

  //---------------------------------------

  std::vector<std::string> vVar = {"ppsTime0", "ppsTime1"};
  for(std::string var : vVar){
    ULong64_t ppsTime;
    int LAPPD_ID;
    
    
    myTree->SetBranchAddress(var.c_str(),&ppsTime);
    myTree->SetBranchAddress("LAPPD_ID",&LAPPD_ID);

    Long64_t pps_prev = 0; // pps timestamp from the previous event
    Long64_t pps_current = 0; // pps timestamp from the current event 

    Long64_t diff = 0;

    //  for(int i=0; i<entries; i++){

    //~~~~~~~~~~~~~~~~~~~~~~~~~
    myTree->BuildIndex(var.c_str());
    TTreeIndex* index = (TTreeIndex*) myTree->GetTreeIndex();
    //~~~~~~~~~~~~~~~~~~~~~~~~~
    for(int i=0; i < index->GetN(); i++){
      diff = 0;
      pps_prev = pps_current; // copy over pps_current from previous event
      //    myTree->GetEntry(i);
      Long64_t local = myTree->LoadTree( index->GetIndex()[i] );
      myTree->GetEntry(local);
      if(ppsTime == 0) std::cout << "FOO A \n";

      // filter out "duplicates"
      if( (ppsTime - pps_prev) < 2e7){
	//	pps_current = ppsTime;
	continue;
      }
      
      pps_current = ppsTime;
      diff = pps_current - pps_prev;

      
      if(var == "ppsTime0"){
	hDiff_vec[LAPPD_ID].first->Fill(diff);
	hDiff_vec_zoom[LAPPD_ID].first->Fill(diff - 3.2e8);
	hDiff_vec_zoom2[LAPPD_ID].first->Fill(diff);
 	if(diff == 0) vCounts[LAPPD_ID].first.at(0)++;
	else if(diff == 3.2e8) vCounts[LAPPD_ID].first.at(1)++;
	else vCounts[LAPPD_ID].first.at(2)++;
      }
      else if(var == "ppsTime1"){
	hDiff_vec[LAPPD_ID].second->Fill(diff);
	hDiff_vec_zoom[LAPPD_ID].second->Fill(diff - 3.2e8);
	hDiff_vec_zoom2[LAPPD_ID].second->Fill(diff);
	if(diff == 0) vCounts[LAPPD_ID].second.at(0)++;
	else if(diff == 3.2e8) vCounts[LAPPD_ID].second.at(1)++;
	else vCounts[LAPPD_ID].second.at(2)++;
      }
      else{
	std::cout << "something wrong here \n";
	abort();
      }
      


      
      
      if(diff > xHi || diff < xLo){
      std::cout << pps_prev << " : " << pps_current << " : " << diff <<  "\n";
      }      
      
    }// end loop over events

  }//end for var
    

  for(int i=0; i<nLAPPD; i++){
    for(int j : vCounts[i].first) std::cout << j << ", ";
    std::cout << "\n";
    for(int j : vCounts[i].second) std::cout << j << ", ";
    std::cout << "\n";
  }

  for(int i=0; i<nLAPPD; i++){
    hDiff_vec.at(i).first->Scale(1./hDiff_vec.at(i).first->Integral(0,nBins+1));
    hDiff_vec.at(i).second->Scale(1./hDiff_vec.at(i).second->Integral(0, nBins+1));

    hDiff_vec_zoom.at(i).first->Scale(1./hDiff_vec_zoom.at(i).first->Integral(0, nBins_zoom+1));
    hDiff_vec_zoom.at(i).second->Scale(1./hDiff_vec_zoom.at(i).second->Integral(0, nBins_zoom+1));

    hDiff_vec_zoom2.at(i).first->Scale(1./hDiff_vec_zoom2.at(i).first->Integral(0, nBins_zoom2+1));
    hDiff_vec_zoom2.at(i).second->Scale(1./hDiff_vec_zoom2.at(i).second->Integral(0, nBins_zoom2+1));


    hDiff_vec.at(i).first->GetYaxis()->SetRangeUser(yLo, yHi);
    hDiff_vec.at(i).second->GetYaxis()->SetRangeUser(yLo, yHi);

    hDiff_vec_zoom.at(i).first->GetYaxis()->SetRangeUser(yLo, yHi);
    hDiff_vec_zoom.at(i).second->GetYaxis()->SetRangeUser(yLo, yHi);

    hDiff_vec_zoom2.at(i).first->GetYaxis()->SetRangeUser(yLo, yHi);
    hDiff_vec_zoom2.at(i).second->GetYaxis()->SetRangeUser(yLo, yHi);

  }
  
  
  PlotAll(hDiff_vec, "all.pdf", vCounts);
  std::cout << "------------------- \n";
  PlotZoom(hDiff_vec_zoom, "all_zoom.pdf");
  PlotZoom(hDiff_vec_zoom2, "all_zoom2.pdf");
}

