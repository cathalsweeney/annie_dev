//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 14 16:18:14 2024 by ROOT version 6.30/04
// from TTree ffmytree/ffmytree
// found on file: ./Analysis.root
//////////////////////////////////////////////////////////

#ifndef ffmytree_h
#define ffmytree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class ffmytree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           T0Bin;
   Int_t           WraparoundBin;
   Int_t           QualityVar;
   Double_t        TrigDeltaT1;
   Double_t        TrigDeltaT2;
   Double_t        PulseHeight;
   Double_t        MaxAmp0;
   Double_t        MaxAmp1;
   Double_t        BeamTime;
   Double_t        EventTime;
   Double_t        TotalCharge;
   Int_t           NHits;
   Double_t        Q[1];   //[NHits]
   Double_t        xpar[1];   //[NHits]
   Double_t        xperp[1];   //[NHits]
   Double_t        time[1];   //[NHits]
   Double_t        deltime[1];   //[NHits]
   Double_t        Vpeak[1];   //[NHits]
   Int_t           Nchannels;
   Double_t        StripPeak[60];   //[Nchannels]
   Double_t        StripPeak_Sm[60];   //[Nchannels]
   Double_t        StripPeakT[60];   //[Nchannels]
   Double_t        StripPeakT_Sm[60];   //[Nchannels]
   Double_t        StripQ[60];   //[Nchannels]
   Double_t        StripQ_Sm[60];   //[Nchannels]
   Double_t        StripMaxAmp[60];   //[Nchannels]
   Double_t        StripMinAmp[60];   //[Nchannels]
   Double_t        StripRMSAmp[60];   //[Nchannels]
   Double_t        StripStandDevAmp[60];   //[Nchannels]
   Double_t        StripVarianceAmp[60];   //[Nchannels]
   Double_t        StripInt[60];   //[Nchannels]
   Int_t           NHits_simp;
   Double_t        Q_simp[1];   //[NHits_simp]
   Double_t        xpar_simp[1];   //[NHits_simp]
   Double_t        xperp_simp[1];   //[NHits_simp]
   Double_t        time_simp[1];   //[NHits_simp]
   Double_t        nnlsParallelP;
   Double_t        nnlsTransverseP;
   Double_t        nnlsArrivalTime;
   Double_t        nnlsAmplitude;
   Double_t        SelectedAmp0;
   Double_t        SelectedAmp1;
   Double_t        SelectedTime0;
   Double_t        SelectedTime1;
   Double_t        GMaxOn0;
   Double_t        GMaxOn1;
   Int_t           Npulses_simp;
   Double_t        pulsestart_simp[12];   //[Npulses_simp]
   Double_t        pulseend_simp[12];   //[Npulses_simp]
   Double_t        pulsepeakbin_simp[12];   //[Npulses_simp]
   Double_t        pulseamp_simp[12];   //[Npulses_simp]
   Double_t        pulseQ_simp[12];   //[Npulses_simp]
   Int_t           pulsestrip_simp[12];   //[Npulses_simp]
   Int_t           pulseside_simp[12];   //[Npulses_simp]
   Int_t           Npulses_cfd;
   Double_t        pulsestart_cfd[12];   //[Npulses_cfd]
   Int_t           pulsestrip_cfd[12];   //[Npulses_cfd]
   Int_t           pulseside_cfd[12];   //[Npulses_cfd]
   Double_t        pulseamp_cfd[12];   //[Npulses_cfd]
   Double_t        pulseQ_cfd[12];   //[Npulses_cfd]

   // List of branches
   TBranch        *b_T0Bin;   //!
   TBranch        *b_WraparoundBin;   //!
   TBranch        *b_QualityVar;   //!
   TBranch        *b_TrigDeltaT1;   //!
   TBranch        *b_TrigDeltaT2;   //!
   TBranch        *b_PulseHeight;   //!
   TBranch        *b_MaxAmp0;   //!
   TBranch        *b_MaxAmp1;   //!
   TBranch        *b_BeamTime;   //!
   TBranch        *b_EventTime;   //!
   TBranch        *b_TotalCharge;   //!
   TBranch        *b_NHits;   //!
   TBranch        *b_Q;   //!
   TBranch        *b_xpar;   //!
   TBranch        *b_xperp;   //!
   TBranch        *b_time;   //!
   TBranch        *b_deltime;   //!
   TBranch        *b_Vpeak;   //!
   TBranch        *b_Nchannels;   //!
   TBranch        *b_StripPeak;   //!
   TBranch        *b_StripPeak_Sm;   //!
   TBranch        *b_StripPeakT;   //!
   TBranch        *b_StripPeakT_Sm;   //!
   TBranch        *b_StripQ;   //!
   TBranch        *b_StripQ_Sm;   //!
   TBranch        *b_StripMaxAmp;   //!
   TBranch        *b_StripMinAmp;   //!
   TBranch        *b_StripRMSAmp;   //!
   TBranch        *b_StripStandDevAmp;   //!
   TBranch        *b_StripVarianceAmp;   //!
   TBranch        *b_StripInt;   //!
   TBranch        *b_NHits_simp;   //!
   TBranch        *b_Q_simp;   //!
   TBranch        *b_xpar_simp;   //!
   TBranch        *b_xperp_simp;   //!
   TBranch        *b_time_simp;   //!
   TBranch        *b_nnlsParallelP;   //!
   TBranch        *b_nnlsTransverseP;   //!
   TBranch        *b_nnlsArrivalTime;   //!
   TBranch        *b_nnlsAmplitude;   //!
   TBranch        *b_SelectedAmp0;   //!
   TBranch        *b_SelectedAmp1;   //!
   TBranch        *b_SelectedTime0;   //!
   TBranch        *b_SelectedTime1;   //!
   TBranch        *b_GMaxOn0;   //!
   TBranch        *b_GMaxOn1;   //!
   TBranch        *b_Npulses_simp;   //!
   TBranch        *b_pulsestart_simp;   //!
   TBranch        *b_pulseend_simp;   //!
   TBranch        *b_pulsepeakbin_simp;   //!
   TBranch        *b_pulseamp_simp;   //!
   TBranch        *b_pulseQ_simp;   //!
   TBranch        *b_pulsestrip_simp;   //!
   TBranch        *b_pulseside_simp;   //!
   TBranch        *b_Npulses_cfd;   //!
   TBranch        *b_pulsestart_cfd;   //!
   TBranch        *b_pulsestrip_cfd;   //!
   TBranch        *b_pulseside_cfd;   //!
   TBranch        *b_pulseamp_cfd;   //!
   TBranch        *b_pulseQ_cfd;   //!

   ffmytree(TTree *tree=0);
   virtual ~ffmytree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ffmytree_cxx
ffmytree::ffmytree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     std::cout << "BAR A \n";
     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("./Analysis.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("./Analysis.root");
      }
      f->GetObject("ffmytree",tree);

   }
   std::cout << "BAR B \n";
   Init(tree);
   std::cout << "BAR C \n";
}

ffmytree::~ffmytree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ffmytree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ffmytree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ffmytree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("T0Bin", &T0Bin, &b_T0Bin);
   fChain->SetBranchAddress("WraparoundBin", &WraparoundBin, &b_WraparoundBin);
   fChain->SetBranchAddress("QualityVar", &QualityVar, &b_QualityVar);
   fChain->SetBranchAddress("TrigDeltaT1", &TrigDeltaT1, &b_TrigDeltaT1);
   fChain->SetBranchAddress("TrigDeltaT2", &TrigDeltaT2, &b_TrigDeltaT2);
   fChain->SetBranchAddress("PulseHeight", &PulseHeight, &b_PulseHeight);
   fChain->SetBranchAddress("MaxAmp0", &MaxAmp0, &b_MaxAmp0);
   fChain->SetBranchAddress("MaxAmp1", &MaxAmp1, &b_MaxAmp1);
   fChain->SetBranchAddress("BeamTime", &BeamTime, &b_BeamTime);
   fChain->SetBranchAddress("EventTime", &EventTime, &b_EventTime);
   fChain->SetBranchAddress("TotalCharge", &TotalCharge, &b_TotalCharge);
   fChain->SetBranchAddress("NHits", &NHits, &b_NHits);
   fChain->SetBranchAddress("Q", Q, &b_Q);
   fChain->SetBranchAddress("xpar", xpar, &b_xpar);
   fChain->SetBranchAddress("xperp", xperp, &b_xperp);
   fChain->SetBranchAddress("time", time, &b_time);
   fChain->SetBranchAddress("deltime", deltime, &b_deltime);
   fChain->SetBranchAddress("Vpeak", Vpeak, &b_Vpeak);
   fChain->SetBranchAddress("Nchannels", &Nchannels, &b_Nchannels);
   fChain->SetBranchAddress("StripPeak", StripPeak, &b_StripPeak);
   fChain->SetBranchAddress("StripPeak_Sm", StripPeak_Sm, &b_StripPeak_Sm);
   fChain->SetBranchAddress("StripPeakT", StripPeakT, &b_StripPeakT);
   fChain->SetBranchAddress("StripPeakT_Sm", StripPeakT_Sm, &b_StripPeakT_Sm);
   fChain->SetBranchAddress("StripQ", StripQ, &b_StripQ);
   fChain->SetBranchAddress("StripQ_Sm", StripQ_Sm, &b_StripQ_Sm);
   fChain->SetBranchAddress("StripMaxAmp", StripMaxAmp, &b_StripMaxAmp);
   fChain->SetBranchAddress("StripMinAmp", StripMinAmp, &b_StripMinAmp);
   fChain->SetBranchAddress("StripRMSAmp", StripRMSAmp, &b_StripRMSAmp);
   fChain->SetBranchAddress("StripStandDevAmp", StripStandDevAmp, &b_StripStandDevAmp);
   fChain->SetBranchAddress("StripVarianceAmp", StripVarianceAmp, &b_StripVarianceAmp);
   fChain->SetBranchAddress("StripInt", StripInt, &b_StripInt);
   fChain->SetBranchAddress("NHits_simp", &NHits_simp, &b_NHits_simp);
   fChain->SetBranchAddress("Q_simp", Q_simp, &b_Q_simp);
   fChain->SetBranchAddress("xpar_simp", xpar_simp, &b_xpar_simp);
   fChain->SetBranchAddress("xperp_simp", xperp_simp, &b_xperp_simp);
   fChain->SetBranchAddress("time_simp", time_simp, &b_time_simp);
   fChain->SetBranchAddress("nnlsParallelP", &nnlsParallelP, &b_nnlsParallelP);
   fChain->SetBranchAddress("nnlsTransverseP", &nnlsTransverseP, &b_nnlsTransverseP);
   fChain->SetBranchAddress("nnlsArrivalTime", &nnlsArrivalTime, &b_nnlsArrivalTime);
   fChain->SetBranchAddress("nnlsAmplitude", &nnlsAmplitude, &b_nnlsAmplitude);
   fChain->SetBranchAddress("SelectedAmp0", &SelectedAmp0, &b_SelectedAmp0);
   fChain->SetBranchAddress("SelectedAmp1", &SelectedAmp1, &b_SelectedAmp1);
   fChain->SetBranchAddress("SelectedTime0", &SelectedTime0, &b_SelectedTime0);
   fChain->SetBranchAddress("SelectedTime1", &SelectedTime1, &b_SelectedTime1);
   fChain->SetBranchAddress("GMaxOn0", &GMaxOn0, &b_GMaxOn0);
   fChain->SetBranchAddress("GMaxOn1", &GMaxOn1, &b_GMaxOn1);
   fChain->SetBranchAddress("Npulses_simp", &Npulses_simp, &b_Npulses_simp);
   fChain->SetBranchAddress("pulsestart_simp", pulsestart_simp, &b_pulsestart_simp);
   fChain->SetBranchAddress("pulseend_simp", pulseend_simp, &b_pulseend_simp);
   fChain->SetBranchAddress("pulsepeakbin_simp", pulsepeakbin_simp, &b_pulsepeakbin_simp);
   fChain->SetBranchAddress("pulseamp_simp", pulseamp_simp, &b_pulseamp_simp);
   fChain->SetBranchAddress("pulseQ_simp", pulseQ_simp, &b_pulseQ_simp);
   fChain->SetBranchAddress("pulsestrip_simp", pulsestrip_simp, &b_pulsestrip_simp);
   fChain->SetBranchAddress("pulseside_simp", pulseside_simp, &b_pulseside_simp);
   fChain->SetBranchAddress("Npulses_cfd", &Npulses_cfd, &b_Npulses_cfd);
   fChain->SetBranchAddress("pulsestart_cfd", pulsestart_cfd, &b_pulsestart_cfd);
   fChain->SetBranchAddress("pulsestrip_cfd", pulsestrip_cfd, &b_pulsestrip_cfd);
   fChain->SetBranchAddress("pulseside_cfd", pulseside_cfd, &b_pulseside_cfd);
   fChain->SetBranchAddress("pulseamp_cfd", pulseamp_cfd, &b_pulseamp_cfd);
   fChain->SetBranchAddress("pulseQ_cfd", pulseQ_cfd, &b_pulseQ_cfd);
   Notify();
}

Bool_t ffmytree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ffmytree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ffmytree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ffmytree_cxx
