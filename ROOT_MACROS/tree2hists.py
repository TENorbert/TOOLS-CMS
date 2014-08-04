#!/usr/bin/env python
"""
Create ROOT Histograms from one or more ROOT TTrees or TNtuples.

Options are specified in the given configuration file.
"""
## Created by Michael Anderson (mbanderson@wisc.edu), May 2010
#
# Create configuration file:
#   tree2hists.py
# Edit, then run with config file:
#   tree2hists.py config.py

######## Import python libraries #############################################

import sys                              # For exiting program
try:
    from ROOT import TFile, TTree, TH1F, TH2F, TH3F, gROOT
except Exception, e:
    print e
    print ("Use a python that has PyROOT installed.")
    sys.exit(0)
from copy import deepcopy     # For copying histograms
from math import pi           # For use in histogram bounds
from array import array       # For making Float_t array ROOT wants (for hists)
from datetime import datetime # For output filename
from os import path           # For finding file

######## Define classes and generators #######################################

class RootTree:
    """Wrapper for TTrees and TNtuples, allowing association with
    a scale and cuts."""
    def __init__(self, treeName, fileName, scale=1.0, cuts=""):
        self.fileName = fileName
        self.treeName = treeName
        self.scale    = scale
        self.cuts     = cuts
        self.tfile    = TFile()
        self.ttree    = TTree()

class Plot:
    """Wrapper for TH1 objects, associating TTree variables with a histogram"""
    def __init__(self, treeVariable, histogram, cuts="", storeErrors=True):
        self.treeVariable = treeVariable
        self.histogram    = histogram
        self.name         = histogram.GetName()
        self.cuts         = cuts
        if storeErrors: self.histogram.Sumw2()

def join_cuts(*list_of_cuts):
    """Joins list of cuts (strings) into something ROOT can handle.
    Example:  given ('1<2','','5>4') returns '1<2&&5>4'"""
    list_of_nonempty_cuts = []
    for cut in list_of_cuts:
        if cut:
            list_of_nonempty_cuts.append(cut)
    return '&&'.join(list_of_nonempty_cuts)

def duration_to_string(start, end):
    timeTaken = end - start
    hours, remainder = divmod(timeTaken.seconds, 3600)
    minutes, seconds = divmod(remainder, 60)
    if hours>0:
        return "%i hours, %i minutes" % (hours, minutes)
    elif minutes>0:
        return "%i minutes" % minutes
    return "%i seconds" % seconds                            

def write_default_T2H_config():
    """Writes configuration file for tree2hists"""
    defaultConfig = '''# Configuration file for tree2hists.py
# Created %s.
from tree2hists import RootTree, Plot, array, pi, TH1F

list_of_files = [RootTree("NTuples/Analysis", fileName="photons.root", scale=1.0, cuts=""),
                 RootTree("NTuples/Analysis", fileName="photons2.root", scale=1.0, cuts="")]

output_filename = "Hists_photons.root"

cut_for_all_files = "(!TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && vtxNdof>4 && abs(vtxZ)<=15)" + \\
                    "&&((isEB[0] && (seedSeverity[0]!=3 && seedSeverity[0]!=4 ) && (seedRecoFlag[0] != 2) ) || isEE[0])"

# All plots are made for each "cut set".
# A "cut set" is 3 things: folder name to store hists in, string to add to hist titles, and cuts for these hists.
# Let cut_sets = [] to make all plots.
cut_sets = [
    ("barrel15to20", "(|#eta|<1.45, 15<E_{T}<20)", "et[0]>15&&et[0]<20&&abs(eta[0])<1.45"),
    ("barrel20to30", "(|#eta|<1.45, 20<E_{T}<30)", "et[0]>20&&et[0]<30&&abs(eta[0])<1.45"),
    ("endcap15to20", "(1.7<|#eta|<2.5, 15<E_{T}<20)", "et[0]>15&&et[0]<20&&abs(eta[0])>1.7&&abs(eta[0])<2.5"),
    ("endcap20to30", "(1.7<|#eta|<2.5, 20<E_{T}<30)", "et[0]>20&&et[0]<30&&abs(eta[0])>1.7&&abs(eta[0])<2.5")
    ]

# Define histograms to plot
bins_et     = array("f", [15.0, 20.0, 30.0, 50.0, 80.0, 120.0])
bins_eta    = array("f", [-2.5, -1.7, -1.45, 0.0, 1.45, 1.7, 2.5])
list_of_plots = [
    Plot("et[0]"           , TH1F("pho_et"           , "Lead #gamma: E_{T};E_{T} (GeV);entries/bin", len(bins_et)-1, bins_et)),
    Plot("eta[0]"          , TH1F("pho_eta"          , "Lead #gamma: #eta;#eta;entries/bin"        , len(bins_eta)-1, bins_eta)),
    Plot("sigmaIetaIeta[0]", TH1F("pho_sigmaIEtaIEta", "Lead #gamma: #sigma_{i#etai#eta};#sigma_{i#etai#eta};entries/bin",20, 0, 0.06)),
    Plot("ESRatio[0]"      , TH1F("pho_ESRatio"      , "Lead #gamma: ESRatio (E3/E21);ESatio;entries/bin", 20, 0, 1.0),"abs(eta[0])>1.65"),
    Plot("metEt/et[0]"     , TH1F("metEt_over_phoEt" , "MET / E_{T}(#gamma);MET/E_{T}(sc);entries/bin"   , 20, 0.0, 3.0)),
    Plot("phi[0]:eta[0]"   , TH2F("phoPhi_vs_phoEta" , "Lead #gamma: #phi vs #eta;#eta;#phi"             , 50, -2.5, 2.5, 18, -pi, pi))
    ]
''' % datetime.now().strftime("%b %d, %Y")
    f = open('t2h_config.py', 'w')
    f.write(defaultConfig)
    f.close()
    print "Created default configuration file: t2h_config.py"
    print "Edit it, and run with:"
    print "  tree2hists.py t2h_config.py"
##############################################################################

def make_all_hists_all_files(list_of_RootTrees, list_of_Plots, cut_for_all_files, list_of_cutSets):
    '''Open root files one at a time, make plots, then close them.'''
    list_of_plots_to_write = []

    # Create plots for each set of cuts
    for set_of_cuts in list_of_cutSets:
        histname_fix, title_fix, current_cut_set = set_of_cuts
        for plot in list_of_Plots:
            new_plot  = deepcopy(plot)
            new_title = ' '.join((plot.histogram.GetTitle(), title_fix))
            new_plot.histogram.SetTitle(new_title)
            list_of_plots_to_write.append((new_plot, set_of_cuts))
    
    for j, rootTree in enumerate(list_of_RootTrees):
        rootTree.tfile = TFile(rootTree.fileName, "read")           # Open TFile
        if rootTree.tfile.IsZombie():
            print "Error opening %s, exiting..." % rootTree.fileName
            sys.exit(0)
        try:                                      # Try to get TTree from file.
            rootTree.tfile.GetObject(rootTree.treeName, rootTree.ttree)
        except:
            print "Error: %s not found in %s, exiting..." % (rootTree.treeName,
                                                             rootTree.fileName)
            sys.exit(1)

        print "\n%s: Opened %s  %i MB" % (datetime.now().strftime("%I:%M%p"),
                                          rootTree.fileName,
                                          path.getsize(rootTree.fileName)/1048576)
        print " %s has %i entries, will plot with scale=%.2e, cuts='%s'" % (rootTree.treeName,
                                                                            rootTree.ttree.GetEntries(),
                                                                            rootTree.scale,
                                                                            rootTree.cuts)
        
        # Loop over plots
        print "   # entries                  var >> histogram"
        for i, (plot, set_of_cuts) in enumerate(list_of_plots_to_write):
            histname_fix, title_fix, current_cut_set = set_of_cuts
            tmp_hist = plot.histogram.Clone("temp")    # Create temp hist
            all_cuts = join_cuts(cut_for_all_files, rootTree.cuts,
                                 current_cut_set, plot.cuts) # Set cuts
            rootTree.ttree.Draw( "%s >> temp" % plot.treeVariable, all_cuts,
                                 "goff")               # Draw with graphics off
            tmp_hist.Scale(rootTree.scale)             # Scale temp
            entries_before = plot.histogram.GetEntries()
            plot.histogram.Add(tmp_hist)               # Add temp hist to total
            entries_after = plot.histogram.GetEntries()
            print " %3i %7i %20s >> %s/%s" % (i, entries_after-entries_before,
                                              plot.treeVariable, histname_fix,
                                              plot.histogram.GetName()),
            if plot.cuts:
                print "\textra cuts: %s" % plot.cuts, # plot-specific cuts
            print
            
        rootTree.tfile.Close()                                    # Close TFile
        print "%s: Closed %s" % (datetime.now().strftime("%I:%M%p"),
                                 rootTree.fileName)
    return list_of_plots_to_write


######## Define the main program #############################################
def tree2hists_main(config_file):
    try:
        # Import only certain variables
        _temp = __import__(config_file, globals(), locals(),
                           ['list_of_files','output_filename',
                            'cut_for_all_files','cut_sets','list_of_plots'], -1)
        list_of_files     = _temp.list_of_files
        output_filename   = _temp.output_filename
        cut_for_all_files = _temp.cut_for_all_files
        cut_sets          = _temp.cut_sets
        list_of_plots     = _temp.list_of_plots
        for rootTree in list_of_files:
            if not path.isfile(rootTree.fileName):
                print "Error:\n  %s\nnot found for input." % rootTree.fileName
                sys.exit(1)
        hist_names = [plot.name for plot in list_of_plots]
        if len(hist_names)>len(set(hist_names)):
            print hist_names
            print "Error: Each plot needs a unique name, exiting..."
            sys.exit(1)
        if path.isfile(output_filename):
            print "Warning: %s exists" % output_filename
    except Exception, e:
        print e
        print "Error with %s" % config_file
        sys.exit(1)

    if path.isfile('rootlogon.C'):
        print "Loading rootlogon.C"
        gROOT.Macro('rootlogon.C')    # Load functions from rootlogon script

    if cut_sets:
        print "\n%i defined cut sets:" % len(cut_sets)
        for cut in cut_sets:
            name, title_fix, current_cut_set = cut
            print "  %s\t: '%s'" % (name, current_cut_set)
        cut_names = [name for name,num,cut in cut_sets]
        if len(cut_names)>len(set(cut_names)):
            print "Error: Each cut set needs a unique name, exiting..."
            sys.exit(1)
    else:
        cut_sets = [("","","")] # Make all plots, no extra cuts

    print "\nCuts to apply to all files:\n\t'%s'" % cut_for_all_files

    start_time = datetime.now()
    list_of_plots_to_write = make_all_hists_all_files(list_of_files,
                                                      list_of_plots,
                                                      cut_for_all_files,
                                                      cut_sets)
    end_time = datetime.now()
    print "Done drawing all plots after %s." % duration_to_string(start_time, end_time)

    #   Store and save/close files
    outputFile = TFile(output_filename, "recreate")
    if outputFile.IsZombie():
        print "Error opening %s for output exiting..." % output_filename
        sys.exit(1)
    print "\nOpened output file. Saving histograms..."
    outputFile.cd()
    for set_of_cuts in cut_sets:
        outputFile.mkdir(set_of_cuts[0])
    print "   # entries  histogram"
    for i, (plot, cutset) in enumerate(list_of_plots_to_write):
        outputFile.cd(cutset[0])
        print " %3i %7i  %s/%s" % (i, plot.histogram.GetEntries(),
                                   cutset[0],
                                   plot.histogram.GetName())
        plot.histogram.Write()
    outputFile.Close()
    print "Done saving."
    print "\nScaled & added histograms from %i TTrees saved in\n  %s" % (len(list_of_files), output_filename)
##############################################################################

if __name__ == "__main__":
    if len(sys.argv) > 1:
        if path.isfile(sys.argv[1]):
            config_file = sys.argv[1].split('.')[0]
            tree2hists_main(config_file)
        else:
            print "%s not found." % sys.argv[1]
            print("Create default config file by running tree2hists.py "
                  "with no argument.")
            sys.exit(1)
    else:
        if path.exists('t2h_config.py'):
            print "Specify a config file, like:"
            print "tree2hists.py t2h_config.py"
            sys.exit(1)
        write_default_T2H_config()
        sys.exit(0)
    #import cProfile
    #cProfile.run('main()', 'fooprof')
    #import pstats
    #p = pstats.Stats('fooprof')
    #p.sort_stats('cumulative').print_stats(15)
