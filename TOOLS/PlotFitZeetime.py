#!/usr/bin/env python
# My simple Macro to Fit Z mass,time, and Plots, hists ***/
# Cannot DO IT with C++ because It root chokes especially when Number bins are 
# not the same.
## Aurthor Tambe E. Norbert a.k.a TEN (norbert@physics.umn.edu)

#Run AS python FitPlotZeetime.py 
#root[1] .X ZeetimeFitAndPlot.C
# Designed by 10Sr @2013 // norbe072@umn.edu

""" OverLay Myhist.png from several  Directories identical structure!"""
__version__ = "1.0"


import sys
import optparse
import shutil
import os
import re
from array import array

try:
    # ROOT stuff
    from ROOT import *
    from ROOT import TFile, TTree, TH1F, TH2F, TH3F, gROOT
    # RooFit stuff
    from ROOT import RooAddPdf, RooArgList, RooArgSet, RooDataHist, RooFit, RooHistPdf, RooRealVar
except Exception, e:
    print e
    print ("Use a python that has PyROOT installed.")
    sys.exit(0)

FitLowRange  = -3 
FitHighRange =  3 
# ROOT Plot Options***/	
gROOT.Reset()
gROOT.Clear()
gROOT.SetStyle("Plain")
gROOT.SetBatch(kFALSE)
gStyle.SetOptTitle(0)
gStyle.SetOptStat(0)
gStyle.SetOptFit(1)
gStyle.SetStatX(.89)
gStyle.SetStatY(.89) 
gStyle.SetStatBorderSize(0)
gStyle.SetCanvasColor(kWhite) #   // background is no longer mouse-dropping white
gStyle.SetPalette(1)        #// blue to red false color palette. Use 9 for b/w
gStyle.SetCanvasBorderMode(0) #     // turn off canvas borders
gStyle.SetPadBorderMode(0) #
gStyle.SetPaintTextFormat("5.2f") #  // What precision to put numbers if plotted with "TEXT"
# For publishing
gStyle.SetLineWidth(2)
gStyle.SetTextSize(1.1)
gStyle.SetLabelSize(0.06,"xy")
gStyle.SetTitleSize(0.08,"xy")
gStyle.SetTitleOffset(1.2,"x")
gStyle.SetTitleOffset(1.0,"y")
gStyle.SetPadTopMargin(0.1)
gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.16)
gStyle.SetPadLeftMargin(0.12)
TGaxis.SetMaxDigits(2) #// Set Axis to be of the form 0.11 10^N



##### 2D Gaus Fit ***/
def gauss2D( x, par):
        double z1 = 0
	if(par[2] != 0):
	      z1 = double((x[0]-par[1])/par[2])
	      double z2 = 0
	if(par[4] != 0 ):
	      z2 = double((x[1]-par[3])/par[4]);          
  
        return par[0]*exp(-0.5*(z1*z1+z2*z2))
 

#/**private Defined  gaus fxn **/
def mygaus(t, par){
	double x = t[0]
	double arg0 = par[0]
	double arg1 = 0
	if(par[2] != 0):
	    arg1 =double( ( x - par[1])/par[2] )
	    double  f   = arg0*TMath::Exp(-0.5*arg1*arg1)
	
        return f;



 #Main fxn doing the job
def main():
        
	ifile =  TFile("single_photon_R2012C_TimePerf-plots.root","read") # get input files
        
	fitFcn  = TF1("fitFcn", mygaus, FitLowRange, FitHighRange, 3 )
	fitFcn.SetNpx(500)
	fitFcn.SetLineWidth(4)
	fitFcn.SetLineStyle(5)
	fitFcn.SetLineColor(kBlue)
        
	h_Seed_TimeEBEB = ifile.Get("EBEB/seed time")
	h_Seed_TimeEEEE = ifile.Get("EEEE/seed time")
	h_TofCor_TimeEBEB = ifile.Get("EBEB/TOF-corr time difference of seeds")
	h_TofCor_TimeEEEE = ifile.Get("EEEE/TOF-corr time difference of seeds")
	
	if(h_Seed_TimeEBEB == 0):
	          print "!! Histogram Does not exist!!"
	if(h_Seed_TimeEEEE == 0):
	          print "!! Histogram Does not exist!!" 
	if(h_TofCor_TimeEBEB == 0):
	          print "!! Histogram Does not exist!!"

	if(h_TofCor_TimeEEEE == 0):
	          print "!! Histogram Does not exist!!" 
	
	
	h_Seed_TimeEBEB.SetTitle("Seed Time[ns]") 
        h_Seed_TimeEBEB.SetMarkerStyle(20)
        h_Seed_TimeEBEB.SetMarkerSize(0.8)
        h_Seed_TimeEBEB.SetStats(1)
        h_Seed_TimeEBEB.SetTitleSize(0.08, "x")   
        h_Seed_TimeEBEB.SetTitleOffset(1.0, "x")    
        h_Seed_TimeEBEB.SetTitleSize(0.06, "y")
        h_Seed_TimeEBEB.SetTitleOffset(0.95, "y")    
        h_Seed_TimeEBEB.SetYTitle("Number of Seeds/0.05ns")
        h_Seed_TimeEBEB.SetXTitle("t_{seed}[ns]") 
        h_Seed_TimeEBEB.GetXaxis().SetRangeUser(FitLowRange, FitHighRange)
        h_Seed_TimeEBEB.GetXaxis().SetNoExponent(kFALSE)

        fitFcn.SetParameters(500, h_Seed_TimeEBEB.GetMean(), h_Seed_TimeEBEB.GetRMS() )
	fitFcn.SetParNames("CONST", "#mu(ns)", "#sigma(ns)")
	h_Seed_TimeEBEB.Fit("fitFcn", "LL")
	print "Printing Fit Parameters for EBEB ......   " 
        print "Integral of function in EBEB = %g\n" % fitFcn.Integral( FitLowRange, FitHighRange)

        npar = fitFcn.GetNpar()
        fit2 = TVirtualFitter::GetFitter()
        fit.PrintResults(2,0.)
        CovMatrix = TMatrixD ( npar, npar, fit.GetCovarianceMatrix() )
	CovMatrix.Print()
        c1 = TCanvas("c1","EB-EB",200,10,800,900)
     	c1.SetGridx()
     	c1.SetGridy()
	c1.GetFrame().SetFillColor(21)
	c1.GetFrame().SetBorderMode(-1)
	c1.GetFrame().SetBorderSize(5)
       #  c1.Divide(2,1); 
	c1.cd();
	h_Seed_TimeEBEB.Draw()
	fitFcn.Draw("sames")
        c1.SetLogy(0)

       #  draw the legend
    	leg = TLegend(0.15,0.72,0.3,0.85)
	leg.Clear()
       	leg.SetTextFont(72)
        leg.SetTextSize(0.04)
        leg.AddEntry(h_Seed_TimeEBEB,"EB","lpe")
        leg.AddEntry(fitFcn,"GAUS","l")
        leg.Draw()
	c1.SaveAs("Seed_Time_DoubleElectron_Run2012A-EB-EB.png")
        
	# EE seed
	h_Seed_TimeEEEE.SetTitle("Seed Time[ns]")  
        h_Seed_TimeEEEE.SetMarkerStyle(20)
        h_Seed_TimeEEEE.SetMarkerSize(0.8)
        h_Seed_TimeEEEE.SetStats(1)
        h_Seed_TimeEEEE.SetTitleSize(0.08, "x")  
        h_Seed_TimeEEEE.SetTitleOffset(1.0, "x")    
        h_Seed_TimeEEEE.SetTitleSize(0.06, "y")
        h_Seed_TimeEEEE.SetTitleOffset(0.95, "y")
        h_Seed_TimeEEEE.SetYTitle("Number of Seeds/0.05ns")
        h_Seed_TimeEEEE.SetXTitle("t_{seed}[ns]") 
        h_Seed_TimeEEEE.GetXaxis()->SetRangeUser(FitLowRange, FitHighRange)   
        h_Seed_TimeEEEE.GetXaxis()->SetNoExponent(kFALSE)
	
	fitFcn.SetParameters(500, h_Seed_TimeEEEE.GetMean(), h_Seed_TimeEEEE.GetRMS() )
	fitFcn.SetParNames("CONST", "#mu(ns)", "#sigma(ns)")
	h_Seed_TimeEEEE.Fit("fitFcn", "LL")
	print"Printing Fit Parameters for EEEE ......   "
        print"Integral of function in EEEE = %g\n" % fitFcn.Integral( FitLowRange, FitHighRange))

        npar2 = fitFcn->GetNpar()
        fit2 = TVirtualFitter::GetFitter()
        fit2->PrintResults(2,0.)
        CovMatrix2 = TMatrixD ( npar2, npar2, fit2->GetCovarianceMatrix() )
	CovMatrix2.Print();
        c2 = TCanvas("c2","EE-EE",200,10,800,900)
     	c2.SetGridx()
     	c2.SetGridy()
	c2.GetFrame().SetFillColor(21)
	c2->GetFrame()->SetBorderMode(-1);
	c2->GetFrame()->SetBorderSize(5);
	c2->cd();
	h_Seed_TimeEEEE->Draw();
	fitFcn->Draw("sames");
        c2->SetLogy(0);
    	
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
	fitFcn->SetParameters(500, h_TofCor_TimeEBEB->GetMean(), h_TofCor_TimeEBEB->GetRMS() );
	fitFcn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
	h_TofCor_TimeEBEB->Fit("fitFcn", "LL"); /**Fit with improved LL**/
	std::cout << "Printing Fit Parameters for EBEB-Tof ......   " << std::endl;
        printf("Integral of function in EBEB Tof = %g\n", fitFcn->Integral( FitLowRange, FitHighRange));

        int npar3 = fitFcn->GetNpar();
        TVirtualFitter *fit3 = TVirtualFitter::GetFitter();
        fit3->PrintResults(2,0.);
        TMatrixD *CovMatrix3 = new TMatrixD ( npar3, npar3, fit3->GetCovarianceMatrix() );
	CovMatrix3->Print();
        TCanvas *c3 = new TCanvas("c3","EB-EB TOF-corr",200,10,800,900);
     	c3->SetGridx();
     	c3->SetGridy();
	c3->GetFrame()->SetFillColor(21);
	c3->GetFrame()->SetBorderMode(-1);
	c3->GetFrame()->SetBorderSize(5);
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
	
	fitFcn->SetParameters(500, h_TofCor_TimeEEEE->GetMean(), h_TofCor_TimeEEEE->GetRMS() );
	fitFcn->SetParNames("CONST", "#mu(ns)", "#sigma(ns)");
	h_TofCor_TimeEEEE->Fit("fitFcn", "LL"); /**Fit with improved LL**/
	std::cout << "Printing Fit Parameters for EEEE-Tof ......   " << std::endl;
        printf("Integral of function in EEEE Tof = %g\n", fitFcn->Integral( FitLowRange, FitHighRange));

        int npar4 = fitFcn->GetNpar();
        TVirtualFitter *fit4 = TVirtualFitter::GetFitter();
        fit4->PrintResults(2,0.);
        TMatrixD *CovMatrix4 = new TMatrixD ( npar4, npar4, fit4->GetCovarianceMatrix() );
	CovMatrix4->Print();

        TCanvas *c4 = new TCanvas("c4","EE-EE TOF-corr",200,10,800,900);
     	c4->SetGridx();
     	c4->SetGridy();
	c4->GetFrame()->SetFillColor(21);
	c4->GetFrame()->SetBorderMode(-1);
	c4->GetFrame()->SetBorderSize(5);
	c4->cd();
	h_TofCor_TimeEEEE->Draw();
	fitFcn->Draw("sames");
        c4->SetLogy(0);
    	
	TLegend *leg3 = new TLegend(0.15,0.72,0.3,0.85);
       	leg3->SetTextFont(72);
        leg3->SetTextSize(0.04);
        leg3->AddEntry(h_TofCor_TimeEEEE,"EE","lpe");
        leg3->AddEntry(fitFcn,"GAUS","l");
        leg3->Draw();
	c4->SaveAs("TOF-Corrected-Seed_Time_DoubleElectron_Run2012A-EE-EE.png");
       
        print "Finished! & Saved Canvances" 

if __name__ == "__main__":
     main()

