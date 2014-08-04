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
#------------------------------------------
f0 =  TFile("./CMSSW53X-ROOT-FILES/gmsb_lambda-120_ctau-1000_8TeV.root")
f1 =  TFile("./CMSSW53X-ROOT-FILES/gmsb_lambda-180_ctau-1000_8TeV.root")
f2 =  TFile("./CMSSW53X-ROOT-FILES/gmsb_lambda-180_ctau-2000_8TeV.root")
f3 =  TFile("./CMSSW53X-ROOT-FILES/gmsb_lambda-180_ctau-3000_8TeV.root")
f4 =  TFile("./CMSSW53X-ROOT-FILES/gmsb_lambda-140_ctau-6000_8TeV.root")
f5 =  TFile("./CMSSW53X-ROOT-FILES/gmsb_lambda-180_ctau-6000_8TeV.root")

g0 =  TFile("gmsb_lambda-180_ctau-250_8TeV.root")
g1 =  TFile("gmsb_lambda-180_ctau-500_8TeV.root")
g2 =  TFile("gmsb_lambda-180_ctau-1000_8TeV.root")
g3 =  TFile("gmsb_lambda-180_ctau-2000_8TeV.root")
g4 =  TFile("gmsb_lambda-180_ctau-3000_8TeV.root")
g5 =  TFile("gmsb_lambda-180_ctau-4000_8TeV.root")
g6 =  TFile("gmsb_lambda-180_ctau-6000_8TeV.root")

# ProperLifetime
hist_0 = g0.Get("TestHepMCEvt/TimeNeut")
hist_1 = g1.Get("TestHepMCEvt/TimeNeut")
hist_2 = g2.Get("TestHepMCEvt/TimeNeut") 
hist_3 = g3.Get("TestHepMCEvt/TimeNeut") 
hist_4 = g4.Get("TestHepMCEvt/TimeNeut") 
hist_5 = g5.Get("TestHepMCEvt/TimeNeut") 
histdl_6 = g6.Get("TestHepMCEvt/TimeNeut") 

#Photon Time
hist_6 = f0.Get("TestHepMCEvt/Gamma_Time")
hist_7 = f1.Get("TestHepMCEvt/Gamma_Time")
hist_8 = f2.Get("TestHepMCEvt/Gamma_Time")
hist_9 = f3.Get("TestHepMCEvt/Gamma_Time")
hist_10 = f4.Get("TestHepMCEvt/Gamma_Time")
hist_11 = f5.Get("TestHepMCEvt/Gamma_Time")


#Neutralino Pt
pthist_0 = g0.Get("TestHepMCEvt/PtNeut")
pthist_1 = g1.Get("TestHepMCEvt/PtNeut")
pthist_2 = g2.Get("TestHepMCEvt/PtNeut")
pthist_3 = g3.Get("TestHepMCEvt/PtNeut")
pthist_4 = g4.Get("TestHepMCEvt/PtNeut")
pthist_5 = g5.Get("TestHepMCEvt/PtNeut")
pthist_6 = g6.Get("TestHepMCEvt/PtNeut")

#Photon Pt
phpthist_0 = f0.Get("TestHepMCEvt/Photon_Pt")
phpthist_1 = f1.Get("TestHepMCEvt/Photon_Pt")
phpthist_2 = f2.Get("TestHepMCEvt/Photon_Pt")
phpthist_3 = f3.Get("TestHepMCEvt/Photon_Pt")
phpthist_4 = f4.Get("TestHepMCEvt/Photon_Pt")
phpthist_5 = f5.Get("TestHepMCEvt/Photon_Pt")


#Beta Distribution
betahist_0 = g0.Get("TestHepMCEvt/BetaNeut")
betahist_1 = g1.Get("TestHepMCEvt/BetaNeut")
betahist_2 = g2.Get("TestHepMCEvt/BetaNeut")
betahist_3 = g3.Get("TestHepMCEvt/BetaNeut")
betahist_4 = g4.Get("TestHepMCEvt/BetaNeut")
betahist_5 = g5.Get("TestHepMCEvt/BetaNeut")
betahist_6 = g6.Get("TestHepMCEvt/BetaNeut")


# Choose Line Colors
hist_0.SetLineColor(2)
hist_1.SetLineColor(8)
hist_2.SetLineColor(4)
hist_3.SetLineColor(6)
hist_4.SetLineColor(28)
hist_5.SetLineColor(1)
histdl_6.SetLineColor(3)

# Choose Line Style
hist_0.SetLineStyle(1)
hist_1.SetLineStyle(7)
hist_2.SetLineStyle(1)
hist_3.SetLineStyle(1)
hist_4.SetLineStyle(1)
hist_5.SetLineStyle(9)
histdl_6.SetLineStyle(1)


#Choose LineWidth
hist_0.SetLineWidth(4)
hist_1.SetLineWidth(4)
hist_2.SetLineWidth(4)
hist_3.SetLineWidth(4)
hist_4.SetLineWidth(4)
hist_5.SetLineWidth(4)
histdl_6.SetLineWidth(4)

#Choose Marker Style!
#hist_0.SetMarkerStyle(2)
#hist_1.SetMarkerStyle(3)
#hist_2.SetMarkerStyle(4)
#hist_3.SetMarkerStyle(23)
#hist_4.SetMarkerStyle(33)
#hist_5.SetMarkerStyle(34)
#histdl_6.SetMarkerStyle(34)

# Scale Histograms
#if hist_0.Integral()!=0:
#                   hist_0.Scale(1/hist_0.Integral())

#if hist_1.Integral()!=0:
#                   hist_1.Scale(1/hist_1.Integral())

#if hist_2.Integral()!=0:
#                   hist_2.Scale(1/hist_2.Integral())

#if hist_3.Integral()!=0:
#                   hist_3.Scale(1/hist_3.Integral())

#if hist_4.Integral()!=0:
#                   hist_4.Scale(1/hist_4.Integral())

#if hist_5.Integral()!=0:
#                   hist_5.Scale(1/hist_5.Integral())
#if histdl_6.Integral()!=0:
#                   histdl_6.Scale(1/hist_5.Integral())


# Label Axis
histdl_6.GetXaxis().SetTitle("c#tau_{#tilde{#chi}^{0}_{1}}[mm]")
histdl_6.GetYaxis().SetTitle("Events")


# For JetseedTime2
# Choose Line Colors
hist_6.SetLineColor(2)
hist_7.SetLineColor(8)
hist_8.SetLineColor(4)
hist_9.SetLineColor(6)
hist_10.SetLineColor(28)
hist_11.SetLineColor(1)

# Choose Line Style
hist_6.SetLineStyle(1)
hist_7.SetLineStyle(7)
hist_8.SetLineStyle(1)
hist_9.SetLineStyle(1)
hist_10.SetLineStyle(1)
hist_11.SetLineStyle(9)


#Choose LineWidth
hist_6.SetLineWidth(4)
hist_7.SetLineWidth(4)
hist_8.SetLineWidth(4)
hist_9.SetLineWidth(4)
hist_10.SetLineWidth(4)
hist_11.SetLineWidth(4)


#Choose Marker Style!
#hist_6.SetMarkerStyle(2)
#hist_7.SetMarkerStyle(3)
#hist_8.SetMarkerStyle(4)
#hist_9.SetMarkerStyle(23)
#hist_10.SetMarkerStyle(33)
#hist_11.SetMarkerStyle(34)

# Scale Histograms
#if hist_6.Integral()!=0:
#                   hist_6.Scale(1/hist_6.Integral())

#if hist_7.Integral()!=0:
#                   hist_7.Scale(1/hist_7.Integral())

#if hist_8.Integral()!=0:
#                   hist_8.Scale(1/hist_8.Integral())

#if hist_9.Integral()!=0:
#                   hist_9.Scale(1/hist_9.Integral())

#if hist_10.Integral()!=0:
#                   hist_10.Scale(1/hist_10.Integral())

#if hist_11.Integral()!=0:
#                   hist_11.Scale(1/hist_11.Integral())


# Label Axis
hist_6.GetXaxis().SetTitle("T^{#gamma}[GEN](ns)")
hist_6.GetYaxis().SetTitle("Events")


# For Neutralino Pt
# Choose Line Colors
pthist_0.SetLineColor(2)
pthist_1.SetLineColor(8)
pthist_2.SetLineColor(4)
pthist_3.SetLineColor(6)
pthist_4.SetLineColor(28)
pthist_5.SetLineColor(1)
pthist_6.SetLineColor(3)
# Choose Line Style
pthist_0.SetLineStyle(1)
pthist_1.SetLineStyle(7)
pthist_2.SetLineStyle(1)
pthist_3.SetLineStyle(1)
pthist_4.SetLineStyle(1)
pthist_5.SetLineStyle(9)
pthist_6.SetLineStyle(1)
#Choose LineWidth
pthist_0.SetLineWidth(4)
pthist_1.SetLineWidth(4)
pthist_2.SetLineWidth(4)
pthist_3.SetLineWidth(4)
pthist_4.SetLineWidth(4)
pthist_5.SetLineWidth(4)
pthist_6.SetLineWidth(4)
#Choose Marker Style!
#pthist_0.SetMarkerStyle(2)
#pthist_1.SetMarkerStyle(3)
#pthist_2.SetMarkerStyle(4)
#pthist_3.SetMarkerStyle(23)
#pthist_4.SetMarkerStyle(33)
#pthist_5.SetMarkerStyle(34)
#pthist_6.SetMarkerStyle(34)

# Scale Histograms
#if pthist_0.Integral()!=0:
#                   pthist_0.Scale(1/pthist_0.Integral())

#if pthist_1.Integral()!=0:
#                   pthist_1.Scale(1/pthist_1.Integral())

#if pthist_2.Integral()!=0:
#                   pthist_2.Scale(1/pthist_2.Integral())

#if pthist_3.Integral()!=0:
#                   pthist_3.Scale(1/pthist_3.Integral())

#if pthist_4.Integral()!=0:
#                   pthist_4.Scale(1/pthist_4.Integral())

#if pthist_5.Integral()!=0:
#                   pthist_5.Scale(1/pthist_5.Integral())
#if pthist_6.Integral()!=0:
#                   pthist_6.Scale(1/pthist_5.Integral())
# Label Axis
pthist_6.GetXaxis().SetTitle("Pt_{#tilde{#chi}^{0}_{1}}(GeV)")
pthist_6.GetYaxis().SetTitle("Events")


# For Photon Pt
# Choose Line Colors
phpthist_0.SetLineColor(2)
phpthist_1.SetLineColor(8)
phpthist_2.SetLineColor(4)
phpthist_3.SetLineColor(6)
phpthist_4.SetLineColor(28)
phpthist_5.SetLineColor(1)
# Choose Line Style
phpthist_0.SetLineStyle(1)
phpthist_1.SetLineStyle(7)
phpthist_2.SetLineStyle(1)
phpthist_3.SetLineStyle(1)
phpthist_4.SetLineStyle(1)
phpthist_5.SetLineStyle(9)
#Choose LineWidth
phpthist_0.SetLineWidth(3)
phpthist_1.SetLineWidth(3)
phpthist_2.SetLineWidth(3)
phpthist_3.SetLineWidth(3)
phpthist_4.SetLineWidth(3)
phpthist_5.SetLineWidth(3)
#Choose Marker Style!
#phpthist_0.SetMarkerStyle(2)
#phpthist_1.SetMarkerStyle(3)
#phpthist_2.SetMarkerStyle(4)
#phpthist_3.SetMarkerStyle(23)
#phpthist_4.SetMarkerStyle(33)
#phpthist_5.SetMarkerStyle(34)

# Scale Histograms
#if phpthist_0.Integral()!=0:
#                   phpthist_0.Scale(1/pthist_0.Integral())

#if phpthist_1.Integral()!=0:
#                   phpthist_1.Scale(1/pthist_1.Integral())

#if phpthist_2.Integral()!=0:
#                   phpthist_2.Scale(1/pthist_2.Integral())

#if phpthist_3.Integral()!=0:
#                   phpthist_3.Scale(1/pthist_3.Integral())

#if phpthist_4.Integral()!=0:
#                   phpthist_4.Scale(1/pthist_4.Integral())

#if phpthist_5.Integral()!=0:
#                   phpthist_5.Scale(1/pthist_5.Integral())
# Label Axis
phpthist_0.GetXaxis().SetTitle("Pt_{#gamma}(GeV)")
phpthist_0.GetYaxis().SetTitle("Events")



# For Beta Distribution
# Choose Line Colors
betahist_0.SetLineColor(2)
betahist_1.SetLineColor(8)
betahist_2.SetLineColor(4)
betahist_3.SetLineColor(6)
betahist_4.SetLineColor(28)
betahist_5.SetLineColor(1)
betahist_6.SetLineColor(7)
# Choose Line Style
betahist_0.SetLineStyle(1)
betahist_1.SetLineStyle(7)
betahist_2.SetLineStyle(2)
betahist_3.SetLineStyle(3)
betahist_4.SetLineStyle(4)
betahist_5.SetLineStyle(9)
betahist_6.SetLineStyle(6)
#Choose LineWidth
betahist_0.SetLineWidth(4)
betahist_1.SetLineWidth(4)
betahist_2.SetLineWidth(4)
betahist_3.SetLineWidth(4)
betahist_4.SetLineWidth(4)
betahist_5.SetLineWidth(4)
betahist_6.SetLineWidth(4)
#Choose Marker Style!
#betahist_0.SetMarkerStyle(2)
#betahist_1.SetMarkerStyle(3)
#betahist_2.SetMarkerStyle(4)
#betahist_3.SetMarkerStyle(23)
#betahist_4.SetMarkerStyle(33)
#betahist_5.SetMarkerStyle(34)
#betahist_6.SetMarkerStyle(34)

# Scale Histograms
if betahist_0.Integral()!=0:
                   betahist_0.Scale(1/betahist_0.Integral())

if betahist_1.Integral()!=0:
                   betahist_1.Scale(1/betahist_1.Integral())

if betahist_2.Integral()!=0:
                   betahist_2.Scale(1/betahist_2.Integral())

if betahist_3.Integral()!=0:
                   betahist_3.Scale(1/betahist_3.Integral())

if betahist_4.Integral()!=0:
                   betahist_4.Scale(1/betahist_4.Integral())

if betahist_5.Integral()!=0:
                   betahist_5.Scale(1/betahist_5.Integral())
if betahist_6.Integral()!=0:
                   betahist_6.Scale(1/betahist_6.Integral())
# Label Axis
betahist_6.GetXaxis().SetTitle("#beta_{#tilde{#chi}^{0}_{1}}")
betahist_6.GetYaxis().SetTitle("Events")



# Draw Plots now
# ************************************************************************************
# Seed Xtal
c1 = TCanvas("c1", "Neutralino Proper Decay Length", 800, 800)
c1.cd()

histdl_6.SetTitle("Neutralino Proper Decay Length")

histdl_6.Draw()
#hist_5.Draw("sames")
hist_4.Draw("sames")
hist_3.Draw("sames")
hist_2.Draw("sames")
hist_1.Draw("sames")
#hist_0.Draw("sames")
histdl_6.GetXaxis().SetRangeUser(0.0, 200000.0)
#histdl_6.GetYaxis().SetRangeUser(0.0, 100000.0)
#Add Legend
leg1 = TLegend( 0.60, 0.70, 0.90, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(histdl_6,"#Lambda=180 TeV, c#tau=6000mm","l")
#leg1.AddEntry(hist_5,"#Lambda=180 TeV, c#tau=4000mm","l")
leg1.AddEntry(hist_4,"#Lambda=180 TeV, c#tau=3000mm","l")
leg1.AddEntry(hist_3,"#Lambda=180 TeV, c#tau=2000mm","l")
leg1.AddEntry(hist_2,"#Lambda=180 TeV, c#tau=1000mm ","l")
leg1.AddEntry(hist_1,"#Lambda=180 TeV, c#tau=500mm","l")
#leg1.AddEntry(hist_0,"#Lambda=180 TeV, c#tau=250mm","l")
leg1.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg1.SetTextSize(0.02)
leg1.Draw()

# Photon Arrival Time
c2 = TCanvas("c2", "Photon Arrival Time", 800, 800)
c2.cd()

hist_6.SetTitle("Photon Arrival Time[ns]")
hist_6.Draw()
hist_7.Draw("sames")
hist_8.Draw("sames")
hist_9.Draw("sames")
hist_10.Draw("sames")
hist_11.Draw("sames")
hist_6.GetXaxis().SetRangeUser(0.0, 200.0)
#Add Legend
leg2 = TLegend( 0.60, 0.70, 0.90, 0.90)
leg2.SetBorderSize(1)
leg2.SetFillColor(0)
leg2.AddEntry(hist_6,"#Lambda=120 TeV, c#tau=1000mm","l")
leg2.AddEntry(hist_7,"#Lambda=180 TeV, c#tau=1000mm","l")
leg2.AddEntry(hist_8,"#Lambda=180 TeV, c#tau=2000mm","l")
leg2.AddEntry(hist_9,"#Lambda=180 TeV, c#tau=3000mm","l")
leg2.AddEntry(hist_10,"#Lambda=140 TeV, c#tau=6000mm","l")
leg2.AddEntry(hist_11,"#Lambda=180 TeV, c#tau=6000mm","l")
leg2.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg2.SetTextSize(0.02)
leg2.Draw()

# Neutralino Pt
c3 = TCanvas("c3", "Neutralino Pt Distribution", 800, 800)
c3.cd()

pthist_6.SetTitle("Neutralino Pt Distribution")
pthist_6.Draw()
#pthist_5.Draw("sames")
pthist_4.Draw("sames")
pthist_3.Draw("sames")
pthist_2.Draw("sames")
pthist_1.Draw("sames")
pthist_0.Draw("sames")
pthist_6.GetXaxis().SetRangeUser(0.0, 2000.0)
#Add Legend
leg3 = TLegend( 0.60, 0.70, 0.90, 0.90)
leg3.SetBorderSize(1)
leg3.SetFillColor(0)
leg3.AddEntry(pthist_6,"#Lambda=180 TeV, c#tau=6000mm","l")
#leg3.AddEntry(pthist_5,"#Lambda=180 TeV, c#tau=4000mm","l")
leg3.AddEntry(pthist_4,"#Lambda=140 TeV, c#tau=3000mm","l")
leg3.AddEntry(pthist_3,"#Lambda=180 TeV, c#tau=2000mm","l")
leg3.AddEntry(pthist_2,"#Lambda=180 TeV, c#tau=1000mm","l")
leg3.AddEntry(pthist_1,"#Lambda=180 TeV, c#tau=500mm","l")
leg3.AddEntry(pthist_0,"#Lambda=180 TeV, c#tau=250mm","l")
leg3.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg3.SetTextSize(0.02)
leg3.Draw()


# Photon Pt
c4 = TCanvas("c4", "Photon Pt Distribution", 800, 800)
c4.cd()
phpthist_0.SetTitle("Photon Pt")
phpthist_0.Draw()
phpthist_1.Draw("sames")
phpthist_2.Draw("sames")
phpthist_3.Draw("sames")
phpthist_4.Draw("sames")
phpthist_5.Draw("sames")
phpthist_0.GetXaxis().SetRangeUser(0.0, 2000.0)
#Add Legend
leg4 = TLegend( 0.60, 0.70, 0.90, 0.90)
leg4.SetBorderSize(1)
leg4.SetFillColor(0)
leg4.AddEntry(phpthist_0,"#Lambda=120 TeV, c#tau=1000mm","l")
leg4.AddEntry(phpthist_1,"#Lambda=180 TeV, c#tau=1000mm","l")
leg4.AddEntry(phpthist_2,"#Lambda=180 TeV, c#tau=2000mm","l")
leg4.AddEntry(phpthist_3,"#Lambda=180 TeV, c#tau=3000mm","l")
leg4.AddEntry(phpthist_4,"#Lambda=140 TeV, c#tau=6000mm","l")
leg4.AddEntry(phpthist_5,"#Lambda=180 TeV, c#tau=6000mm","l")
leg4.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg4.SetTextSize(0.02)
leg4.Draw()

# Beta Neutralino
c5 = TCanvas("c5", "Neutralino Beta Distribution", 800, 800)
c5.cd()
betahist_6.SetTitle("Neutralino #beta Distribution")
betahist_6.Draw()
#betahist_5.Draw("sames")
betahist_4.Draw("sames")
betahist_3.Draw("sames")
betahist_2.Draw("sames")
betahist_1.Draw("sames")
betahist_0.Draw("sames")
betahist_6.GetXaxis().SetRangeUser(0.0, 2.0)
betahist_6.GetYaxis().SetRangeUser(0.000001, 1.2)
#Add Legend
leg5 = TLegend( 0.20, 0.65, 0.50, 0.90)
leg5.SetBorderSize(1)
leg5.SetFillColor(0)
leg5.AddEntry(betahist_6,"#Lambda=180 TeV, c#tau=6000mm","l")
#leg5.AddEntry(betahist_5,"#Lambda=180 TeV, c#tau=4000mm","l")
leg5.AddEntry(betahist_4,"#Lambda=180 TeV, c#tau=3000mm","l")
leg5.AddEntry(betahist_3,"#Lambda=180 TeV, c#tau=2000mm","l")
leg5.AddEntry(betahist_2,"#Lambda=180 TeV, c#tau=1000mm","l")
leg5.AddEntry(betahist_1,"#Lambda=180 TeV, c#tau=500mm","l")
leg5.AddEntry(betahist_0,"#Lambda=180 TeV, c#tau=250mm","l")
leg5.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg5.SetTextSize(0.02)
leg5.Draw()
Lambda = "Lambda-180-TeV"


  
#_______________________ Save Plots_______________________#

c1.SetLogy(1)
c1.SaveAs("GMSB-SPS8-MODEL-Neutralino-Proper-DecayLength_" + Lambda + ".png")

c2.SetLogy(1)
c2.SaveAs("GMSB-SPS8-MODEL-Photon-Arrival-Time.png")

c3.SetLogy(1)
c3.SaveAs("GMSB-SPS8-MODEL-Neutralinio-Pt_" + Lambda + ".png")

c4.SetLogy(1)
c4.SaveAs("GMSB-SPS8-MODEL-Photon-Pt.png")

c5.SetLogy(1)
c5.SaveAs("GMSB-SPS8-MODEL-Neutralino-Beta_" + Lambda + ".png")

















