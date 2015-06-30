#include "MathFunctions.h"
MathFunctions::MathFunctions(){
  
}

MathFunctions::~MathFunctions(){
 
}

Double_t MathFunctions::fitPolyReject(Double_t *x, Double_t *par) {

        
         Double_t fitval =  par[0]
                          + (par[1]* x[0] )
                          + (par[2]* x[0]*x[0]  )
                          + (par[3]* x[0]*x[0]*x[0] );

          if( x[0] > 75.0 && x[0] < 100.0  ) 
          //if( x[0] > 80.0 && x[0] < 100.0  ) 
          //if( x[0] > 86.0 && x[0] < 96.0  ) 
          {
           TF1::RejectPoint();
	   return 0;
	  }
        
         return fitval;
}

Double_t MathFunctions::fitPoly6(Double_t *x, Double_t *par) {

         Double_t fitval =  par[0]
                          + (par[1]* x[0] )
                          + (par[2]* x[0]*x[0]  )
                          + (par[3]* x[0]*x[0]*x[0] )
                          + (par[4]* x[0]*x[0]*x[0]*x[0] )
                          + (par[5]* x[0]*x[0]*x[0]*x[0]*x[0] );

         return fitval;
}


Double_t MathFunctions::fitPoly(Double_t *x, Double_t *par) {

         Double_t fitval =  par[0]
                          + (par[1]* x[0] )
                          + (par[2]* x[0]*x[0]  )
                          + (par[3]* x[0]*x[0]*x[0] );

         return fitval;
}

Double_t MathFunctions::fitPoly3(Double_t *x, Double_t *par) {

         Double_t fitval =  par[0]
                          + (par[1]* x[0] )
                          + (par[2]* x[0]*x[0]  );

         return fitval;
}

// Gaussian distribution , p0: Amplitude, p1: mean, p2: sigma
Double_t MathFunctions::fitGS(Double_t *x, Double_t *par) {

     Double_t gs_Value = TMath::Gaus(x[0],par[1],par[2]) ;
     Double_t fitV = par[0]*gs_Value ; 
     return fitV;
}

// log-normal distribution
Double_t MathFunctions::fitLG(Double_t *x, Double_t *par) {

     Double_t A0 =  log( x[0] ) - par[1] ;
     Double_t A1 =   (-1.*par[2]*A0*A0)  ;
     Double_t fitV = par[0]*exp( A1 ) / x[0]  ;
     return fitV;
}

// Landua fxn
Double_t MathFunctions::fitLD(Double_t *x, Double_t *par) {

      Double_t ld_Value = TMath::Landau(x[0],par[1],par[2]) ;
      Double_t fitV = par[0]*ld_Value ;
      return fitV ;

}

// Parabola
Double_t MathFunctions::fitParabola( Double_t *x, Double_t *par ){

     Double_t chi = (x[0] - par[0]) / ( 1.414*par[1] ) ;
     Double_t fV = chi*chi + par[2] ;
     return fV;
}

// Bret-Wigner
Double_t MathFunctions::fitBW(Double_t *x, Double_t *par) {

     Double_t gm = par[0] + 0.001;
     Double_t chi = (x[0] - par[1]) / gm ;
     Double_t C1 = 1. + (chi*chi) ;
     Double_t fV = par[2]/C1 ;
     return fV;
}

// Gaussian + Logarithmic
Double_t MathFunctions::fitSG(Double_t *x, Double_t *par) {

     Double_t gs = TMath::Gaus(x[0],par[1],par[2]);
     Double_t cb_Val = gs + fitLG( x, &par[3] ) ;
     return cb_Val = par[0] * cb_Val ;
     //return fitGS(x,par) + fitLG(x,&par[3]);
     //return fitGS(x,par) + fitLG(x,&par[3]);
}


// Gaussian + Landau
Double_t MathFunctions::fitSG1(Double_t *x, Double_t *par) {

     Double_t gs = TMath::Gaus(x[0],par[1],par[2]);
     Double_t cb_Val = gs + fitLD( x, &par[3] ) ;
     return cb_Val = par[0] * cb_Val ;
}

Double_t MathFunctions::fitData(Double_t *x, Double_t *par) {

     Double_t gs = TMath::Gaus(x[0],par[1],par[2]);

     Double_t A0 =  log( x[0] ) - par[4] ;
     Double_t A1 =   (-1.*par[5]*A0*A0)  ;
     Double_t LG_Val = par[3]*exp( A1 ) / x[0]  ;

     Double_t ld1_Val = TMath::Landau(x[0],par[6],par[7]) ;

     Double_t sg_Val = gs + LG_Val + (par[8]*ld1_Val)  ;
     Double_t fitV = par[0]*sg_Val ;

     return fitV ;
}

Double_t MathFunctions::fitData1(Double_t *x, Double_t *par) {

     Double_t gs = TMath::Gaus(x[0],par[1],par[2]);

     Double_t A0 =  log( x[0] ) - par[4] ;
     Double_t A1 =   (-1.*par[5]*A0*A0)  ;
     Double_t LG_Val = par[3]*exp( A1 ) / x[0]  ;

     Double_t ld1_Val = TMath::Landau(x[0],par[6],par[7]) ;

     Double_t ld2_Val = TMath::Landau(x[0],par[8],par[9]) ;

     Double_t sg_Val = gs + LG_Val + (par[10]*ld1_Val) + (par[11]*ld2_Val) ;
     Double_t fitV = par[0]*sg_Val ;

     return fitV ;
}

Double_t MathFunctions::fitData2(Double_t *x, Double_t *par) {

     Double_t gs = TMath::Gaus(x[0],par[1],par[2]);


     Double_t ld0_Val = TMath::Landau(x[0],par[4],par[5]) ;

     Double_t ld1_Val = TMath::Landau(x[0],par[6],par[7]) ;

     Double_t ld2_Val = TMath::Landau(x[0],par[8],par[9]) ;

     Double_t sg_Val = gs + (par[3]*ld0_Val) + (par[10]*ld1_Val) + (par[11]*ld2_Val) ;
     Double_t fitV = par[0]*sg_Val ;

     return fitV ;
}

Double_t MathFunctions::ConvBWGS(Double_t *x, Double_t *par) {

   Double_t np   = 150 ;
   Double_t sg   = 4.0 ;
   Double_t xlow = x[0] - sg*par[4];
   Double_t xup  = x[0] + sg*par[4];
   Double_t step = (xup - xlow) / np ;
   Double_t sum = 0. ;
   Double_t xx, fbw; 

   for (double i=1.; i<= np/2; i++  ) {
       xx = xlow + (i-.5) * step;
       //fbw = TMath::BreitWigner(xx,par[1],par[0]);
       fbw = fitBW(x,par);
       sum += fbw * TMath::Gaus(x[0],par[3],par[4]);

       xx = xup - (i-.5) * step;
       //fbw = TMath::BreitWigner(xx,par[1],par[0]);
       fbw = fitBW(x,par);
       sum += fbw * TMath::Gaus(x[0],par[3],par[4]);
   }

   //return par[2]*step*sum ;
   return step*sum ;

}

Double_t MathFunctions::ConvSGGS(Double_t *x, Double_t *par) {

   Double_t np   = 200 ;
   Double_t sg   = 5.0 ;
   Double_t xlow = x[0] - sg*par[7];
   Double_t xup  = x[0] + sg*par[7];
   Double_t step = (xup - xlow) / np ;
   Double_t sum = 0. ;
   Double_t xx, fbw; 

   for (double i=1.; i<= np/2; i++  ) {
       xx = xlow + (i-.5) * step;
       fbw = fitSG(x,par);
       sum += fbw * TMath::Gaus(x[0],par[6],par[7]);

       xx = xup - (i-.5) * step;
       fbw = fitSG(x,par);
       sum += fbw * TMath::Gaus(x[0],par[6],par[7]);
   }

   return step*sum ;
}

Double_t MathFunctions::fJacob(Double_t x, Double_t par0, Double_t par1, Double_t par2, Double_t par3, Double_t par4 ) {

   Double_t jaco  = 0. ;
   Double_t scale = 1.  ;
   Double_t wM    = par4 ;
   Double_t Mt    = par1*x ;

   Double_t jacob = 0.;
   if ( Mt <= wM && Mt > par2 ) {
      jaco  = sqrt(  (wM*wM) - ( Mt*Mt )  ) ;
      scale = ( par0 + Mt  );
      jacob = scale / jaco ;
   } else if ( Mt <= wM && Mt <= par2 ) {
      jaco  = sqrt(  (wM*wM) - ( Mt*Mt )  ) ;
      scale = (Mt*Mt/wM);
      jacob = scale / jaco ;
   } else {
      jacob =  0. ;
   }
   return jacob*par3 ;
}

Double_t MathFunctions::fitJacob(Double_t* x, Double_t* par ) {

   Double_t xx = x[0] ;
   Double_t jacob = fJacob( xx, par[0], par[1], par[2], par[3], par[4] )  ;
   return jacob ;
}

Double_t MathFunctions::fBWJacob(Double_t x, Double_t par0, Double_t par1, Double_t par2, Double_t par3, Double_t par4, Double_t par5 ) {

   Double_t np   = 202 ;
   Double_t nsg  = 3.0 ;
   Double_t p4low = par4 - nsg*par5;
   Double_t p4up  = par4 + nsg*par5;
   Double_t step = (p4up - p4low) / np ;
   Double_t sum = 0. ;
   Double_t p4, fjb; 

   for (double i=1.; i<= np/2; i++  ) {
       p4 = p4low + (i-.5) * step;
       fjb = fJacob(x, par0, par1, par2, par3, p4 ) ;
       sum += fjb * TMath::BreitWigner(par4, p4, par5 ) ;

       p4 = p4up - (i-.5) * step;
       fjb = fJacob(x, par0, par1, par2, par3, p4 ) ;
       sum += fjb * TMath::BreitWigner(par4, p4, par5 ) ;
   } 

   return step*sum  ;
}

Double_t MathFunctions::ConvJacob(Double_t *x, Double_t *par) {

   Double_t np   = 200 ;
   Double_t sg   = 3.0 ;
   Double_t width = 1 ;
   //if ( x[0] >= 0 ) width = 4 + par[6]*sqrt(x[0] );
   if ( x[0] >= 0 ) width = par[6];
   Double_t xlow = x[0] - width*sg ;
   Double_t xup  = x[0] + width*sg ;
   Double_t step = (xup - xlow) / np ;
   Double_t sum = 0. ;
   Double_t xx, fjb; 

   for (double i=1.; i<= np/2; i++  ) {
       xx = xlow + (i-.5) * step;
       fjb = fBWJacob(xx,par[0], par[1], par[2], par[3], par[4], par[5] ) ;
       //fjb = fJacob(xx,par[0], par[1], par[2], par[3], par[4] )*par[5] ;
       sum += fjb * TMath::Landau(x[0], xx, width, kTRUE );
       //sum += fjb * TMath::Gaus(x[0], xx, width, kTRUE )  ;

       xx = xup - (i-.5) * step;
       fjb = fBWJacob(xx,par[0], par[1], par[2] , par[3], par[4], par[5] ) ;
       //fjb = fJacob(xx,par[0], par[1], par[2] , par[3], par[4] )*par[5] ;
       sum += fjb * TMath::Landau(x[0], xx, width, kTRUE ) ;
       //sum += fjb * TMath::Gaus(x[0], xx, width, kTRUE )  ;
   } 

   return step*sum  ;
}

Double_t MathFunctions::fGSJacob(Double_t* x, Double_t* par) {

   Double_t np   = 202 ;
   Double_t nsg  = 3.0 ;
   Double_t p4low = par[4] - nsg*par[5];
   Double_t p4up  = par[4] + nsg*par[5];
   Double_t step = (p4up - p4low) / np ;
   Double_t sum = 0. ;
   Double_t p4, fjb; 

   for (double i=1.; i<= np/2; i++  ) {
       p4 = p4low + (i-.5) * step;
       fjb = fJacob(x[0], par[0], par[1], par[2], par[3], p4 ) ;
       sum += fjb * TMath::Gaus(par[4], p4, par[5], kTRUE ) ;

       p4 = p4up - (i-.5) * step;
       fjb = fJacob(x[0], par[0], par[1], par[2], par[3], p4 ) ;
       sum += fjb * TMath::Gaus(par[4], p4, par[5], kTRUE ) ;
   } 

   return step*sum*par[6]  ;
}

vector<bool> MathFunctions::DataRejection( TF1* fitfunc, Double_t* x, Double_t* y, int N_data ) {

     // calculate sigma of the fit
     double dv=0.;
     for ( int i=0; i< N_data; i++) {
         double expy = fitfunc->Eval( x[i] );
         dv += (y[i] - expy)*(y[i] - expy) ;
     }
     double sigma = sqrt( dv/( (N_data*1.) - 1.) ) ;

     // test each point
     std::vector<bool> rejV ;
     for ( int i=0; i< N_data; i++) {
         double expy = fitfunc->Eval( x[i] );
         double dv1 = fabs( y[i] - expy ) ;
         bool reject = DataRejection( sigma, dv1, N_data);   
         rejV.push_back( reject );
     }

     return rejV;
}

// sigma : sigma of the data set w.r.t mean
// deviation : the deviation of data and mean/prefit value
// N_data : number of data point
bool MathFunctions::DataRejection(double sigma, double deviation, int N_data ) {

    bool reject = false ;
    /// gaussian probability for data point
    double p_gaus = 0.0;
    double k = 0.0;
    for (int i=0; i != 10000; i++ ) {
        k += ( deviation*0.0001) ;
        double n1 = 1.0/ (sigma*sqrt(2.0*3.14159)) ;
        double x2 = (-1.0*k*k)/(2.0*sigma*sigma) ;
        double gaus1 = n1*exp(x2);
        p_gaus += (gaus1*deviation*0.0001);
    }

    /// expected number outside the deviation of the distribution
    double nExpected = (1.0-(p_gaus*2.0))*(N_data*1.0);

    if ( nExpected < 0.99 ) reject = true;

    return reject;
}

vector<double>  MathFunctions::StatErr( double m ){

  vector<double> pErr ;
  if ( m < 1. ) {
     pErr.push_back( -1*m ) ;
     pErr.push_back( m ) ;
  }
  else if ( m > 25. ) {
     pErr.push_back( -1*sqrt(m) ) ;
     pErr.push_back( sqrt(m) ) ;
  }
  else {

     double step = 0.01 ;

     // -34%
     double k = m ;
     double lm = 0. ;
     double pp = 0. ;
     while (lm <= 0.34 || k < 0 ) {
          k = k - step ;
          pp = TMath::Poisson( k, m );
          lm = lm + (pp*step) ;
     }
     // +34%
     double j = m ;
     double hm = 0 ;
     double hp = 0 ;
     while ( hm <=0.34 || j < 0 ) {
           j = j + step ;
           hp = TMath::Poisson( j, m );
           hm = hm + (hp*step) ;
           //cout<<" j = "<< j <<" , p = "<< hp <<" int_P = "<< hm <<endl;
     }
     pErr.push_back( k - m ); // downward fluctuation
     pErr.push_back( j - m ); // upward fluctuation
  }
  return pErr ;

}
/*
vector<double>  MathFunctions::ErrAovB( double A, double s_A, double B, double s_B ){

    vector<double> sA = StatErr( A ) ;
    double sAp = ( s_A != -1 ) ? s_A : sA[1];
    double sAn = ( s_A != -1 ) ? s_A : -1*sA[0];
    vector<double> sB = StatErr( B ) ;
    double sBp = ( s_B != -1 ) ? s_B : sB[1];
    double sBn = ( s_B != -1 ) ? s_B : -1*sB[0];

    double f = A / B ;
    double s_fp = sqrt( sAp*sAp + f*f*sBp*sBp ) / B ;
    double s_fn = sqrt( sAn*sAn + f*f*sBn*sBn ) / B ;

    vector<double> sf ;
    sf.push_back( s_fn );
    sf.push_back( s_fp );
    return sf ;

}
*/
double MathFunctions::ErrAovB( double A, double B, double s_A, double s_B, bool upward ){

    vector<double> sA = StatErr( A ) ;
    double sAp = ( s_A != -1 ) ? s_A : sA[1];
    double sAn = ( s_A != -1 ) ? s_A : -1*sA[0];
    vector<double> sB = StatErr( B ) ;
    double sBp = ( s_B != -1 ) ? s_B : sB[1];
    double sBn = ( s_B != -1 ) ? s_B : -1*sB[0];

    double f = A / B ;
    double s_fp = sqrt( (sAp*sAp) + (f*f*sBp*sBp) ) / B ;
    double s_fn = sqrt( (sAn*sAn) + (f*f*sBn*sBn) ) / B ;

    double sf = ( upward ) ? s_fp : s_fn ;
    return sf ;

}

double MathFunctions::ErrAxB( double A, double B, double s_A, double s_B, bool upward ){

    vector<double> sA = StatErr( A ) ;
    double sAp = ( s_A != -1 ) ? s_A : sA[1];
    double sAn = ( s_A != -1 ) ? s_A : -1*sA[0];
    vector<double> sB = StatErr( B ) ;
    double sBp = ( s_B != -1 ) ? s_B : sB[1];
    double sBn = ( s_B != -1 ) ? s_B : -1*sB[0];

    //double f = A * B ;
    double s_fp = sqrt( B*B*sAp*sAp + A*A*sBp*sBp ) ;
    double s_fn = sqrt( B*B*sAn*sAn + A*A*sBn*sBn ) ;

    double sf = ( upward ) ? s_fp : s_fn ;
    return sf ;

}

// My Lil fxns
/*** 2D Gaus Fit ***/
Double_t MathFunctions::gauss2D(Double_t *x, Double_t *par) 
  {
	Double_t z1 = 0;
	if(par[2] != 0) z1 = Double_t((x[0]-par[1])/par[2]);
	Double_t z2 = 0;
	if(par[4] != 0 ) z2 = Double_t((x[1]-par[3])/par[4]);          
  
    return par[0]*exp(-0.5*(z1*z1+z2*z2));
  } 

/**private Defined  gaus fxn **/
Double_t MathFunctions::mygauss(Double_t *t, Double_t *par)

  {
	Double_t x = t[0];
	Double_t arg0 = par[0];
	Double_t arg1 = 0;
	if(par[2] != 0) arg1 = Double_t( ( x - par[1])/par[2] );
	Double_t  f   = arg0*TMath::Exp(-0.5*arg1*arg1);
	
    return f;
 }


