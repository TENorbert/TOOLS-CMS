#!/usr/bin/env python
#
# Creates a histogram that is a function of
# other histograms, in other files.
# Run by typing:
# $  ./bkgrd_subtract.py
#
# Michael Anderson, Oct 2010

from ROOT import TFile, TH1F

##### Variables #####
file0 = TFile("Hists_Data.root", "read")
file1 = TFile("Hists_QCD.root", "read")
file2 = TFile("Hists_Wenu.root", "read")

out_file = TFile("data_sub_bgd.root", "update")

data = file0.Get('pass_MtMetSumEt/h_photon_pt')
mcbg = file1.Get('pass_MtMetSumEt/h_photon_pt')
mcsig = file2.Get('pass_MtMetSumEt/h_photon_pt')

##### Create histogram that is a function of these 3 histograms #####
out_hist = data.Clone()
for i in range(0, data.GetNbinsX()): # bin 0 = underflow, Nbins = overflow
    if (mcsig.GetBinContent(i) + mcbg.GetBinContent(i)>0):
        # Reduce the data histogram my the % of background predicted by MC to be in it
        content = data.GetBinContent(i)*( 1 - mcbg.GetBinContent(i)/(mcsig.GetBinContent(i) + mcbg.GetBinContent(i)) )
    else:
        content = 0
    error = data.GetBinError(i)   # Find a function that actually propagates error
    out_hist.SetBinContent(i, content)
    out_hist.SetBinError(i, error)

out_hist.Write()
out_file.Close()
print "Created %s" % out_file.GetName()
