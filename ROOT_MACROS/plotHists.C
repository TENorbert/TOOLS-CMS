/*************************************************
  Plot given histograms together (up to 4)

  Parameters to draw 2 histograms:
   plotHists(hist1, hist2, label1, label2, Title, xTitle, yTitle)

  Example use (first step is to prevent a bug in ROOT crashing):
   root [1] .O0
   root [2] .L plotHistsAndRatio.C
   root [3] plotHists(photonEt_Barrel,photonEt_Barrel3TrkCut,"Photon E_{T}","E_{T}","entries/20 GeV Bin","#gamma","#gamma track cut")

  Michael Anderson
  March 18, 2009
*************************************************/

#include "TH1.h"
#include <vector>
#include <cmath>

TCanvas* plotHists(vector<TH1*> histogramsToPlot, vector<TString> histogramLabels, TString title="", TString xTitle="", TString yTitle="", bool areaNormalize=false) {


  //*************************************************
  // Variables
  bool plotLogY = true; // false = no log; true = log

  vector<int> histColors;
  histColors.push_back(kBlack); // change colors as you like
  histColors.push_back(kBlue);
  histColors.push_back(kRed);
  histColors.push_back(kGreen-1);

  float histLineWidth = 2.0;

  bool useMarkers = true;         // search online for TAttMarker
  vector<int> histMarkerStyles;    // to see all available markers
  histMarkerStyles.push_back( 20 );
  histMarkerStyles.push_back( 22 );
  histMarkerStyles.push_back( 21 );
  histMarkerStyles.push_back( 23 );
  float histMarkerSize = 0.9;

  int plotWidth=600;
  int plotHeight=600;

  // END of Variables
  //*************************************************


  // Create the canvas
  TCanvas *c1 = new TCanvas("c1", "c1",0,0,plotWidth,plotHeight);

  // Turn off stats box
  gStyle->SetOptStat(0);

  // Create copies of histograms provided
  //  and area normalize, if requested
  vector<TH1*> hists;
  for (int i=0; i<histogramsToPlot.size(); i++) {
    hists.push_back( (TH1*)histogramsToPlot[i]->Clone() );
    if (areaNormalize) {
      Double_t integral = hists[i]->Integral();
      if (integral>0.0) hists[i]->Scale(1/integral);
    }
  }


  // Set histogram plotting variables
  //  and add them to the histogram stack & legend
  THStack *tempStack = new THStack();
  TLegend *infoBox = new TLegend(0.65, 0.70, 0.90, 0.85, "");
  for (int i=0; i<histogramsToPlot.size(); i++) {

    hists[i]->SetLineColor(histColors[i]);
    hists[i]->SetLineWidth(histLineWidth);

    if (useMarkers) {
      hists[i]->SetMarkerStyle(histMarkerStyles[i]);
      hists[i]->SetMarkerColor(histColors[i]);
      hists[i]->SetMarkerSize(histMarkerSize);
    }

    infoBox->AddEntry(hists[i],histogramLabels[i],"LP");

    tempStack->Add(hists[i]);
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
  tempStack->SetTitle(title+";"+xTitle+";"+yTitle);

  // Draw legend as long as the labels aren't empty
  infoBox->SetShadowColor(0);  // 0 = transparent
  infoBox->SetLineColor(0);
  infoBox->SetFillStyle(0);
  if (histogramLabels[0]!="") {
    infoBox->Draw();
  }

  c1->SetLogy(plotLogY);

  return c1;
}



TCanvas* plotHists(TH1* hist1, TH1* hist2, TString title="", TString xTitle="", TString yTitle="", TString label1="", TString label2="", bool areaNormalize=false) {
  vector<TH1*> histogramsToPlot;
  histogramsToPlot.push_back( hist1 );
  histogramsToPlot.push_back( hist2 );
  vector<TString> histogramLabels;
  histogramLabels.push_back( label1 );
  histogramLabels.push_back( label2 );
  TCanvas* c1 = plotHists(histogramsToPlot, histogramLabels, title, xTitle, yTitle, areaNormalize);
  return c1;
}

TCanvas* plotHists(TH1* hist1, TH1* hist2, TH1* hist3, TString title="", TString xTitle="", TString yTitle="", TString label1="", TString label2="", TString label3="", bool areaNormalize=false) {
  vector<TH1*> histogramsToPlot;
  histogramsToPlot.push_back( hist1 );
  histogramsToPlot.push_back( hist2 );
  histogramsToPlot.push_back( hist3 );
  vector<TString> histogramLabels;
  histogramLabels.push_back( label1 );
  histogramLabels.push_back( label2 );
  histogramLabels.push_back( label3 );
  TCanvas* c1 = plotHists(histogramsToPlot, histogramLabels, title, xTitle, yTitle, areaNormalize);
  return c1;
}

TCanvas* plotHists(TH1* hist1, TH1* hist2, TH1* hist3, TH1* hist4, TString title="", TString xTitle="", TString yTitle="", TString label1="", TString label2="", TString label3="", TString label4="", bool areaNormalize=false) {
  vector<TH1*> histogramsToPlot;
  histogramsToPlot.push_back( hist1 );
  histogramsToPlot.push_back( hist2 );
  histogramsToPlot.push_back( hist3 );
  histogramsToPlot.push_back( hist4 );
  vector<TString> histogramLabels;
  histogramLabels.push_back( label1 );
  histogramLabels.push_back( label2 );
  histogramLabels.push_back( label3 );
  histogramLabels.push_back( label4 );
  TCanvas* c1 = plotHists(histogramsToPlot, histogramLabels, title, xTitle, yTitle, areaNormalize);
  return c1;
}
