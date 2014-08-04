/*************************************************
  This will add a branch to the specified TTree
  in the specified files.  Useful for adding
  event weights, cross-sections, etc...

  Michael B. Anderson
  Feb 20, 2009
*************************************************/

#include <vector>
#include <cmath>

void addBranchToTreesInFiles() {
  //************************************************************
  //                      Variables                           //
  vector<TString> fileName;
  fileName.push_back( "PhotonJetPt15_realPhotons.root" );
  fileName.push_back( "PhotonJetPt30_realPhotons.root" );

  // The following 4 variables set the scale
  float invLuminosityToScaleTo = 200; // in pb-1

  vector<float> crossSection;
  crossSection.push_back( 2.9E5 );
  crossSection.push_back( 3.2E4 );

  vector<float> filterEffeciency;
  filterEffeciency.push_back( 1.0 );
  filterEffeciency.push_back( 1.0 );

  vector<float> eventsAnalyzied;
  eventsAnalyzied.push_back( 950E3 );
  eventsAnalyzied.push_back( 670E3 );
  // END of setting scale

  TString treeName = "TreePhotonMatched";

  //                  END of Variables                        //
  //************************************************************


  // Loop over all the Files
  for (int i=0; i < fileName.size(); i++) {
    TFile* currentFile = new TFile(fileName[i],"update");

    // this is the variable to be added
    Float_t scale=crossSection[i]*invLuminosityToScaleTo*filterEffeciency[i]/eventsAnalyzied[i];

    cout << "Opened " << fileName[i] << ", adding new branch with value=" << scale;

    TTree *tree = (TTree*)currentFile->Get(treeName);
    TBranch *newBranch = tree->Branch("weight", &scale,"weight/F");

    // Loop over all the entries, and add the new branch
    Int_t numEntries = (Int_t)tree->GetEntries();
    for (Int_t j=0; j<numEntries; j++) {
      newBranch->Fill();
    }
    tree->Write("",TObject::kOverwrite); // save new version only
    currentFile->Close();
    cout << "...closed file." << endl;
  }
}
