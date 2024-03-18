#include <iostream>
#include <TFile.h>
#include <TTree.h>

void FindMaxIndex(const char* filename, const char* treeName, const char* branchName) {
  // Open the ROOT file
  TFile* file = TFile::Open(filename);
  if (!file || file->IsZombie()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return;
  }

  // Get the tree from the file
  TTree* tree = dynamic_cast<TTree*>(file->Get(treeName));
  if (!tree) {
    std::cerr << "Error: Could not find tree " << treeName << " in file " << filename << std::endl;
    file->Close();
    return;
  }

    
  // Declare variables to store branch data
  int arraySize = 58;
  double* array = nullptr;

  // Connect branch to variables
  tree->SetBranchAddress(branchName, &array);
  //    tree->SetBranchAddress(Form("%s_size", branchName), &arraySize);

  tree->Print();
  
	
  // Loop over events
  Long64_t nEntries = 5; //tree->GetEntries();
  for (Long64_t i = 1; i < nEntries; ++i) {
    std::cout << "FOO A \n";
      
    tree->GetEntry(i);

    std::cout << "FOO B \n";
    // Find index of maximum element in the array
    int maxIndex = 0;
    double maxValue = array[0];
    std::cout << "FOO C \n";
    for (int j = 1; j < arraySize; ++j) {
      if (array[j] > maxValue) {
	maxValue = array[j];
	maxIndex = j;
      }
      std::cout << "FOO D \n";
    }

    // Print the index of the maximum element for each event
    std::cout << "Event " << i << ": Max index = " << maxIndex << std::endl;
  }

  std::cout << "FOO E \n";
  // Close the file
  file->Close();
}

void myAna() {
  const char* filename = "Analysis.root";
  const char* treeName = "ffmytree";
  const char* branchName = "StripPeak";

  FindMaxIndex(filename, treeName, branchName);
}
