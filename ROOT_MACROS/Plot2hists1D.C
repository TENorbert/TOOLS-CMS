/*************************************************
  Plots 2 histograms from an already open file
  into the same plot.

  Must be used with my rootlogon.C


  Example commands typed 
  at ROOT prompt:
    Plot2hists1D("muon1pt", "muon2pt", "file1.root", "file2.root")
    Plot2hists1D("muon1pt", "muon2pt", "file1.root", "file2.root", "1st muon", "2nd muon")
    Plot2hists1D("muon1pt", "muon2pt", "file1.root", "file2.root", "1st muon", "2nd muon", 2.0, 3.0)
    Plot2hists1D("muon1pt", "muon2pt")
    Plot2hists1D("muon1pt", "muon2pt", 100.0, 1.0)

  with this format:
    PlotFromTTree("hist1name","hist2name","file with hist1","file with hist2", "label1", "label2", scaleFactor1, scaleFactor2)

  Michael B. Anderson
  July 12, 2006
*************************************************/

// If two histograms from two files are to be ploted onto
// the same plot.  Open the first, and make a copy of the
// histogram and place it in the 2nd file. Then
void Plot2hists1D(TString hist1Name, 
		TString hist2Name, 
		TString file1Name, 
		TString file2Name, 
                TString hist1Label = "",
                TString hist2Label = "",
		Double_t scaleFactor1 = 1.0, 
		Double_t scaleFactor2 = 1.0) {

  // Open the 1st file
  TFile *file1 = new TFile(file1Name);

  // Grab a copy of its histogram
  TH1* histTemp = (TH1*)file1->Get(hist1Name);

  // Open the 2nd file
  TFile *file2 = new TFile(file2Name,"UPDATE");
  file2->cd();

  // Change the name 
  histTemp->SetName(hist1Name+"1");

  // Write the histogram to the 2nd file
  histTemp->Write();

  // Now file2 is the currently active file
  Plot2hists1D(hist1Name+"1",hist2Name, scaleFactor1, scaleFactor2, hist1Label, hist2Label);

  // Close the files, but if they close, the histograms disappear
  //file1->Close();
  //file2->Close();
}


void Plot2hists1D(TH1* histogram1,
                  TH1* histogram2,
		  TString hist1Label = "",
                  TString hist2Label = "") {

  int lineWidth = 2; // Line width for 1D histograms.
  
  // Set the color
  histogram1->SetLineColor(kBlue);
  histogram2->SetLineColor(kRed);

  // Set the line width
  histogram1->SetLineWidth(lineWidth);
  histogram2->SetLineWidth(lineWidth);

  // Get rid of Stats box
  histogram1->SetStats(kFALSE);

  // ****
  // Find the max value
  // & rescale Y axis
  Double_t max1 = histogram1->GetMaximum();
  Double_t max2 = histogram2->GetMaximum();

  if (max1 > max2) {
    histogram1->SetMaximum(1.05*max1);
  } else {
    histogram1->SetMaximum(1.05*max2);
  }
  // ****

  // Draw them!
  histogram1->Draw();
  histogram2->Draw("same");

  // ****************
  // Draw Info Label
  TLegend *infoBox = new TLegend(0.74, 0.74, 0.98, 0.98, "");
  if ( (hist1Label=="") || (hist2Label=="") ) {
    infoBox->AddEntry(histogram1,hist1Name,"L");
    infoBox->AddEntry(histogram2,hist2Name,"L");
  } else {
    infoBox->AddEntry(histogram1,hist1Label,"L");
    infoBox->AddEntry(histogram2,hist2Label,"L");
  }
  infoBox->Draw();
  // ****************

}


void Plot2hists1D(TString hist1Name,
		TString hist2Name, 
		Double_t scaleFactor1=1.0, 
		Double_t scaleFactor2=1.0, 
		TString hist1Label = "",
		TString hist2Label = "")
{

  Int_t LogY = 0;  // Y-axis, No log = 0, Log = 1
  int lineWidth = 2; // Line width for 1D histograms.
  TString outputFolder("Images");
  TString outputName(outputFolder+"/"+hist1Name+"-"+hist2Name+".gif");



  gSystem->MakeDirectory(outputFolder);

  // This is the cool part where we find the two histograms
  // based only on their names.  This is stored as a TKey for
  // the moment.
  TKey *key1 = (TKey*)gDirectory->GetListOfKeys()->FindObject(hist1Name);
  TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(hist2Name);

  if ((key1)&&(key2)) {

    // Now turn the TKeys into histograms
    TH1 *hist1 = (TH1*)key1->ReadObj();
    TH1 *hist2 = (TH1*)key2->ReadObj();

    // Scale them
    hist1->Scale(scaleFactor1);
    hist2->Scale(scaleFactor2);

    // Set the color
    hist1->SetLineColor(kBlue);
    hist2->SetLineColor(kRed);

    // Set the line width
    hist1->SetLineWidth(lineWidth);
    hist2->SetLineWidth(lineWidth);

    // Get rid of Stats box
    hist1->SetStats(kFALSE);

    // ****
    // Find the max value
    // & rescale Y axis
    Double_t max1 = hist1->GetMaximum();
    Double_t max2 = hist2->GetMaximum();

    if (max1 > max2) {
      hist1->SetMaximum(1.05*max1);
    } else {
      hist1->SetMaximum(1.05*max2);
    }
    // ****

    // Draw them!
    hist1->Draw();
    hist2->Draw("same");

/*
    // ****************
    // Draw Info Label
    TPaveText *infoBox = new TPaveText(0.85, 0.85, 0.98, 0.98, "NDC");
    TText *t1;
    TText *t2;
    if ( (hist1Label=="") || (hist2Label=="") ) {
     *t1 = infoBox->AddText(hist1Name);
     *t2 = infoBox->AddText(hist2Name);
    } else {
     *t1 = infoBox->AddText(hist1Label);
     *t2 = infoBox->AddText(hist2Label);
    }
    infoBox->SetFillColor(kWhite);
    t1->SetTextColor(kBlue);
    t2->SetTextColor(kRed);
    infoBox->Draw();
    // ****************
*/

    // ****************
    // Draw Info Label
    TLegend *infoBox = new TLegend(0.74, 0.74, 0.98, 0.98, "");
    if ( (hist1Label=="") || (hist2Label=="") ) {
      infoBox->AddEntry(hist1,hist1Name,"L");
      infoBox->AddEntry(hist2,hist2Name,"L");
    } else {
      infoBox->AddEntry(hist1,hist1Label,"L");
      infoBox->AddEntry(hist2,hist2Label,"L");
    }
    infoBox->Draw();
    // ****************

    c1->SetLogy(LogY);       // Set the axis to a log or not-log scale
    c1->Print(outputName);   // Save the histogram to a file

  } else {
    cout << "ERROR: One of the histogram names is wrong." << endl;
  }
}
