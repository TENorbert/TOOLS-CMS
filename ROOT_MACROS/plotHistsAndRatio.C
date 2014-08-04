/*************************************************
  Plot histograms together, then plot
  their ratio on a histogram below.

  Must provied at least 1 histogram 
  to be the "denomintator", and
  up to 3 histograms to be "numerators".

  All the plots will be shown together in a
  big plot, and below that
  will be a plot showing the ratio of all
  the "numerators" to the "denominator".

  
  Example use (first step is to prevent a bug in ROOT crashing):
   root [1] .O0
   root [2] .L plotHistsAndRatio.C
   root [3] plotHistsAndRatio(photonEt_BarrelPIDcut,photonEt_Barrel,"Photon E_{T}","E_{T} (GeV)","entries/20 GeV bin")  

  Michael Anderson
  March 18, 2009
*************************************************/

#include "TH1F.h"
#include <vector>
#include <cmath>


TCanvas* plotHistsAndRatio(TH1F* numeratorHist, TH1F* denominatorHist, TString title="", TString xTitle="", TString yTitle="") {  
  vector<TH1F*> numeratorHistograms;
  numeratorHistograms.push_back( numeratorHist );
  TCanvas* c1 = plotHistsAndRatio(numeratorHistograms, denominatorHist, title, xTitle, yTitle);
  return c1;
}




TCanvas* plotHistsAndRatio(TH1F* numeratorHist1, TH1F* numeratorHist2, TH1F* denominatorHist, TString title="", TString xTitle="", TString yTitle="") {
  vector<TH1F*> numeratorHistograms;
  numeratorHistograms.push_back( numeratorHist1 );
  numeratorHistograms.push_back( numeratorHist2 );
  TCanvas* c1 = plotHistsAndRatio(numeratorHistograms, denominatorHist, title, xTitle, yTitle);
  return c1;
}



TCanvas* plotHistsAndRatio(TH1F* numeratorHist1, TH1F* numeratorHist2, TH1F* numeratorHist3, TH1F* denominatorHist, TString title="", TString xTitle="", TString yTitle="") {
  vector<TH1F*> numeratorHistograms;
  numeratorHistograms.push_back( numeratorHist1 );
  numeratorHistograms.push_back( numeratorHist2 );
  numeratorHistograms.push_back( numeratorHist3 );
  TCanvas *c1 = plotHistsAndRatio(numeratorHistograms, denominatorHist, title, xTitle, yTitle);
  return c1;
}



TCanvas* plotHistsAndRatio(vector<TH1F*> numeratorHistograms, TH1F* denominatorHist, TString title="", TString xTitle="", TString yTitle="")

{

  int numberOfNumeratorHists = numeratorHistograms.size();
  if (numberOfNumeratorHists>3) {
    cout << "Too many histograms for numerator (currently only supports up to 3)" << endl;
    exit;
  }
  if (!denominatorHist) {
    cout << "denominatorHist provided does not exist" << endl;
    exit;
  }

  //*************************************************
  // Variables
  bool topPlotLogY = 1;      // 0 = no log; 1= log
  TString yTitle2 = "ratio"; // bottom plot y axis title

  vector<int> histColors; 
  histColors.push_back(kBlue);  // change colors as you like
  histColors.push_back(kRed);
  histColors.push_back(kGreen-1);

  int histDenominatorColor = kBlack;

  float defaultRatioYmin = 1.02;
  float defaultRatioYmax = 0.60;
  // END of Variables
  //*************************************************

  TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,500);
  c1->Range(0,0,1,1);

  vector<TH1F*> hists;
  for (int i=0; i<numberOfNumeratorHists; i++) {
    hists.push_back( (TH1F*)numeratorHistograms[i]->Clone() );
  }
  TH1F* denominatorHistogram = (TH1F*)denominatorHist->Clone();


  // Create ratio histograms
  vector<TH1F*> hist_over_denomHist;
  for (int i=0; i<numberOfNumeratorHists; i++) {
    hist_over_denomHist.push_back( (TH1F*)numeratorHistograms[i]->Clone() );
    hist_over_denomHist[i]->GetTitle();   
    hist_over_denomHist[i]->Divide(denominatorHistogram);
  }


  //*************************************************
  // Bottom plot
  TPad *c1_1 = new TPad("c1_1", "newpad",0.01,0.0.01,0.99,0.32);
  c1_1->Draw();
  c1_1->cd();
  c1_1->SetTopMargin(0.01);
  c1_1->SetBottomMargin(0.3);
  c1_1->SetRightMargin(0.1);
  c1_1->SetFillStyle(0);


  hist_over_denomHist[0]->Draw();
  hist_over_denomHist[0]->SetLineWidth(1);
  hist_over_denomHist[0]->SetLineColor(histColors[0]);
  hist_over_denomHist[0]->SetMinimum(defaultRatioYmin);
  hist_over_denomHist[0]->SetMaximum(defaultRatioYmax);
  hist_over_denomHist[0]->GetYaxis()->SetNdivisions(5);
  hist_over_denomHist[0]->SetTitle(";"+xTitle+";"+yTitle2);
  hist_over_denomHist[0]->GetXaxis()->SetTitleSize(0.14);
  hist_over_denomHist[0]->GetXaxis()->SetLabelSize(0.14);
  hist_over_denomHist[0]->GetYaxis()->SetLabelSize(0.11);
  hist_over_denomHist[0]->GetYaxis()->SetTitleSize(0.14);
  hist_over_denomHist[0]->GetYaxis()->SetTitleOffset(0.28);
  for (int i=1; i<numberOfNumeratorHists; i++) {
    hist_over_denomHist[i]->SetLineWidth(1);
    hist_over_denomHist[i]->SetLineColor(histColors[i]);
    hist_over_denomHist[i]->Draw("same");
  }
  // End bottom plot
  //*************************************************


  //*************************************************
  // Top Plot
  c1->cd();
  c1_2 = new TPad("c1_2", "newpad",0.01,0.33,0.99,0.99);
  c1_2->Draw(); 
  c1_2->cd();
  c1_2->SetTopMargin(0.1);
  c1_2->SetBottomMargin(0.01);
  c1_2->SetRightMargin(0.1);
  c1_1->SetFillStyle(0);

  denominatorHistogram->SetLineWidth(2);
  denominatorHistogram->SetLineColor(histDenominatorColor);
  denominatorHistogram->Draw();
  denominatorHistogram->SetLabelSize(0.0);
  denominatorHistogram->GetXaxis()->SetTitleSize(0.00);
  denominatorHistogram->GetYaxis()->SetLabelSize(0.07);
  denominatorHistogram->GetYaxis()->SetTitleSize(0.08);
  denominatorHistogram->GetYaxis()->SetTitleOffset(0.76);
  denominatorHistogram->SetTitle(title+";;"+yTitle);

  for (int i=0; i<numberOfNumeratorHists; i++) {
    hists[i]->SetLineWidth(2);
    hists[i]->SetLineColor(histColors[i]);
    hists[i]->Draw("same");
  }

  c1_2->SetLogy(topPlotLogY);
  // End bottom plot
  //*************************************************

  return c1;
}
