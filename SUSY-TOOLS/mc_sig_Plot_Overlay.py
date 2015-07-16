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

Lowlim  = 0.0 
Highlim = 20.0 

Lambda = "Lambda-180-TeV"
ctau   = "ctau-4000-mm"

# ROOT Plot Options***/	
gROOT.Reset()
gROOT.Clear()
gROOT.SetStyle("Plain")
gROOT.SetBatch(kFALSE)
gStyle.SetOptTitle(0)
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
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
gStyle.SetTextSize(1.0)
gStyle.SetLabelSize(0.02,"xy")
gStyle.SetTitleSize(0.06,"xy")
gStyle.SetTitleOffset(1.0,"x")
gStyle.SetTitleOffset(1.0,"y")
gStyle.SetPadTopMargin(0.1)
gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.20)
gStyle.SetPadLeftMargin(0.14)
#TGaxis.SetMaxDigits(2) #// Set Axis to be of the form 0.11 10^N
### No stats
gStyle.SetOptStat(0)
gStyle.SetPadGridX(1);
gStyle.SetPadGridY(1);
# Get Files!
#------------------------------------------
fdata = TFile("Analysis_DPTrigger_Displaced_Photon_ALL_NJets_outputfile.root")
fmc_sig = TFile("Analysis_DPTrigger_gmsb_180_6000_ALLjet.root")
# MC samples
#/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/
f0 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L180-Ctau4000.root")
f1 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L160-Ctau4000.root")
f2 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L140-Ctau4000.root")
f3 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L120-Ctau4000.root")
f4 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L100-Ctau4000.root")
f5 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L220-Ctau4000.root")
f6 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb-L300-Ctau4000.root")

g0 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-250_8TeV.root")
g1 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-500_8TeV.root")
g2 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-1000_8TeV.root")
g3 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-2000_8TeV.root")
g4 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-3000_8TeV.root")
g5 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-4000_8TeV.root")
g6 =  TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/GEN/mc-GEN-GMSB-Ntuples/gmsb_lambda-180_ctau-6000_8TeV.root")

### DPAnalysis files
fDP0 = TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/OUT-PUT-FILE/GMSB-180-6000.root")
fDP1 = TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/OUT-PUT-FILE/GMSB-180-4000.root")
fDP2 = TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/OUT-PUT-FILE/GMSB-180-2000.root")
fgjet = TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/OUT-PUT-FILE/GammaJet-MC_120-170.root")

# Get Historgrams
#Event Met
evt_met0 = fDP0.Get("ev_met")
evt_met1 = fDP1.Get("ev_met")
evt_met2 = fDP2.Get("ev_met")
evt_metgjet = fgjet.Get("ev_met")
## Number Of Photons
nph0 = fDP0.Get("nphos") 
nph1 = fDP1.Get("nphos") 
nph2 = fDP2.Get("nphos")
nphgjet = fgjet.Get("nphos")
## Number Of Jets
njet0 = fDP0.Get("njets") 
njet1 = fDP1.Get("njets")  
njet2 = fDP2.Get("njets") 
njetgjet = fgjet.Get("njets") 

#Neutralino Pt
pthist_0 = f0.Get("TestHepMCEvt/PtNeut")
pthist_1 = f1.Get("TestHepMCEvt/PtNeut")
pthist_2 = f2.Get("TestHepMCEvt/PtNeut")
pthist_3 = f3.Get("TestHepMCEvt/PtNeut")
pthist_4 = f4.Get("TestHepMCEvt/PtNeut")
pthist_5 = f5.Get("TestHepMCEvt/PtNeut")
pthist_6 = f6.Get("TestHepMCEvt/PtNeut")

#Photon Pt
phpthist_0 = f0.Get("TestHepMCEvt/Photon_Pt")
phpthist_1 = f1.Get("TestHepMCEvt/Photon_Pt")
phpthist_2 = f2.Get("TestHepMCEvt/Photon_Pt")
phpthist_3 = f3.Get("TestHepMCEvt/Photon_Pt")
phpthist_4 = f4.Get("TestHepMCEvt/Photon_Pt")
phpthist_5 = f5.Get("TestHepMCEvt/Photon_Pt")
phpthist_6 = f6.Get("TestHepMCEvt/Photon_Pt")

# ProperLifetime
hist_0 = g0.Get("TestHepMCEvt/TimeNeut")
hist_1 = g1.Get("TestHepMCEvt/TimeNeut")
hist_2 = g2.Get("TestHepMCEvt/TimeNeut") 
hist_3 = g3.Get("TestHepMCEvt/TimeNeut") 
hist_4 = g4.Get("TestHepMCEvt/TimeNeut") 
hist_5 = g5.Get("TestHepMCEvt/TimeNeut") 
histdl_6 = g6.Get("TestHepMCEvt/TimeNeut") 

#Photon Time
hist_6 = g0.Get("TestHepMCEvt/Gamma_Time")
hist_7 = g1.Get("TestHepMCEvt/Gamma_Time")
hist_8 = g2.Get("TestHepMCEvt/Gamma_Time")
hist_9 = g3.Get("TestHepMCEvt/Gamma_Time")
hist_10 = g4.Get("TestHepMCEvt/Gamma_Time")
hist_11 = g5.Get("TestHepMCEvt/Gamma_Time")
hist_12 = g6.Get("TestHepMCEvt/Gamma_Time")



#Beta Distribution
betahist_0 = f0.Get("TestHepMCEvt/Gravitino_N")
betahist_1 = f1.Get("TestHepMCEvt/Gravitino_N")
betahist_2 = f2.Get("TestHepMCEvt/Gravitino_N")
betahist_3 = f3.Get("TestHepMCEvt/Gravitino_N")
betahist_4 = f4.Get("TestHepMCEvt/Gravitino_N")
betahist_5 = f5.Get("TestHepMCEvt/Gravitino_N")
betahist_6 = f6.Get("TestHepMCEvt/Gravitino_N")


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
hist_0.SetLineWidth(3)
hist_1.SetLineWidth(3)
hist_2.SetLineWidth(3)
hist_3.SetLineWidth(3)
hist_4.SetLineWidth(3)
hist_5.SetLineWidth(3)
histdl_6.SetLineWidth(3)

#Choose Marker Style!
#hist_0.SetMarkerStyle(2)
#hist_1.SetMarkerStyle(3)
#hist_2.SetMarkerStyle(4)
#hist_3.SetMarkerStyle(23)
#hist_4.SetMarkerStyle(33)
#hist_5.SetMarkerStyle(34)
#histdl_6.SetMarkerStyle(34)

# Scale Histograms
if hist_0.Integral()!=0:
                   hist_0.Scale(1/hist_0.Integral())

if hist_1.Integral()!=0:
                   hist_1.Scale(1/hist_1.Integral())

if hist_2.Integral()!=0:
                   hist_2.Scale(1/hist_2.Integral())

if hist_3.Integral()!=0:
                   hist_3.Scale(1/hist_3.Integral())

if hist_4.Integral()!=0:
                   hist_4.Scale(1/hist_4.Integral())

if hist_5.Integral()!=0:
                   hist_5.Scale(1/hist_5.Integral())
if histdl_6.Integral()!=0:
                   histdl_6.Scale(1/histdl_6.Integral())


# Label Axis
hist_1.GetXaxis().SetTitle("Neutralino Mean Time(c#tau)[mm]")
hist_1.GetYaxis().SetTitle("Events")


# For JetseedTime2
# Choose Line Colors
hist_6.SetLineColor(2)
hist_7.SetLineColor(8)
hist_8.SetLineColor(4)
hist_9.SetLineColor(6)
hist_10.SetLineColor(28)
hist_11.SetLineColor(1)
hist_12.SetLineColor(3)

# Choose Line Style
hist_6.SetLineStyle(1)
hist_7.SetLineStyle(7)
hist_8.SetLineStyle(1)
hist_9.SetLineStyle(1)
hist_10.SetLineStyle(1)
hist_11.SetLineStyle(9)
hist_12.SetLineStyle(1)


#Choose LineWidth
hist_6.SetLineWidth(3)
hist_7.SetLineWidth(3)
hist_8.SetLineWidth(3)
hist_9.SetLineWidth(3)
hist_10.SetLineWidth(3)
hist_11.SetLineWidth(3)
hist_12.SetLineWidth(3)


#Choose Marker Style!
#hist_6.SetMarkerStyle(2)
#hist_7.SetMarkerStyle(3)
#hist_8.SetMarkerStyle(4)
#hist_9.SetMarkerStyle(23)
#hist_10.SetMarkerStyle(33)
#hist_11.SetMarkerStyle(34)
#hist_12.SetMarkerStyle(44)

# Scale Histograms
if hist_6.Integral()!=0:
                   hist_6.Scale(1/hist_6.Integral())

if hist_7.Integral()!=0:
                   hist_7.Scale(1/hist_7.Integral())

if hist_8.Integral()!=0:
                   hist_8.Scale(1/hist_8.Integral())

if hist_9.Integral()!=0:
                   hist_9.Scale(1/hist_9.Integral())

if hist_10.Integral()!=0:
                   hist_10.Scale(1/hist_10.Integral())

if hist_11.Integral()!=0:
                   hist_11.Scale(1/hist_11.Integral())

if hist_12.Integral()!=0:
                   hist_12.Scale(1/hist_12.Integral())

# Label Axis
hist_6.GetXaxis().SetTitle("Photon Time(GEN)[ns]")
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
pthist_0.SetLineWidth(3)
pthist_1.SetLineWidth(3)
pthist_2.SetLineWidth(3)
pthist_3.SetLineWidth(3)
pthist_4.SetLineWidth(3)
pthist_5.SetLineWidth(3)
pthist_6.SetLineWidth(3)
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
pthist_0.GetXaxis().SetTitle("Neutralino Pt_{GEN}[GeV/c]")
pthist_0.GetYaxis().SetTitle("Events")


# For Photon Pt
# Choose Line Colors
phpthist_0.SetLineColor(2)
phpthist_1.SetLineColor(8)
phpthist_2.SetLineColor(4)
phpthist_3.SetLineColor(6)
phpthist_4.SetLineColor(28)
phpthist_5.SetLineColor(1)
phpthist_6.SetLineColor(3)
# Choose Line Style
phpthist_0.SetLineStyle(1)
phpthist_1.SetLineStyle(7)
phpthist_2.SetLineStyle(1)
phpthist_3.SetLineStyle(1)
phpthist_4.SetLineStyle(1)
phpthist_5.SetLineStyle(9)
phpthist_6.SetLineStyle(1)
#Choose LineWidth
phpthist_0.SetLineWidth(3)
phpthist_1.SetLineWidth(3)
phpthist_2.SetLineWidth(3)
phpthist_3.SetLineWidth(3)
phpthist_4.SetLineWidth(3)
phpthist_5.SetLineWidth(3)
phpthist_6.SetLineWidth(3)
#Choose Marker Style!
#phpthist_0.SetMarkerStyle(2)
#phpthist_1.SetMarkerStyle(3)
#phpthist_2.SetMarkerStyle(4)
#phpthist_3.SetMarkerStyle(23)
#phpthist_4.SetMarkerStyle(33)
#phpthist_5.SetMarkerStyle(34)
#phpthist_6.SetMarkerStyle(44)

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
#if phpthist_6.Integral()!=0:
#                   phpthist_6.Scale(1/pthist_6.Integral())
# Label Axis
phpthist_0.GetXaxis().SetTitle("Photon Pt_{GEN}[GeV]/c")
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
betahist_0.SetLineWidth(3)
betahist_1.SetLineWidth(3)
betahist_2.SetLineWidth(3)
betahist_3.SetLineWidth(3)
betahist_4.SetLineWidth(3)
betahist_5.SetLineWidth(3)
betahist_6.SetLineWidth(3)
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
betahist_0.GetXaxis().SetTitle("Number Of Gravitinos")
betahist_0.GetYaxis().SetTitle("Events")


# Choose Line Colors
evt_met0.SetLineColor(1)
evt_met1.SetLineColor(4)
evt_met2.SetLineColor(2)
evt_metgjet.SetLineColor(6)

nph1.SetLineColor(4)
nph2.SetLineColor(2)
nph0.SetLineColor(1)
nphgjet.SetLineColor(6)

njet1.SetLineColor(4)
njet2.SetLineColor(2)
njet0.SetLineColor(1)
njetgjet.SetLineColor(6)
## Choose Line Widths
evt_met1.SetLineWidth(3)
evt_met2.SetLineWidth(3)
evt_met0.SetLineWidth(3)
evt_metgjet.SetLineWidth(3)

nph1.SetLineWidth(3)
nph2.SetLineWidth(3)
nph0.SetLineWidth(3)
nphgjet.SetLineWidth(3)

njet1.SetLineWidth(3)
njet2.SetLineWidth(3)
njet0.SetLineWidth(3)
njetgjet.SetLineWidth(3)
# Choose Fill Colors
#s_time.SetFillColor(6)
#A_time.SetFillColor(9)

#Choose Marker Style!
evt_met1.SetMarkerStyle(6)
evt_met2.SetMarkerStyle(9)
evt_met0.SetMarkerStyle(8)
evt_metgjet.SetMarkerStyle(5)

nph1.SetMarkerStyle(6)
nph2.SetMarkerStyle(9)
nph0.SetMarkerStyle(8)
nphgjet.SetMarkerStyle(5)

njet1.SetMarkerStyle(6)
njet2.SetMarkerStyle(9)
njet0.SetMarkerStyle(8)
njetgjet.SetMarkerStyle(5)
# Scale Histograms

if evt_met1.Integral()!=0:
   evt_met1.Scale(1/evt_met1.Integral())
if evt_met2.Integral()!=0:
   evt_met2.Scale(1/evt_met2.Integral())
if evt_met0.Integral()!=0:
   evt_met0.Scale(1/evt_met0.Integral())
if evt_metgjet.Integral()!=0:
   evt_metgjet.Scale(1/evt_metgjet.Integral())

if nph1.Integral()!=0:
   nph1.Scale(1/nph1.Integral())
if nph2.Integral()!=0:
   nph2.Scale(1/nph2.Integral())
if nph0.Integral()!=0:
   nph0.Scale(1/nph0.Integral())
if nphgjet.Integral()!=0:
   nphgjet.Scale(1/nphgjet.Integral())

if njet1.Integral()!=0:
   njet1.Scale(1/njet1.Integral())
if njet2.Integral()!=0:
   njet2.Scale(1/njet2.Integral())
if njet0.Integral()!=0:
   njet0.Scale(1/njet0.Integral())
if njetgjet.Integral()!=0:
   njetgjet.Scale(1/njetgjet.Integral())

## Rename Axis
evt_met0.GetXaxis().SetTitle("MET[GeV]")
evt_met0.GetYaxis().SetTitle("Events")

nph0.GetXaxis().SetTitle("Number Of Photons")
nph0.GetYaxis().SetTitle("Events")

njet0.GetXaxis().SetTitle("Number Of Jets")
njet0.GetYaxis().SetTitle("Events")


# ************************************************************************************
# Draw Plots now
# *************************************************************************************

## Event MET
c0 = TCanvas("c0", "SPS8: Event MET", 800, 800)
c0.SetGridx();
c0.SetGridy();
c0.GetFrame().SetFillColor(10);
c0.GetFrame().SetBorderMode(-1);
c0.GetFrame().SetBorderSize(5);
c0.cd()
## Draw MET plots here!!

evt_met0.SetTitle("Event MET")
evt_met0.Draw()
evt_met2.Draw("sames")
evt_met1.Draw("sames")
evt_metgjet.Draw("sames")
evt_met0.GetXaxis().SetRangeUser(0, 1500)
#evt_met0.GetYaxis().SetRangeUser(0.0, 10.0)


#Add Legend
leg0 = TLegend( 0.55, 0.70, 0.90, 0.90)
leg0.SetBorderSize(1)
leg0.SetFillColor(0)
leg0.AddEntry(evt_met0,"#Lambda=180 TeV, c#tau=6000mm","l")
leg0.AddEntry(evt_met1,"#Lambda=180 TeV, c#tau=2000mm","l")
leg0.AddEntry(evt_met2,"#Lambda=180 TeV, c#tau=4000mm","l")
leg0.AddEntry(evt_metgjet,"#gamma + Jet, 120 < Pt < 170","l")
leg0.SetTextSize(0.025)
leg0.SetHeader("")
leg0.Draw()

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
cmstex.DrawLatex(0.30, 0.865, "CMS");
cmstex.DrawLatex(0.30, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);

## Number of Photons
c1 = TCanvas("c1", "SPS8:  Number Of Photons", 800, 800)
c1.SetGridx();
c1.SetGridy();
c1.GetFrame().SetFillColor(10);
c1.GetFrame().SetBorderMode(-1);
c1.GetFrame().SetBorderSize(5);
c1.cd()

nph0.SetTitle("Number Of Photons")
nph0.Draw()
nph1.Draw("sames")
nph2.Draw("sames")
nphgjet.Draw("sames")
nph0.GetXaxis().SetRangeUser(0, 15)
#Add Legend
leg1 = TLegend( 0.60, 0.70, 0.90, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(nph0,"#Lambda=180 TeV, c#tau=6000mm","l")
leg1.AddEntry(nph1,"#Lambda=180 TeV, c#tau=2000mm","l")
leg1.AddEntry(nph2,"#Lambda=180 TeV, c#tau=4000mm","l")
leg1.AddEntry(nphgjet,"#gamma + Jet, 120 < Pt < 170","l")
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
cmstex.DrawLatex(0.40, 0.865, "CMS");
cmstex.DrawLatex(0.40, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);

### Number of Jets
c2 = TCanvas("c2", "SPS8: Number of Jets", 800, 800)
c2.SetGridx();
c2.SetGridy();
c2.GetFrame().SetFillColor(10);
c2.GetFrame().SetBorderMode(-1);
c2.GetFrame().SetBorderSize(5);
c2.cd()

njet0.SetTitle("Number Of Jets")
njet0.Draw()
njet1.Draw("sames")
njet2.Draw("sames")
njetgjet.Draw("sames")
njet0.GetXaxis().SetRangeUser(0, 15)
#Add Legend
leg2 = TLegend( 0.60, 0.70, 0.90, 0.90)
leg2.SetBorderSize(1)
leg2.SetFillColor(0)
leg2.AddEntry(njet0,"#Lambda=180 TeV, c#tau=6000mm","l")
leg2.AddEntry(njet1,"#Lambda=180 TeV, c#tau=2000mm","l")
leg2.AddEntry(njet2,"#Lambda=180 TeV, c#tau=4000mm","l")
leg2.AddEntry(njetgjet,"#gamma + Jet,120 < Pt < 170","l")
leg2.SetTextSize(0.025)
leg2.SetHeader("")
leg2.Draw()

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
cmstex.DrawLatex(0.30, 0.865, "CMS");
cmstex.DrawLatex(0.30, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);


c3 = TCanvas("c3", "Neutralino Proper Decay Length", 800, 800)
c3.SetGridx();
c3.SetGridy();
c3.GetFrame().SetFillColor(10);
c3.GetFrame().SetBorderMode(-1);
c3.GetFrame().SetBorderSize(5);
c3.cd()

hist_1.SetTitle("Neutralino Mean Time")
hist_1.Draw()
hist_5.Draw("sames")
hist_4.Draw("sames")
hist_3.Draw("sames")
histdl_6.Draw("sames")
hist_2.Draw("sames")
hist_0.Draw("sames")
hist_1.GetXaxis().SetRangeUser(0.0, 300000.0)
#histdl_6.GetYaxis().SetRangeUser(0.0, 100000.0)
#Add Legend
leg3 = TLegend( 0.55, 0.60, 0.90, 0.90)
leg3.SetBorderSize(1)
leg3.SetFillColor(0)
leg3.AddEntry(histdl_6,"#Lambda=180 TeV, c#tau=6000mm","l")
leg3.AddEntry(hist_5,"#Lambda=180 TeV, c#tau=4000mm","l")
leg3.AddEntry(hist_4,"#Lambda=180 TeV, c#tau=3000mm","l")
leg3.AddEntry(hist_3,"#Lambda=180 TeV, c#tau=2000mm","l")
leg3.AddEntry(hist_2,"#Lambda=180 TeV, c#tau=1000mm ","l")
leg3.AddEntry(hist_1,"#Lambda=180 TeV, c#tau=500mm","l")
leg3.AddEntry(hist_0,"#Lambda=180 TeV, c#tau=250mm","l")
#leg3.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg3.SetTextSize(0.025)
leg3.Draw()

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
cmstex.DrawLatex(0.30, 0.865, "CMS");
cmstex.DrawLatex(0.30, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);


# Photon GEN Arrival Time
c4 = TCanvas("c4", "Photon GEN Arrival Time", 800, 800)
c4.SetGridx();
c4.SetGridy();
c4.GetFrame().SetFillColor(10);
c4.GetFrame().SetBorderMode(-1);
c4.GetFrame().SetBorderSize(5);
c4.cd()

hist_6.SetTitle("Photon Arrival Time[ns]")
hist_6.Draw()
hist_11.Draw("sames")
hist_10.Draw("sames")
hist_9.Draw("sames")
hist_7.Draw("sames")
hist_8.Draw("sames")
hist_12.Draw("sames")
hist_6.GetXaxis().SetRangeUser(0.0, 300.0)
#hist_12.GetYaxis().SetRangeUser(0.0, 10000.0)
#Add Legend
leg4 = TLegend( 0.55, 0.60, 0.90, 0.90)
leg4.SetBorderSize(1)
leg4.SetFillColor(0)
leg4.AddEntry(hist_12,"#Lambda=180 TeV, c#tau=6000mm","l")
leg4.AddEntry(hist_11,"#Lambda=180 TeV, c#tau=4000mm","l")
leg4.AddEntry(hist_10,"#Lambda=180 TeV, c#tau=3000mm","l")
leg4.AddEntry(hist_9,"#Lambda=180 TeV, c#tau=2000mm","l")
leg4.AddEntry(hist_8,"#Lambda=180 TeV, c#tau=1000mm","l")
leg4.AddEntry(hist_7,"#Lambda=180 TeV, c#tau=500mm","l")
leg4.AddEntry(hist_6,"#Lambda=180 TeV, c#tau=250mm","l")
#leg4.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg4.SetTextSize(0.025)
leg4.Draw()

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
cmstex.DrawLatex(0.30, 0.865, "CMS");
cmstex.DrawLatex(0.30, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);

# Neutralino Pt
c5 = TCanvas("c5", "Neutralino Pt Distribution", 800, 800)
c5.SetGridx();
c5.SetGridy();
c5.GetFrame().SetFillColor(10);
c5.GetFrame().SetBorderMode(-1);
c5.GetFrame().SetBorderSize(5);
c5.cd()

pthist_0.SetTitle("Neutralino Pt Distribution")
pthist_0.Draw()
pthist_5.Draw("sames")
pthist_4.Draw("sames")
pthist_3.Draw("sames")
pthist_2.Draw("sames")
pthist_1.Draw("sames")
pthist_6.Draw("sames")
pthist_0.GetXaxis().SetRangeUser(0.0, 2000.0)
#Add Legend
leg5 = TLegend( 0.55, 0.60, 0.90, 0.90)
leg5.SetBorderSize(1)
leg5.SetFillColor(0)
leg5.AddEntry(pthist_0,"#Lambda=180 TeV, c#tau=4000mm","l")
leg5.AddEntry(pthist_1,"#Lambda=160 TeV, c#tau=4000mm","l")
leg5.AddEntry(pthist_2,"#Lambda=140 TeV, c#tau=4000mm","l")
leg5.AddEntry(pthist_3,"#Lambda=120 TeV, c#tau=4000mm","l")
leg5.AddEntry(pthist_4,"#Lambda=100 TeV, c#tau=4000mm","l")
leg5.AddEntry(pthist_5,"#Lambda=220 TeV, c#tau=4000mm","l")
leg5.AddEntry(pthist_6,"#Lambda=300 TeV, c#tau=4000mm","l")
#leg5.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg5.SetTextSize(0.025)
leg5.Draw()

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
cmstex.DrawLatex(0.30, 0.865, "CMS");
cmstex.DrawLatex(0.30, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);

# Photon Pt
c6 = TCanvas("c6", "Photon Pt Distribution", 800, 800)
c6.SetGridx();
c6.SetGridy();
c6.GetFrame().SetFillColor(10);
c6.GetFrame().SetBorderMode(-1);
c6.GetFrame().SetBorderSize(5);
c6.cd()

phpthist_0.SetTitle("Photon Pt")
phpthist_0.Draw()
phpthist_6.Draw("sames")
phpthist_5.Draw("sames")
phpthist_4.Draw("sames")
phpthist_3.Draw("sames")
phpthist_2.Draw("sames")
phpthist_1.Draw("sames")
phpthist_0.GetXaxis().SetRangeUser(0.0, 2000.0)
#Add Legend
leg6 = TLegend( 0.55, 0.60, 0.90, 0.90)
leg6.SetBorderSize(1)
leg6.SetFillColor(0)
leg6.AddEntry(phpthist_0,"#Lambda=180 TeV, c#tau=4000mm","l")
leg6.AddEntry(phpthist_1,"#Lambda=160 TeV, c#tau=4000mm","l")
leg6.AddEntry(phpthist_2,"#Lambda=140 TeV, c#tau=4000mm","l")
leg6.AddEntry(phpthist_3,"#Lambda=120 TeV, c#tau=4000mm","l")
leg6.AddEntry(phpthist_4,"#Lambda=100 TeV, c#tau=4000mm","l")
leg6.AddEntry(phpthist_5,"#Lambda=220 TeV, c#tau=4000mm","l")
leg6.AddEntry(phpthist_6,"#Lambda=300 TeV, c#tau=4000mm","l")
#leg6.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg6.SetTextSize(0.025)
leg6.Draw()

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
cmstex.DrawLatex(0.30, 0.865, "CMS");
cmstex.DrawLatex(0.30, 0.832, "Preliminary");
cmstex.DrawLatex(0.72, 0.920, lint);


# Number of Gravitino
c7 = TCanvas("c7", "SPS8:Number of Gravitinos", 800, 800)
c7.SetGridx();
c7.SetGridy();
c7.GetFrame().SetFillColor(10);
c7.GetFrame().SetBorderMode(-1);
c7.GetFrame().SetBorderSize(5);
c7.cd()
betahist_0.SetTitle("Number Of Gravitinos")
betahist_0.Draw()
betahist_6.Draw("sames")
betahist_5.Draw("sames")
betahist_4.Draw("sames")
betahist_3.Draw("sames")
betahist_2.Draw("sames")
betahist_1.Draw("sames")
betahist_0.GetXaxis().SetRangeUser(0.0, 10.0)
#betahist_0.GetYaxis().SetRangeUser(0.0, 1000000)
#Add Legend
leg7 = TLegend( 0.55, 0.65, 0.90, 0.90)
leg7.SetBorderSize(1)
leg7.SetFillColor(0)
leg7.AddEntry(betahist_0,"#Lambda=180 TeV, c#tau=4000mm","l")
leg7.AddEntry(betahist_1,"#Lambda=160 TeV, c#tau=4000mm","l")
leg7.AddEntry(betahist_2,"#Lambda=140 TeV, c#tau=4000mm","l")
leg7.AddEntry(betahist_3,"#Lambda=120 TeV, c#tau=4000mm","l")
leg7.AddEntry(betahist_4,"#Lambda=100 TeV, c#tau=4000mm","l")
leg7.AddEntry(betahist_5,"#Lambda=260 TeV, c#tau=4000mm","l")
leg7.AddEntry(betahist_6,"#Lambda=300 TeV, c#tau=4000mm","l")
#leg7.SetHeader("CMS Preliminary 2012,   #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
leg7.SetTextSize(0.02)
leg7.Draw()

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


###---------------Save Histograms ------------------------------------
###-------------------------------------------------------------------

### Gravitino as Missing ET
c0.SetLogy(1)
c0.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Event-MET_" + Lambda + " TeV_" + ".png")
c0.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Event-MET_" + Lambda + " TeV_" + ".pdf")
c0.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Event-MET_" + Lambda + " TeV_" + ".eps")

### Number of Photons
c1.SetLogy(1)
c1.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfPhotons_" + Lambda + " TeV_" + ".png")
c1.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfPhotons_" + Lambda + " TeV_" + ".pdf")
c1.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfPhotons_" + Lambda + " TeV_" + ".eps")

### Number of Jets
c2.SetLogy(1)
c2.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfJets_" + Lambda + " TeV_" + ".png")
c2.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfJets_" + Lambda + " TeV_"  + ".pdf")
c2.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfJets_" + Lambda + " TeV_"  + ".eps")

### Neutralino Proper Decay Length
c3.SetLogy(1)
c3.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Neutralino-Proper-DecayLength_" + Lambda + ".png")
c3.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Neutralino-Proper-DecayLength_" + Lambda + ".pdf")
c3.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Neutralino-Proper-DecayLength_" + Lambda + ".eps")

### Photon Time
c4.SetLogy(1)
c4.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Photon-GEN-Time_"+ Lambda + " TeV_" +".png")
c4.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Photon-GEN-Time_"+ Lambda + " TeV_" +".pdf")
c4.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Photon-GEN-Time_"+ Lambda + " TeV_" +".eps")

### Neutralino Pt
c5.SetLogy(1)
c5.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Neutralinio-Pt_" + ctau + ".png")
c5.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Neutralinio-Pt_" + ctau + ".pdf")
c5.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Neutralinio-Pt_" + ctau +".eps")

### Photon Pt
c6.SetLogy(1)
c6.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Photon-Pt_" + ctau + ".png")
c6.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Photon-Pt_" + ctau + ".pdf")
c6.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-Photon-Pt_" + ctau +".eps")

### Number Of Gravitinos
c7.SetLogy(1)
c7.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfGravitinos_" + ctau + ".png")
c7.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfGravitinos_" + ctau + ".pdf")
c7.SaveAs("/home/user1/norbert/public_html/LL_Neutral_Parts/MC_STUDY/EVENT-KINEMATICS/GMSB-SPS8-MODEL-NumberOfGravitinos_" + ctau + ".eps")
