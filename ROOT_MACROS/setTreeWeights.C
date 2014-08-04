/************************************************************
 
  This will SetWeight to TTree's in given files.  This
  Allows for them to be TChain'ed together with correct
  cross sections.

  Example use of this command:
   root [0] .x setTreeWeights.C("filelist.txt","TreePhotonAndJet")

  Format the list of file names with weights like so:

  QCDEnrichedpt20-30.root 175.0
  QCDEnrichedpt30-50.root 105.2
  QCDEnrichedpt50-80.root 80.1
  QCDEnrichedpt80-120.root 53.7
  QCDEnrichedpt120-170.root 25.4
  QCDEnrichedpt170-300.root 3.2


  After running the command you can TChain things together:
    TChain *whatever = new TChain("TreePhotonAndJet");
    whatever->Add("*.root")
    whatever->Draw("genJet1.et","genJet1.et>0&&genJet1.et<200")

  Michael B. Anderson
  June 10, 2008
************************************************************/

#include "vector.h"
#include "Riostream.h"

void setTreeWeights(TString filelistName, TString treeName) {

 ifstream fileToReadFrom;

 cout << "Reading filenames and weights from " << filelistName << endl;
 fileToReadFrom.open(filelistName);

 // Make sure the file is ok to read from
 if ( !fileToReadFrom.good() ) {
   cout << "ERROR reading from file " << filelistName << endl;
   return;
 }

 vector<TString> filename;
 vector<Float_t> weight;

 //************************************************************
 // Read from the text file to 
 // figure out file names and weights
 TString currentFilename;
 Float_t currentWeight;
 Int_t numberOfFiles = 0;

 while (1) {
   fileToReadFrom >> currentFilename >> currentWeight;
   if ( !fileToReadFrom.good() ) break;
   //cout << currentFilename << currentWeight << endl;

   filename.push_back(currentFilename);
   weight.push_back(currentWeight);

   numberOfFiles++;
 }
 cout << "  found " << numberOfFiles << " in " << filelistName << endl;
 //************************************************************


 // If no files were found, exit
 if ( numberOfFiles == 0 ) {
   cout << "ERROR: " << filelistName << " may be formatted incorrectly."  << endl;
   cout << "Each line must be in format: <filename> <weight>"<< endl;
   cout << "Such as" << endl;
   cout << "QCDEnrichedpt20-30.root 175.0" << endl;
   return;
 }


 //************************************************************
 // Set the weight to the given TTree
 cout << "Copying TTree " << treeName << " to new files..." << endl;
 for (int i = 0; i < numberOfFiles ; i ++) {

   // Open the file
   TFile* file1   = new TFile( filename[i], "update");

   // Make sure file was found
   if ( file1->IsZombie() ) {
     cout << "ERROR: " << filename[i] << " was not found" << endl;
     continue;
   }

   // Create the output filename and open that file
   TString newFilename = filename[i];
   newFilename = newFilename.ReplaceAll(".root","");
   newFilename = newFilename+"_setWeight.root";
   TFile* outputFile = new TFile(newFilename, "RECREATE");

   // Grab the original TTree
   TTree* treeAny = file1->Get(treeName);
   
   // Make sure the TTree was found
   if ( !treeAny ) {
     cout << "ERROR: " << filename[i] << " does not contain TTree named " << treeName << endl;
     continue;
   } else {
     cout << "  " << filename[i] << " -> " << newFilename << ", doing SetWeight( " << weight[i] << " )" << endl;
   }

   // Create a new TTree and copy information into it
   TTree *newTree = treeAny->CloneTree(0);
   newTree->CopyEntries(treeAny);
   // Set the weight, write file, and close the files
   newTree->SetWeight( weight[i] );
   outputFile->Write();
   outputFile->Close();
   file1->Close();
 }
 //************************************************************

 cout << endl << "To chain up the trees: " << endl;
 cout << "    TChain *theChain = new TChain(\"TreePhotonAndJet\"); " << endl;
 cout << "    theChain->Add(\"*_setWeight.root\") " << endl;

}
