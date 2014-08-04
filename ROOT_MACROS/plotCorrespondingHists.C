/*************************************************
  Automatically plots histograms from two or more
  files onto the same plot and saves them.

  This first looks for ALL histograms in the first
  file then and plots the corresponding histograms 
  from the rest of the files onto the sample plot.

  Images are saved into a directory structure 
  that copies the structure of the root files.
  
  Can be run from a bash prompt as well:
    root -b -l -q "plotCorrespondingHists.C(\"fileA.root\",\"fileB.root\")"
    root -b -l -q "plotCorrespondingHists.C(\"fileA.root\",\"fileB.root\",\"fileC.root\")"

  Michael B. Anderson
  Sept 29, 2009
*************************************************/



// *******************************************
// Variables
TString imageType = "gif";
TString outputFolder = "HistogramsTogether/";
int plotWidth   = 480;
int plotHeight  = 360;
bool plotLogY = false;
bool areaNormalize = true;
// Turn off stats box                                                                                         
gStyle->SetOptStat(0);
// End of Variables
// *******************************************

void plotCorrespondingHists(TString fileName1, TString fileName2) {
  vector<TString> fileNames;
  fileNames.push_back( fileName1 ) ;
  fileNames.push_back( fileName2 ) ;
  plotCorrespondingHists( fileNames );
}

void plotCorrespondingHists(TString fileName1, TString fileName2, TString fileName3) {
  vector<TString> fileNames;
  fileNames.push_back( fileName1 ) ;
  fileNames.push_back( fileName2 ) ;
  fileNames.push_back( fileName3 ) ;
  plotCorrespondingHists( fileNames );
}

void plotCorrespondingHists(TString fileName1, TString fileName2, TString fileName3, TString fileName4) {
  vector<TString> fileNames;
  fileNames.push_back( fileName1 ) ;
  fileNames.push_back( fileName2 ) ;
  fileNames.push_back( fileName3 ) ;
  fileNames.push_back( fileName4 ) ;
  plotCorrespondingHists( fileNames );
}

void plotCorrespondingHists(vector<TString> fileName) {

  TString firstFileName = fileName[0];

  if (firstFileName == "" ) return;

  // Open all the files & create a list of Labels
  TList *fileList = new TList();
  TList *listOfLabels = new TList();
  for (int i=0; i<fileName.size(); i++) {
    cout << "Opening " << fileName[i] << " ..." << endl;
    TFile *tempFile = new TFile(fileName[i], "READ");
    if ( tempFile->IsZombie() ) {
      cout << "Error opening " << fileName[i] << endl;
      return;
    }
    fileList->Add( tempFile );
    listOfLabels->Add( new TObjString(fileName[i].ReplaceAll(".root","")) );
  }

  // Create a list of all the histograms in the file
  TList *listOfHistograms = new TList();
  TFile *firstFile = (TFile*)fileList->First();
  gSystem->MakeDirectory(outputFolder);
  recurseOverKeys( firstFile, listOfHistograms );

  cout << "Found " << listOfHistograms->GetSize() << " histograms in " << firstFileName << endl;

  // Loop over histograms, then loop over files
  //  plot the same histogram from all the files together
  //  save the canvas
  TCanvas *c1 = new TCanvas("c1", "c1",0,0,plotWidth,plotHeight);
  TObjString *currentHistString = (TObjString*)listOfHistograms->First();
  for (int i_hist=0; i_hist<listOfHistograms->GetSize(); i_hist++) {
    TFile *current_file = (TFile*)fileList->First();

    // Loop over the files, building list of histograms
    TList *listOfHistsToPlot = new TList();
    for (int i_file=0; i_file<fileName.size(); i_file++) {
      TH1* htemp;
      //cout << currentHistString->GetString() << endl;
      TString currentHistName = currentHistString->GetString();
      if (currentHistName[0] == '/') {
	currentHistName = currentHistName(1,currentHistName.Length());
      }
      current_file->GetObject(currentHistName, htemp);

      if ( !htemp ) {
	cout << "Error getting " << currentHistString->GetString() << " from " << current_file->GetName() << endl;
	return;
      }

      listOfHistsToPlot->Add(htemp);
      current_file = (TFile*)fileList->After( current_file );
    }

    // Created a list of histograms to plot, now plot and save file
    plotHists(listOfHistsToPlot, listOfLabels, c1);
    c1->SaveAs(outputFolder+currentHistString->GetString()+"."+imageType);

    currentHistString = (TObjString*)listOfHistograms->After(currentHistString);
  }

  // Close all the files
  TFile *current_file = (TFile*)fileList->First();
  for (int i=0; i<fileName.size(); i++) {
    cout << "Closing " << fileName[i] << " ..." << endl;
    current_file->Close();
    current_file = (TFile*)fileList->After( current_file );
  }
}

void recurseOverKeys( const TDirectory *target, TList *listOfHistograms) {

  // Figure out where we are
  TString path( (char*)strstr( target->GetPath(), ":" ) );
  path.Remove( 0, 2 );

  TDirectory *current_sourcedir = gDirectory;

  TKey *key;
  TIter nextkey(current_sourcedir->GetListOfKeys());

  while (key = (TKey*)nextkey()) {

    obj = key->ReadObj();

    // Check if this is a 1D histogram or a directory
    if (obj->IsA()->InheritsFrom("TH1F")) {

      TH1F *htemp = (TH1F*)obj;
      TString histName = htemp->GetName();
      TObjString *histToAdd = new TObjString(path+"/"+histName);
      listOfHistograms->Add(histToAdd);

    } else if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      // it's a subdirectory

      const TDirectory *tempDir = (TDirectory*)obj;
      current_sourcedir->cd ( tempDir->GetName() );

      gSystem->MakeDirectory(outputFolder+path+"/"+tempDir->GetName());

      // obj still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      recurseOverKeys( tempDir , listOfHistograms );

    } // end of IF a TDriectory
  }
} 

void plotHists(TList *histogramsToPlot, 
	       TList *histogramLabels,
	       TCanvas *c1) {


  //*************************************************
  // Variables
  vector<int> histColors;
  histColors.push_back(kBlack); // change colors as you like
  histColors.push_back(kBlue);
  histColors.push_back(kRed);
  histColors.push_back(kGreen-1);

  float histLineWidth = 2.0;

  bool useMarkers = false;         // search online for TAttMarker
  vector<int> histMarkerStyles;    // to see all available markers
  histMarkerStyles.push_back( 20 );
  histMarkerStyles.push_back( 22 );
  histMarkerStyles.push_back( 21 );
  histMarkerStyles.push_back( 23 );
  float histMarkerSize = 0.9;

  // END of Variables
  //*************************************************

  // Create copies of histograms provided
  //  and area normalize, if requested
  TList *hists = new TList();
  TH1* currentHistogram = (TH1*)histogramsToPlot->First();
  for (int i=0; i<histogramsToPlot->GetSize(); i++) {
    TH1* tempCurrentClone = (TH1*)currentHistogram->Clone();
    hists->Add( tempCurrentClone );
    if (areaNormalize) {
      Double_t integral = tempCurrentClone->Integral();
      if (integral>0.0) tempCurrentClone->Scale(1/integral);
    }
    currentHistogram = (TH1*)histogramsToPlot->After( currentHistogram );
  }


  // Set histogram plotting variables
  //  and add them to the histogram stack & legend
  THStack *tempStack = new THStack();
  TLegend *infoBox = new TLegend(0.75, 0.83, 0.99, 0.99, "");
  currentHistogram = (TH1*)hists->First();
  TObjString *currentHistString = (TObjString*)histogramLabels->First();
  for (int i=0; i<histogramsToPlot->GetSize(); i++) {

    currentHistogram->SetLineColor(histColors[i]);
    currentHistogram->SetLineWidth(histLineWidth);

    if (useMarkers) {
      currentHistogram->SetMarkerStyle(histMarkerStyles[i]);
      currentHistogram->SetMarkerColor(histColors[i]);
      currentHistogram->SetMarkerSize(histMarkerSize);
    }

    infoBox->AddEntry(currentHistogram,currentHistString->GetString(),"LP");
    currentHistString = (TObjString*)histogramLabels->After( currentHistString);

    tempStack->Add(currentHistogram);

    currentHistogram = (TH1*)hists->After( currentHistogram );
  }

  // Draw the stack of histograms
  tempStack->Draw("nostack");

  // Set title/label sizes and offsets
  tempStack->GetXaxis()->SetTitleOffset(0.9);
  tempStack->GetYaxis()->SetTitleOffset(1.2);
  tempStack->GetXaxis()->SetLabelSize(0.04);
  tempStack->GetYaxis()->SetLabelSize(0.05);
  tempStack->GetXaxis()->SetTitleSize(0.06);
  tempStack->GetYaxis()->SetTitleSize(0.05);

  // Set axis titles
  TString title = ((TH1*)hists->First())->GetTitle();
  TString xTitle = ((TH1*)hists->First())->GetXaxis()->GetTitle();
  TString yTitle = ((TH1*)hists->First())->GetYaxis()->GetTitle();
  tempStack->SetTitle(title+";"+xTitle+";"+yTitle);

  // Draw legend
  infoBox->SetShadowColor(0);  // 0 = transparent
  //infoBox->SetLineColor(0);
  infoBox->SetFillColor(kWhite);
  //infoBox->SetFillStyle(0);
  infoBox->Draw();

  c1->SetLogy(plotLogY);
}
