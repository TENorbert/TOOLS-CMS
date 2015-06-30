# This Simple Code Overlays Histograms Plots from Different Files
# Cannot DO IT with C++ because It root chokes especially when Number bins are
# not the same.
## Aurthor Tambe E. Norbert a.k.a TEN (norbert@physics.umn.edu)
#!/usr/bin/env python
""" OverLay Myhist.png from several Directories identical structure!"""
__version__ = "1.0"
import sys
import optparse
import shutil
import os
import re
from ROOT import *
from array import array

Lowlim  = -3 
Highlim =  3 

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
gStyle.SetTitleOffset(1.8,"y")
gStyle.SetPadTopMargin(0.1)
gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.16)
gStyle.SetPadLeftMargin(0.12)
TGaxis.SetMaxDigits(2) #// Set Axis to be of the form 0.11 10^N

# Get Files!
#---------------MET60Cut---------------------------
f1 = TFile("TimePerf-plots.root")
# Gte Historgrams
#s_time = f1.Get("EBEB/seed time")  ##  TOF-Corr time difference of seeds
#A_time = f1.Get("EBEB/cluster time") ## TOF-Corr cluster time difference
s_time = f1.Get("EBEB/TOF-corr time difference of seeds")
A_time = f1.Get("EBEB/TOF-corr cluster time difference")
# Choose Line Colors
s_time.SetLineColor(6)
A_time.SetLineColor(9)
## Choose Line Widths
s_time.SetLineWidth(4)
A_time.SetLineWidth(4)
# Choose Fill Colors
#s_time.SetFillColor(6)
#A_time.SetFillColor(9)

#Choose Marker Style!
s_time.SetMarkerStyle(6)
A_time.SetMarkerStyle(9)
# Scale Histograms
if s_time.Integral()!=0:
   s_time.Scale(1/s_time.Integral())
if A_time.Integral()!=0:
   A_time.Scale(1/A_time.Integral())
A_time.GetXaxis().SetTitle("ECAL Time(ns)")
A_time.GetYaxis().SetTitle("Events")






# Draw Plots now
# *************************************************************************************
c1 = TCanvas("c1", "ECAL Time", 800, 800)
c1.SetGridx();
c1.SetGridy();
c1.GetFrame().SetFillColor(10);
c1.GetFrame().SetBorderMode(-1);
c1.GetFrame().SetBorderSize(5);
c1.cd()


A_time.SetTitle("ECAL Time")
A_time.Draw()
s_time.Draw("sames")
A_time.GetXaxis().SetRangeUser(Lowlim, Highlim)
#Add Legend
leg1 = TLegend( 0.60, 0.75, 0.90, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(A_time,"Average Time(#sigma=336ps) ","l")
leg1.AddEntry(s_time,"Seed Time(#sigma=345ps)","l")
leg1.SetTextSize(0.020)
leg1.SetHeader("")
leg1.Draw()

## CMS Things
#s_lumi = "19.1"
s_lumi = ""
#CoMEr = "#sqrt{(S)} = 8 TeV"
CoMEr = "#sqrt{s} = 8 TeV"
#std::string lint = "#int Ldt= "+s_lumi+" fb^{-1}"
#std::string lint = s_lumi + "fb^{-1}"+ CoMEr
lint = s_lumi + CoMEr
cmstex = TLatex()
cmstex.SetTextAlign(12)
cmstex.SetTextSize(0.035)
cmstex.SetTextFont(22)
cmstex.SetTextAngle(0)
cmstex.SetTextColor(kBlack)
cmstex.SetNDC()
#cmstex.DrawLatex(0.155, 0.967, "CMS Preliminary");
#cmstex.DrawLatex(0.43, 0.967, CoMEr.c_str());
#cmstex.DrawLatex(0.75, 0.96, lint.c_str());
cmstex.DrawLatex(0.20, 0.865, "CMS");
cmstex.DrawLatex(0.20, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);
#_______________________ Save Plots_______________________#
#c1.SaveAs("PhotonTimeComparing.png")
c1.SetLogy(0)
c1.SaveAs("/home/user1/norbert/public_html/ECALTiming/ZeeTimePerformance/ECAL-TIME-2013-2014/ECAL-Time-Zee.png")
c1.SaveAs("/home/user1/norbert/public_html/ECALTiming/ZeeTimePerformance/ECAL-TIME-2013-2014/ECAL-Time-Zee.pdf")
c1.SaveAs("/home/user1/norbert/public_html/ECALTiming/ZeeTimePerformance/ECAL-TIME-2013-2014/ECAL-Time-Zee.eps")
