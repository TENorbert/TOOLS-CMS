//** My simple Macro to Fit Z mass,time, and Plots, hists ***/
//Run AS 
//root[0] .L ZeetimeFitAndPlot.C
//root[1] .X ZeetimeFitAndPlot.C
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
//#include  "TVirtualFitter.h"
//#include  "TMatrixT.h"
//#include  "TMatrixD.h"
#include  "TROOT.h"
#include  <iostream>
#include  <string>
#include  <cmath>


//** Define Fit Constants **//
#define FitLowRange  -3 
#define FitHighRange  3 

using namespace std;

//TStyle *gStyle = new TStyle();

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


//***############## main fitting Fxn ################ *****//
void ZeetimeFitAndPlot( char *Ifile ){
 
/** Plot Options***/	
//gROOT->Reset();
// gROOT->Clear();
gROOT->SetStyle("Plain") ;
//gROOT->SetBatch(kFALSE);
gStyle->SetOptTitle(0);
gStyle->SetOptStat(0);
gStyle->SetOptFit(1);
gStyle->SetStatX(.89);
gStyle->SetStatY(.89) ;
gStyle->SetStatBorderSize(0);
//gStyle->SetOptStat(1111111)
gStyle->SetCanvasColor(kWhite);   // background is no longer mouse-dropping white
gStyle->SetPalette(1);        // blue to red false color palette. Use 9 for b/w
gStyle->SetCanvasBorderMode(0);     // turn off canvas borders
gStyle->SetPadBorderMode(0);
gStyle->SetPaintTextFormat("5.2f");  // What precision to put numbers if plotted with "TEXT"

// For publishing:
gStyle->SetLineWidth(2);
gStyle->SetTextSize(1.1);
gStyle->SetLabelSize(0.06,"xy");
gStyle->SetTitleSize(0.08,"xy");
gStyle->SetTitleOffset(1.2,"x");
gStyle->SetTitleOffset(1.0,"y");
gStyle->SetPadTopMargin(0.1);
gStyle->SetPadRightMargin(0.1);
gStyle->SetPadBottomMargin(0.16);
gStyle->SetPadLeftMargin(0.12);
TGaxis::SetMaxDigits(2); // Set Axis to be of the form 0.11 10^N


       TFile *ifile  = new TFile(Ifile);
        
	TF1 *fitFcn  = new TF1("fitFcn", mygaus, FitLowRange, FitHighRange, 3 );
	fitFcn->SetNpx(500);
	fitFcn->SetLineWidth(4);
	fitFcn->SetLineStyle(5);
	fitFcn->SetLineColor(kBlue);
        
	TH1F*h_Seed_TimeEBEB = (TH1F*)ifile->Get("EBEB/seed time");
	TH1F*h_Seed_TimeEEEE = (TH1F*)ifile->Get("EEEE/seed time");
	TH1F*h_TofCor_TimeEBEB = (TH1F*)ifile->Get("EBEB/TOF-corr time difference of seeds");
	TH1F*h_TofCor_TimeEEEE = (TH1F*)ifile->Get("EEEE/TOF-corr time difference of seeds");
	
	if(h_Seed_TimeEBEB == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
	if(h_Seed_TimeEEEE == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
	if(h_TofCor_TimeEBEB == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
	if(h_TofCor_TimeEEEE == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
	
	
	
	
	h_Seed_TimeEBEB->SetTitle("Seed Time[ns]");   
        h_Seed_TimeEBEB->SetMarkerStyle(20);
        h_Seed_TimeEBEB->SetMarkerSize(0.8);
        h_Seed_TimeEBEB->SetStats(1);
        h_Seed_TimeEBEB->SetTitleSize(0.08, "x");   
        h_Seed_TimeEBEB->SetTitleOffset(1.0, "x");    
        h_Seed_TimeEBEB->SetTitleSize(0.06, "y"); 
        h_Seed_TimeEBEB->SetTitleOffset(0.95, "y");    
        h_Seed_TimeEBEB->SetYTitle("Number of Seeds/0.05ns"); 
        h_Seed_TimeEBEB->SetXTitle("t_{seed}[ns]"); 
        h_Seed_TimeEBEB->GetXaxis()->SetRangeUser(FitLowRange, FitHighRange);   
        h_Seed_TimeEBEB->GetXaxis()->SetNoExponent(kFALSE);
       /** Set parms as parms of Fit Fxn **/
/*	fitFcn->SetParameters(500, h_Seed_TimeEBEB->GetMean(), h_Seed_TimeEBEB->GetRMS() );
	fitFcn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
	h_Seed_TimeEBEB->Fit("fitFcn", "LL"); //Fit with improved LL/
	std::cout << "Printing Fit Parameters for EBEB ......   " << std::endl;
        printf("Integral of function in EBEB = %g\n", fitFcn->Integral( FitLowRange, FitHighRange));

        ///// retrive fit results //
        int npar = fitFcn->GetNpar();
        TVirtualFitter *fit = TVirtualFitter::GetFitter();
        fit->PrintResults(2,0.);
        TMatrixD *CovMatrix = new TMatrixD ( npar, npar, fit->GetCovarianceMatrix() );
	CovMatrix->Print();
        TCanvas *c1 = new TCanvas("c1","EB-EB",200,10,800,900);
     	c1->SetGridx();
     	c1->SetGridy();
	c1->GetFrame()->SetFillColor(21);
	c1->GetFrame()->SetBorderMode(-1);
	c1->GetFrame()->SetBorderSize(5);
        // c1->Divide(2,1); 
	c1->cd();
	h_Seed_TimeEBEB->Draw();
	fitFcn->Draw("sames");
        c1->SetLogy(0);

	//  draw the legend
    	TLegend *leg = new TLegend(0.15,0.72,0.3,0.85);
	leg->Clear();
       	leg->SetTextFont(72);
        leg->SetTextSize(0.04);
        leg->AddEntry(h_Seed_TimeEBEB,"EB","lpe");
        leg->AddEntry(fitFcn,"GAUS","l");
        leg->Draw();
	c1->SaveAs("Seed_Time_DoubleElectron_Run2012A-EB-EB.png");
        */
	//EE seed
	h_Seed_TimeEEEE->SetTitle("Seed Time[ns]");   
        h_Seed_TimeEEEE->SetMarkerStyle(20);
        h_Seed_TimeEEEE->SetMarkerSize(0.8);
        h_Seed_TimeEEEE->SetStats(1);
        h_Seed_TimeEEEE->SetTitleSize(0.08, "x");   
        h_Seed_TimeEEEE->SetTitleOffset(1.0, "x");    
        h_Seed_TimeEEEE->SetTitleSize(0.06, "y"); 
        h_Seed_TimeEEEE->SetTitleOffset(0.95, "y");    
        h_Seed_TimeEEEE->SetYTitle("Number of Seeds/0.05ns"); 
        h_Seed_TimeEEEE->SetXTitle("t_{seed}[ns]"); 
        h_Seed_TimeEEEE->GetXaxis()->SetRangeUser(FitLowRange, FitHighRange);   
        h_Seed_TimeEEEE->GetXaxis()->SetNoExponent(kFALSE);
       /** Set parms as parms of Fit Fxn **/
	fitFcn->SetParameters(500, h_Seed_TimeEEEE->GetMean(), h_Seed_TimeEEEE->GetRMS() );
	fitFcn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
	h_Seed_TimeEEEE->Fit("fitFcn", "LL"); /**Fit with improved LL**/
	std::cout << "Printing Fit Parameters for EEEE ......   " << std::endl;
        printf("Integral of function in EEEE = %g\n", fitFcn->Integral( FitLowRange, FitHighRange));

        //*** retrive fit results***//
      /*  int npar2 = fitFcn->GetNpar();
        TVirtualFitter *fit2 = TVirtualFitter::GetFitter();
        fit2->PrintResults(2,0.);
        TMatrixD *CovMatrix2 = new TMatrixD ( npar2, npar2, fit2->GetCovarianceMatrix() );
	CovMatrix2->Print();
	*/
        TCanvas *c2 = new TCanvas("c2","EE-EE",200,10,800,900);
     	c2->SetGridx();
     	c2->SetGridy();
	c2->GetFrame()->SetFillColor(21);
	c2->GetFrame()->SetBorderMode(-1);
	c2->GetFrame()->SetBorderSize(5);
        /* c1->Divide(2,1);  */
	c2->cd();
	h_Seed_TimeEEEE->Draw();
	fitFcn->Draw("sames");
        c2->SetLogy(0);
	//  draw the legend
    	TLegend *leg1 = new TLegend(0.15,0.72,0.3,0.85);
	leg1->Clear();
       	leg1->SetTextFont(72);
        leg1->SetTextSize(0.04);
        leg1->AddEntry(h_Seed_TimeEEEE,"EE","lpe");
        leg1->AddEntry(fitFcn,"GAUS","l");
        leg1->Draw();
	c2->SaveAs("Seed_Time_DoubleElectron_Run2012A-EE-EE.png");
        
	//EB TOF-corr seed Diff
	h_TofCor_TimeEBEB->SetTitle("TOF-Corrected(t_{1}-t_{2})[ns]");   
        h_TofCor_TimeEBEB->SetMarkerStyle(20);
        h_TofCor_TimeEBEB->SetMarkerSize(0.8);
        h_TofCor_TimeEBEB->SetStats(1);
        h_TofCor_TimeEBEB->SetTitleSize(0.08, "x");   
        h_TofCor_TimeEBEB->SetTitleOffset(1.0, "x");    
        h_TofCor_TimeEBEB->SetTitleSize(0.06, "y"); 
        h_TofCor_TimeEBEB->SetTitleOffset(0.95, "y");    
        h_TofCor_TimeEBEB->SetYTitle("Number Of Seed Pair/0.05ns"); 
        h_TofCor_TimeEBEB->SetXTitle("(t_{seed1} - t_{seed2})TOF-Corrected[ns]"); 
        h_TofCor_TimeEBEB->GetXaxis()->SetRangeUser(FitLowRange, FitHighRange);   
        h_TofCor_TimeEBEB->GetXaxis()->SetNoExponent(kFALSE);
       /** Set parms as parms of Fit Fxn **/
	fitFcn->SetParameters(500, h_TofCor_TimeEBEB->GetMean(), h_TofCor_TimeEBEB->GetRMS() );
	fitFcn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
	h_TofCor_TimeEBEB->Fit("fitFcn", "LL"); /**Fit with improved LL**/
	std::cout << "Printing Fit Parameters for EBEB-Tof ......   " << std::endl;
        printf("Integral of function in EBEB Tof = %g\n", fitFcn->Integral( FitLowRange, FitHighRange));

        //*** retrive fit results***//
       /* int npar3 = fitFcn->GetNpar();
        TVirtualFitter *fit3 = TVirtualFitter::GetFitter();
        fit3->PrintResults(2,0.);
        TMatrixD *CovMatrix3 = new TMatrixD ( npar3, npar3, fit3->GetCovarianceMatrix() );
	CovMatrix3->Print();
	*/
        TCanvas *c3 = new TCanvas("c3","EB-EB TOF-corr",200,10,800,900);
     	c3->SetGridx();
     	c3->SetGridy();
	c3->GetFrame()->SetFillColor(21);
	c3->GetFrame()->SetBorderMode(-1);
	c3->GetFrame()->SetBorderSize(5);
        /* c1->Divide(2,1);  */
	c3->cd();
	h_TofCor_TimeEBEB->Draw();
	fitFcn->Draw("sames");
        c3->SetLogy(0);
	//  draw the legend
    	TLegend *leg2 = new TLegend(0.15,0.72,0.3,0.85);
       	leg2->SetTextFont(72);
        leg2->SetTextSize(0.04);
        leg2->AddEntry(h_TofCor_TimeEBEB,"EB","lpe");
        leg2->AddEntry(fitFcn,"GAUS","l");
        leg2->Draw();
	c3->SaveAs("TOF-Corrected-Seed_Time_DoubleElectron_Run2012A-EB-EB.png");
          
	//EE: TOF-corr seed Diff
	h_TofCor_TimeEEEE->SetTitle("TOF-Corrected(t_{1}-t_{2})[ns]");   
        h_TofCor_TimeEEEE->SetMarkerStyle(20);
        h_TofCor_TimeEEEE->SetMarkerSize(0.8);
        h_TofCor_TimeEEEE->SetStats(1);
        h_TofCor_TimeEEEE->SetTitleSize(0.08, "x");   
        h_TofCor_TimeEEEE->SetTitleOffset(1.0, "x");    
        h_TofCor_TimeEEEE->SetTitleSize(0.06, "y"); 
        h_TofCor_TimeEEEE->SetTitleOffset(0.95, "y");    
        h_TofCor_TimeEEEE->SetYTitle("Number Of Seed Pair/0.05ns"); 
        h_TofCor_TimeEEEE->SetXTitle("(t_{seed1} - t_{seed2})TOF-Corrected[ns]"); 
        h_TofCor_TimeEEEE->GetXaxis()->SetRangeUser(FitLowRange, FitHighRange);   
        h_TofCor_TimeEEEE->GetXaxis()->SetNoExponent(kFALSE);
       /** Set parms as parms of Fit Fxn **/
	fitFcn->SetParameters(500, h_TofCor_TimeEEEE->GetMean(), h_TofCor_TimeEEEE->GetRMS() );
	fitFcn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
	h_TofCor_TimeEEEE->Fit("fitFcn", "LL"); /**Fit with improved LL**/
	std::cout << "Printing Fit Parameters for EEEE-Tof ......   " << std::endl;
        printf("Integral of function in EEEE Tof = %g\n", fitFcn->Integral( FitLowRange, FitHighRange));

        //*** retrive fit results***//
       /* int npar4 = fitFcn->GetNpar();
        TVirtualFitter *fit4 = TVirtualFitter::GetFitter();
        fit4->PrintResults(2,0.);
        TMatrixD *CovMatrix4 = new TMatrixD ( npar4, npar4, fit4->GetCovarianceMatrix() );
	CovMatrix4->Print();
	*/
        TCanvas *c4 = new TCanvas("c4","EE-EE TOF-corr",200,10,800,900);
     	c4->SetGridx();
     	c4->SetGridy();
	c4->GetFrame()->SetFillColor(21);
	c4->GetFrame()->SetBorderMode(-1);
	c4->GetFrame()->SetBorderSize(5);
        /* c1->Divide(2,1);  */
	c4->cd();
	h_TofCor_TimeEEEE->Draw();
	fitFcn->Draw("sames");
        c4->SetLogy(0);
	//  draw the legend
    	TLegend *leg3 = new TLegend(0.15,0.72,0.3,0.85);
       	leg3->SetTextFont(72);
        leg3->SetTextSize(0.04);
        leg3->AddEntry(h_TofCor_TimeEEEE,"EE","lpe");
        leg3->AddEntry(fitFcn,"GAUS","l");
        leg3->Draw();
	c4->SaveAs("TOF-Corrected-Seed_Time_DoubleElectron_Run2012A-EE-EE.png");
       
}       
       /*
        TPaveText *fitlabel = new TPaveText( 0.6, 0.5, 0.9, 0.90, "NDC");
        fitlabel->SetTextAlign(12);
        fitlabel->SetTextSize(0.04);
	fitlabel->SetFillColor(0);
	TText *pte = 0;
	Double_t meanV, meanE;
	char display[50];

        // -- Extract the fit parameters from minuit
	gMinuit->GetParameter(1, meanV, meanE);
	sprintf(display, "mean  = %6.3f", meanV); 
	pte = fitlabel->AddText(display);
	gMinuit->GetParameter(2, meanV, meanE);
	sprintf(display, "sigma = %6.3f", meanV); 
	pte = fitlabel->AddText(display);	
        fitlabel->Draw();
        c1->Update();
      */
      /*
	TCanvas *c2 = new TCanvas("c2_fit","Fit Canvas EE+",200,10,800,900);
     	c2->SetGridx();
     	c2->SetGridy();
	c2->GetFrame()->SetFillColor(21);
	c2->GetFrame()->SetBorderMode(-1);
	c2->GetFrame()->SetBorderSize(5);
        c2->cd();
	h_Seed_TimeEEEE->Draw();
	fitFcn->Draw("sames");

    	TLegend *leg1 = new TLegend(0.5,0.55,0.78,0.65);
       	leg1->SetTextFont(72);
        leg1->SetTextSize(0.04);
        leg1->AddEntry(h_Seed_TimeEEEE,"EE+ Data","lpe");
        leg1->AddEntry(fitFcn,"Global Fit","l");
        leg1->Draw();

	TCanvas *c3 = new TCanvas("EB_fit","Fit Canvas EB",200,10,800,900);
     	c3->SetGridx();
     	c3->SetGridy();
	c3->GetFrame()->SetFillColor(21);
	c3->GetFrame()->SetBorderMode(-1);
	c3->GetFrame()->SetBorderSize(5);
        c3->cd();
	h_Seed_TimeEEEB->Draw();
	fitFcn->Draw("sames");

    	TLegend *leg2 = new TLegend(0.5,0.55,0.78,0.65);
       	leg2->SetTextFont(72);
        leg2->SetTextSize(0.04);
        leg2->AddEntry(h_Seed_TimeEEEB,"EB Data","lpe");
        leg2->AddEntry(fitFcn,"Global Fit","l");
        leg2->Draw();
       */
/*
#ifndef __CINT__
int main() {
           ZeetimeFitAndPlot();
}
#endif
*/
