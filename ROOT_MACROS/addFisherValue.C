/*************************************************
  This provides an example of how to add a branch
  to the specified TTree the specified files.  

  The value placed in the new branch is a function
  of values currently in the TTree.

  Michael B. Anderson
  Oct 8, 2009
*************************************************/

#include <vector>
#include <cmath>

void addFisherValue() {
  //************************************************************
  //                      Variables                           //
  TString fileName[] = {"QCD_Pt15_Summer09.root",
			"QCD_Pt30_Summer09.root",
			"QCD_Pt80_Summer09.root" };
  int numberOfFiles = 3;

  TString treeName = "TreePhotonJet";

  // Variables in the TTree used to determine new value
  TString variableNames[] = {"photon_ecalRecHitSumEtConeDR03",
			     "photon_hcalTowerSumEtConeDR03",
			     "photon_trkSumPtHollowConeDR03",
			     "photon_hadronicOverEm",
			     "photon_r2x5"};
  Float_t variableValues[] = {0.0, 0.0, 0.0, 0.0, 0.0};  // Dummy values, just initializing array
  Int_t numberOfVariables  = 5;

  Float_t coeff[] = {-1.0799E0, -1.1344E-1, -5.0319E-2, -1.0400E-1, -2.6775E0, 1.5899E0};
  //                  END of Variables                        //
  //************************************************************


  // Loop over all the Files
  for (int i=0; i < numberOfFiles; i++) {
    TFile* currentFile = new TFile(fileName[i],"update");

    cout << "Opened " << fileName[i] << ", adding new branch" << endl;

    TTree *tree = (TTree*)currentFile->Get(treeName);

    // Loop over all the entries, and add the new branch
    for (int j=0;j<numberOfVariables;j++) {
      tree->SetBranchAddress(variableNames[j], &variableValues[j]);
    }
    float fisherValue;
    TBranch *newBranch = tree->Branch("fisherValue", &fisherValue,"fisherValue/F");

    Int_t numEntries = (Int_t)tree->GetEntries();
    for (Int_t j=0; j<numEntries; j++) {
      // Push values from TTree into "var" vector
      tree->GetEntry(j);
      // Calculate fisher value
      fisherValue=coeff[0]+coeff[1]*variableValues[0]+coeff[2]*variableValues[1]+coeff[3]*variableValues[2]+coeff[4]*variableValues[3]+coeff[5]*variableValues[4];
      newBranch->Fill();
    }
    tree->Write("",TObject::kOverwrite); // save new version only
    currentFile->Close();
    cout << "    ...closed file." << endl;
  }
}
