# This Simple Code Overlays Histograms Plots  from Different  Jet Multiplicity Files
# Cannot DO IT with C++ because It root chokes especially when Number bins are 
# not the same.
## Aurthor Tambe E. Norbert a.k.a TEN (norbert@physics.umn.edu)

#!/usr/bin/env python
""" OverLay Myhist.png from several  Directories identical structure!"""
__version__ = "1.0"


import sys
import optparse
import shutil
import os
import re
from ROOT import *
from ROOT import TFile, TH1D, TLegend, TCanvas
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
gStyle.SetPadGridX(1);
gStyle.SetPadGridY(1);

# Get Files!
#---------------MET60Cut---------------------------

##----------------No--Met60Cut-------------------------
f0 =  TFile("./NoCut_JetTiming_DPTrigger_Displaced_Photon_outputfile.root")
f1 =  TFile("./1GeV_Cut_JetTiming_DPTrigger_Displaced_Photon_outputfile.root")
f2 =  TFile("./3GeV_Cut_JetTiming_DPTrigger_Displaced_Photon_outputfile.root")
f3 =  TFile("./5GeV_Cut_JetTiming_DPTrigger_Displaced_Photon_outputfile.root")
f4 =  TFile("./7GeV_Cut_JetTiming_DPTrigger_Displaced_Photon_outputfile.root")
f5 =  TFile("./10GeV_Cut_JetTiming_DPTrigger_Displaced_Photon_outputfile.root")


# my.JetseedTime1
jseedTime10 = f0.Get("EEUntagable/my.Jseedtime1")
jseedTime11 = f1.Get("EEUntagable/my.Jseedtime1")
jseedTime12 = f2.Get("EEUntagable/my.Jseedtime1") 
jseedTime13 = f3.Get("EEUntagable/my.Jseedtime1") 
jseedTime14 = f4.Get("EEUntagable/my.Jseedtime1") 
jseedTime15 = f5.Get("EEUntagable/my.Jseedtime1") 

# my.JetseedTime2
jseedTime20 = f0.Get("EEUntagable/my.Jseedtime2")
jseedTime21 = f1.Get("EEUntagable/my.Jseedtime2")
jseedTime22 = f2.Get("EEUntagable/my.Jseedtime2")
jseedTime23 = f3.Get("EEUntagable/my.Jseedtime2")
jseedTime24 = f4.Get("EEUntagable/my.Jseedtime2")
jseedTime25 = f5.Get("EEUntagable/my.Jseedtime2")


# my W.Ave SC photime Time
jseedBCWaveTime0 = f0.Get("EEUntagable/my.JseedBCtime")
jseedBCWaveTime1 = f1.Get("EEUntagable/my.JseedBCtime")
jseedBCWaveTime2 = f2.Get("EEUntagable/my.JseedBCtime")
jseedBCWaveTime3 = f3.Get("EEUntagable/my.JseedBCtime")
jseedBCWaveTime4 = f4.Get("EEUntagable/my.JseedBCtime")
jseedBCWaveTime5 = f5.Get("EEUntagable/my.JseedBCtime")

# For Seed Time
# Choose Line Colors
jseedTime10.SetLineColor(2)
jseedTime11.SetLineColor(8)
jseedTime12.SetLineColor(4)
jseedTime13.SetLineColor(6)
jseedTime14.SetLineColor(28)
jseedTime15.SetLineColor(1)

# Choose Line Style
jseedTime10.SetLineStyle(1)
jseedTime11.SetLineStyle(7)
jseedTime12.SetLineStyle(1)
jseedTime13.SetLineStyle(1)
jseedTime14.SetLineStyle(1)
jseedTime15.SetLineStyle(9)


#Choose LineWidth
jseedTime10.SetLineWidth(1)
jseedTime11.SetLineWidth(1)
jseedTime12.SetLineWidth(1)
jseedTime13.SetLineWidth(1)
jseedTime14.SetLineWidth(1)
jseedTime15.SetLineWidth(1)

#Choose Marker Style!
#jseedTime10.SetMarkerStyle(2)
#jseedTime11.SetMarkerStyle(3)
#jseedTime12.SetMarkerStyle(4)
#jseedTime13.SetMarkerStyle(23)
#jseedTime14.SetMarkerStyle(33)
#jseedTime15.SetMarkerStyle(34)

# Scale Histograms
if jseedTime10.Integral()!=0:
                   jseedTime10.Scale(1/jseedTime10.Integral())

if jseedTime11.Integral()!=0:
                   jseedTime11.Scale(1/jseedTime11.Integral())

if jseedTime12.Integral()!=0:
                   jseedTime12.Scale(1/jseedTime12.Integral())

if jseedTime13.Integral()!=0:
                   jseedTime13.Scale(1/jseedTime13.Integral())

if jseedTime14.Integral()!=0:
                   jseedTime14.Scale(1/jseedTime14.Integral())

if jseedTime15.Integral()!=0:
                   jseedTime15.Scale(1/jseedTime15.Integral())


# Label Axis
jseedTime10.GetXaxis().SetTitle("Seed Crys Time(ns)")
jseedTime10.GetYaxis().SetTitle("Events/0.5ns")


# For JetseedTime2
# Choose Line Colors
jseedTime20.SetLineColor(2)
jseedTime21.SetLineColor(8)
jseedTime22.SetLineColor(4)
jseedTime23.SetLineColor(6)
jseedTime24.SetLineColor(28)
jseedTime25.SetLineColor(1)

# Choose Line Style
jseedTime20.SetLineStyle(1)
jseedTime21.SetLineStyle(7)
jseedTime22.SetLineStyle(1)
jseedTime23.SetLineStyle(1)
jseedTime24.SetLineStyle(1)
jseedTime25.SetLineStyle(9)


#Choose LineWidth
jseedTime20.SetLineWidth(1)
jseedTime21.SetLineWidth(1)
jseedTime22.SetLineWidth(1)
jseedTime23.SetLineWidth(1)
jseedTime24.SetLineWidth(1)
jseedTime25.SetLineWidth(1)


#Choose Marker Style!
#jseedTime20.SetMarkerStyle(2)
#jseedTime21.SetMarkerStyle(3)
#jseedTime22.SetMarkerStyle(4)
#jseedTime23.SetMarkerStyle(23)
#jseedTime24.SetMarkerStyle(33)
#jseedTime25.SetMarkerStyle(34)

# Scale Histograms
if jseedTime20.Integral()!=0:
                   jseedTime20.Scale(1/jseedTime20.Integral())

if jseedTime21.Integral()!=0:
                   jseedTime21.Scale(1/jseedTime21.Integral())

if jseedTime22.Integral()!=0:
                   jseedTime22.Scale(1/jseedTime22.Integral())

if jseedTime23.Integral()!=0:
                   jseedTime23.Scale(1/jseedTime23.Integral())

if jseedTime24.Integral()!=0:
                   jseedTime24.Scale(1/jseedTime24.Integral())

if jseedTime25.Integral()!=0:
                   jseedTime25.Scale(1/jseedTime25.Integral())


# Label Axis
jseedTime20.GetXaxis().SetTitle("Seed Crys Time(ns)")
jseedTime20.GetYaxis().SetTitle("Events/0.5ns")


# For JetseedBC W.Ave time
# Choose Line Colors
jseedBCWaveTime0.SetLineColor(2)
jseedBCWaveTime1.SetLineColor(8)
jseedBCWaveTime2.SetLineColor(4)
jseedBCWaveTime3.SetLineColor(6)
jseedBCWaveTime4.SetLineColor(28)
jseedBCWaveTime5.SetLineColor(1)

# Choose Line Style
jseedBCWaveTime0.SetLineStyle(1)
jseedBCWaveTime1.SetLineStyle(7)
jseedBCWaveTime2.SetLineStyle(1)
jseedBCWaveTime3.SetLineStyle(1)
jseedBCWaveTime4.SetLineStyle(1)
jseedBCWaveTime5.SetLineStyle(9)


#Choose LineWidth
jseedBCWaveTime0.SetLineWidth(1)
jseedBCWaveTime1.SetLineWidth(1)
jseedBCWaveTime2.SetLineWidth(1)
jseedBCWaveTime3.SetLineWidth(1)
jseedBCWaveTime4.SetLineWidth(1)
jseedBCWaveTime5.SetLineWidth(1)


#Choose Marker Style!
#jseedBCWaveTime0.SetMarkerStyle(2)
#jseedBCWaveTime1.SetMarkerStyle(3)
#jseedBCWaveTime2.SetMarkerStyle(4)
#jseedBCWaveTime3.SetMarkerStyle(23)
#jseedBCWaveTime4.SetMarkerStyle(33)
#jseedBCWaveTime5.SetMarkerStyle(34)



# Scale Histograms
if jseedBCWaveTime0.Integral()!=0:
                   jseedBCWaveTime0.Scale(1/jseedBCWaveTime0.Integral())

if jseedBCWaveTime1.Integral()!=0:
                   jseedBCWaveTime1.Scale(1/jseedBCWaveTime1.Integral())

if jseedBCWaveTime2.Integral()!=0:
                   jseedBCWaveTime2.Scale(1/jseedBCWaveTime2.Integral())

if jseedBCWaveTime3.Integral()!=0:
                   jseedBCWaveTime3.Scale(1/jseedBCWaveTime3.Integral())

if jseedBCWaveTime4.Integral()!=0:
                   jseedBCWaveTime4.Scale(1/jseedBCWaveTime4.Integral())

if jseedBCWaveTime5.Integral()!=0:
                   jseedBCWaveTime5.Scale(1/jseedBCWaveTime5.Integral())


# Label Axis
jseedBCWaveTime0.GetXaxis().SetTitle("W.Ave seed BC Time(ns)")
jseedBCWaveTime0.GetYaxis().SetTitle("Events/0.5ns")


# Draw Plots now
# ************************************************************************************
# Seed Xtal
c1 = TCanvas("c1", "Jet_Seed_Time_Cleaned", 800, 800)
c1.cd()

jseedTime10.SetTitle("PFJet Time")

jseedTime10.Draw()
jseedTime11.Draw("sames")
jseedTime12.Draw("sames")
jseedTime13.Draw("sames")
jseedTime14.Draw("sames")
jseedTime15.Draw("sames")
jseedTime10.GetXaxis().SetRangeUser(-25.0, 25.0)
#Add Legend
leg1 = TLegend( 0.14, 0.70, 0.45, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(jseedTime10,"NO E_{T} CUT","l")
leg1.AddEntry(jseedTime11,"E_{T} > 1 GeV","l")
leg1.AddEntry(jseedTime12,"E_{T} > 3 GeV","l")
leg1.AddEntry(jseedTime13,"E_{T} > 5 GeV","l")
leg1.AddEntry(jseedTime14,"E_{T} > 7 GeV","l")
leg1.AddEntry(jseedTime15,"E_{T} > 10 GeV","l")
leg1.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg1.SetTextSize(0.02)
leg1.Draw()

#  JetseedTime Uncleaned
c2 = TCanvas("c2", "Jet_Seed_Uncleaned_Time", 800, 800)
c2.cd()

jseedTime20.SetTitle("PFJet Time Uncleaned")
jseedTime20.Draw()
jseedTime21.Draw("sames")
jseedTime22.Draw("sames")
jseedTime23.Draw("sames")
jseedTime24.Draw("sames")
jseedTime25.Draw("sames")
jseedTime20.GetXaxis().SetRangeUser(-25.0, 25.0)
#Add Legend
leg2 = TLegend( 0.14, 0.70, 0.45, 0.90)
leg2.SetBorderSize(1)
leg2.SetFillColor(0)
leg2.AddEntry(jseedTime20,"NO E_{T} CUT","l")
leg2.AddEntry(jseedTime21,"E_{T} > 1 GeV","l")
leg2.AddEntry(jseedTime22,"E_{T} > 3 GeV","l")
leg2.AddEntry(jseedTime23,"E_{T} > 5 GeV","l")
leg2.AddEntry(jseedTime24,"E_{T} > 7 GeV","l")
leg2.AddEntry(jseedTime25,"E_{T} > 10 GeV","l")
leg2.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg2.SetTextSize(0.02)
leg2.Draw()

# SC Time
c3 = TCanvas("c3", "Jet_Wave_seedBC_Time", 800, 800)
c3.cd()

jseedBCWaveTime0.SetTitle("PFJet(W.Ave SBC)Time")
jseedBCWaveTime0.Draw()
jseedBCWaveTime1.Draw("sames")
jseedBCWaveTime2.Draw("sames")
jseedBCWaveTime3.Draw("sames")
jseedBCWaveTime4.Draw("sames")
jseedBCWaveTime5.Draw("sames")
jseedBCWaveTime0.GetXaxis().SetRangeUser(-25.0, 25.0)
#Add Legend
leg3 = TLegend( 0.14, 0.70, 0.45, 0.90)
leg3.SetBorderSize(1)
leg3.SetFillColor(0)
leg3.AddEntry(jseedBCWaveTime0,"NO E_{T} CUT","l")
leg3.AddEntry(jseedBCWaveTime1,"E_{T} > 1 GeV","l")
leg3.AddEntry(jseedBCWaveTime2,"E_{T} > 3 GeV","l")
leg3.AddEntry(jseedBCWaveTime3,"E_{T} > 5 GeV","l")
leg3.AddEntry(jseedBCWaveTime4,"E_{T} > 7 GeV","l")
leg3.AddEntry(jseedBCWaveTime5,"E_{T} > 10 GeV","l")
leg3.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg3.SetTextSize(0.02)
leg3.Draw()


  
#_______________________ Save Plots_______________________#

c1.SetLogy(1)
c1.SaveAs("JetSeedTimeCleanDistribution.png")

c2.SetLogy(1)
c2.SaveAs("JetSeedTimeUncleanedDistrubution.png")

c3.SetLogy(1)
c3.SaveAs("JetSeedBCW.AveUncleanTime.png")



















