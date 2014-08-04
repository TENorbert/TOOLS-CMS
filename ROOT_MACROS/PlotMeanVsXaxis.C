/*************************************************
Given a 2D plot, this will make vertical 
slices, fit each of those around the peak 
bin, and then plot the mean vs the x axis.

Michael Anderson
July 15, 2008
*************************************************/

#include <stdio.h>

TGraphErrors *gr1;

void PlotMeanVsXaxis(TH2* aHistogram,
                     TString title = "",
                     TString xAxisTitle = "",
                     Double_t ymin = 0.0,
                     Double_t ymax = 0.0) 
{

  TH2D *aHist = aHistogram->Clone(); 
  int numberOfXbins = aHist->GetNbinsX();

  TString outputFolder="Images/";

  // If the number of entries in
  // a column is less than this, a fit
  // will not be made for that column.
  Int_t minNumEntries = 10;
  //TString xAxisTitle = aHist->GetXaxis()->GetTitle();

  // Low edge if the first bin
  Float_t xMin = aHist->GetXaxis()->GetXmin();
  // High edge of the last bin
  Float_t xMax = aHist->GetXaxis()->GetXmax();
  cout << "Number of bins = " << numberOfXbins << endl;
  cout << "From xMin = " << xMin << " to xMax = " << xMax << endl;

  Double_t x;
  Double_t mean;
  Double_t errorOnMean;
  Double_t chiSquareOfFit;
  TH1D* hist;

  cout << "Bin\t X\tMean     \tError    \tChi2" << endl;

  gr1 = new TGraphErrors();
  Int_t pointNumber = 0; // counter of points
  // Start with 0 to include underflow?
  for (Int_t i = 1; i <= numberOfXbins; i++) {

    // Get a verticle slice in the 2D histogram
    hist = aHist->ProjectionY("_py",i,i);
    Double_t currentBinLowEdge = aHist->GetBinLowEdge(i);
    Double_t currentBinHiEdge  = currentBinLowEdge+aHist->GetBinWidth(i);
    Int_t numOfEntries = hist->GetEntries();

    // This bin should not be plotted if
    // the number of entries is low
    if ( numOfEntries < minNumEntries ) {
      cout << Form("%.2i\tEntries<%i\n",i,minNumEntries);
      continue;
    }
    
    // Get location on x axis of the slice
    x = aHist->GetBinCenter(i);
    
    // In the slice, get the location of the max and rms 
    Double_t locationOfMaxBin = hist->GetBinCenter(hist->GetMaximumBin());
    Double_t rms = hist->GetRMS();
    
    // Fit around the location of the maximum bin
    Double_t upper = locationOfMaxBin + rms;
    Double_t lower = locationOfMaxBin - rms;
    //    cout << "fitting " << endl;
    hist->Fit("gaus", "Q", "", lower, upper);
    
    mean =        hist->GetFunction("gaus")->GetParameter(1);
    errorOnMean = hist->GetFunction("gaus")->GetParError(1);
    chiSquareOfFit = hist->GetFunction("gaus")->GetChisquare();
    
    // Add the point to the graph
    gr1->SetPoint(pointNumber, x, mean);
    gr1->SetPointError(pointNumber, 0, errorOnMean);
    pointNumber++;

    // Save this histogram and the fit
    TString tempTitle=xAxisTitle;
    char low[10];
    char hi[10];
    sprintf(low,"%.2e < ", currentBinLowEdge);
    sprintf(hi ," < %.2e", currentBinHiEdge);
    tempTitle.Prepend(low);tempTitle.Append(hi);
    hist->SetTitle(tempTitle);
    hist->Draw();
    TString fileName(outputFolder+"slice");
    fileName+=Form("%.2i",i);
    fileName+=".gif";
    c1->SaveAs(fileName);

    // Let the user know how the fit went
    cout << Form("%.2i\t%.1f\t%.2e\t%.2e\t%.2e\n",i,x,mean,errorOnMean,chiSquareOfFit);    
  }

  // Draw the Graph of the peak vs the x axis
  gr1->SetMarkerStyle(7);
  gr1->SetTitle(title);
  if (ymin < ymax) {
    gr1->SetMinimum(ymin);
    gr1->SetMaximum(ymax);
  }
  //gr1->GetXaxis()->SetTitle(xAxisTitle);
  gr1->Draw("AP");


  /*
  //Make an animated gif file using gifsicle.
  gSystem->Exec("~mbanderson/gifsicle-1.52/src/gifsicle --delay=150 --loop "+outputFolder+"slice*.gif > anim.gif");
  gSystem->Exec("rm -f "+outputFolder+"slice*.gif");
  */

}
