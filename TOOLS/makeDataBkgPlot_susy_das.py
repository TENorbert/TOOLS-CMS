#!/usr/bin/env python

print "Importing modules"
import sys
import optparse
import os
from drawUtils import *
from susy_das_inputs import *
from susy_das_functions import *

import ROOT
from math import exp, sqrt

#######################
# Get options
#######################

print "Getting options"

parser = optparse.OptionParser("usage: %prog [options]\
<input directory> \n")

parser.add_option ('--dat', dest='fdat', type='string',
                   default = 'susy_das_inputfiles/data_geq4jets_das_2col.txt',
                   help="Input data")
parser.add_option ('--o', dest='outd', type='string',
                   default = 'make',
                   help="directory for output gifs and eps")
parser.add_option ('--bw', dest='binWidth', type='float',
                   default = 100.,
                   help="Bin width.")

options, args = parser.parse_args()
outd     = options.outd
fdat     = options.fdat
binWidth = options.binWidth

def getPBI(obs, bkg, bkge):
    if not bkg > 0:
        print "Bkg must be >0 for ZBI.  Exiting."
        sys.exit()
    if obs==0: return 0
    tau = bkg/(bkge*bkge);
    n_off = tau*bkg;
    P_BI = ROOT.TMath.BetaIncomplete(1./(1.+tau), obs, n_off+1)
    return P_BI

if outd == "-1" :
    outdir = "./"
else :
    outdir = outd+"/"
if not os.path.isdir(outdir) : os.system("mkdir "+outdir)


print "Setting ROOT options"
ROOT.gROOT.SetBatch()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetNdivisions(405,"x");
ROOT.gStyle.SetEndErrorSize(0.)
ROOT.gStyle.SetErrorX(0.001)

#####################################
# Define things
#####################################

# We will make histograms for 3 events types: 4-jets, >=5-jets, and >=4-jets.
types =[4,5,"all"]
# Hardcode maximum y-value for each type and bin width (100 or 50 GeV):
max = {}
max["all", 100.] = 80.
max[4    , 100.] = 50.
max[5    , 100.] = 40.
max["all",  50.] = 55.
max[4    ,  50.] = 30.
max[5    ,  50.] = 25.


sig  = "SS7"
func = "f1"

# Define dictionaries
res = {}    # results
files  = {} # ascii input files
tfiles = {} # root tfiles
hists  = {} # histograms

stCut = sbrange[0]
fitrange = (stCut, 1500.) # for last data point



#fsig = '/uscms/home/jhirsch/CMSSW_5_3_0/CMSSW_5_3_0/test/flatfiles/MM7_st_all_jets4-5inc_ST'+str(int(stCut))+'_combIso.txt'
fsig = 'susy_das_inputfiles/SS7_geq4jets_das.txt'

files["sig"] = fsig
files["dat"] = fdat




############################################
# Get data and models
############################################
nevents = 0
for cat in ["sig", "dat"]:

    # First make ROOT TH1Fs for data and signal MC
    # Get number of bins based on range and     
    nbins = (fitrange[1]-fitrange[0])/binWidth
    # Make sure nbins is an integer.
    if nbins%1 != 0:
        print "nbins != integer.  Please debug.   Exiting."
        print "nbins != ", nbins%1 
        sys.exit()

    # Define histogram for >=4 jets
    name = "h"+cat
    hists[cat,"all"] = ROOT.TH1F(name, name, int(nbins), fitrange[0], fitrange[1])
    for njet in (4,5):
        # Define histograms for 4 jets and >=5 jets
        name = "h"+cat+"_"+str(njet)+"jets"
        hists[cat, njet] = ROOT.TH1F(name, name, int(nbins), fitrange[0], fitrange[1])
    
        
    # Open files and fill TH1Fs for data and sig
    lines = open(files[cat], 'r').readlines()
    for line in lines:
        # Get njets and ST from lines
        inj = int(line.split()[0])
        ist = float(line.split()[1])

        # Make sure that inj is 4 or 5
        if (cat, inj) not in hists: continue

        # Make sure that ST is in range
        if ist< fitrange[0] or ist>fitrange[1]: continue

        # Fill histograms
        hists[cat,   inj].Fill(ist)
        hists[cat, "all"].Fill(ist)

# Dictionaries that will hold graphs
gr = {}
grNorm = {}

nbins = int(nbins)
# For each event type define a TGraph for the data
# and a box for the normalization region
for type in types:
    gr    [type]      = ROOT.TGraphAsymmErrors(nbins)
    grNorm[type]      = ROOT.TBox(sbrange[0], 0., sbrange[1], max[type,binWidth])

    ipt = 0
    # Loop over data histogram and fill TGraph for plotting
    for ibin in range(1, nbins+1):
        # Get ST value and event counts
        st  = hists["dat", type].GetBinCenter(ibin)
        val = hists["dat", type].GetBinContent(ibin)
        # Compute Poisson error bars by hand:
        alpha = 1. - 0.6827
        qlo = 0.5*ROOT.TMath.ChisquareQuantile(alpha/2.,2*val)
        qhi0   = 0.5*ROOT.TMath.ChisquareQuantile(1.-alpha   ,2*(val+1))
        qhigt0 = 0.5*ROOT.TMath.ChisquareQuantile(1.-alpha/2.,2*(val+1))
        qhi = qhi0 if val==0 else qhigt0
        elo = val-qlo
        ehi = qhi-val

        # Set value and lower/upper errors for observation at ST=st:
        res[st, "obs"       , type] = val
        res[st, "obs", "elo", type] = elo
        res[st, "obs", "ehi", type] = ehi

        # Set points in graph
        if val!=0:
            gr[type].SetPoint(ipt, st, val)
        else:
            # set pts with val=0 to 0.0001 so that they appear on plot
            # y-min = 0.0001 so that "0" isn't drawn
            gr[type].SetPoint(ipt, st, val+0.0001)

        gr[type].SetPointError(ipt, 0., 0., elo, ehi)
        ipt += 1

    
#######################
# Make normalized background shapes
#######################

# Define Function
bkg  = {}
for type in types:
    bkg[type] = ROOT.TF1("tf1_bkg_"+str(type)   , str(binWidth)+"*[0]/(x/7000.)^[1]", fitrange[0], fitrange[1]) # multiply by binWidth
    bkg[type].SetParameter(0, 1.) # unnormalized
    bkg[type].SetParameter(1, par[func])
    bkg[type].SetLineWidth(2)
    bkg[type].SetLineStyle(7)
    bkg[type].SetLineColor(4)

# number in sideband
nsb = {}
nsb[4]     = float(nobs["sb", 4])
nsb[5]     = float(nobs["sb", 5])
nsb["all"] = nsb[4]+nsb[5]

# Integrate function over plot range -- make sure to divide by bin width
intgrl          = bkg["all"].Integral(fitrange[0], fitrange[1])/binWidth
# Get sideband fraction for plot range
# pdffrac_600_700 = bkg["all"].Integral(600., 700.)/binWidth/intgrl
pdffrac_600_700 = getPDFFrac(600., 700., 600., 1500., "f1", [par["f1"]])

# Compute normalization for background shape
norm = {}
for type in types:
    norm[type] = nsb[type]/pdffrac_600_700/intgrl

# Set normalization
bkg["all"].SetParameter(0, norm["all"])
bkg[4].SetParameter(0, norm[4])
bkg[5].SetParameter(0, norm[5])

#####################
# Make Error bands
#####################

# Compute error info with getErrVST() function
#stInfo = [lower end of range, upper end of range, ST step size (use 5 GeV)]
stInfo = [fitrange[0], fitrange[1], 5.]
errsVST  = getErrVST(stInfo)

errPts   = errsVST[0]
errIn    = errsVST[1]
errInPar = errsVST[2]
errInFnc = errsVST[3]



# Define graphs
err = {}
nbins = len(errPts)
for type in types:
    err[type] = ROOT.TGraphAsymmErrors(nbins)
    err[type, "hi"] = ROOT.TGraph(nbins)
    err[type, "lo"] = ROOT.TGraph(nbins)

# Fill graphs
ibin = 0
for ist in errPts:
    val = {}
    etot = {}
    for type in [4, 5]:
        # value is integral in bin
        val[type] = bkg[type].Integral(ist-binWidth/2., ist+binWidth/2.)/binWidth

        # Fill etot and err dictionaries for use later
        etot[type] = errIn[ist]*val[type]
        err[type].SetPoint(ibin, ist, val[type])
        err[type].SetPointError(ibin,  0., 0., etot[type], etot[type])
        err[type, "hi"].SetPoint(ibin, ist, val[type]+etot[type])
        err[type, "lo"].SetPoint(ibin, ist, val[type]-etot[type])
        if val[type] != bkg[type].Integral(ist-binWidth/2., ist+binWidth/2.)/binWidth: # sanity check, feel free to delete at any time
            print "val[type] != Integral of bkg[type]"
            sys.exit()
        # Fill res dictionary
        res[ist, "bkg"      , type] = val[type]
        res[ist, "bkg","elo", type] = etot[type]
        res[ist, "bkg","ehi", type] = etot[type]

    # Same as above for 4+5-jets
    type = "all"
    val[type] = val[4]+val[5]
    etot[type] = sqrt(etot[4]*etot[4]+etot[5]*etot[5])
    err[type].SetPoint(ibin, ist, val[type])
    err[type].SetPointError(ibin,  0., 0., etot[type], etot[type])
    err[type, "hi"].SetPoint(ibin, ist, val[type]+etot[type])
    err[type, "lo"].SetPoint(ibin, ist, val[type]-etot[type])
    res[ist, "bkg"      , type] = val[type]
    res[ist, "bkg","elo", type] = etot[type]
    res[ist, "bkg","ehi", type] = etot[type]
    ibin += 1

# Get integral of tail of background shape beyond edge of plot and error and put in res dictionary
bkgtail = {}
for type in types:
    bkgtail[type] = bkg[type].Integral(1500., 5000.)/binWidth
    res[errPts[len(errPts)-1]+binWidth+1., "bkg", type] = bkgtail[type]
    res[errPts[len(errPts)-1]+binWidth+1., "bkg", "elo", type] = bkgtail[type]*errIn[ist]
    res[errPts[len(errPts)-1]+binWidth+1., "bkg", "ehi", type] = bkgtail[type]*errIn[ist]
    

# Make dictionary for colors and styles and names
fill = {}
fill["col","nrm"] = 1
fill["col","sig"] = 1
fill["col","err"] = 38

fill["sty","nrm"] = 3353
fill["sty","sig"] = 1
fill["sty","err"] = 38


name = {}
name["all"] = "\\geq4"
name[4] = "4"
name[5] = "\\geq5"
canv = {}

#########################
# Make, draw, and save canvases
##########################
for type in types:
    cname = "dat_bkg_"+str(type)+"_bin"+str(int(binWidth))
    canv[type]  = ROOT.TCanvas(cname,cname,400,424)
    pad = canv[type].GetPad(0)
    setPadPasMargin(pad)

    #WORKING HERE
    hists["sig", type].GetYaxis().SetLabelSize(0.055)
    hists["sig", type].GetYaxis().SetTitleSize(0.055)
    hists["sig", type].GetYaxis().SetTitleOffset(1.26)
    hists["sig", type].GetXaxis().SetLabelSize(0.055)
    hists["sig", type].GetXaxis().SetLabelSize(0.055)
    hists["sig", type].GetXaxis().SetTitleSize(0.055)
    hists["sig", type].GetXaxis().SetNdivisions(508)
    hists["sig", type].GetXaxis().SetTitleOffset(1.15)
    hists["sig", type].GetXaxis().SetLabelFont(62)
    hists["sig", type].GetYaxis().SetLabelFont(62)
    hists["sig", type].GetXaxis().SetTitleFont(62)
    hists["sig", type].GetYaxis().SetTitleFont(62)
    hists["sig", type].GetXaxis().SetTitle("S_{T} (GeV)")
    hists["sig", type].GetYaxis().SetTitle("Events / (%i GeV)" % int(binWidth))
    
    hists["sig", type].GetXaxis().SetRangeUser(fitrange[0],fitrange[1])
    err[type].SetFillColor(fill["col","err"])

    # scale signal histogram by xsec*lumi/generated
    hists["sig", type].Scale(sigxsecs[sig]*lumi_all/nsiggen)
    hists["sig", type].GetYaxis().SetRangeUser(0.0001,max[type,binWidth]) # 0.0001 so that "0" isn't drawn
    hists["sig", type].SetLineColor(fill["col","sig"])
    hists["sig", type].SetLineWidth(2)
    hists["sig", type].Draw()
    grNorm[type].SetFillColor(fill["col","nrm"])
    grNorm[type].SetFillStyle(fill["sty","nrm"])
    grNorm[type].SetLineColor(fill["col","nrm"])
    grNorm[type].SetLineWidth(1)
    grNorm[type].Draw("l")
    vline = ROOT.TLine(700., 0., 700., max[type,binWidth])
    vline.SetLineWidth(1)
    vline.SetLineColor(1)
    vline.Draw()
    err   [type].Draw("3")
    err   [type, "hi"].Draw("l")
    err   [type, "lo"].Draw("l")
    bkg   [type].Draw("same")
    gr    [type].SetMarkerSize(0.9)
    gr    [type].SetMarkerStyle(20)
    gr    [type].Draw("p")
    pad.RedrawAxis()
    
    textsize = 0.04; xstart = 0.42; ystart = 0.85; ystartleg = 0.0
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAlign(12)
    latex.SetTextSize(textsize)
    text = []
    text.append("CMS DAS")
    text.append("5.03 fb^{-1}, \sqrt{s} = 7 TeV")
    ntxt = 0
    for itxt in text:
        ystartleg = ystart-ntxt*1.05*textsize
        latex.DrawLatex(xstart, ystart-ntxt*1.05*textsize, itxt)
        ntxt += 1
    ystartleg -= 1.05*textsize
    legend = ROOT.TLegend(xstart, ystartleg-6.6*textsize, xstart+0.50, ystartleg)
    legend.SetFillColor(0)
    legend.SetTextSize(textsize)
    legend.SetColumnSeparation(0.0)
    legend.SetEntrySeparation(0.1)
    legend.SetMargin(0.2)
    legend.AddEntry(gr [type] , "Data, "+name[type]+"-jets", "pe")
    legend.AddEntry(bkg[type] , "Expected Background", "l")
    legend.AddEntry(err[type] , "Syst. Uncertainty" , "f")
    mass = str(int(sigmass[sig]))
    legend.AddEntry(hists["sig", type] , "M_{#tilde{q}}="+mass+"GeV" , "l")
    legend.AddEntry(grNorm[type] , "S_{T} Sideband" , "f")
    legend.Draw()

    # Save
    for end in [".pdf",".gif"]:
        canv[type].SaveAs(outdir+cname+end)

##############################################
# Make tables of results
# Both bin-by-bin and tail comparisons
##############################################

# Make list of ST values
stList = []
nsteps = int((fitrange[1]-fitrange[0])/binWidth)

for ist in range(nsteps):
    stList.append(fitrange[0]+binWidth/2. + ist*binWidth)
tsList = list(stList)
# reverse the list
tsList.reverse()

# Add up bins for both observation (obs) and background expectation (bkg)
# sum uncertainties as well
runTot = {}
for type in [4,5]:
    runTot[type, "bkg"] = bkgtail[type] # starting with tail integral
    runTot[type, "obs"] = 0.
    runTot[type, "bkg", "err"] = bkgtail[type]
    runTot[type, "obs", "err"] = 0.
    for ist in tsList:
        runTot[type, "obs"]        += res[ist, "obs", type]
        runTot[type, "bkg"]        += res[ist, "bkg", type]
        res[ist, "obs", "int", type]        = runTot[type, "obs"]
        res[ist, "bkg", "int", type]        = runTot[type, "bkg"]
        res[ist, "bkg", "int", "err", type] = runTot[type, "bkg"] * errIn[ist]

# combine for 4+5 jet results
for type in ["all"]:
    for ist in tsList:
        res[ist, "obs", "int", type] = res[ist, "obs", "int", 4]+res[ist, "obs", "int", 5]
        res[ist, "bkg", "int", type] = res[ist, "bkg", "int", 4]+res[ist, "bkg", "int", 5]
        res[ist, "bkg", "int", "err", type] = sqrt(res[ist, "bkg", "int", "err", 4]*res[ist, "bkg", "int", "err", 4]+res[ist, "bkg", "int", "err", 5]*res[ist, "bkg", "int", "err", 5])

# Make and print tabulated results
zbi = {}
texfile = {}
doTex = False
if doTex:
    for type in types:
        for type2 in ["ex", "int"]:
            texfilename = "restab_"+str(type)+"_"+type2+"_bin"+str(int(binWidth))+".tex"
            texfile[type, type2] = open(texfilename, 'w')
            texfile[type, type2].write("\\begin{table}\n")
            texfile[type, type2].write("\\begin{center}\n")
            texfile[type, type2].write("\\begin{tabular}{cccc}\n")
            texfile[type, type2].write("\\hline\\hline\n")
            texfile[type, type2].write("$S_T$ Range (GeV) & $Background Expected & $Observed & Local $p$-value \\\\\n")
            texfile[type, type2].write("\\hline\n")

for type in types:
    print "\nResults for "+name[type]+"-jets"
    print "================================="
    for ist in stList:
        if type != "all":
            zbi[ist, type] = getPBI(res[ist, "obs", type], res[ist, "bkg", type], res[ist, "bkg", "ehi", type])
        else:
            zbi[ist, type] = getPBI(res[ist, "obs", type], res[ist, "bkg", type], res[ist, "bkg", "ehi", type])
            #            zbi[ist, type] = 2.*zbi[ist, 4]*zbi[ist, 5]

        
        line = "%4i-%4i &  %4.1f +%4.1f -%4.1f & %4.1f \pm %4.1f & %4.3f \\\\" % (int(ist-binWidth/2.),int(ist+binWidth/2.),
                                                                                  res[ist, "obs"       , type],
                                                                                  res[ist, "obs", "ehi", type],
                                                                                  res[ist, "obs", "elo", type],
                                                                                  res[ist, "bkg"       , type],
                                                                                  res[ist, "bkg", "ehi", type],
                                                                                  zbi[ist, type]
                                                                                  )
        print line
        if doTex:
            linetex = "%4i-%4i & $%4.1f \pm %4.1f$ & %4i & %4.3f \\\\\n" % (int(ist-binWidth/2.),int(ist+binWidth/2.),
                                                                          res[ist, "bkg"       , type],
                                                                          res[ist, "bkg", "ehi", type],
                                                                          int(res[ist, "obs"       , type]),
                                                                          zbi[ist, type]
                                                                          )
            texfile[type, "ex"].write(linetex)
            
# Exit here.  Print integrated numbers in next step        
sys.exit()
for type in types:
    print "\nResults for "+name[type]+"-jets"
    print "================================="
    print stList
    for ist in stList: #[650., 750., 850., 950., 1050., 1150., 1250., 1350., 1450.]:
        #        ist_int = int(ist-binWidth/2.)
        ist_int = int(ist)
        val = res[ist, "obs", "int", type]
        alpha = 1. - 0.6827
        qlo = 0.5*ROOT.TMath.ChisquareQuantile(alpha/2.,2*val)
        qhi0   = 0.5*ROOT.TMath.ChisquareQuantile(1.-alpha   ,2*(val+1))
        qhigt0 = 0.5*ROOT.TMath.ChisquareQuantile(1.-alpha/2.,2*(val+1))
        qhi = qhi0 if val==0 else qhigt0
        elo = val-qlo
        ehi = qhi-val
        if type != "all":
            zbi[ist, type,"int"] = getPBI(res[ist, "obs", "int",type], res[ist, "bkg","int", type], res[ist, "bkg", "int","err", type])
        else:
            zbi[ist, type,"int"] = getPBI(res[ist, "obs", "int",type], res[ist, "bkg","int", type], res[ist, "bkg", "int","err", type])

        line = "\\geq%4i &  %4.1f +%4.1f -%4.1f & %4.1f \pm %4.1f & %4.3f \\\\" % (ist_int,
                                                                                   res[ist_int, "obs","int"       , type],
                                                                                   res[ist_int, "obs","int", "ehi", type],
                                                                                   res[ist_int, "obs","int", "elo", type],
                                                                                   res[ist_int, "bkg","int"       , type],
                                                                                   res[ist_int, "bkg","int", "ehi", type],
                                                                                   zbi[ist, type,"int"]
                                                                                   )
        
        print line
        if doTex:
            linetex = "\\geq%4i & $%4.1f \pm %4.1f$ & %4i & %4.3f \\\\\n" % (int(ist-binWidth/2.),
                                                                                 res[ist_int, "bkg", "int"       , type],
                                                                                 res[ist_int, "bkg", "int", "ehi", type],
                                                                             int(res[ist_int, "obs", "int", type]),
                                                                             zbi[ist, type,"int"]
                                                                             )
            texfile[type, "int"].write(linetex)

            

        
if doTex:
    for type in types:
        for type2 in ["ex", "int"]:
            if type2 == "ex":
                caption = "$%s$-jet Sample: Numbers of expected background and observed events in exclusive %i GeV bins of $S_T$ with $p$-value of observation with respect to background expectation." % (name[type], int(binWidth))
            else:
                caption = "$%s$-jet Sample: Numbers of expected background and observed events in integrated bins of $S_T$ (with specified low edge) with $p$-value of observation with respect to background expectation." % name[type]
                
            if type == "all": ref = "data"
            else: ref = "data"+str(type)
            if binWidth != 50.: ref += "_"+str(int(binWidth))
            
            texfile[type, type2].write("\\hline\\hline\n")
            texfile[type, type2].write("\\end{tabular}\n")
            texfile[type, type2].write("\\caption{ %s }\n" % caption)
            texfile[type, type2].write("\\label{tab:%s}\n" % ref)
            texfile[type, type2].write("\\end{center}\n")
            texfile[type, type2].write("\\end{table}\n")
            texfile[type, type2].close()

        
# Make PAS table
if binWidth != 50. : sys.exit()
ref = "res"

texfilename = "restab_pas.tex" # 50 GeV bin, 4- and >=5-jet in same table, no p-value
type = "pas"
texfile[type] = open(texfilename, 'w')
texfile[type].write("\\begin{table}\n")
texfile[type].write("\\begin{center}\n")
texfile[type].write("\\begin{tabular}{ccccc}\n")
texfile[type].write("\\hline\\hline\n")
texfile[type].write("\\multirow{2}{*}{$S_T$ Range (GeV)} & \\multicolumn{2}{c}{4-jet Data} & \\multicolumn{2}{c}{$\geq$5-jet Data} \\\\\n")
texfile[type].write(" & Expected Background & Observed & Expected Background & Observed \\\\\n")
texfile[type].write("\\hline\n")
for ist in stList: #[650., 750., 850., 950., 1050., 1150., 1250., 1350., 1450.]:

    if int(ist+binWidth/2.)<1000.:
        rangeStr = "%3i-%3i" % (int(ist-binWidth/2.),int(ist+binWidth/2.))
    else:
        rangeStr = "%4i-%4i" % (int(ist-binWidth/2.),int(ist+binWidth/2.))

    linetex = "%10s & $%4.1f \pm %4.1f$ & %4i & $%4.1f \pm %4.1f$ & %4i \\\\\n" % (rangeStr,
                                                                                   res[ist, "bkg"       , 4],
                                                                                   res[ist, "bkg", "ehi", 4],
                                                                                   int(res[ist, "obs" , 4]),
                                                                                   res[ist, "bkg"       , 5],
                                                                                   res[ist, "bkg", "ehi", 5],
                                                                                   int(res[ist, "obs" , 5])
                                                                                   )


    texfile[type].write(linetex)

rangeStr = "$\\geq$1500"
ist = 1550.
linetex = "%10s & $%4.1f \pm %4.1f$ & %4i & $%4.1f \pm %4.1f$ & %4i \\\\\n" % (rangeStr,
                                                                               res[ist, "bkg"       , 4],
                                                                               res[ist, "bkg", "ehi", 4],
                                                                               0,
                                                                               res[ist, "bkg"       , 5],
                                                                               res[ist, "bkg", "ehi", 5],
                                                                               0
                                                                               )
texfile[type].write(linetex)

caption = "Numbers of expected background events and observed events in exclusive %i GeV bins of $S_T$ for the 4-jet and $\geq$5-jet multiplicity bins." % binWidth
texfile[type].write("\\hline\\hline\n")
texfile[type].write("\\end{tabular}\n")
texfile[type].write("\\caption{ %s }\n" % caption)
texfile[type].write("\\label{tab:%s}\n" % ref)
texfile[type].write("\\end{center}\n")
texfile[type].write("\\end{table}\n")
texfile[type].close()


