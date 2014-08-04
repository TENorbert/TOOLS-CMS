/*************************************************
  Plots 3 histograms from an already open file
  into the same plot.
                                                                                                                                                      
  Must be used with my rootlogon.C
                                                                                                                                                      
  Michael B. Anderson
  July 12, 2006
*************************************************/

void Plot3hists1D(TString hist1Name,TString hist2Name,TString hist3Name)
{
  Plot3hists1D( hist1Name , hist2Name , hist3Name, 1.0, 1.0, 1.0);
}


void Plot3hists1D(TString hist1Name,TString hist2Name,TString hist3Name, Double_t scaleFactor1, Double_t scaleFactor2,Double_t scaleFactor3)
{

  Int_t LogY = 0;  // Y-axis, No log = 0, Log = 1
  int lineWidth = 2; // Line width for 1D histograms.
  TString outputFolder("Images");
  TString outputName(outputFolder+"/"+hist1Name+"-"+hist2Name+"-"+hist3Name+".gif");



  gSystem->MakeDirectory(outputFolder);

  // This is the cool part where we find the two histograms
  // based only on their names.  This is stored as a TKey for
  // the moment.
  TKey *key1 = (TKey*)gDirectory->GetListOfKeys()->FindObject(hist1Name);
  TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(hist2Name);
  TKey *key3 = (TKey*)gDirectory->GetListOfKeys()->FindObject(hist3Name);

  if ((key1)&&(key2)&&(key3)) {

    // Now turn the TKeys into histograms
    TH1 *hist1 = (TH1*)key1->ReadObj();
    TH1 *hist2 = (TH1*)key2->ReadObj();
    TH1 *hist3 = (TH1*)key3->ReadObj();

    // Scale them
    hist1->Scale(scaleFactor1);
    hist2->Scale(scaleFactor2);
    hist2->Scale(scaleFactor3);

    // Set the color
    hist1->SetLineColor(kBlue);
    hist2->SetLineColor(kRed);
    hist3->SetLineColor(kGreen);

    // Set the line width
    hist1->SetLineWidth(lineWidth);
    hist2->SetLineWidth(lineWidth);
    hist3->SetLineWidth(lineWidth);

    // Get rid of Stats box
    hist1->SetStats(kFALSE);

    // ****
    // Find the max value
    // & rescale Y axis
    Double_t max1 = hist1->GetMaximum();
    Double_t max2 = hist2->GetMaximum();
    Double_t max3 = hist2->GetMaximum();

    if ( (max1 > max2) && (max1 > max3) ) {
      hist1->SetMaximum(1.05*max1);
    } else if ( (max2 > max1) && (max2 > max3) ) {
      hist1->SetMaximum(1.05*max2);
    } else {
      hist1->SetMaximum(1.05*max3);
    }
    // ****

    // Draw them!
    hist1->Draw();
    hist2->Draw("same");
    hist3->Draw("same");

/*
    // ***
    // Draw Info Label
    TPaveText *infoBox = new TPaveText(0.80, 0.80, 0.98, 0.98, "NDC");
    TText *t1 = infoBox->AddText(hist1Name);
    TText *t2 = infoBox->AddText(hist2Name);
    TText *t3 = infoBox->AddText(hist3Name);
    infoBox->SetFillColor(kWhite);
    t1->SetTextColor(kBlue);
    t2->SetTextColor(kRed);
    t3->SetTextColor(kGreen);
    infoBox->Draw();
    // ***
*/

    // ****************
    // Draw Info Label 
    TLegend *infoBox = new TLegend(0.74, 0.74, 0.98, 0.98, "");
    infoBox->AddEntry(hist1,hist1Name,"L");
    infoBox->AddEntry(hist2,hist2Name,"L");
    infoBox->AddEntry(hist3,hist3Name,"L");
    infoBox->Draw();  
    // ****************

    c1->SetLogy(LogY);       // Set the axis to a log or not-log scale
    c1->Print(outputName);   // Save the histogram to a file

  } else {
    cout << "ERROR: One of the histogram names is wrong.";
  }
}
