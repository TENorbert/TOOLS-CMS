//** My simple Macro to Fit Z mass,time, and Plots, hists ***/
//Run AS 
//>> root -l BackgroundEstimation.cc
// OR 
//>>> ./BkgCompile 
//>>>./BkGEstimate

// Designed by 10Sr @2013 // norbe072@umn.edu
#include  "TObject.h"
#include  "TF1.h"
#include  "TFile.h"
#include  "TH1.h"
#include  "TH1D.h"
#include  "TFrame.h"
#include  "TCanvas.h"
#include  "TLegend.h"
#include  "Riostream.h"
#include  "TDirectory.h"
#include  "TKey.h"
#include  "TGaxis.h"
#include  "TString.h"
#include  "TStyle.h"
#include  "TSystem.h"
#include  "TMath.h"
#include  "TPaveText.h"
#include  "TPaveLabel.h"
#include  "TVirtualFitter.h"
#include  "TMatrixT.h"
#include  "TMatrixD.h"
#include  "TROOT.h"
#include  <iostream>
#include  <string>
#include  <cmath>
#include <stdio.h>

#include "MathFunctions.h"

//** Define Fit Constants **//
#define MassL 25 //36  // 86GeV Bin  76 GeV = Bin 26 80 GeV = Bin 31 75GeV = bin 25
#define MassH 50 //47  // 96 GeV=Bin 47 While 100GeV = bin 51
#define bmin  50  //60  //64 //86
#define bmax  130 //102 //100 //96
#define Mmin  50
#define Mmax  130
#define tmin -4.0
#define tmax  15.0
#define LRange  46 //43  // -3ns Bin   -2ns = binX= 47
#define HRange  73 //73 //78  // 3 ns bin  +2ns = binX = 73

#define binWL  48  //  -2ns = binX= 47
#define binWU  73  //   2ns = binX= 73

#define binTL  43  //   -3ns = binX= 43
#define binTU  78  //    3ns = binX= 78


#define lowM  1     //50GeV bin
#define hiM  80      // 130 GeV bin
#define lowt 1       // -10 ns bin
#define hit  120     // 10 ns Bin
#define FitLowRange (-3)
#define FitHighRange (3)
#define NewMergeBinNum 6
#define MinBinC 5

#define Title   "Seed Time[ns]"
#define Title1  "Invariant Mass"
#define Title2  "Seed Time[ns]"
//#define YTitle "numb. electron pairs/0.05ns" 
//#define YTIT2 "numb. electrons/0.05ns"
#define YTIT2 "Numb. electrons/0.2ns"
#define XTIT2 "t_{electron}[ns]"  
#define YTIT "numb. Events/1GeV"
#define XTIT "M_{#gamma^{1}, #gamma^{2}}[GeV/c^{2}]"  
#define FITTYPE "GAUS FIT"
#define CMSRESULT "CMS Preliminary 2012"
#define LHC     "#sqrt{s}=8 TeV  L=8.5 fb^{-1}"
#define DET "EB"
#define CANV1 "EB_ZMmass"
#define CANV2 "EB_SeedTime"

#define SIGMA "#sigma_{eff} = 0.340 ns"   // EE_(t1-t2) = 0.384 ns EB_(t1-t2) = 0.232 ns, EB_(t) = 0.340 ns, EE_(t) = 0.342 ns
//#define HIST "EEEE/TOF-corr time difference of seeds"  // Change name of Hist
//#define HIST "EBEB/seed time"  // Change name of Hist
#define ZMASS "EBEB/mass" 
#define HZbkgTime "EBEB/NZMass seed time" 
//#define HZbkgTime "EBEB/seed time" 
#define HZsigTime "EBEB/NZMass seed time" 
//#define HZsigTime "EBEB/seed time" 
#define HNZMass "EBEB/NZmass" 

#define CNAME2 "Seed_Time_From_Uncleaned_di_photon_Mass_Fit_DoubleElectron_Run2012A.png" 
#define PNAME2 "Seed_Time_From_Uncleaned_di_photon_Mass_Fit_DoubleElectron_Run2012A..pdf"  

#define CNAME1 "Uncleaned_di_Photon_ZMass_Fit_DoubleElectron_Run2012A.png" 
#define PNAME1 "Uncleaned_di_Photon_ZMass_Fit_DoubleElectron_Run2012A..pdf"  
using namespace std;
/*
const signed FitLowRange  = -3; 
const signed FitHighRange = 3;
const signed LRange = -3; 
const signed HRange = 3; 
*/
 
 // Set Style for PLots
 
void PlotStyle()
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
   ////TGaxis::SetMaxDigits(2); // Set Axis to be of the form 0.11 10^N
 } 


// Hist Style /////////
void HistS( TH1F* i_hist )
  {
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
   i_hist->SetYTitle(YTIT2); 
   i_hist->SetXTitle(XTIT2); 
   i_hist->GetXaxis()->SetRangeUser(tmin, tmax);    
 }

// Hists Style /////////
void HistStyle( TH1F* hzm, TH1F * hzt, TH1F* hzbkg ){
   hzm->SetTitle(Title1);   
   hzm->SetMarkerStyle(20);
   hzm->SetMarkerSize(1.0);
   hzm->SetMarkerColor(6);
   hzm->SetLineStyle(1);
   hzm->SetLineWidth(5);
   hzm->SetLineColor(1);
   hzm->SetStats(1);
   hzm->SetTitleSize(0.08, "x");   
   hzm->SetTitleOffset(1.0, "x");    
   hzm->SetTitleSize(0.06, "y"); 
   hzm->SetTitleOffset(0.95, "y");    
   hzm->SetYTitle(YTIT); 
   hzm->SetXTitle(XTIT); 
   hzm->GetXaxis()->SetRangeUser(Mmin, Mmax);    

   hzt->SetTitle(Title2);   
   hzt->SetMarkerStyle(20);
   hzt->SetMarkerSize(1.0);
   hzt->SetMarkerColor(1);
   hzt->SetLineStyle(1);
   hzt->SetLineWidth(5);
   hzt->SetLineColor(1);
   hzt->SetStats(1);
   hzt->SetTitleSize(0.08, "x");   
   hzt->SetTitleOffset(1.0, "x");    
   hzt->SetTitleSize(0.06, "y"); 
   hzt->SetTitleOffset(0.95, "y");    
   hzt->SetYTitle(YTIT2); 
   hzt->SetXTitle(XTIT2); 
   hzt->GetXaxis()->SetRangeUser(tmin, tmax);    

   hzbkg->SetTitle(Title1);   
   hzbkg->SetMarkerStyle(20);
   hzbkg->SetMarkerSize(1.0);
   hzbkg->SetMarkerColor(6);
   hzbkg->SetLineStyle(1);
   hzbkg->SetLineWidth(5);
   hzbkg->SetLineColor(1);
   hzbkg->SetStats(1);
   hzbkg->SetTitleSize(0.08, "x");   
   hzbkg->SetTitleOffset(1.0, "x");    
   hzbkg->SetTitleSize(0.06, "y"); 
   hzbkg->SetTitleOffset(0.95, "y");    
   hzbkg->SetYTitle(YTIT); 
   hzbkg->SetXTitle(XTIT); 
   hzbkg->GetXaxis()->SetRangeUser(Mmin, Mmax);    


}



//// Fit Fxn style////
void FfxnS( TF1* fxnfit){
   fxnfit->SetNpx(500);
  // fxnfit->SetLineWidth(4);
  // fxnfit->SetLineStyle(5);
   //fxnfit->SetLineColor(kBlue);
}

//// Canvas Style ////
void CanS ( TCanvas* ct ){
   ct->SetGridx();
   ct->SetGridy();
   ct->GetFrame()->SetFillColor(21);
   ct->GetFrame()->SetBorderMode(-1);
   ct->GetFrame()->SetBorderSize(5);
/* c1->Divide(2,1);  */
}

///### Legend Style ### ///

void LegS ( TLegend* lg, TF1* ffxn, TH1F* hf )
 {
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


// Fxn Rebin Histograms
// Automatic rebin
TH1F* RebinHistogram( TH1F* h1, TH1F* h2, double minBC ) {

     // accumuate bin information
     vector<double> xV ;
     vector<double> yV ;
     double aveBC = 0 ;
     double sumBC = 0 ;
     double rbin_ = 0 ;
     for ( int i= 1 ; i<= h1->GetNbinsX() ; i++ ) {

         double bc_   = h1->GetBinContent(i) ;
         double x_    = h1->GetBinCenter(i) ;
         double bc1_  = ( i == h1->GetNbinsX() ) ? minBC+1 : h1->GetBinContent(i+1) ;
         //cout<<" x: "<< x_ <<" y:"<< bc_   ;
         if ( aveBC < minBC ) {
            sumBC += bc_ ;
            rbin_ += 1. ;
            aveBC = sumBC / rbin_ ;
            if ( (aveBC >  minBC && bc1_ > minBC) || i == h1->GetNbinsX() ) {
               xV.push_back( x_ ) ;
               yV.push_back( aveBC ) ;
               //cout<<" aveBC: "<< aveBC<<"  from sum:"<< sumBC<<"/"<<rbin_   ;
               sumBC = 0 ;
               rbin_ = 0 ;
            }
            aveBC = 0 ;
         }
         //cout<<" "<<endl ; 
     }
     //cout<<" ====================== "<<endl ; 
     // refill the histogram 
     h2 = (TH1F*) h1->Clone() ;
     //h2->SetName( newHistoName->GetName() ) ;
     int ii = 0 ;
     for ( int i= 1 ; i<= h1->GetNbinsX() ; i++ ) {
         h2->SetBinContent(i, 0 ) ;
         for ( size_t j= ii ; j< xV.size(); j++ ) {
              double x_    = h2->GetBinCenter(i) ;
              if ( x_ <= xV[j] ) {
                 h2->SetBinContent(i, yV[j] ) ;
                 ii = j ;
                 //cout<<" x: "<< x_ <<" xB:"<< xV[j]<<" y: "<< yV[j] <<endl ;
                 break ;
              }
         }
     }
     return h2 ;
}

// Protect zero bins
 void protectZeroBin(TH1* hin) 
 
  {
   hin->Print("v");

      for(int i = 1; i<=hin->GetNbinsX()+1; i++){
           for(int j =1; j<= hin->GetNbinsY()+1;j++){
	       if(hin->GetBinContent(i, j) <=0)  {
	          hin->SetBinContent(i, j, 1.e-10);
                  hin->SetBinError(i, j, 1.e-10);
	}
      }
    }
  }

 Double_t CalcFxnInt( TH1F* h2calc, TF1* fxnf, int minB, int maxB ) 
  { 
    //TF1 *ffxn  = new TF1("ffxn", fxnf, bmin, bmax, 4 );
    int Nxbins = h2calc->GetNbinsX();
    int Nybins = h2calc->GetNbinsY();
    std::cout <<"X Bins Num =  " << Nxbins << "\t" << "Y Bins Num =  " << Nybins << std::endl;
   //Loop over histogrambins
    Double_t Sum = 0 ;
    for ( int b = minB; b <= maxB; b++ ) 
        {
         Double_t binC = h2calc->GetBinCenter( b ) ;
         //Sum += fxnf( binC ) ; 
        }
        std::cout << "Integral Over " << "[ " << minB << " ," << maxB << "]" << " == " << Sum << std::endl; 
     
     return Sum ;
 }


// Fit Hist && Get ScaleF
//
Double_t FitAndGetScaleF(TH1F *h2fit, TH1F *h_zmbkg,  TF1* fitf, TF1* fxnCalc) 
  {
    
    Double_t ScaleF = 1.0;
    Double_t Nin = 0;
    Double_t M1out = 0;
    Double_t M2out = 0;
    Double_t M_Out   = 0;
    Double_t  sfactor = 1.0;

    //h2fit->Fit("fitf", "LLRQ");
    //h2fit->Fit(fitf, "LLRQ");
    h_zmbkg->Fit(fitf, "LL");

    //Get Fit Parameters to use for fitCalC fxn;
    Double_t par0 = fitf->GetParameter(0);
    Double_t par1 = fitf->GetParameter(1);
    Double_t par2 = fitf->GetParameter(2);
    Double_t par3 = fitf->GetParameter(3);
    std::cout <<"Fit Parameters used == ...\t" << "[ " << par0 << ", " << par1 << ", " << par2 << ", " << par3 << " ]" <<  std::endl;
    double chisq=fitf->GetChisquare();
    double ndf=fitf->GetNDF();
    double chisqdf=chisq/ndf;
    std::cout << "Chisquare: " << chisq << "/" << ndf << " : " << chisqdf << std::endl;
 
    // fitCalc  with new parameters
    fxnCalc->SetParameters( fitf->GetParameters() );

   // Calculate Scaling From Integrals
    Nin = fxnCalc->Integral( 76.0, 100.0);
    //Nin = fxnCalc->Integral( 80.0, 100.0);
    //Nin = CalcFxnInt( h2fit, fxnCalc, MassL, MassH ) ;
    //M1out = fxnCalc->Integral(50.0 , 75.0 );
    M1out = fxnCalc->Integral(50.0 , 75.0 );
    M2out = fxnCalc->Integral(101.0, 130.0 );
     
    M_Out = h_zmbkg->Integral(lowM, 25) + h_zmbkg->Integral(52, hiM);
 
    printf("@@@@@@@@@@@@@@@@@@@  RESULTS  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

    std::cout <<"NUMBER OF EVENTS WITH Zs WITH  MASS IN [76, 100] GeV/c^2  =  " << Nin << std::endl;
    std::cout <<"NUMBER OF  NON-Z EVENTS OUTSIDE Z MASS  REGION =  " << M1out + M2out <<  std::endl;
    std::cout <<"USING BACKGROUND HISTORGRAM, NUMBER OF NON-Z EVENTS OUTSIDE Z MASS REGION =  " << M_Out  << std::endl;

    printf("*****************************************************\n");
    std::cout <<"SCALE FACTOR DERIVED ...=  " << Nin/((M1out + M2out)) << std::endl;
    printf("*****************************************************\n");
    
    sfactor  = (M_Out == 0 ) ? (1.0) : (Nin/M_Out) ;

    return ScaleF =  ( (M1out + M2out) != 0 ) ? (Nin/((M1out + M2out))) : (1);  // 2 bc seed time hist is double electrons
   //return  sfactor/2;
 }	

//Fxn From Fit Parameters
//
TF1* GetFxnFromFitPams( TH1F* fithist, TF1 *ffxn, TF1* fxnfff ) 
 {
   fithist->Fit( ffxn, "LLRQ" );
   fxnfff->SetParameters( ffxn->GetParameters() );
   return fxnfff;
}
// Fxn Scale, Subtract And Print Ratio
//
TH1F * ScaleAndSubtractHists( TH1F *hstime, TH1F *hbtime, Double_t sf ) 
  {
    Double_t intimeNum, tailNumL, tailNumR, Ratio ;
    Double_t intimeNumB4, tailNumLB4, tailNumRB4, RatioB4 ;

    TH1F *hs = (TH1F*)hstime->Clone();
    hs->SetName("ExpectedSignalTime");
    printf("#####################################################\n");
    std::cout <<" TOTAL BACKGROUND EVENTS BEFORE SCALING... =  " << hbtime->Integral(lowt, hit) << std::endl;
    printf("#####################################################\n");
    tailNumLB4 = hstime->Integral(lowt, binTL ); //  t < -3ns
    tailNumRB4 = hstime->Integral(binTU, hit);   //   t > 3ns 
    intimeNumB4 = hstime->Integral(binWL , binWU); // -2 < t < 2
   
    //RatioB4 = (intimeNumB4 == 0) ? (0) : ( (tailNumLB4 + tailNumRB4)/intimeNumB4 );
    RatioB4 = (tailNumLB4 <= 0) ? ( tailNumRB4/intimeNumB4 ) : ( (tailNumLB4 + tailNumRB4)/intimeNumB4 );
    
    printf("==============================================================================================\n");
    
    std::cout << "==> B4 SCALING: ELECTRON CANDIDATES WITHIN [-2, 2]ns =\t " <<  intimeNumB4 << std::endl ; 
    std::cout << "==> B4 SCALING: ELECTRON CANDIDATES WITH   t < -3 ns =\t" <<  tailNumLB4  << std::endl ; 
    std::cout << "==> B4 SCALING: ELECTRON CANDIDATES WITH   t > 3 ns =\t" << tailNumRB4 << std::endl;  
    std::cout << "==> B4 SCALING: ELECTRON CANDIDATES WITH  |t| > 3 ns =\t" << tailNumLB4 + tailNumRB4 << std::endl; 
    std::cout << "==> B4 SCALING: RATION OF ELECTRON CANDIDATES  WITH |t| > 3 ns / |t| < 2ns =\t" <<  RatioB4 << std::endl ;

    printf("==============================================================================================\n");


    // Now Scale
    hbtime->Scale(sf);
   
   // hbtime->Sumw2();
   // hstime->Sumw2();
    //hs->Sumw2();
    // Rebin Hists
    //TH1F *hstimenew = (TH1F*)hstime->Rebin(NewMergeBinNum,"hstimenew" ) ;
    //TH1F *hbtimenew = (TH1F*)hbtime->Rebin(NewMergeBinNum,"hbtimenew" ) ;
    TH1F* hstimenew = (TH1F*)hstime->Clone();
    TH1F* hbtimenew = (TH1F*)hbtime->Clone();
   // hstimenew = (TH1F*)RebinHistogram(hstime, hstimenew, MinBinC) ;
   // hbtimenew = (TH1F*)RebinHistogram(hbtime, hbtimenew, MinBinC) ;
    
    printf("#####################################################\n");
    std::cout <<" TOTAL BACKGROUND EVENTS AFTER SCALING ... =  " << hbtime->Integral(lowt, hit) << std::endl;
    std::cout <<" TOTAL BACKGROUND EVENTS AFTER SCALING ... =  " << hbtimenew->Integral(lowt, hit) << std::endl; 
    printf("#####################################################\n");
    std::cout <<" SCALE FACTOR USED ... =  " << sf << std::endl;
    //std::cout <<"Total Events BEFORE Subtraction =  " << hstime->Integral(lowt, hit) << std::endl;
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    std::cout <<"TOTAL EVENTS BEFORE SUBTRACTION=  " << hstimenew->Integral(lowt, hit) << std::endl;
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    //protectZeroBin( hstime );
    //protectZeroBin( hstime );
      protectZeroBin( hstimenew );
      protectZeroBin( hbtimenew );

  /*
    hstime->Add(hbtime, -1);
    hstime->ResetStats();
    hstime->ResetStats();

    std::cout <<" Total Remaining Events AFTER Subtraction =  " << hstime->Integral(lowt, hit) << std::endl;
    tailNumL = hstime->Integral(lowt, LRange-1 );
    tailNumR = hstime->Integral(HRange-1, hit);
    intimeNum = hstime->Integral(LRange , HRange);
   */

    hbtimenew->Sumw2();
    hstimenew->Sumw2();
    hstimenew->Add(hbtimenew, -1);
    printf("*****************************************************\n");
    std::cout <<" TOTAL EVENTS  AFTER SUBTRACTION =  " << hstimenew->Integral(lowt, hit) << std::endl;
    printf("*****************************************************\n");
    tailNumL = hstimenew->Integral(lowt, binTL );    // t < -3 ns
    tailNumR = hstimenew->Integral(binTU, hit);   // t > 3 ns
    intimeNum = hstimenew->Integral(binWL , binWU);   // -2 < t < 2 ns
   
  //  Ratio = (intimeNum == 0) ? (0) : ( (tailNumL + tailNumR)/intimeNum );
    Ratio  = (tailNumL <= 0) ? ( tailNumR/intimeNum ) : ( (tailNumL + tailNumR)/intimeNum );
    printf("======================================================================================================\n");
    
    std::cout << "==> AFTER SUBTRACTION:  ELECTRON CANDIDATES WITHIN [-2, 2] ns =\t " <<  intimeNum << std::endl ; 
    std::cout << "==> AFTER SUBTRACTION:  ELECTRON CANDIDATES WITH   t < -3 ns =\t" <<  tailNumL  << std::endl ; 
    std::cout << "==> AFTER SUBTRACTION:  ELECTRON CANDIDATES WITH   t > 3 ns =\t" << tailNumR << std::endl;  
    std::cout << "==> AFTER SUBTRACTION:  ELECTRON CANDIDATES WITH   |t| > 3 ns =\t" << tailNumL + tailNumR << std::endl; 
    std::cout << "==> AFTER SUBTRACTION:  RATIO OF ELECTRON CANDIDATES  WITH      |t| > 3 ns / |t| < 2ns =\t" <<  Ratio << std::endl ;

    printf("=======================================================================================================\n");
  
    //hstimenew->Scale(1.0/hstimenew->Integral());
    protectZeroBin(hstimenew);
    /*TH1F* hbck_expt = (TH1F*)RebinHistogram(hstimenew, hbck_expt, MinBinC); 
    hbck_expt->Scale(1.0/hbck_expt->Integral()); 
    hbck_expt->Rebin(3);
    hbck_expt->GetXaxis()->SetRangeUser(-4.5, 15.5);
    return hbck_expt ; 
    */
   // hstimenew->Scale(1.0/hstimenew->Integral()); 
    hstimenew->Rebin(4);
    hstimenew->GetXaxis()->SetRangeUser(-4.0, 15.0);
   // hstimenew->GetYaxis()->SetRangeUser(1.5e-7, 1.0);
    hstimenew->GetYaxis()->SetRangeUser(1.0, 1.5e6);
    return hstimenew ; 
 } 
 

 // Fxn Plot Hists
 //
void PlotHists( TH1F *h_zmass, TH1F *h_stime,TH1F* h_zbkg, TF1 *fitf, TF1* ffpfxn, TCanvas *c1, TCanvas *c2, TCanvas *c3 ) 
  {            
  // LegS(leg, fitfxn, ihist );
  // AddTex(pav1);
  // AddCMS(pav2);
   //leg->Draw();
  // pav1->Draw();
  // pav2->Draw();
   HistStyle(h_zmass, h_stime, h_zbkg);
   CanS( c1 );	
   c1->cd();
   h_zbkg->Draw();
   fitf->SetLineColor(kRed);
   fitf->SetLineWidth(5);
   fitf->Draw("sames");
   c1->SetLogy(1);
   c1->Update();
   
   CanS( c2 );	
   c2->cd();
   h_stime->Draw(); // time after subtraction
   c2->SetLogy(1);
   c2->Update();
  
   CanS( c3 );	
   c3->cd();
   h_zmass->Draw();
   ffpfxn->SetLineColor(kBlue);
   ffpfxn->SetLineWidth(5);
   ffpfxn->Draw("sames");
   c3->SetLogy(1);
   c3->Update();
   
   printf("++++++++++ now saving canvas...\n");
   TString plotname1 = PNAME1 ;
   c1->Print( plotname1 );
   c1->SaveAs(CNAME1);
   
   TString plotname2 = PNAME2 ;
   c2->Print( plotname2 );
   c2->SaveAs(CNAME2);
   
   c3->Print("Background_In_ZMass-From-Di-Photon.pdf"); 
   c3->SaveAs("Background_In_ZMass-From-Di-Photon.png"); 
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
   TString plotname = PNAME2 ;
   c1->Print( plotname );
   c1->SaveAs(CNAME2);
 }       



// Main fxn
void BackgroundEstimation( )
 {
  // PlotStyle();
   // Input Files // 75 t0 100GeV
   TFile* fileD  = new TFile("SIGNAL_TimePerf-plots.root","READ");
   TFile* fileS  = new TFile("SIGNAL_TimePerf-plots.root","READ");
   TFile* fileB  = new TFile("BACKGROUND_TimePerf-plots.root","READ");
   // Input files // 80 to 100GeV
   //TFile* fileD  = new TFile("SIG_TimePerf-plots.root","READ");
  // TFile* fileS  = new TFile("SIG_TimePerf-plots.root","READ");
  // TFile* fileB  = new TFile("BKG_TimePerf-plots.root","READ");
   //TF1 *fitf  = new TF1("fitFcn", mygaus, FitLowRange, FitHighRange, 3 );
  // TF1 *gfit  = new TF1("gfit", MathFunctions::mygauss, bmin, bRejectmax, 3 );
     TF1 *gfit  = new TF1("gfit", MathFunctions::fitPolyReject, bmin, bmax, 4 );
     TF1 *gfxn  = new TF1("gfxn", MathFunctions::fitPoly, 50.0, 130.0, 4 );
  
   TH1F *h_ZMass = (TH1F*)fileD->Get(ZMASS);
   TH1F *h_ZStime = (TH1F*)fileS->Get(HZsigTime);
   TH1F *h_ZBtime = (TH1F*)fileB->Get(HZbkgTime);
   TH1F *h_ZmassBkg = (TH1F*)fileB->Get(HNZMass);
   if(h_ZMass == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
   if(h_ZStime == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
   if(h_ZBtime == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
   if(h_ZmassBkg == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
   
  // TLegend *lgEB = new TLegend(0.15,0.65,0.40,0.85);
   TCanvas *C1 = new TCanvas("C1",CANV1,800, 700); //10,800,900);
   TCanvas *C2 = new TCanvas("C2",CANV2,800, 700); //10,800,900);
   TCanvas *C3 = new TCanvas("C3","Background",800, 700); //10,800,900);
  // TPaveLabel *pv1 = new TPaveLabel(1.3,16000,3.0,19000,SIGMA);  // for EB time diff
   //TPaveLabel *pv1 = new TPaveLabel(1.3,1000,3.0,1200,SIGMA);       // for EE time diff
   //TPaveLabel *pv1 = new TPaveLabel(1.2,2000,3.0,2450,SIGMA);       // for EE time 
  // TPaveLabel *pv1 = new TPaveLabel(1.2,22000,3.0,26500,SIGMA);       // for EB time 
  // TPaveText *pv2 = new TPaveText(0.15,0.65,0.40,0.85,"blNDC");
 
   std::cout <<" Calling Fit & Scale Fxn" << std::endl;
 //   Fit_Hist(h_sEB, fitf, C, lgEB, pv1, pv2);
   Double_t SF = FitAndGetScaleF( h_ZMass, h_ZmassBkg, gfit, gfxn );
   
   // Scaled and Subtract Binned Histograms
   TH1F* exp_bkgtime = ScaleAndSubtractHists( h_ZStime, h_ZBtime, SF );
   
   TF1* fffpams = GetFxnFromFitPams( h_ZmassBkg, gfit, gfxn );
   // Plot Histograms in Subtract && Devide Format
   PlotHists( h_ZMass, exp_bkgtime, h_ZmassBkg, gfit, fffpams, C1, C2, C3 );


   printf("=======>>>>>> Done with Ploting .... =====<<<<<<\n");
   
   TFile* Ofile = new TFile("Bkg_dataSet.root","RECREATE");
   Ofile->cd();
   exp_bkgtime->SetName("histo_exp_bkgtime");
   exp_bkgtime->Rebin(2);
   exp_bkgtime->GetXaxis()->SetRangeUser(-4.5, 15.5);
   exp_bkgtime->Write();
   Ofile->Close();
   Ofile->Save();

   delete Ofile;

 }

// Here is MAIN
#ifndef __CINT__
int main() {
   PlotStyle();
   BackgroundEstimation();
   return 0; 
}
#endif
