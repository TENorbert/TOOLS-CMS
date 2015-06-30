#ifndef MathFunctions_H
#define MathFunctions_H

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
using namespace std; 

class MathFunctions : public TObject {

private:


public:

   MathFunctions();     
   ~MathFunctions();     
 
   static Double_t fitPoly(Double_t *x, Double_t *par); 
   static Double_t fitPolyReject(Double_t *x, Double_t *par); 
   static Double_t fitPoly6(Double_t *x, Double_t *par); 
   static Double_t fitPoly3(Double_t *x, Double_t *par); 
   static Double_t fitLG( Double_t* x, Double_t* par);
   static Double_t fitGS( Double_t* x, Double_t* par);
   static Double_t fitLD( Double_t* x, Double_t* par);
   static Double_t fitSG( Double_t* x, Double_t* par);
   static Double_t fitSG1( Double_t* x, Double_t* par);
   static Double_t fitBW( Double_t* x, Double_t* par);
   static Double_t fitData( Double_t* x, Double_t* par);
   static Double_t fitData1( Double_t* x, Double_t* par);
   static Double_t fitData2( Double_t* x, Double_t* par);
   static Double_t fitParabola( Double_t *x, Double_t *par);
   static Double_t ConvBWGS( Double_t* x, Double_t* par);
   static Double_t ConvSGGS( Double_t* x, Double_t* par);
   static Double_t ConvJacob( Double_t* x, Double_t* par);
   static Double_t fitJacob( Double_t* x, Double_t* par);
   static Double_t fJacob( Double_t x, Double_t par0, Double_t par1, Double_t par2, Double_t par3, Double_t par4 );
   static Double_t fBWJacob( Double_t x, Double_t par0, Double_t par1, Double_t par2, Double_t par3, Double_t par4, Double_t par5 );
   static Double_t fGSJacob( Double_t* x, Double_t* par);

   static Double_t gauss2D( Double_t *x, Double_t *par);
   static Double_t mygauss( Double_t *t, Double_t * par);

   vector<bool> DataRejection( TF1* fitfunc, Double_t* x, Double_t* y, int N_data );
   bool DataRejection(double sigma, double deviation, int N_data );

   static vector<double> StatErr( double m );
   //static vector<double> ErrAovB( double A, double s_A, double B, double s_B );
   static double ErrAovB( double A, double B, double s_A = -1, double s_B = -1, bool upward = true );
   static double ErrAxB( double A, double B, double s_A = -1, double s_B = -1, bool upward = true );

   //ClassDef(MathFunctions, 1);

};

//#if !defined(__CINT__)
//    ClassImp(MathFunctions);
#endif
