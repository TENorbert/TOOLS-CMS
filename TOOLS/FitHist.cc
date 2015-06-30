//** My simple Macro to Fit Z mass,time, and Plots, hists ***/
//Run AS 
//>> root -l FitHist.cc
// OR 
//>>> ./compile 
//>>>./HFit
// Designed by 10Sr @2013 // norbe072@umn.edu
#include "TObject.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TFrame.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "Riostream.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TGaxis.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include  "TMath.h"
#include  "TPaveText.h"
#include "TPaveLabel.h"
#include  "TVirtualFitter.h"
#include  "TMatrixT.h"
#include  "TMatrixD.h"
#include  "TROOT.h"
#include  <iostream>
#include  <string>
#include  <cmath>


//** Define Fit Constants **//
#define FitLowRange (-2)
#define FitHighRange (2)
#define LRange (-12)
#define HRange (12)
#define Title  "" //"Seed Time[ns]"
//#define YTitle "numb. electron pairs/0.05ns" 
#define YTitle "numb. electrons/0.05ns"
//#define XTitle "time of corrected: (t_{electron1} - t_{electron2})[ns]" 
#define XTitle "t_{electron}[ns]"  
#define FITTYPE "GAUS FIT"
#define CMSRESULT "CMS Preliminary 2012"
#define LHC     "#sqrt{s}=8 TeV  L=8.5 fb^{-1}"
#define DET "EB"
#define SIGMA "#sigma_{eff} = 0.340 ns"   // EE_(t1-t2) = 0.384 ns EB_(t1-t2) = 0.232 ns, EB_(t) = 0.340 ns, EE_(t) = 0.342 ns
//#define HIST "EEEE/TOF-corr time difference of seeds"  // Change name of Hist
#define HIST "EBEB/seed time"  // Change name of Hist
//#define CNAME "EE-EE-TOF-Corr-difference-of-seed-Time-DoubleElectron_Run2012A.png"  
#define CNAME "EB_EB_Seed_Time_DoubleElectron_Run2012A.png" 
//#define PNAME "EE-EE-TOF-Corr-difference-of-seed-Time-DoubleElectron_Run2012A.pdf"  
#define PNAME "EB_EB_Seed_Time_DoubleElectron_Run2012A.pdf" 
/*
"EBEB/seed time"
"EEEE/seed time"
"EBEB/TOF-corr time difference of seeds"
"EEEE/TOF-corr time difference of seeds"
*/

using namespace std;
/*
const signed FitLowRange  = -3; 
const signed FitHighRange = 3;
const signed LRange = -3; 
const signed HRange = 3; 
*/
/*** 2D Gaus Fit ***/
double gauss2D(double *x, double *par) {
	double z1 = 0;
	if(par[2] != 0) z1 = double((x[0]-par[1])/par[2]);
	double z2 = 0;
	if(par[4] != 0 ) z2 = double((x[1]-par[3])/par[4]);          
  
    return par[0]*exp(-0.5*(z1*z1+z2*z2));
} 

/**private Defined  gaus fxn **/
double mygaus(double *t, double *par){
	double x = t[0];
	double arg0 = par[0];
	double arg1 = 0;
	if(par[2] != 0) arg1 =double( ( x - par[1])/par[2] );
	double  f   = arg0*TMath::Exp(-0.5*arg1*arg1);
	
    return f;
}



//## Style of Hist ###/////////
void HistS( TH1F* i_hist ){

   i_hist->SetTitle(Title);   
   i_hist->SetMarkerStyle(20);
   i_hist->SetMarkerSize(1.0);
   i_hist->SetMarkerColor(1);
   i_hist->SetLineStyle(1);
   i_hist->SetLineWidth(3);
   i_hist->SetLineColor(1);
   i_hist->SetStats(1);
   i_hist->SetTitleSize(0.08, "x");   
   i_hist->SetTitleOffset(1.0, "x");    
   i_hist->SetTitleSize(0.06, "y"); 
   i_hist->SetTitleOffset(0.95, "y");    
   i_hist->SetYTitle(YTitle); 
   i_hist->SetXTitle(XTitle); 
   i_hist->GetXaxis()->SetRangeUser(LRange, HRange);   
   

}

////### Fit Function ##### ////
void FfxnS( TF1* fxnfit){
   fxnfit->SetNpx(500);
  // fxnfit->SetLineWidth(4);
  // fxnfit->SetLineStyle(5);
   //fxnfit->SetLineColor(kBlue);

}

////### Canvas Style ###////
void CanS ( TCanvas* ct ){
   ct->SetGridx();
   ct->SetGridy();
   ct->GetFrame()->SetFillColor(21);
   ct->GetFrame()->SetBorderMode(-1);
   ct->GetFrame()->SetBorderSize(5);
/* c1->Divide(2,1);  */
}

///### Legend Style ### ///

void LegS ( TLegend* lg, TF1* ffxn, TH1F* hf ){
//  draw the legend
   lg->SetTextFont(72);
   lg->SetTextSize(0.04);
   lg->SetHeader("CMS Preliminary 2012");
  // lg->AddText(DET);
 //  lg->AddEntry(hf,"Data","lpe");
 //  lg->AddEntry(ffxn,FITTYPE,"l");
}

//## Add CMS Text ###////
void AddCMS(TPaveText *pt)
  {
	pt->SetBorderSize(2);
	pt->SetFillColor(19);
	TText *text = pt->AddText(CMSRESULT);
	text = pt->AddText(LHC);
	text = pt->AddText(DET);
        //pt = new TPaveStats(0.78,0.82,0.98,0.98,"brNDC");
        //pt->SetBorderSize(2);
        //pt->SetFillColor(19);
        //pt->SetTextAlign(12);

  }

//**Label Adding ****////
void AddTex(TPaveLabel* pave)
  {
   //Can add more text
   pave->SetFillColor(19); 
   pave->SetTextColor(1); 
   pave->SetTextSize(0.5); 
   //TText *tx = pave->AddText(SIGMA);
  // pave->AddText(SIGMA); 
  }


//***############## main fitting Fxn ################ *****//
void Fit_Hist( TH1F* ihist, TF1* fitfxn, TCanvas*c1, TLegend *leg, TPaveLabel* pav1, TPaveText *pav2 ){
            
   HistS( ihist );
  /* FfxnS( fitfxn );
   /// Set parms as parms of Fit Fxn///
   fitfxn->SetParameters(500, ihist->GetMean(), ihist->GetRMS() );
   fitfxn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
   ihist->Fit("fitFcn", "LL"); // Fit with improved LL///
   //ihist->Fit("gaus", "LL"); ///Fit with improved LL///
   std::cout << "Printing Fit Parameters for EBEB ......   " << std::endl;
   printf("Integral of function in EBEB = %g\n", fitfxn->Integral( FitLowRange, FitHighRange));

   // retrive fit results ///
   int npar = fitfxn->GetNpar();
   TVirtualFitter *fit = TVirtualFitter::GetFitter();
   fit->PrintResults(2,0.);
   TMatrixD *CovMatrix = new TMatrixD ( npar, npar, fit->GetCovarianceMatrix() );
   CovMatrix->Print();
   */ 
   // Draw Plot with style ///  	
   CanS( c1 );	
   c1->cd();
   ihist->Draw();
   //fitfxn->Draw("sames");
   c1->SetLogy(1);
  // LegS(leg, fitfxn, ihist );
   AddTex(pav1);
   AddCMS(pav2);
   //leg->Draw();
   pav1->Draw();
   pav2->Draw();
   c1->Update();
   cout <<"Saving Canvas..." << endl;
   TString plotname = PNAME ;

   c1->Print( plotname );
   c1->SaveAs(CNAME);
}       


void FitHist( )
{


   /** Plot Options***/	
   //gROOT->Reset();
   // gROOT->Clear();
   gROOT->SetStyle("Plain") ;
   gROOT->SetBatch(kFALSE);
   gStyle->SetOptTitle(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);
   gStyle->SetStatX(.89);
   gStyle->SetStatY(.89) ;
   gStyle->SetStatBorderSize(0);
   gStyle->SetCanvasColor(kWhite);   // background is no longer mouse-dropping white
   gStyle->SetPalette(1);        // blue to red false color palette. Use 9 for b/w
   gStyle->SetCanvasBorderMode(0);     // turn off canvas borders
   gStyle->SetPadBorderMode(0);
   gStyle->SetPaintTextFormat("5.2f");  // What precision to put numbers if plotted with "TEXT"
  // For publishing:
   gStyle->SetLineWidth(2);
   gStyle->SetTextSize(1.1);
   gStyle->SetLabelSize(0.06,"xy");
   gStyle->SetTitleSize(0.06,"xy");
   gStyle->SetTitleOffset(0.4,"x");
   gStyle->SetTitleOffset(1.0,"y");
   gStyle->SetPadTopMargin(0.1);
   gStyle->SetPadRightMargin(0.1);
   gStyle->SetPadBottomMargin(0.20);
   gStyle->SetPadLeftMargin(0.12);
   TGaxis::SetMaxDigits(2); // Set Axis to be of the form 0.11 10^N

   // Begin Fitting
   TFile* ifile  = new TFile("TimePerf-plots.root","READ");
   TF1 *fitf  = new TF1("fitFcn", mygaus, FitLowRange, FitHighRange, 3 );
   TH1F*h_sEB = (TH1F*)ifile->Get(HIST);
   if(h_sEB == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
   TLegend *lgEB = new TLegend(0.15,0.65,0.40,0.85);
   TCanvas *C = new TCanvas("C",DET,1100, 700); //10,800,900);
  // TPaveLabel *pv1 = new TPaveLabel(1.3,16000,3.0,19000,SIGMA);  // for EB time diff
   //TPaveLabel *pv1 = new TPaveLabel(1.3,1000,3.0,1200,SIGMA);       // for EE time diff
   //TPaveLabel *pv1 = new TPaveLabel(1.2,2000,3.0,2450,SIGMA);       // for EE time 
   TPaveLabel *pv1 = new TPaveLabel(1.2,22000,3.0,26500,SIGMA);       // for EB time 
   TPaveText *pv2 = new TPaveText(0.15,0.65,0.40,0.85,"blNDC");
   cout <<" Calling Fitting Fxn" << endl;
   Fit_Hist(h_sEB, fitf, C, lgEB, pv1, pv2);
}

#ifndef __CINT__
int main() {
   
     FitHist();
    return 0; 
}
#endif
