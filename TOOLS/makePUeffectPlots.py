# This Tool actually Just Overlays Different Histograms
# @ La TEN

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
gStyle.SetOptStat(0)

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


##-------------MET60Cut-----------------------
#f1 =   TFile("met50Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")
#f2 =   TFile("met50Cut_MoreThan3Jets_DP_All_Trig_NoJetID.root")
#f3 =   TFile("metcut50_ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")

##------------------No-OffLine-MetCUt------------------
f1 =   TFile("LessThan3Jets_DP_All_Trig_NoJetID.root")
f2 =   TFile("MoreThan3Jets_DP_All_Trig_NoJetID.root")
f3 =   TFile("ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root")



# compare Photon Time Def for DIff Samples
#g1 =   TFile("met50Cut_LessThan3Jets_DP_All_Trig_NoJetID.root") 
#g2 =   TFile("met50Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")
#g3 =   TFile("met50Cut_LessThan3Jets_DP_All_Trig_NoJetID.root")
 

#Ecal Iso
EcalIso1 = f1.Get("NeutKin/my.ecalIso")
EcalIso2 = f2.Get("NeutKin/my.ecalIso")
EcalIso3 = f3.Get("NeutKin/my.ecalIso")

# HcalIso
HcalIso1 = f1.Get("NeutKin/my.hcalIso")
HcalIso2 = f2.Get("NeutKin/my.hcalIso")
HcalIso3 = f3.Get("NeutKin/my.hcalIso")

# TrkIso
TrkIso1 = f1.Get("NeutKin/my.trkIso")
TrkIso2 = f2.Get("NeutKin/my.trkIso")
TrkIso3 = f3.Get("NeutKin/my.trkIso")

# 2012 HcalIso
nHcalIso1 = f1.Get("NeutKin/my.HcalIso")
nHcalIso2 = f2.Get("NeutKin/my.HcalIso")
nHcalIso3 = f3.Get("NeutKin/my.HcalIso")

# sMinor
sminor1 = f1.Get("NeutKin/my.sminor")
sminor2 = f2.Get("NeutKin/my.sminor")
sminor3 = f3.Get("NeutKin/my.sminor")

# EcalTiming Vs PU
photVtx1= f1.Get("NeutKin/my.tV1")
photVtx2= f1.Get("NeutKin/my.tV2")
photVtx3= f1.Get("NeutKin/my.tV3")
photVtx4= f1.Get("NeutKin/my.tV4")
photVtx5= f1.Get("NeutKin/my.tV5")
photVtx6= f1.Get("NeutKin/my.tV6")
photVtx7= f1.Get("NeutKin/my.tV7")
# photVtx8= f1.Get("NeutKin/my.tV8")
#  photVtx9= f1.Get("NeutKin/my.tV9")
#   photVtx10= f1.Get("NeutKin/my.tV10")
#   photVtx11= f1.Get("NeutKin/my.tV11")

#  W AVe SC Time
photVtx12= f1.Get("NeutKin/my.tV12")
photVtx13= f1.Get("NeutKin/my.tV13")
photVtx14= f1.Get("NeutKin/my.tV14")
photVtx15= f1.Get("NeutKin/my.tV15")
photVtx16= f1.Get("NeutKin/my.tV16")
photVtx17= f1.Get("NeutKin/my.tV17")
photVtx18= f1.Get("NeutKin/my.tV18")
# photVtx19= f1.Get("NeutKin/my.tV19")
# photVtx20= f1.Get("NeutKin/my.tV20")
# photVtx21= f1.Get("NeutKin/my.tV21")
# photVtx22= f1.Get("NeutKin/my.tV22")

#  W AVe Seed BC Time
phoWAvSBCtime13= f1.Get("NeutKin/my.py13")
phoWAvSBCtime14= f1.Get("NeutKin/my.py14")
phoWAvSBCtime15= f1.Get("NeutKin/my.py15")
phoWAvSBCtime16= f1.Get("NeutKin/my.py16")
phoWAvSBCtime17= f1.Get("NeutKin/my.py17")
phoWAvSBCtime18= f1.Get("NeutKin/my.py18")
phoWAvSBCtime19= f1.Get("NeutKin/my.py19")

# Set Colors
EcalIso1.SetLineColor(6)
EcalIso2.SetLineColor(9)
EcalIso3.SetLineColor(12)

HcalIso1.SetLineColor(6)
HcalIso2.SetLineColor(9)
HcalIso3.SetLineColor(12)

TrkIso1.SetLineColor(6)
TrkIso2.SetLineColor(9)
TrkIso3.SetLineColor(12)

nHcalIso1.SetLineColor(6)
nHcalIso2.SetLineColor(9)
nHcalIso3.SetLineColor(12)

sminor1.SetLineColor(6)
sminor2.SetLineColor(9)
sminor3.SetLineColor(12)

#  W Ave Seed  BC Time
photVtx1.SetLineColor(1)
photVtx2.SetLineColor(2)
photVtx3.SetLineColor(3)
photVtx4.SetLineColor(4)
photVtx5.SetLineColor(5)
photVtx6.SetLineColor(6)
photVtx7.SetLineColor(7)
# photVtx8.SetLineColor(8)
# photVtx9.SetLineColor(9)
# photVtx10.SetLineColor(10)
# photVtx11.SetLineColor(11)

#  W Ave SC Time
photVtx12.SetLineColor(1)
photVtx13.SetLineColor(2)
photVtx14.SetLineColor(3)
photVtx15.SetLineColor(4)
photVtx16.SetLineColor(5)
photVtx17.SetLineColor(6)
photVtx18.SetLineColor(7)
# photVtx19.SetLineColor(8)
# photVtx20.SetLineColor(9)
# photVtx21.SetLineColor(10)
# photVtx22.SetLineColor(11)


# W Ave Seed BC Time
phoWAvSBCtime13.SetLineColor(2)
phoWAvSBCtime14.SetLineColor(3)
phoWAvSBCtime15.SetLineColor(4)
phoWAvSBCtime16.SetLineColor(5)
phoWAvSBCtime17.SetLineColor(6)
phoWAvSBCtime18.SetLineColor(7)
phoWAvSBCtime19.SetLineColor(8)

# Set Marker Style
EcalIso1.SetMarkerStyle(1)
EcalIso2.SetMarkerStyle(3)
EcalIso3.SetMarkerStyle(5)

HcalIso1.SetMarkerStyle(1)
HcalIso2.SetMarkerStyle(3)
HcalIso3.SetMarkerStyle(5)

TrkIso1.SetMarkerStyle(1)
TrkIso2.SetMarkerStyle(3)
TrkIso3.SetMarkerStyle(5)

nHcalIso1.SetMarkerStyle(1)
nHcalIso2.SetMarkerStyle(3)
nHcalIso3.SetMarkerStyle(5)

sminor1.SetMarkerStyle(1)
sminor2.SetMarkerStyle(3)
sminor3.SetMarkerStyle(5)

# Scale Histograms
if EcalIso1.Integral()!=0: 
                          EcalIso1.Scale(1/EcalIso1.Integral())
if EcalIso2.Integral()!=0: 
                          EcalIso2.Scale(1/EcalIso2.Integral())
if EcalIso3.Integral()!=0: 
                          EcalIso3.Scale(1/EcalIso3.Integral())

if HcalIso1.Integral()!=0:
                          HcalIso1.Scale(1/HcalIso1.Integral())
if HcalIso2.Integral()!=0:
                          HcalIso2.Scale(1/HcalIso2.Integral())
if HcalIso3.Integral()!=0:
                          HcalIso3.Scale(1/HcalIso3.Integral())

if TrkIso1.Integral()!=0:
                         TrkIso1.Scale(1/TrkIso1.Integral())
if TrkIso2.Integral()!=0: 
                         TrkIso2.Scale(1/TrkIso2.Integral())
if TrkIso3.Integral()!=0: 
                         TrkIso3.Scale(1/TrkIso3.Integral())

if nHcalIso1.Integral()!=0: 
                          nHcalIso1.Scale(1/nHcalIso1.Integral())
if nHcalIso2.Integral()!=0: 
                          nHcalIso2.Scale(1/nHcalIso2.Integral())
if nHcalIso3.Integral()!=0: 
                          nHcalIso3.Scale(1/nHcalIso3.Integral())

if sminor1.Integral()!=0:
                         sminor1.Scale(1/sminor1.Integral())
if sminor2.Integral()!=0: 
                         sminor2.Scale(1/sminor2.Integral())
if sminor3.Integral()!=0: 
                         sminor3.Scale(1/sminor3.Integral())

if photVtx1.Integral()!=0: 
                          photVtx1.Scale(1/photVtx1.Integral())
if photVtx2.Integral()!=0: 
                          photVtx2.Scale(1/photVtx2.Integral())
if photVtx3.Integral()!=0: 
                          photVtx3.Scale(1/photVtx3.Integral())
if photVtx4.Integral()!=0: 
                          photVtx4.Scale(1/photVtx4.Integral())
if photVtx5.Integral()!=0: 
                          photVtx5.Scale(1/photVtx5.Integral())
if photVtx6.Integral()!=0: 
                          photVtx6.Scale(1/photVtx6.Integral())
if photVtx7.Integral()!=0: 
                          photVtx7.Scale(1/photVtx7.Integral())

# if photVtx8.Integral()!=0:  photVtx8.Scale(1/photVtx8.Integral())
# if photVtx9.Integral()!=0:  photVtx9.Scale(1/photVtx9.Integral())
# if photVtx10.Integral()!=0:  photVtx10.Scale(1/photVtx10.Integral())
# if photVtx11.Integral()!=0:  photVtx11.Scale(1/photVtx11.Integral())


# W AVe SC Time
if photVtx12.Integral()!=0: 
                           photVtx12.Scale(1/photVtx12.Integral())
if photVtx13.Integral()!=0: 
                           photVtx13.Scale(1/photVtx13.Integral())
if photVtx14.Integral()!=0: 
                           photVtx14.Scale(1/photVtx14.Integral())
if photVtx15.Integral()!=0: 
                           photVtx15.Scale(1/photVtx15.Integral())
if photVtx16.Integral()!=0: 
                           photVtx16.Scale(1/photVtx16.Integral())
if photVtx17.Integral()!=0:
                           photVtx17.Scale(1/photVtx17.Integral())
if photVtx18.Integral()!=0: 
                           photVtx18.Scale(1/photVtx18.Integral())


# if photVtx19.Integral()!=0:  photVtx19.Scale(1/photVtx19.Integral())
# if photVtx20.Integral()!=0:  photVtx20.Scale(1/photVtx20.Integral())
# if photVtx21.Integral()!=0:  photVtx21.Scale(1/photVtx21.Integral())
# if photVtx22.Integral()!=0:  photVtx22.Scale(1/photVtx22.Integral())

#  W AVe seed BC Time
if phoWAvSBCtime13.Integral()!=0: 
                                 phoWAvSBCtime13.Scale(1/phoWAvSBCtime13.Integral())
if phoWAvSBCtime14.Integral()!=0: 
                                 phoWAvSBCtime14.Scale(1/phoWAvSBCtime14.Integral())
if phoWAvSBCtime15.Integral()!=0: 
                                 phoWAvSBCtime15.Scale(1/phoWAvSBCtime15.Integral())
if phoWAvSBCtime16.Integral()!=0: 
                                 phoWAvSBCtime16.Scale(1/phoWAvSBCtime16.Integral())
if phoWAvSBCtime17.Integral()!=0: 
                                  phoWAvSBCtime17.Scale(1/phoWAvSBCtime17.Integral())
if phoWAvSBCtime18.Integral()!=0: 
                                  phoWAvSBCtime18.Scale(1/phoWAvSBCtime18.Integral())
if phoWAvSBCtime19.Integral()!=0: 
                                  phoWAvSBCtime19.Scale(1/phoWAvSBCtime19.Integral())


# Label Axis
EcalIso1.GetXaxis().SetTitle("nVtx")
EcalIso1.GetYaxis().SetTitle("EcalIso")
EcalIso1.SetTitle("EcalIso Vs PU")
HcalIso1.GetXaxis().SetTitle("nVtx")
HcalIso1.GetYaxis().SetTitle("HcalIso")
HcalIso1.SetTitle("HcalIso Vs PU")
TrkIso1.GetXaxis().SetTitle("nVtx")
TrkIso1.GetYaxis().SetTitle("TrkIso")
TrkIso1.SetTitle("TrackIso Vs PU")
nHcalIso1.GetXaxis().SetTitle("nVtx")
nHcalIso1.GetYaxis().SetTitle("2012 HcalIso")
nHcalIso1.SetTitle("2012 HcalIso")
sminor1.GetXaxis().SetTitle("nVtx")
sminor1.GetYaxis().SetTitle("sMinor")
sminor1.SetTitle("SMinor Vs PU")


photVtx6.GetXaxis().SetTitle("Photon Time[ns]")
photVtx6.GetYaxis().SetTitle("No. Events X10^4")
#photVtx6.GetYaxis().SetRangeUser(1.0,10000.0) 
photVtx6.Clear("")
photVtx6.SetTitle("Photon(Seed XTal) Time")

photVtx17.GetXaxis().SetTitle("Photon Time[ns]")
photVtx17.GetYaxis().SetTitle("No. Events X10^4")
#photVtx17.GetYaxis().SetRangeUser(1.0,10000.0) 
photVtx17.Clear("")
photVtx17.SetTitle("Photon(W. Ave SC) Time")

phoWAvSBCtime19.GetXaxis().SetTitle("Photon Time[ns]")
phoWAvSBCtime19.GetYaxis().SetTitle("No. Events X10^4")
#phoWAvSBCtime19.GetYaxis().SetRangeUser(1.0,10000.0)
phoWAvSBCtime19.Clear("")
phoWAvSBCtime19.SetTitle("Photon(W. Ave Seed BC) Time")


#_____________________Draw Plots ____________________#


c1 =   TCanvas("c1","EcalIso", 800, 800)
c1.cd()
EcalIso1.Draw()
EcalIso2.Draw("Sames")
EcalIso3.Draw("Sames")
lg1 =   TLegend(0.14, 0.70, 0.47, 0.90)
lg1.SetBorderSize(1)
lg1.SetFillColor(0)
lg1.AddEntry(EcalIso1, "< 3Jets + VLIso#gamma","l")
lg1.AddEntry(EcalIso2, ">=3Jets + VLIso#gamma","l")
lg1.AddEntry(EcalIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
lg1.SetTextSize(0.018)
lg1.SetHeader("Samples")
lg1.Draw()


c2 =   TCanvas("c2","HcalIso", 800, 800)
c2.cd()
HcalIso1.Draw()
HcalIso2.Draw("Sames")
HcalIso3.Draw("Sames")
lg2 =   TLegend(0.35, 0.20, 0.70, 0.47)
lg2.SetBorderSize(1)
lg2.SetFillColor(0)
lg2.AddEntry(HcalIso1, "< 3Jets + VLIso#gamma","l")
lg2.AddEntry(HcalIso2, ">=3Jets + VLIso#gamma","l")
lg2.AddEntry(HcalIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
lg2.SetTextSize(0.018)
lg2.SetHeader("Samples")
lg2.Draw()


c3 =   TCanvas("c3","TrkIso", 800, 800)
c3.cd()
TrkIso1.Draw()
TrkIso2.Draw("Sames")
TrkIso3.Draw("Sames")
lg3 =   TLegend(0.35, 0.20, 0.70, 0.45)
lg3.SetBorderSize(1)
lg3.SetFillColor(0)
lg3.AddEntry(TrkIso1, "< 3Jets + VLIso#gamma","l")
lg3.AddEntry(TrkIso2, ">=3Jets + VLIso#gamma","l")
lg3.AddEntry(TrkIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
lg3.SetTextSize(0.018)
lg3.SetHeader("Samples")
lg3.Draw()


c4 =   TCanvas("c4","nHcalIso", 800, 800)
c4.cd()
nHcalIso1.Draw()
nHcalIso2.Draw("Sames")
nHcalIso3.Draw("Sames")
lg4 =   TLegend(0.35, 0.20, 0.70, 0.45)
lg4.SetBorderSize(1)
lg4.SetFillColor(0)
lg4.AddEntry(nHcalIso1, "< 3Jets + VLIso#gamma","l")
lg4.AddEntry(nHcalIso2, ">=3Jets + VLIso#gamma","l")
lg4.AddEntry(nHcalIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
lg4.SetTextSize(0.018)
lg4.SetHeader("Samples")
lg4.Draw()


c5 =   TCanvas("c5","sminor", 800, 800)
c5.cd()
sminor1.Draw()
sminor2.Draw("Sames")
sminor3.Draw("Sames")
lg5 =   TLegend(0.14, 0.70, 0.47, 0.90)
lg5.SetBorderSize(1)
lg5.SetFillColor(0)
lg5.AddEntry(sminor1, "< 3Jets + VLIso#gamma","l")
lg5.AddEntry(sminor2, ">=3Jets + VLIso#gamma","l")
lg5.AddEntry(sminor3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l")
lg5.SetTextSize(0.018)
lg5.SetHeader("Samples")
lg5.Draw()


c6 =   TCanvas("c6","photonTimeVsPU", 800, 800)
c6.cd()
photVtx6.Draw()
photVtx2.Draw("Sames")
photVtx3.Draw("Sames")
photVtx4.Draw("Sames")
photVtx5.Draw("Sames")
photVtx1.Draw("Sames")
photVtx7.Draw("Sames")
# photVtx8.Draw("Sames")
# photVtx9.Draw("Sames")
# photVtx10.Draw("Sames")
# photVtx11.Draw("Sames")
lg6 =   TLegend(0.25, 0.65, 0.45, 0.90)
lg6.SetBorderSize(1)
lg6.SetFillColor(0)
lg6.AddEntry(photVtx1,"0<= nVtx <=7","l")
lg6.AddEntry(photVtx2,"7 <= nVtx <=14","l")
lg6.AddEntry(photVtx3,"14 < nVtx <=21","l")
lg6.AddEntry(photVtx4,"21 < nVtx <=28","l")
lg6.AddEntry(photVtx5,"28 < nVtx <=35","l")
lg6.AddEntry(photVtx6,"35 < nVtx <=42","l")
lg6.AddEntry(photVtx7,"42 < nVtx <=49","l")
# lg6.AddEntry(photVtx8,"35 < nVtx <=40","l")
# lg6.AddEntry(photVtx9,"40 < nVtx <=45","l")
# lg6.AddEntry(photVtx10,"45 < nVtx <=50","l")
# lg6.AddEntry(photVtx11,"50 < nVtx <=55","l")
lg6.SetTextSize(0.018)
lg6.SetHeader("NVertices")
lg6.Draw()


c7 =   TCanvas("c7","photonSCTimeVsPU", 800, 800)
c7.cd()
photVtx17.Draw("")
photVtx13.Draw("Sames")
photVtx14.Draw("Sames")
photVtx15.Draw("Sames")
photVtx16.Draw("Sames")
photVtx12.Draw("Sames")
photVtx18.Draw("Sames")
# photVtx19.Draw("Sames")
# photVtx20.Draw("Sames")
# photVtx21.Draw("Sames")
# photVtx22.Draw("Sames")
lg7 =   TLegend(0.25, 0.65, 0.45, 0.90)
lg7.SetBorderSize(1)
lg7.SetFillColor(0)
lg7.AddEntry(photVtx12,"0<= nVtx <=7","l")
lg7.AddEntry(photVtx13,"7<= nVtx <=14","l")
lg7.AddEntry(photVtx14,"14 < nVtx <=21","l")
lg7.AddEntry(photVtx15,"21 < nVtx <=28","l")
lg7.AddEntry(photVtx16,"28 < nVtx <=35","l")
lg7.AddEntry(photVtx17,"35 < nVtx <=42","l")
lg7.AddEntry(photVtx18,"42 < nVtx <=49","l")
# lg7.AddEntry(photVtx19,"35 < nVtx <=40","l")
# lg7.AddEntry(photVtx20,"40 < nVtx <=45","l")
# lg7.AddEntry(photVtx21,"45 < nVtx <=50","l")
# lg7.AddEntry(photVtx22,"50 < nVtx <=55","l")
lg7.SetTextSize(0.018)
lg7.SetHeader("NVertices")
lg7.Draw()


c8 =   TCanvas("c8","photonSeedBCTimeVsPU", 800, 800)
c8.cd()
phoWAvSBCtime19.Draw("")
phoWAvSBCtime14.Draw("Sames")
phoWAvSBCtime15.Draw("Sames")
phoWAvSBCtime16.Draw("Sames")
phoWAvSBCtime17.Draw("Sames")
phoWAvSBCtime18.Draw("Sames")
phoWAvSBCtime13.Draw("Sames")
lg8 =   TLegend(0.25, 0.65, 0.42, 0.90)
lg8.SetBorderSize(1)
lg8.SetFillColor(0)
lg8.AddEntry(phoWAvSBCtime13,"45 < nVtx <=55","l")
lg8.AddEntry(phoWAvSBCtime14,"35 < nVtx <=45","l")
lg8.AddEntry(phoWAvSBCtime15,"24 < nVtx <=35","l")
lg8.AddEntry(phoWAvSBCtime16,"18 < nVtx <=24","l")
lg8.AddEntry(phoWAvSBCtime17,"12 < nVtx <=18","l")
lg8.AddEntry(phoWAvSBCtime18,"6 < nVtx <=12","l")
lg8.AddEntry(phoWAvSBCtime19,"0 <= nVtx <=6","l")
lg8.SetTextSize(0.018)
lg8.SetHeader("NVertices")
lg8.Draw()



#__________________Save Plots_____________________#

c1.SetLogy()
c1.SaveAs("EcalIso_Met60Cut.png")
c2.SetLogy()
c2.SaveAs("HcalIso_Met60Cut.png")
c3.SetLogy()
c3.SaveAs("TrackIso_Met60Cut.png")
c4.SetLogy()
c4.SaveAs("NewHcalIso_Met60Cut.png")
c5.SetLogy()
c5.SaveAs("SMinor_Met60Cut.png")
c6.SetLogy()
c6.SaveAs("PhotonSeedTimeVsPU_Met60Cut.png")
c7.SetLogy()
c7.SaveAs("PhotonWAVeSCTimeVsPU_Met60Cut.png")
c8.SetLogy()
c8.SaveAs("PhotonWAVeSeedBCTimeVsPU_Met60Cut.png")











