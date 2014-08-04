#include "TF1.h"
#include "Math/WrappedTF1.h"
#include "Math/GaussIntegrator.h"
 
int NumericalIntegration()
{
 
   TF1 f("Sin Function", "sin(x)", 0, TMath::Pi());
 
   ROOT::Math::WrappedTF1 wf1(f);
 
   ROOT::Math::GaussIntegrator ig;
 
   ig.SetFunction(wf1, false);
   ig.SetRelTolerance(0.001);
 
   cout << ig.Integral(0, TMath::PiOver2()) << endl;
 
   return 0;
}


