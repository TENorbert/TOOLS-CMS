# This Simple Code Overlays Histograms Plots  from Different Files
# Cannot DO IT with C++ because It root chokes especially when Number bins are 
# not the same.
## Created By Tambe E. Norbert a.k.a TEN (norbert@physics.umn.edu)

#!/usr/bin/env python
""" OverLay the Histograms from several root files with identical structure!"""
__version__ = "1.0"


import sys
import optparse
import shutil
import os
import re
from ROOT import *
from array import array

# ROOT general options
gROOT.SetBatch(kTRUE)
gROOT.SetStyle("Plain")
gStyle.SetOptStat("emruo")


# input TFiles to read Histograms From
f1 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_May05_ADC_low-1.root")
f2 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_May05_ADC_low-2.root")
f3 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_May05_ADC_low-3.root")
f4 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_April21_ADC_low1.root")
f5 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_April21_ADC_low2.root")
f6 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_April21_ADC_low3.root")
f7 =  TFile("ElectronHad_Run2012A-PromptReco-v1_AOD-190705-191043_April21_ADC_low4.root")


#  Mean Time
tA_EBMod4  = f1.Get("single-bias/timeVsAoSigmaMod4EBlog_1")
tA_EE_low1 = f1.Get("single-bias/timeVsAoSigmaHighEElog_1")
tA_EE_low2 = f2.Get("single-bias/timeVsAoSigmaHighEElog_1")
tA_EE_low3 = f3.Get("single-bias/timeVsAoSigmaHighEElog_1")
tA_EE_low4 = f4.Get("single-bias/timeVsAoSigmaHighEElog_1")
tA_EE_low5 = f5.Get("single-bias/timeVsAoSigmaHighEElog_1")
tA_EE_low6 = f6.Get("single-bias/timeVsAoSigmaHighEElog_1")
tA_EE_low7 = f7.Get("single-bias/timeVsAoSigmaHighEElog_1")

#  Sigma of Time 
sA_EBMod4  = f1.Get("single-bias/timeVsAoSigmaMod4EBlog_2")
sA_EE_low1 = f1.Get("single-bias/timeVsAoSigmaHighEElog_2")
sA_EE_low2 = f2.Get("single-bias/timeVsAoSigmaHighEElog_2")
sA_EE_low3 = f3.Get("single-bias/timeVsAoSigmaHighEElog_2")
sA_EE_low4 = f4.Get("single-bias/timeVsAoSigmaHighEElog_2")
sA_EE_low5 = f5.Get("single-bias/timeVsAoSigmaHighEElog_2")
sA_EE_low6 = f6.Get("single-bias/timeVsAoSigmaHighEElog_2")
sA_EE_low7 = f7.Get("single-bias/timeVsAoSigmaHighEElog_2")



# Choose Line Colors
tA_EBMod4.SetLineColor(1)
tA_EE_low1.SetLineColor(2)
tA_EE_low2.SetLineColor(3)
tA_EE_low3.SetLineColor(4)
tA_EE_low4.SetLineColor(5)
tA_EE_low5.SetLineColor(6)
tA_EE_low6.SetLineColor(7)
tA_EE_low7.SetLineColor(8)

sA_EBMod4.SetLineColor(1)
sA_EE_low1.SetLineColor(2)
sA_EE_low2.SetLineColor(3)
sA_EE_low3.SetLineColor(4)
sA_EE_low4.SetLineColor(5)
sA_EE_low5.SetLineColor(6)
sA_EE_low6.SetLineColor(7)
sA_EE_low7.SetLineColor(8)

# Choose Fill Colors
tA_EBMod4.SetFillColor(1)
tA_EE_low1.SetFillColor(2)
tA_EE_low2.SetFillColor(3)
tA_EE_low3.SetFillColor(4)
tA_EE_low4.SetFillColor(5)
tA_EE_low5.SetFillColor(6)
tA_EE_low6.SetFillColor(7)
tA_EE_low7.SetFillColor(8)

sA_EBMod4.SetFillColor(1)
sA_EE_low1.SetFillColor(2)
sA_EE_low2.SetFillColor(3)
sA_EE_low3.SetFillColor(4)
sA_EE_low4.SetFillColor(5)
sA_EE_low5.SetFillColor(6)
sA_EE_low6.SetFillColor(7)
sA_EE_low7.SetFillColor(8)

#Choose Marker Style!
tA_EBMod4.SetMarkerStyle(1)
tA_EE_low1.SetMarkerStyle(2)
tA_EE_low2.SetMarkerStyle(3)
tA_EE_low3.SetMarkerStyle(4)
tA_EE_low4.SetMarkerStyle(5)
tA_EE_low5.SetMarkerStyle(6)
tA_EE_low6.SetMarkerStyle(7)
tA_EE_low7.SetMarkerStyle(8)
 #SIgma
sA_EBMod4.SetMarkerStyle(1)
sA_EE_low1.SetMarkerStyle(2)
sA_EE_low2.SetMarkerStyle(3)
sA_EE_low3.SetMarkerStyle(4)
sA_EE_low4.SetMarkerStyle(5)
sA_EE_low5.SetMarkerStyle(6)
sA_EE_low6.SetMarkerStyle(7)
sA_EE_low7.SetMarkerStyle(8)

#SetMaxRange XY Axis
tA_EBMod4.GetYaxis().SetRangeUser(-1.0,0.4)
tA_EBMod4.GetXaxis().SetRangeUser(0.0,10000.0)
tA_EBMod4.GetXaxis().SetTitle("Amplitude(ADC Counts)")
tA_EBMod4.GetYaxis().SetTitle("Mean Time(ns)")

sA_EBMod4.GetYaxis().SetRangeUser(0.0,3.5)
sA_EBMod4.GetXaxis().SetRangeUser(0.0,10000.0)
sA_EBMod4.GetXaxis().SetTitle("Amplitude(ADC Counts)")
sA_EBMod4.GetYaxis().SetTitle("#sigma Time(ns)")


# Draw Plots now
# *************************************************************************************
c1 = TCanvas("c1", "Mean Time Stability", 800, 800)
c1.cd()
# ht->GetXaxis()->SetTitle("Energy(GeV)");
#  ht->GetYaxis()->SetTitle("Mean Time(ns)");
#  ht->GetYaxis()->SetRangeUser(-0.8,0.3);
#  ht->GetXaxis()->SetRangeUser(0, 10000);
tA_EBMod4.SetTitle("Mean Time Vs Amplitude Stability")
tA_EBMod4.Draw()
tA_EE_low1.Draw("sames")
tA_EE_low2.Draw("sames")
tA_EE_low3.Draw("sames")
tA_EE_low4.Draw("sames")
tA_EE_low5.Draw("sames")
tA_EE_low6.Draw("sames")
tA_EE_low7.Draw("sames")

#Add Legend
leg1 = TLegend( 0.45, 0.70, 0.65, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(tA_EBMod4,"EB Mod4","l")
leg1.AddEntry(tA_EE_low1,"1.5 <|#eta|<1.7","l")
leg1.AddEntry(tA_EE_low2,"1.7 <|#eta|<1.9","l")
leg1.AddEntry(tA_EE_low3,"1.9 <|#eta|<2.2","l")
leg1.AddEntry(tA_EE_low4,"2.2 <|#eta|<2.4","l")
leg1.AddEntry(tA_EE_low5,"2.4 <|#eta|<2.6","l")
leg1.AddEntry(tA_EE_low6,"2.6 <|#eta|<2.8","l")
leg1.AddEntry(tA_EE_low7,"2.8 <|#eta|<3.0","l")
leg1.Draw()


c2 = TCanvas ( "c2", "Sigma Time Stability", 800, 800)
c2.cd()
#  ht->GetXaxis()->SetTitle("Energy(GeV)");
#  ht->GetYaxis()->SetTitle("#sigma Time(ns)");
#  ht->GetYaxis()->SetRangeUser(0,3.5);
#  ht->GetXaxis()->SetRangeUser(0, 10000);
sA_EBMod4.SetTitle("#sigma Vs Amplitude Stability") 
sA_EBMod4.Draw()
sA_EE_low1.Draw("sames")
sA_EE_low2.Draw("sames")
sA_EE_low3.Draw("sames")
sA_EE_low4.Draw("sames")
sA_EE_low5.Draw("sames")
sA_EE_low6.Draw("sames")
sA_EE_low7.Draw("sames")

# Add Legend
leg2 = TLegend( 0.8, 0.55, 0.98,0.77)
leg2.SetBorderSize(1)
leg2.SetFillColor(0)
leg2.AddEntry(sA_EBMod4,"EB Mod4","l")
leg2.AddEntry(sA_EE_low1,"1.5 <|#eta|<1.7","l")
leg2.AddEntry(sA_EE_low2,"1.7 <|#eta|<1.9","l")
leg2.AddEntry(sA_EE_low3,"1.9 <|#eta|<2.2","l")
leg2.AddEntry(sA_EE_low4,"2.2 <|#eta|<2.4","l")
leg2.AddEntry(sA_EE_low5,"2.4 <|#eta|<2.6","l")
leg2.AddEntry(sA_EE_low6,"2.6 <|#eta|<2.8","l")
leg2.AddEntry(sA_EE_low7,"2.8 <|#eta|<3.0","l")
leg2.Draw()
  
# Save Plots
c1.SaveAs("TimeVsAmplitude_Stability.png")
c1.SetLogy()
c1.SaveAs("TimeVsAMplitude_Stability_Log.png")
c2.SaveAs("SigmaVsAmplitude_Stability.png")
c2.SetLogy()
c2.SaveAs("SigmaVsAmplitude_Stability_Log.png")




