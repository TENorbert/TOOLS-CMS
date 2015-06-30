/******************************
 * Simple class to hold histogram 
 * methods
 * Just include into 
 * any program
 * Desinger: Tambe E. Norbert
 * Date    : APril 15, 2014
 * Place   : UMN.EDU(TWIN CITIES)
 *
 * 
*/////////////////////////////

#ifndef Histogramming_H
#define Histogramming_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <TMath.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include "TList.h"
#include "TH1.h"
#include "TTree.h"

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "TCanvas.h"
#include "RooAbsPdf.h"
#include "RooFit.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"



using namespace std; 
using namespace RooFit;
using namespace RooStats;


class Histogramming : public TObject {

private:


public:
   
   Histogramming();
   ~Histogramming();

   static TH1D* RebinHist( );
   static TH1D* Rebin ( TH1D* Ihist , int RebinVal );
   static RooWorkspace* Hist2Ntuple( TH1D* Ihist, TFile* ifile);
   static void AddToWorkspace ( RooWorkspace* ws, TH1D* ihist );

};

//#if !define(__CINT__)
// ClassImp(Histogramming);
#endif
