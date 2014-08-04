# This Simple Code Overlays Histograms Plots  from Different Files
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
gStyle.SetTitleOffset(1.0,"y")
#gStyle.SetPadTopMargin(0.1)
#gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.16)
gStyle.SetPadLeftMargin(0.12)

# Get Files!
#---------------MET60Cut---------------------------
f1 =  TFile("met60Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")
f2 =  TFile("met60Cut_MoreThan3Jets_DP_All_Trig_NoJetID.root")
f3 =  TFile("metcut60_ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")

# Compare Diff Time Defs on Samples

#g1 =  TFile("met60Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")
#g2 =  TFile("met60Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")
#g3 =  TFile("met60Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")


##----------------No--Met60Cut-------------------------
#f1 =  TFile("LessThan3Jets_DP_All_Trig_NoJetID.root")
#f2 =  TFile("MoreThan3Jets_DP_All_Trig_NoJetID.root")
#f3 =  TFile("ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")

# Compare Diff Time Defs on Samples

#g1 =  TFile("LessThan3Jets_DP_All_Trig_NoJetID.root")
#g2 =  TFile("LessThan3Jets_DP_All_Trig_NoJetID.root")
#g3 =  TFile("LessThan3Jets_DP_All_Trig_NoJetID.root")

g1 =  TFile("metcut60_ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")
g2 =  TFile("metcut60_ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")
g3 =  TFile("metcut60_ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")


SeedXtalTime = g1.Get("NeutKin/my.h_Time")
WaBCTime     = g2.Get("NeutKin/my.WAveSeedBCphotime")
WaSCTime     = g3.Get("NeutKin/my.AveSCphotime")

# Seed Xtal Time
#ph_time1 = f1.Get("NeutKin/my.h_Time")
#ph_time2 = f2.Get("NeutKin/my.h_Time")
#ph_time3 = f3.Get("NeutKin/my.h_Time") 

# my.WAveSeedBCphotime
#ph_time1 = f1.Get("NeutKin/my.WAveSeedBCphotime")
#ph_time2 = f2.Get("NeutKin/my.WAveSeedBCphotime")
#ph_time3 = f3.Get("NeutKin/my.WAveSeedBCphotime")


# my W.Ave SC photime Time
ph_time1 = f1.Get("NeutKin/my.AveSCphotime")
ph_time2 = f2.Get("NeutKin/my.AveSCphotime")
ph_time3 = f3.Get("NeutKin/my.AveSCphotime")





#Event Met
evt_met1 = f1.Get("NeutKin/my.evt_met") 
evt_met2 = f2.Get("NeutKin/my.evt_met")
evt_met3 = f3.Get("NeutKin/my.evt_met")

# N photons
nph1   = f1.Get("NeutKin/my.npho")
nph2   = f2.Get("NeutKin/my.npho")
nph3   = f3.Get("NeutKin/my.npho")

# N Jets
njets1 = f1.Get("NeutKin/my.njets")
njets2 = f2.Get("NeutKin/my.njets")
njets3 = f3.Get("NeutKin/my.njets")



# Choose Line Colors
ph_time1.SetLineColor(6)
ph_time2.SetLineColor(9)
ph_time3.SetLineColor(12)

evt_met1.SetLineColor(6)
evt_met2.SetLineColor(9)
evt_met3.SetLineColor(12)

nph1.SetLineColor(6)
nph2.SetLineColor(9)
nph3.SetLineColor(12)

njets1.SetLineColor(6)
njets2.SetLineColor(9)
njets3.SetLineColor(12)

SeedXtalTime.SetLineColor(6)
WaBCTime.SetLineColor(9)
WaSCTime.SetLineColor(7)

ph_time1.SetLineWidth(1)
ph_time2.SetLineWidth(2)
ph_time3.SetLineWidth(1)


evt_met1.SetLineWidth(4)
evt_met2.SetLineWidth(2)
evt_met3.SetLineWidth(1)

nph1.SetLineWidth(4)
nph2.SetLineWidth(2)
nph3.SetLineWidth(1)

njets1.SetLineWidth(4)
njets2.SetLineWidth(2)
njets3.SetLineWidth(1)

SeedXtalTime.SetLineWidth(1)
WaBCTime.SetLineWidth(1)
WaSCTime.SetLineWidth(1)




# Choose Fill Colors
#ph_time1.SetFillColor(6)
#ph_time2.SetFillColor(9)
#ph_time3.SetFillColor(12)

#evt_met1.SetFillColor(6)
#evt_met2.SetFillColor(9)
#evt_met3.SetFillColor(12)


#Choose Marker Style!
ph_time1.SetMarkerStyle(6)
ph_time2.SetMarkerStyle(9)
ph_time3.SetMarkerStyle(12)

evt_met1.SetMarkerStyle(6)
evt_met2.SetMarkerStyle(9)
evt_met3.SetMarkerStyle(12)


nph1.SetMarkerStyle(6)
nph2.SetMarkerStyle(9)
nph3.SetMarkerStyle(10)


njets1.SetMarkerStyle(6)
njets2.SetMarkerStyle(9)
njets3.SetMarkerStyle(10)


SeedXtalTime.SetMarkerStyle(3)
WaBCTime.SetMarkerStyle(5)
WaSCTime.SetMarkerStyle(1)



# Scale Histograms
if ph_time1.Integral()!=0:
                   ph_time1.Scale(1/ph_time1.Integral())

if ph_time2.Integral()!=0:
                   ph_time2.Scale(1/ph_time2.Integral())

if ph_time3.Integral()!=0:
                   ph_time3.Scale(1/ph_time3.Integral())



if evt_met1.Integral()!=0:
                   evt_met1.Scale(1/evt_met1.Integral())


if evt_met2.Integral()!=0:
                   evt_met2.Scale(1/evt_met2.Integral())


if evt_met3.Integral()!=0:
                  evt_met3.Scale(1/evt_met3.Integral())


if SeedXtalTime.Integral()!=0:
                  SeedXtalTime.Scale(1/SeedXtalTime.Integral())

if WaBCTime.Integral()!=0:
                  WaBCTime.Scale(1/WaBCTime.Integral())

if WaSCTime.Integral()!=0:
                  WaSCTime.Scale(1/WaSCTime.Integral())


#SetMaxRange XY Axis
#tA_EBMod4.GetYaxis().SetRangeUser(-1.0,0.4)
#tA_EBMod4.GetXaxis().SetRangeUser(0.0,10000.0)

ph_time1.GetXaxis().SetTitle("Photon Time(ns)")
ph_time1.GetYaxis().SetTitle("Event Number")

evt_met1.GetXaxis().SetTitle("MET(GeV)")
evt_met1.GetYaxis().SetTitle("Event Number")



nph1.GetXaxis().SetTitle("Number of Leading Pt Photons")
nph1.GetYaxis().SetTitle("Event Number")


njets1.GetXaxis().SetTitle("Number of Leading Pt Jets")
njets1.GetYaxis().SetTitle("Event Number")


SeedXtalTime.GetXaxis().SetTitle("Photon Time")
SeedXtalTime.GetYaxis().SetTitle("Event Number")



#sA_EBMod4.GetYaxis().SetRangeUser(0.0,3.5)
#sA_EBMod4.GetXaxis().SetRangeUser(0.0,10000.0)



# Draw Plots now
# *************************************************************************************
c1 = TCanvas("c1", "Photon Time", 800, 800)
c1.cd()
# ht->GetXaxis()->SetTitle("Energy(GeV)");
#  ht->GetYaxis()->SetTitle("Mean Time(ns)");
#  ht->GetYaxis()->SetRangeUser(-0.8,0.3);
#  ht->GetXaxis()->SetRangeUser(0, 10000);

#ph_time1.SetTitle("Photon(W.Ave Seed BC)Time Distribution")
#ph_time1.SetTitle("Photon(Seed Xtal)Time Distribution")
ph_time1.SetTitle("Photon(W.Ave SC)Time Distribution")

ph_time1.Draw()
ph_time2.Draw("sames")
ph_time3.Draw("sames")
ph_time1.GetXaxis().SetRangeUser(-25.0, 25.0)
#Add Legend
leg1 = TLegend( 0.14, 0.70, 0.47, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(ph_time1,"< 3Jets + VLIso#gamma ","l")
leg1.AddEntry(ph_time2,">= 3Jets + VLIso#gamma ","l")
leg1.AddEntry(ph_time3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
leg1.SetTextSize(0.018)
leg1.SetHeader("Samples")
leg1.Draw()

# Use for MET Plot 
c2 = TCanvas ( "c2", "Sigma Time Stability", 800, 800)
c2.cd()
evt_met1.SetTitle("Missing Energy Distribution") 
evt_met1.Draw()
evt_met2.Draw("sames")
evt_met3.Draw("sames")
evt_met1.GetYaxis().SetRangeUser(0.000001, 1.0)

#Add Legend
leg2 = TLegend( 0.35, 0.55, 0.73, 0.77)
leg2.SetBorderSize(1)
leg2.SetFillColor(0)
leg2.AddEntry(evt_met1,"< 3Jets + VLIso #gamma ","l")
leg2.AddEntry(evt_met2,">= 3Jets + VLIso #gamma","l")
leg2.AddEntry(evt_met3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
leg2.SetHeader("Samples")
leg2.SetTextSize(0.02)
#leg2 = TLegend( 0.8, 0.55, 0.98,0.77)
#leg2.SetBorderSize(1)
#leg2.SetFillColor(0)
leg2.Draw()

# Number of Photons in Event
c3 = TCanvas("c3", "Number of Photons", 800, 800)
c3.cd()

nph1.SetTitle("Number of Leading Pt Photons")
nph1.GetYaxis().SetRangeUser(1, 100000)

nph1.Draw()
nph2.Draw("sames")
nph3.Draw("sames")
#ph_time3.Draw("sames")

#Add Legend
leg3 = TLegend( 0.52, 0.45, 0.85, 0.70)
leg3.SetBorderSize(1)
leg3.SetFillColor(0)
leg3.AddEntry(ph_time1,"< 3Jets + VLIso #gamma ","l")
leg3.AddEntry(ph_time2,">= 3Jets + VLIso #gamma ","l")
leg3.AddEntry(ph_time3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
leg3.SetTextSize(0.018)
leg3.SetHeader("Samples")

#leg1.AddEntry(tA_EE_low3,"1.9 <|#eta|<2.2","l")
#leg1.AddEntry(tA_EE_low4,"2.2 <|#eta|<2.4","l")
#leg1.AddEntry(tA_EE_low5,"2.4 <|#eta|<2.6","l")
#leg1.AddEntry(tA_EE_low6,"2.6 <|#eta|<2.8","l")
#leg1.AddEntry(tA_EE_low7,"2.8 <|#eta|<3.0","l")
leg3.Draw()

# Number of Jets
c4 = TCanvas("c4", "Number of Jets", 800, 800)
c4.cd()
# ht->GetXaxis()->SetTitle("Energy(GeV)");
#  ht->GetYaxis()->SetTitle("Mean Time(ns)");
#  ht->GetYaxis()->SetRangeUser(-0.8,0.3);
#  ht->GetXaxis()->SetRangeUser(0, 10000);

njets1.SetTitle("Number of Leading Pt Jets")
njets1.Draw()
njets2.Draw("sames")
njets3.Draw("sames")
njets1.GetYaxis().SetRangeUser(1, 100000)
#Add Legend
leg4 = TLegend( 0.44, 0.70, 0.77, 0.90)
leg4.SetBorderSize(1)
leg4.SetFillColor(0)
leg4.AddEntry(njets1,"< 3Jets + VLIso#gamma","l")
leg4.AddEntry(njets2,">= 3Jets + VLIso#gamma ","l")
leg4.AddEntry(njets3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
leg4.SetTextSize(0.018)
leg4.SetHeader("Samples")
leg4.Draw()


# Compare Times
c5 = TCanvas("c5", "Photon Time Def", 800, 800)
c5.cd()

SeedXtalTime.SetTitle("Photon Time Def")
SeedXtalTime.Draw()
WaBCTime.Draw("sames")
WaSCTime.Draw("sames")
SeedXtalTime.GetXaxis().SetRangeUser(-25.0, 25.0)
#Add Legend
leg5 = TLegend( 0.14, 0.70, 0.37, 0.90)
leg5.SetBorderSize(1)
leg5.SetFillColor(0)
leg5.AddEntry(SeedXtalTime,"Seed Xtal Time","l")
leg5.AddEntry(WaBCTime,"W.Ave Seed BC Time","l")
leg5.AddEntry(WaSCTime,"W.Ave SC Time","l")
leg5.SetTextSize(0.018)
leg5.SetHeader("Photon Time Def")
leg5.Draw()

  
#_______________________ Save Plots_______________________#

#c1.SaveAs("PhotonTimeComparing.png")
c1.SetLogy(1)
#c1.SaveAs("PhotonAveSeedBCTime_Met60CutCompareLog.png")
#c1.SaveAs("Photon_SeedXtalTime_Met60CutCompareLog.png")
c1.SaveAs("Photon_WAveSCTime_Met60CutCompareLog.png")

#c2.SaveAs("EventMet.png")
c2.SetLogy(1)
c2.SaveAs("EventMet_Met60CutComparingLog.png")

c3.SetLogy(1)
c3.SaveAs("NumberPhotons_Met60CutComparingLog.png")

c4.SetLogy(1)
c4.SaveAs("NumberOfJets_Met60CutComparingLog.png")

c5.SetLogy(1)
c5.SaveAs("Photon_Time_Defination_Compare_GMSB_Sum2012_Met60Cut.png")




