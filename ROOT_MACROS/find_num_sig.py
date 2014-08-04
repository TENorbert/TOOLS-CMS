#!/usr/bin/env python
#
# Simple example script that uses RooFit to
# determine number of signal and background events in a data histogram.
# Requires 3 histograms (TH1F): data, mc_signal, mc_background.
# It basically determines the scales needed on mc_signal and mc_background
# to fit the data histogram.
#
# Fitting is done in get_num_sig_bkg().
# Setup of the histograms is done in main().
# Run this script by typing at the prompt:
# $ ./find_num_sig.py
#
# Michael Anderson, Jan 2011

import sys  # For exiting program

try:
    # ROOT stuff
    from ROOT import TFile, TTree, TH1F, TH2F, TH3F, gROOT
    # RooFit stuff
    from ROOT import RooAddPdf, RooArgList, RooArgSet, RooDataHist, RooFit, RooHistPdf, RooRealVar
except Exception, e:
    print e
    print ("Use a python that has PyROOT installed.")
    sys.exit(0)

def get_num_sig_bkg(hist_DataTemplate, hist_SignalTemplate, hist_BackgdTemplate,
                    fit_range_min, fit_range_max):
    '''Given 3 input histograms (TH1F), and a fit range, this function finds
    the amount of signal and background that sum up to the data histogram.
    It does histogram fits.'''
    # Find range of data template
    data_min = hist_DataTemplate.GetXaxis().GetXmin()
    data_max = hist_DataTemplate.GetXaxis().GetXmax()
    
    # Create basic variables
    x = RooRealVar("x","x",data_min,data_max)
    x.setBins(hist_DataTemplate.GetXaxis().GetNbins())  # Binned x values
    nsig = RooRealVar("nsig","number of signal events"    , 0, hist_DataTemplate.Integral())
    nbkg = RooRealVar("nbkg","number of background events", 0, hist_DataTemplate.Integral())
    
    # Create RooDataHists from input TH1Fs
    dh = RooDataHist("dh","dh",RooArgList(x),hist_DataTemplate)
    ds = RooDataHist("ds","ds",RooArgList(x),hist_SignalTemplate)
    db = RooDataHist("db","db",RooArgList(x),hist_BackgdTemplate)
    
    # Create Probability Distribution Functions from Monte Carlo
    sigPDF = RooHistPdf("sigPDF", "sigPDF", RooArgSet(x), ds)
    bkgPDF = RooHistPdf("bkgPDF", "bkgPDF", RooArgSet(x), db)
    
    model = RooAddPdf("model","(g1+g2)+a",RooArgList(bkgPDF,sigPDF),RooArgList(nbkg,nsig))
    
    # Find the edges of the bins that contain the fit range min/max
    data_min = hist_DataTemplate.GetXaxis().GetBinLowEdge(hist_DataTemplate.GetXaxis().FindFixBin(fit_range_min))
    data_max = hist_DataTemplate.GetXaxis().GetBinUpEdge(hist_DataTemplate.GetXaxis().FindFixBin(fit_range_max))
    
    r = model.fitTo(dh,RooFit.Save(),RooFit.Minos(0),RooFit.PrintEvalErrors(0),
                    RooFit.Extended(),RooFit.Range(data_min,data_max))
    r.Print("v")

    #print nsig.getVal(), nsig.getError(), nbkg.getVal(), nbkg.getError()
    return [nsig.getVal(), nsig.getError(), nbkg.getVal(), nbkg.getError()]
    
def main():
    # Open input files
    file_data   = TFile("Hists_Data.root", "read")
    file_signal = TFile("Hists_PhotonJet.root", "read")
    file_backgd = TFile("Hists_QCD.root", "read")
    
    # Get input TH1F
    hist_data = file_data.Get('g_pass_Liso_barrel/h_photon_sieie')
    hist_McSig = file_signal.Get('g_pass_Liso_barrel/h_photon_sieie')
    hist_McBkg = file_backgd.Get('g_pass_Liso_barrel/h_photon_sieie')

    # Determine amount of signal/background in data
    num_sig, num_sig_error, num_bkg, num_bkg_error  = \
        get_num_sig_bkg(hist_data, hist_McSig, hist_McBkg,0.006,0.02)
    
    # Save scaled MC plots to output root file
    file_output = TFile("Hists_purityFits.root","recreate")
    file_output.cd()
    hist_McBkg.Scale(num_bkg/hist_McBkg.Integral())
    hist_McSig.Scale(num_sig/hist_McSig.Integral())
    hist_data.SetName("data")
    hist_McBkg.SetName("MC_bgd")
    hist_McSig.SetName("MC_sig")
    hist_data.Write()
    hist_McBkg.Write()
    hist_McSig.Write()
    file_output.Close()
    
    print "Created %s" % file_output.GetName()
    
if __name__ == "__main__":
    main()