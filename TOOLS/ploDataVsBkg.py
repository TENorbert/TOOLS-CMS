#!/usr/bin/env python
"""plotDataVsBkg_Stack!"""
__version__ = "1.0"
#
#  Simple Script to plot data amd background stacked!
#  Creator: Tambe E. Norbert  TEN@UMN.EDU 
# To Run do: python plotDataVsBkg.py


import sys
import optparse
import shutil
import os
import re
from ROOT import *
from ROOT import TFile, TH1D, TLegend, TCanvas,THStack
from array import array

# ROOT general options
gROOT.SetBatch(kTRUE)
gROOT.SetStyle("Plain")
gStyle.SetCanvasColor(kWhite)  #   // background is no longer mouse-dropping white
gStyle.SetPalette(1) #           // blue to red false color palette. Use 9 for b/w
gStyle.SetCanvasBorderMode(0) #     // turn off canvas borders
gStyle.SetPadBorderMode(0)
gStyle.SetPaintTextFormat("5.2f") #  // What precision to put numbers if plotted with "TEXT"

#  // For publishing:
gStyle.SetLineWidth(2)
gStyle.SetTextSize(1.1)
gStyle.SetLabelSize(0.06,"xy")
gStyle.SetTitleSize(0.06,"xy")
gStyle.SetTitleOffset(1.2,"x")
gStyle.SetTitleOffset(1.05,"y")
#gStyle.SetPadTopMargin(0.1)
#gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.16)
gStyle.SetPadLeftMargin(0.12)
gStyle.SetOptStat(0)
gStyle.SetPadGridX(1)
gStyle.SetPadGridY(1)

#Get inut files 
If = TFile("hStats_v3.root")
#Get Histograms
#Time
h_data_t   = If.Get("h_dataTime") 
h_bkg_t    = If.Get("h_bgTime") 
h_sig250_t = If.Get("h_sgTime_250") 
h_sig500_t = If.Get("h_sgTime_500")
h_sig1000_t =If.Get("h_sgTime_1000") 
h_sig2000_t =If.Get("h_sgTime_2000") 
h_sig3000_t =If.Get("h_sgTime_3000") 
h_sig4000_t =If.Get("h_sgTime_4000") 
h_sig6000_t =If.Get("h_sgTime_6000")

# MET
h_data_met    =If.Get("h_MET") 
h_bkg_met     =If.Get("h_bgMET") 
h_sig250_met  =If.Get("h_sgMET_250") 
h_sig500_met  =If.Get("h_sgMET_500") 
h_sig1000_met =If.Get("h_sgMET_1000") 
h_sig2000_met =If.Get("h_sgMET_2000") 
h_sig3000_met =If.Get("h_sgMET_3000") 
h_sig4000_met =If.Get("h_sgMET_4000") 
h_sig6000_met =If.Get("h_sgMET_6000")
   
# Njets
h_data_njet    = If.Get("h_NJets") 
h_bkg_njet     = If.Get("h_NJets")  # can't find bkg Njets
h_sig250_njet  = If.Get("h_sgNJets_250") 
h_sig500_njet  = If.Get("h_sgNJets_500") 
h_sig1000_njet = If.Get("h_sgNJets_1000") 
h_sig2000_njet = If.Get("h_sgNJets_2000") 
h_sig3000_njet = If.Get("h_sgNJets_3000") 
h_sig4000_njet = If.Get("h_sgNJets_4000") 
h_sig6000_njet = If.Get("h_sgNJets_6000")

# Stack Timing Plots 
hstack_t = THStack("hstack_t","Stacked Time Histograms")
hstack_t.Add(h_bkg_t)
hstack_t.Add(h_sig250_t)
hstack_t.Add(h_sig500_t)
hstack_t.Add(h_sig1000_t)
hstack_t.Add(h_sig2000_t)
hstack_t.Add(h_sig3000_t)
hstack_t.Add(h_sig4000_t)
hstack_t.Add(h_sig6000_t)

# Legend For Time
legt = TLegend(0.60, 0.70, 0.90, 0.90)
legt.SetBorderSize(0)
legt.SetFillColor(0)
legt.SetFillStyle(0)
legt.SetTextFont(42)
legt.AddEntry(h_data_t,"Data", "P")
legt.AddEntry(h_bkg_t, "BEAM HALO", "F")
legt.AddEntry(h_sig250_t, "c#tau=250 mm", "l")
legt.AddEntry(h_sig500_t, "c#tau=500 mm", "l")
legt.AddEntry(h_sig1000_t, "c#tau=1000 mm", "l")
legt.AddEntry(h_sig2000_t, "c#tau=2000 mm", "l")
legt.AddEntry(h_sig3000_t, "c#tau=3000 mm", "l")
legt.AddEntry(h_sig4000_t, "c#tau=4000 mm", "l")
legt.AddEntry(h_sig6000_t, "c#tau=6000 mm", "l")
legt.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV")
legt.SetTextSize(0.02)

# TCanvas for Time  
ct = TCanvas("ct","Time Distribution", 800, 800)
ct.cd()
hstack_t.Draw("HIST")
h_data_t.Draw("PE1same")
legt.Draw("same")
ct.SetLogy()
gPad.RedrawAxis()

# Stack MET Plots 
hstack_met = THStack("hstack_met","Stacked MET Histograms")
hstack_met.Add(h_bkg_met)
hstack_met.Add(h_sig250_met)
hstack_met.Add(h_sig500_met)
hstack_met.Add(h_sig1000_met)
hstack_met.Add(h_sig2000_met)
hstack_met.Add(h_sig3000_met)
hstack_met.Add(h_sig4000_met)
hstack_met.Add(h_sig6000_met)
  
#  Legend For MET 
legmet = TLegend(0.60, 0.70, 0.90, 0.90)
legmet.SetBorderSize(0)
legmet.SetFillColor(0)
legmet.SetFillStyle(0)
legmet.SetTextFont(42)
legmet.AddEntry(h_data_met,"Data", "P")
legmet.AddEntry(h_bkg_met, "BEAM HALO", "F")
legmet.AddEntry(h_sig250_met, "c#tau=250 mm", "l")
legmet.AddEntry(h_sig500_met, "c#tau=500 mm", "l")
legmet.AddEntry(h_sig1000_met, "c#tau=1000 mm", "l")
legmet.AddEntry(h_sig2000_met, "c#tau=2000 mm", "l")
legmet.AddEntry(h_sig3000_met, "c#tau=3000 mm", "l")
legmet.AddEntry(h_sig4000_met, "c#tau=4000 mm", "l")
legmet.AddEntry(h_sig6000_met, "c#tau=6000 mm", "l")
legmet.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV")
legmet.SetTextSize(0.02)
# TCanvas for MET 
cmet = TCanvas("cmet","MET Distribution", 800, 800)
cmet.cd()
hstack_met.Draw("HIST")
h_data_met.Draw("PE1same")
legmet.Draw("same")
cmet.SetLogy()
gPad.RedrawAxis()

#Stack Njets Plots
hstack_njet = THStack("hstack_njet","Stacked Njets_Histograms")
hstack_njet.Add(h_data_njet)
hstack_njet.Add(h_bkg_njet)
hstack_njet.Add(h_sig250_njet)
hstack_njet.Add(h_sig500_njet)
hstack_njet.Add(h_sig1000_njet)
hstack_njet.Add(h_sig2000_njet)
hstack_njet.Add(h_sig3000_njet)
hstack_njet.Add(h_sig4000_njet)
hstack_njet.Add(h_sig6000_njet)

# Legend For Njets
legnjet = TLegend(0.60, 0.70, 0.90, 0.90)
legnjet.SetBorderSize(0)
legnjet.SetFillColor(0)
legnjet.SetFillStyle(0)
legnjet.SetTextFont(42)
legnjet.AddEntry(h_data_njet,"Data", "P")
legnjet.AddEntry(h_bkg_njet, "BEAM HALO", "F")
legnjet.AddEntry(h_sig250_njet, "c#tau=250 mm", "l")
legnjet.AddEntry(h_sig500_njet, "c#tau=500 mm", "l")
legnjet.AddEntry(h_sig1000_njet, "c#tau=1000 mm", "l")
legnjet.AddEntry(h_sig2000_njet, "c#tau=2000 mm", "l")
legnjet.AddEntry(h_sig3000_njet, "c#tau=3000 mm", "l")
legnjet.AddEntry(h_sig4000_njet, "c#tau=4000 mm", "l")
legnjet.AddEntry(h_sig6000_njet, "c#tau=6000 mm", "l")
legnjet.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV")
legnjet.SetTextSize(0.02)

# TCanvas for Time  
cnjet = TCanvas("cnjet","Time Distribution", 800, 800);
cnjet.cd()
hstack_njet.Draw("HIST")
h_data_njet.Draw("PE1same")
legnjet.Draw("same")
cnjet.SetLogy()
gPad.RedrawAxis()

#................. Save Histograms ................

cmet.SaveAs("MET_ObservedDataVsExpectedBkgWithSignal.png")
cmet.SaveAs("MET_ObservedDataVsExpectedBkgWithSignal.pdf")

ct.SaveAs("Time_ObservedDataVsExpectedBkgWithSignal.png")
ct.SaveAs("TimeObservedDataVsExpectedBkgWithSignal.pdf")

cnjet.SaveAs("NJET_ObservedDataVsExpectedBkgWithSignal.png")
cnjet.SaveAs("NJET_ObservedDataVsExpectedBkgWithSignal.pdf")



