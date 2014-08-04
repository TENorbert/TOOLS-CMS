/*************************************************
  Divides 2 histograms either from the same or
  different files and plots the output.

  Example commands typed 
  at ROOT prompt:
    Divide2hists1D("JetEt", "Jet2Et", "aFile.root", 1.0, 100.0)
    Divide2hists1D("JetEt", "Jet2Et", "firstFile.root", "secondFile.root", 1.0, 100.0)

  with this format:
    Divide2hists1D("Hist1Name", "Hist2Name", "File1.root", "File2.root", Hist1Scale, Hist2Scale)


  Michael B. Anderson
  May 13, 2006
*************************************************/


void Divide2hists1D(TString hist1Name, 
		    TString hist2Name,
                    TString file1Name,
                    TString file2Name = "",
                    Double_t scale1 = 1.0,
                    Double_t scale2 = 1.0) {

  int lineWidth = 2;
  TString outputFolder("Images");
  Bool_t displayStatsBox = 0;  // 0 = false, 1 = true
  TString imageType = "gif";


  // Image output info
  TString outputType("."+imageType); // OR: png, eps, pdf, etc...
  TString outputName = ""; // set depending on whether file2Name is set - see later

  // Make the output folder (if it exists, nothing is done)
  gSystem->MakeDirectory(outputFolder);

  // Open the 1st file
  TFile *file1 = new TFile(file1Name);

  if ( !file1 ) {
    cout << file1Name << " does not exist." << endl;
    return;
  }

  // Grab a copy of its histogram
  TH1* hist1Copy = (TH1*)file1->Get(hist1Name);

  if ( !hist1Copy ) {
    cout << file1Name << " does not contain " << hist1Name << endl;
    return;
  }

  // Open the 2nd file - if needed
  TFile *file2;
  TH1* hist2Copy;
  if (file2Name == "") {

    hist2Copy = (TH1*)file1->Get(hist2Name);
    if ( !hist2Copy ) {
      cout << file1Name << " does not contain " << hist1Name << endl;
      return;
    }

    outputName = outputFolder+"/"+file1Name+"-"+hist1Name+"-div-"+hist2name+outputType;

  } else {

    file2 = new TFile(file2Name);

    if ( !file2 ) {
      cout << file2Name << " does not exist." << endl;
      return;
    }

    hist2Copy = (TH1*)file2->Get(hist2Name);

    if ( !hist2Copy ) {
      cout << file2Name << " does not contain " << hist2Name << endl;
      return;
    }

    outputName = outputFolder+"/"+hist1Name+"-"+file1Name+"-div-"+file2Name+outputType;

  } // end of if file2 was chosen

  TH1* finalHist = hist1Copy->Clone();
  finalHist->Divide(hist1Copy,hist2Copy,scale1,scale2);
  finalHist->SetLineWidth(lineWidth);
  finalHist->SetStats(displayStatsBox);
  finalHist->Draw();
  
  outputName.ReplaceAll(".root",""); // remove ".root" from the output image name
  c1->Print(outputName);   // Save the histogram to a file
}
