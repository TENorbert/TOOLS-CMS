/*************************************************
  This script copies only a subset of a TTree 
  to a new TTree.

  Give a list of files, and a list of cuts. 
  A new file will be created for every original
  file.

  Michael B. Anderson
  March 6, 2009
*************************************************/

void copyTreeSubset() {
  //********************************************************************
  //****                      Variables                           ****//
  // List of files to copy TTree from
  vector<TString> fileName;
  fileName.push_back( "QCD_Pt20to30-fakePhotons.root"  );//file0
  fileName.push_back( "QCD_Pt30to80-fakePhotons.root"  );//file1
  fileName.push_back( "QCD_Pt80to170-fakePhotons.root"  );//file2
  fileName.push_back( "QCD_pt170-fakePhotons.root" );//file3
  fileName.push_back( "QCD_pt300-fakePhotons.root" );//file4
  fileName.push_back( "QCD_pt470-fakePhotons.root" );//file5
  fileName.push_back( "QCD_pt800-fakePhotons.root" );//file6

  TString treeName = "TreePhotonAll";

  // Each cut is applied only to one file
  vector<TString> fileCuts;
  fileCuts.push_back( "genEventScale<30" );  //file0
  fileCuts.push_back( "genEventScale>30&&genEventScale<80" );//file1
  fileCuts.push_back( "genEventScale>80&&genEventScale<170" );//file2
  fileCuts.push_back( "genEventScale>170&&genEventScale<300" );//file3
  fileCuts.push_back( "genEventScale>300&&genEventScale<470" );//file4
  fileCuts.push_back( "genEventScale>470&&genEventScale<800" );//file5
  fileCuts.push_back( "genEventScale>800" ); //file6
  //****                  END of Variables                        ****//
  //********************************************************************



  //********************************************************************
  //****                Main part of Program                      ****//
  // Loop over all the Files
  for (int i=0; i < fileName.size(); i++) {
    TFile* currentFile = new TFile(fileName[i],"read");

    cout << "Opened " << fileName[i];

    TTree *currentTree = (TTree*)currentFile->Get(treeName);
    Int_t nentries = (Int_t)currentTree->GetEntries();

    cout << ", found " << treeName << " has " << nentries << " entries before cuts." << endl;

    // Create a new file
    TString newFileName = fileName[i];
    newFileName.ReplaceAll(".root","-TreeSubset.root");
    TFile *newFile = new TFile(newFileName,"recreate");

    // Copy the part of the tree that passes cuts
    TTree *newTree = currentTree->CopyTree(fileCuts[i]);
    cout << " saved entries passing" << endl << fileCuts[i] << endl << " to new file, " << newFileName << endl;

    //newTree->Print();
    newTree->AutoSave();
    //delete oldfile;
    //delete newfile;
    currentFile->Close();
  } // END of loop over files
}
