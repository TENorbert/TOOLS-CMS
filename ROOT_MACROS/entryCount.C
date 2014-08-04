/*************************************************
  Counts the number of entries in a histogram in
  a root file.

  Can be used in three ways, depending on what is
  given:

  1) A root file name:  
        List the entries in all the histograms in the file

  2) A root file name & a histogram name:
        Just list the entries in one histgram from one file

  3) A histogram name:
        Run over all root files in a directory and 
        find the number of entries in a given histogram.

  Michael B. Anderson
  April 15, 2008
*************************************************/

#include <TFile.h>
#include <TObjString.h>
#include <iomanip.h>

// Figure out if we were given
//  a root file name or a histogram name
//  and call the correct function.
void entryCount(TString argument) {

  if ( argument.Contains(".root") ) {
    countEntriesInFile(argument);
  } else {
    entryCountInAllFiles(argument);
  }
}


// List the entries in all the histograms in the file
void countEntriesInFile(TString fileName) {

  // Open the File
  TFile f(fileName);

  TH1 *h;
  TTree *t;

  TKey *key;
  TIter nextkey(gDirectory->GetListOfKeys());

  cout << setw(29) << "  Object " << " entries" << endl;
  cout << setw(29) << "---------" << " -------" << endl;

  while (key = (TKey*)nextkey()) {

    obj = key->ReadObj();

    if (obj->IsA()->InheritsFrom("TH1")) {

      h = (TH1*)obj;
      TString histName = h->GetName();
      Long64_t countOfEntries = h->GetEntries();
  
      cout << "TH1    : " << setw(20) << histName << " " << countOfEntries << endl; 
      
    } else if (obj->IsA()->InheritsFrom("TNtuple")) {
        
      t = (TTree*)obj;
      TString histName = t->GetName();
      Long64_t countOfEntries = t->GetEntries();
  
      cout << "TNtuple: " << setw(20) << histName << " " << countOfEntries << endl;
  
    }

  } // End of loop over all keys in file
  f.Close();
}


// Just list the entries in one histgram from one file
void entryCount(TString fileName, TString histName) {
  // Open the File
  TFile f(fileName); 

  TObject *obj = f.Get(histName);

  if ( !obj ) {
    cout << fileName << " does not contain " << histName << endl;
    return;
  }

  if (obj->IsA()->InheritsFrom("TH1")) {

    h = (TH1*)obj;
    TString histName = h->GetName();
    Long64_t countOfEntries = h->GetEntries();

    cout << "TH1    : " << setw(20) << histName << " " << countOfEntries << endl;

  } else if (obj->IsA()->InheritsFrom("TNtuple")) {

    t = (TTree*)obj;
    TString histName = t->GetName();
    Long64_t countOfEntries = t->GetEntries();

    cout << "TNtuple: " << setw(20) << histName << " " << countOfEntries << endl;

  }
}


// Run over all root files in a directory and
//  find the number of entries in a given histogram.
void entryCountInAllFiles(TString histName) {

  TString dirname = ".";     // . = this directory
  TString pattern = ".root"; // match only root files

  cout << "Files in " << dirname << " that match '" << pattern << "'" << endl;
  cout << "Entries in histograms that match '" << histName << "'" << endl;

  void *dir = gSystem->OpenDirectory(dirname);

  if (!dir) {
    cerr <<  "couldn't open directory" << dirname << endl;
    return 0;
  }

  TObjArray*  fileList = new TObjArray;
  const char* file;
  TString     fileName;
  TRegexp     regexp(pattern);

  Int_t maxFileNameLength = 10;

  while ( (file = gSystem->GetDirEntry(dir)) ) {

    fileName = file;

    if (!fileName.CompareTo(".") || !fileName.CompareTo("..")) continue;

    // Check if the filename matches the regexp
    if (fileName.Index(regexp) != kNPOS) { 

      // if it does, add to list
      fileList->Add(new TObjString(fileName)); 
      if ( fileName.Length() > maxFileNameLength ) {
	maxFileNameLength = fileName.Length();
      }

    }

  }  

  gSystem->FreeDirectory(dir);



  TIter       next(fileList);
  TObjString* fn;

  cout << setw(maxFileNameLength+1) << "Filename" << " entries" << endl;
  cout << setw(maxFileNameLength+1) << "--------" << " -------" << endl;
  
  while ( fn = (TObjString*)next() ) { 
  
    TString s = fn->GetString();
    TFile* f = TFile::Open( s ); 
 
    if ( !f )       continue; 

    TObject *obj = f->Get(histName);

    if ( !obj ) {
      cout << setw(maxFileNameLength+1) << s << " does not contain " << histName << endl;
      continue;
    }

    // ********************************************
    // ** Change what you want in here to do
    //     whatever to the files.
    if (obj->IsA()->InheritsFrom("TH1")) {

      h = (TH1*)obj;
      TString histName = h->GetName();
      Long64_t countOfEntries = h->GetEntries();

      cout << setw(maxFileNameLength+1) << s << " " << countOfEntries << endl;

    } else if (obj->IsA()->InheritsFrom("TNtuple")) {
      t = (TTree*)obj;
      TString histName = t->GetName();
      Long64_t countOfEntries = t->GetEntries();

      cout << setw(maxFileNameLength+1) << s << " " << countOfEntries << endl;
    }
    // ********************************************

    
    f->Close();
  }
}

