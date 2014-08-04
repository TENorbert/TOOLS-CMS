#include <fstream>
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include <vector>
#include <string>
#include"TCanvas.h"
#include "TGraphErrors.h"
#include <math.h>
using namespace std;


void efficiencyplots(char *file1, char*file2)
{
  //  TFile *f = new TFile ("UncalibJetMEt_Apr21ReReco-v1.root");

  cout << " Read File with name:" << file1 <<","<< file2 << endl;
  TFile *f1 = new TFile(file1);  // File with Combine Halo + Real Photons
   TFile *f2 = new TFile(file2);

  TH1D* timing1 = (TH1D*)f1->Get("NeutKin/my.h_Time");
  TH1D* timing2 = (TH1D*)f2->Get("NeutKin/my.h_time");
  TH1D* photime3 = (TH1D*)f1-Get("NeutKin/Hg.hal_seedTime");
 
  cout << " Open Histogram:"<< timing1 << endl;
  cout << " Open Histogram:"<< timing2 << endl;
 cout << " Open Histogram:"<< photime3 << endl
  TH2F *MeanTimeDiff = new TH2F("MeanTimeDiff","MeanTimeDiff Vs Energy",100,0,20000, 10,-1.0,1.0);
ofstream fout;

 int Nxbins1 = (int)timing1->GetNbinsX(); 
 int Nxbins2 = (int)timing2->GetNbinsX();
 int Nbins =0;

 if(Nxbins1!= Nxbins2)
{ cout << " Bins of Two Histograms Not the same, which is bigger?" << endl;

    if( Nxbins1 > Nxbins2)
      Nbins = Nxbins1;
else
  //     swap(Nxbins1,Nxbins2);

    Nbins = Nxbins2;
 }
 else
   { 
  Nbins = Nxbins1;
   }

 cout <<" Number of Xbins is : " << Nbins << endl;
 std::vector<double> MeanTime1;
 std::vector<double> MeanTime2;
 std::vector<double> DiffMean;
 std::vector<double> amplitude;
 std::vector<double> ErrorAmp;
 std::vector<double> ErrorTime;
// loop over all bins

 if(Nbins !=0)

 for( int j=0; j< Nbins+1; ++j)
   {
    double t1 = timing1->TH1::GetBinContent(j);
       MeanTime1.push_back(t1);
    double t2 = timing2->TH1::GetBinContent(j)+0.2;
       MeanTime2.push_back(t2);
       double Amp = timing2->TH1::GetBinCenter(j);///(0.039);
       amplitude.push_back(Amp);
       double errAmp = timing2->TH1::GetBinWidth(j)/(2);//0.039);
   ErrorAmp.push_back(errAmp);


     double difft = MeanTime1[j]-MeanTime2[j]; 

     double errtime = timing2->TH1::GetBinError(j);
     ErrorTime.push_back(errtime);

     DiffMean.push_back(difft);
   }

 fout.open("Difference_In_Mean_Time_Bin_by_Bin_EE.txt",fstream::in | fstream::out);

if(fout.is_open())
{
  cout << "Yeah file is open" << endl;
}

 for(int i=0; i < fabs( DiffMean.size()); i++)
   {
     fout << DiffMean[i] <<"(ns)" << amplitude[i]<<"(GeV)"<< "\n";
     
     MeanTimeDiff->Fill(amplitude[i],DiffMean[i]);

   }

       cout << "Close file now" << endl;

fout.close();

int minx = -1;
int maxx = 1;
 int MinAmp = 2;
 int MaxAmp = 1000000;

TCanvas*c = new TCanvas("c","MeanTimeDiff",400,400);
 c->cd();
MeanTimeDiff->GetYaxis()->SetTitle("DiffInMeanTime (ns)");
MeanTimeDiff->GetXaxis()->SetTitle("Energy(GeV)");
MeanTimeDiff->SetTitle("DifferenceInMeanTime Vs Energy LowEE");
MeanTimeDiff->GetYaxis()->SetRangeUser(minx,maxx);
MeanTimeDiff->GetXaxis()->SetRangeUser(MinAmp,MaxAmp);
MeanTimeDiff->SetFillColor(5);
 c->SetLogx(1);
MeanTimeDiff->SetMarkerStyle(8);
MeanTimeDiff->Draw("*");


 double x[66];
 double y[66];
 double ex[66];
 // double y[62];
  double ey[66];
 
  for( int j=0; j<66; j++)
   {
     x[j]= amplitude[j];
     y[j]= DiffMean[j];
    ex[j]= ErrorAmp[j];
    ey[j]= ErrorTime[j];
   }
 TGraphErrors *Tgraph = new TGraphErrors(66,x,y,ex,ey);
 TCanvas *myc = new TCanvas("myc","GraphWithErrors",800,800);
 // myc->Divide(2,1);
 myc->cd();
 Tgraph->SetFillColor(8);
 Tgraph->GetXaxis()->SetTitle("Energy (GeV)");
 Tgraph->GetYaxis()->SetTitle("DiffInMeanTime (ns)");
 Tgraph->GetYaxis()->SetRangeUser(minx,maxx);
 Tgraph->GetXaxis()->SetRangeUser(MinAmp,MaxAmp);
 Tgraph->SetTitle("DifferenceInMeanTime Vs Energy EE");
 Tgraph->SetMarkerColor(4);
 Tgraph->SetMarkerStyle(21);
 // Tgraph->SetTitle()->Center(1);
// Tgraph->Fit("");
 myc->SetLogx(1);
 Tgraph->Draw("AP*");

}

