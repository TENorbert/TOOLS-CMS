/*************************************************
  Given a root file containing a TTree, this
  macro will place half the contents into 2 new
  files.

  Can be run from a bash prompt as well:
    root -b -l -q "splitTreeInFile.C(\"PhotonJetPt15_Summer09.root\",\"TreePhotonJet\")"

  Or used within root:
    $  root -l
    root [0] .L splitTreeInFile.C
    root [1] splitTreeInFile("PhotonJetPt15_Summer09.root","TreePhotonJet")
    Opened PhotonJetPt15_Summer09.root...
    Copying TreePhotonJet to two new files...
    Created
      PhotonJetPt15_Summer09_1stHalf.root
      PhotonJetPt15_Summer09_2ndHalf.root
    done.
    root [2]    

  Michael B. Anderson
  Oct 8, 2009
*************************************************/


void splitTreeInFile(TString origFilename, TString treeName) {

  // Create names for the two new files
  TString filenameString = origFilename;
  filenameString.ReplaceAll(".root","");
  TString newFilename1 = filenameString + "_1stHalf.root";
  TString newFilename2 = filenameString + "_2ndHalf.root";
  
  ///////////////////////////////////////////////////////////
  //Get old file, old tree and set top branch address
  TFile *oldFile = new TFile(origFilename);
  if (oldFile->IsZombie()) {
    cout << "ERROR: " << origFilename << " not found" << endl;
    return;
  }
  cout << "Opened " << origFilename << "..." << endl;
  TTree *oldTree = (TTree*)oldFile->Get(treeName);
  if (!oldTree) {
    cout << "ERROR: " << treeName << " not found in " << origFilename << endl;
    return;
  }
  Int_t numOfEntries = (Int_t)oldTree->GetEntries();
  ///////////////////////////////////////////////////////////


  //Create a new file + a clone of old tree in new file
  TFile *newFile1 = new TFile(newFilename1,"recreate");
  TTree *newTree1 = oldTree->CloneTree(0);
  TFile *newFile2 = new TFile(newFilename2,"recreate");
  TTree *newTree2 = oldTree->CloneTree(0);

  // Copy tree into new trees
  cout << "Copying " << treeName << " to two new files..." << endl;
  for (Int_t i=0;i<numOfEntries; i++) {
    oldTree->GetEntry(i);
    if (i<numOfEntries/2) {
      newTree1->Fill();
    } else {
      newTree2->Fill();
    }
  }

  newTree1->AutoSave();
  newTree2->AutoSave();
  newFile1->Close();
  newFile2->Close();
  cout << "Created\n  " << newFilename1 << "\n  " << newFilename2 << endl << "done." << endl;
}
