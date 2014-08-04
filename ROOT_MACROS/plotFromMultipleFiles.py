#
# Run by typing:
#   python plotFromMultipleFiles.py
#
# Creates histograms from TTree in several files
# that have different scales.
#
# Written in python for its power, and also because
# ROOT's C-interpreter (CINT) chokes and dies on
# code that tries to do things here.
#
# Michael Anderson
# Nov 4, 2009

from ROOT import TFile, TH1F, TH2F, TH3F, TTree # Import any ROOT class you want
from math import pi
from array import array                 # used to make Float_t array ROOT wants
from datetime import datetime           # used in output filename
import sys                              # used for exiting program

########################################
# Class for root files to store
# file's name, scale, cuts, etc...
class rootFile:
    def __init__(self, fileName, scale=1.0, cuts=""):
        self.name  = fileName
        self.scale = scale
        self.cuts  = cuts
        self.file  = TFile(fileName, "read")     # Open TFile
        if self.file.IsZombie():
            print "Error opening %s, exiting..." % self.name
            sys.exit(0)
        print "Opened %s, scale=%.2e, cuts='%s'" % (fileName, scale, cuts)
        self.ttree = TTree()                           # Create empty TTree, and
        try:                                           # try to get TTree from file.
            self.file.GetObject(ttreeName, self.ttree) # ttreeName set in variables below
        except:
            print "Error: %s not found in %s, exiting..." % (ttreeName, fileName)
            sys.exit(0)
########################################


########################################
#           Variables
invLum = 50.0      # Set inverse lum to scale plots to

ttreeName = "TreePhotonJet"  # TTree name in all files

#                        Filename,              scale=invLum*(cross section)/events analyzed,                cuts
listOfFiles = [rootFile("QCD_Pt15_Summer09.root",invLum*(1.457E9-1.091E7)/4.667392e+06,"event_genEventScale>15&&event_genEventScale<30"),
               rootFile("QCD_Pt30_Summer09.root",invLum*(1.091E7-1.93E6)/1.779232e+06, "event_genEventScale>30&&event_genEventScale<80"),
               rootFile("QCD_Pt80_Summer09.root",invLum*(1.93E6-6.2E4)/2.14780e+06,    "event_genEventScale>80&&event_genEventScale<170")]

# Cut(s) applied to all files
cutForAllFiles = "photon_et>15.0&&abs(photon_eta)<2.5"

outputFilename = "QCD_combined_%s.root" % datetime.now().strftime("%Y_%m_%d_%H_%M")

# Histogram bins
#   Converting with "array" is done because ROOT wants an array of Float_t
bins_et     = array('f', [15.0, 20.0, 27.0, 35.0, 45.0, 57.0, 72.0, 90.0, 120.0, 150.0, 200.0, 300.0, 400.0, 550.0])
bins_eta    = array('f', [-2.5, -1.55, -1.45, -0.9, 0.0, 0.9, 1.45, 1.55, 2.5])
bins_fisher = array('f', [i/20.0 for i in range(-40,21)]) # runs from -2.0 to 1.0 in steps of 0.05

#          Variable name (in TTree) : Histogram to plot into
listOfPlots = {'photon_et' : TH1F("photonEt",  "Photon E_{T} ;E_{T} (GeV);entries/bin"  , len(bins_et)-1 , bins_et  ),
               'photon_eta': TH1F("photonEta", "Photon #eta ;#eta;entries/bin"          , len(bins_eta)-1, bins_eta ),
               'photon_phi': TH1F("photonPhi", "Photon #phi ;#phi;entries/bin"          , 32, (-1.-1./15.)*pi, (1.+1./15.)*pi),
               'fishValue' : TH1F("photonFisherValue","Photon Fisher Value;fisher Value", 40, -2.0, 1.0),
               'fishValue:abs(photon_eta):photon_et' : TH3F("photonEtEtaFishValue", "Photon E_{T} vs |#eta| vs FishValue;E_{T};|#eta|;Fisher Value", len(bins_et)-1, bins_et, len(bins_eta)-1, bins_eta, len(bins_fisher)-1, bins_fisher)}
#        END of Varibles
########################################


########################################
# Open output file
outputFile = TFile(outputFilename, "recreate")
if not outputFile.IsZombie():
    print "Opened %s for output." % outputFilename
else:
    print "Error opening %s for output exiting..." % outputFilename
    sys.exit(0)

print "\nCuts applied to all files:\n  %s" % cutForAllFiles
print "Creating plots..."
# Loop over all things to plot
for plot in listOfPlots:
    print "  %s >> %s" % (plot, listOfPlots[plot].GetName())  # Say what plot is being made
    # Loop over all TTrees (from the different files)
    for aFile in listOfFiles:
        tempHist = listOfPlots[plot].Clone("temp")            # Create temp histogram
        cuts = "%s&&%s" % (cutForAllFiles, aFile.cuts)        # Set cuts
        aFile.ttree.Draw( "%s >> temp" % plot, cuts, "goff" ) # Draw into it (with cuts) graphics off
        tempHist.Scale(aFile.scale)                           # Scale it
        listOfPlots[plot].Add(tempHist)                       # Add it to total histogram
print "done."
########################################


########################################
#   Store and save/close files
outputFile.cd()
for plot in listOfPlots:
    listOfPlots[plot].Write()

print "Closing files...",
outputFile.Close()
for aFile in listOfFiles:
    aFile.file.Close()
print "done.\n\nHistograms stored in %s" % outputFilename
########################################
