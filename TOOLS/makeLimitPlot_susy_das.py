#!/usr/bin/env python

import commands
import os
import sys
import optparse
from math import exp, sqrt
from drawUtils import *
from susy_das_inputs import *
import ROOT

print "Getting options"

parser = optparse.OptionParser("usage: %prog [options]\
<input directory> \n")

parser.add_option ('--o', dest='outd', type='string',
                   default = '-1',
                   help="directory for output gifs and eps")

parser.add_option ('--do2sig', action="store_true",
                   dest="do2sig", default=False,
                   help="Draw 2 sigma band")


options, args = parser.parse_args()
outd   = options.outd
do2sig = options.do2sig

# Set up output directory
if outd == "-1" :
    outdir = "./"
else :
    outdir = outd+"/"
if not os.path.isdir(outdir) : os.system("mkdir "+outdir)

ROOT.gROOT.SetBatch()
ROOT.gROOT.SetStyle("Plain")

#ROOT.gStyle.SetOptStat(1111)
ROOT.gStyle.SetOptStat()
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetNdivisions(405,"x");
ROOT.gStyle.SetEndErrorSize(0.)
ROOT.gStyle.SetErrorX(0.001)

# Define lists for which mass points to plot
modList  = ["SS2", "SS3","SS4","SS5", "SS6", "SS7", "SS8","SS9","SS10", "SS11", "SS12", "SS13", "SS14", "SS15", "SS16", "SS17", "SS18"]
pdfList = modList
npts = len(modList)
npdf = len(pdfList)

# Define TGraphs
gr = {}
gr["obs"]  = ROOT.TGraph(npts)
gr["exp"]  = ROOT.TGraph(npts)
gr["sig"]  = ROOT.TGraph(npts)
gr["exp1"] = ROOT.TGraphAsymmErrors(npts)
gr["exp2"] = ROOT.TGraphAsymmErrors(npts)
gr["frq"]  = ROOT.TGraph(npts)
gr["1sig"] = ROOT.TGraph(npts)

gr["pdf", "hi"]  = ROOT.TGraph(npdf)
gr["pdf", "lo"]  = ROOT.TGraph(npdf)
gr["pdf"]        = ROOT.TGraphAsymmErrors(npdf)

# Exclusion dictionary needed for interpolating mass exclusion
exc = {}
exc["last" ,"exp"] = [0.0, 0.0, 0.0] 
exc["first","exp"] = [0.0, 0.0, 0.0] 
exc["last" ,"frq"] = [0.0, 0.0, 0.0] 
exc["first","frq"] = [0.0, 0.0, 0.0] 
exc["last" ,"obs"] = [0.0, 0.0, 0.0] 
exc["first","obs"] = [0.0, 0.0, 0.0] 

# Make PDF uncertainty band.
ipt = 0
for mod in pdfList:
    plo = systs[mod, "pdf"]*sigxsecs[mod]
    phi = systs[mod, "pdf"]*sigxsecs[mod]
    gr["pdf"] .SetPoint(ipt , sigmass[mod], sigxsecs[mod])
    gr["pdf"] .SetPointError(ipt , 0., 0., plo, phi)
    ipt += 1

ipt = 0
for mod in modList:
    # DAS_EX9
    # For each mass point set the x (mass) and y (cross section limit) values
    # of the TGraphs for "observed", "expected", "expected +- 1sd", "expected +- 2sd",
    # and "signal"

    #    gr["obs"] .SetPoint(ipt , , )
    #    gr["exp"] .SetPoint(ipt , , )
    #    gr["exp1"].SetPoint(ipt , , )
    #    gr["exp2"].SetPoint(ipt , , )
    #
    #
    #    gr["exp1"].SetPointError(ipt , , , ,)
    #    gr["exp2"].SetPointError(ipt , , , ,)
    #
    #    gr["sig"] .SetPoint(ipt , , )

    ipt += 1
    pdfCorr = 1.
    

    # Get numbers for interpolation.
    # To get mass limit, interpolate intersection of predicted cross
    # section and cross section limit between mass point points.
    
                     #Mass (x)          # Limit (y)                            #Xsec (y)
    forInterpExp = [sigmass[mod], limit_info[mod][1]*sigxsecs[mod], sigxsecs[mod]*pdfCorr]
    forInterpObs = [sigmass[mod], limit_info[mod][0]*sigxsecs[mod], sigxsecs[mod]*pdfCorr]
    
        
    if limit_info[mod][0]<pdfCorr: exc["last" ,"obs"] = forInterpObs
    elif exc["first" ,"obs"][0] == 0.0: exc["first" ,"obs"] = forInterpObs        
    
    if limit_info[mod][1]<pdfCorr: exc["last" ,"exp"] = forInterpExp
    elif exc["first" ,"exp"][0] == 0.0: exc["first" ,"exp"] = forInterpExp
        
# Print limit:
def slopePoint(x1,y1 , x2,y2):
    m = -99.
    if (x2-x1) != 0.:
        m = (y2-y1)/(x2-x1)
    b = -m*x1+y1
    return (m, b)

types = ["obs", "exp"]

# Do interpolation
mb = {}
lim = {}
for type in types:
    mb["sig",type] = slopePoint(exc["last",type][0], exc["last",type][2] , exc["first",type][0], exc["first",type][2])
    mb["lim",type] = slopePoint(exc["last",type][0], exc["last",type][1] , exc["first",type][0], exc["first",type][1])
    bdiff = mb["sig",type][1]-mb["lim",type][1]
    mdiff = mb["lim",type][0]-mb["sig",type][0] # notice switch sign or just use abs, i guess
    if mdiff != 0.:
        lim[type] = bdiff/mdiff
    else:
        lim[type] = -9999.


# Print interpolated mass limit:
if exc["last" ,"obs"] == 0.0 : print "No exclusion."
else: print  "Observed limit = %4.5f" % lim["obs"]


if exc["last" ,"exp"] == 0.0 : print "No expected exclusion."
else: print  "Expected limit = %4.5f" % lim["exp"]

# Make plots:
cname = "limit_scan"
if do2sig:
    cname += "_2sigband"

canv = ROOT.TCanvas(cname,cname,400,424)
pad=canv.GetPad(0)
pad.SetLogy()
pad.SetGridy()
pad.SetGridx()
setPadPasMargin(pad)

gr["exp1"].GetYaxis().SetLabelSize(0.055)
gr["exp1"].GetYaxis().SetTitleSize(0.055)
gr["exp1"].GetYaxis().SetTitleOffset(1.26)
gr["exp1"].GetXaxis().SetLabelSize(0.055)
gr["exp1"].GetXaxis().SetLabelSize(0.055)
gr["exp1"].GetXaxis().SetTitleSize(0.055)
gr["exp1"].GetXaxis().SetNdivisions(506)
gr["exp1"].GetXaxis().SetTitleOffset(1.15)
gr["exp1"].GetXaxis().SetLabelFont(62)
gr["exp1"].GetYaxis().SetLabelFont(62)
gr["exp1"].GetXaxis().SetTitleFont(62)
gr["exp1"].GetYaxis().SetTitleFont(62)
gr["exp1"].GetXaxis().SetNdivisions(504,1);
gr["exp1"].GetXaxis().SetTitle("M_{#tilde{q}} (GeV)")
gr["exp1"].GetYaxis().SetTitle("#bf{#sigma} (fb)")

gr["exp1"].GetYaxis().SetRangeUser(0.5, 400.)
gr["exp2"].GetYaxis().SetRangeUser(0.5, 400.)
gr["exp1"].GetXaxis().SetRangeUser(400., 2000.)
gr["exp2"].GetXaxis().SetRangeUser(400., 2000.)

gr["pdf" ].SetFillColor(46)

gr["pdf" ].SetLineColor(1)
gr["pdf" ].SetLineWidth(2)
gr["pdf" ].SetLineStyle(3)
gr["sig"] .SetLineColor(1)
gr["sig"] .SetLineWidth(2)
gr["sig"] .SetLineStyle(3)


gr["exp2"].SetFillColor(5)
gr["exp1"].SetFillColor(3)

gr["exp"] .SetLineWidth(2)
gr["exp"] .SetLineStyle(7)
gr["obs"] .SetLineWidth(2)

if do2sig:
    gr["exp2"].Draw("a3")
    gr["exp1"].Draw("3")
else:
    gr["exp1"].Draw("a3")
gr["exp"] .Draw("l")
gr["pdf"] .Draw("l3")
gr["sig"] .Draw("l")
gr["obs"] .Draw("l")

text = []
text2 = []
text.append("CMS Preliminary")
text.append("5 fb^{-1}, #sqrt{s} = 7 TeV")
text.append("M_{#tilde{#chi}^{0}} = 1/2#timesM_{#tilde{q}}")
text.append("M_{#tilde{s}}=100GeV, M_{s}=90GeV")
text.append("BR(#tilde{#chi}^{0}#rightarrow#gamma#tilde{s}) = 1")
textsize = 0.04; xstart = 0.5; ystart = 0.85; ystartleg = 0.0
latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextAlign(12)
latex.SetTextSize(textsize)
ntxt = 0
for itxt in text:
    ystartleg = ystart-ntxt*1.05*textsize
    latex.DrawLatex(xstart, ystart-ntxt*1.2*textsize, itxt)
    ntxt += 1
    ystartleg -= 2.0*textsize

nleglines = 4.
if do2sig: nleglines = 5.


legend = ROOT.TLegend(xstart, ystartleg-1.1*nleglines*textsize, xstart+0.4, ystartleg)
legend.SetFillColor(0)
legend.SetTextSize(textsize)
legend.SetColumnSeparation(0.0)
legend.SetEntrySeparation(0.1)
legend.SetMargin(0.2)
legend.AddEntry(gr["obs"]  , "Observed Limit","l")
legend.AddEntry(gr["exp"]  , "Expected Limit","l")
legend.AddEntry(gr["pdf"]  , "Stealth SUSY","lf")
legend.AddEntry(gr["exp1"] , "#pm1 SD Expected","f")
if do2sig:
    legend.AddEntry(gr["exp2"] , "#pm2 SD Expected","f")
legend.Draw()

pad.RedrawAxis()
for end in [".pdf",".gif"]:
    canv.SaveAs(outdir+cname+end)


