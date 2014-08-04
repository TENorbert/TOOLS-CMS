#!/usr/bin/env python
#########################################################################
#
# tprime.py
#
# Assorted data mining and other procedures for the CMS tprime analysis
#
# Usage: ./tprime.py --help
#
#   List available histograms with basic info, optionally merge EWK
#     ./tprime.py -l '*wildcard*' -f input.root [--merge-ewk]
#
#   Merge histograms, zero overflow, create 1D projections:
#     ./tprime.py -m -n ejets_merge [--clear-overflow] [--projection-x mass] [--merge-ewk]
#     ./tprime.py -m -f input1.root -f input2.root -o output.root
#                 [-s suffix1 -s suffix2...]
#                 [--merge-ewk] [--merge-top]
#                 [--strip-suffix] [--add-prefix 'pre_']
#                 [--add-pseudodata]
#                 [--clear-overflow]
#                 [--projection-x] [--projection-y]
#                 [--rebin-ngroup 2] [--rebin-low 200] [--rebin-high 800]
#                 [--scale-bin-error 1.41]
#
#   Model bias toy study
#     ./tprime.py -a toys
#     ./tprime.py --toys-pvalue
#
#   Mapping 2D templates on 1D
#     ./tprime.py --combine-bins brown (method name: brown, copy, mass)
#                 --mass 450
#                 --error '0.10'
#                 --max-err-sig '0.20'
#                 -f input.root -o output.root
#                 --channel 'mujets'
#                 [--make-plots]  # - makes fit plots and s/B plot
#                 [--scale-factor '1.0']
#                 [--add-prefix 'process_']
#
#   Counting:
#     ./tprime.py --combine-bins signal_box_1bin --mass 450.0 --channel mujets -f input_merged_mu.root -o output.root
#
#
#   Rename histograms
#     ./tprime.py --rename-hists 'h1=hMass:h2=hPt...' -f input1.root -f input2.root -o output.root
#
#   Limits and toys (settings in tprime_toys.py, RunToys()
#     ./tprime.py --toys limit
#     ./tprime.py --toys pvalue
#     ./tprime.py --toys templates
#
#   Fit
#     ./tprime.py --fit <type>
#                       type can be mike,plots,fit
#
#   Draw a bin map
#     ./tprime.py --bin-map dummy -f input.binmap --channel '#mu+jets'
#
# Author: Gena Kukartsev, May 2011
#
#########################################################################
from __future__ import division



########################################
#
# banner
#
def banner():
    print '''
+--------------------------------------------------------------
|
| tprime.py
|
| Assorted data mining and other procedures for the CMS tprime analysis
|
| author: Gena Kukartsev, May 2011
|
+--------------------------------------------------------------
    '''
banner()

_legend = '[tprime]:'



import sys, ROOT
from ROOT import TFile
from ROOT import TTree
from ROOT import TH1F
from ROOT import TCanvas
from ROOT import TLegend
from ROOT import SetOwnership
from ROOT import THStack
from ROOT import TLatex
from ROOT import TStyle
from ROOT import EColor


########################################
#
# Data
#
from load_data import TprimeData


########################################
#
# parse command line parameters
#
from optparse import OptionParser
add_help_option = "./tprime.py -ACTION [other options]"

parser = OptionParser(add_help_option)

parser.add_option("-t", "--test", dest="test", default=False,
                  help="Test",
                  action="store_true")

parser.add_option("-a", "--action", dest="action", default=None,
                  help="Action name", metavar="ACTION")

parser.add_option("-n", "--name", dest="name", default=None,
                  help="Name of an object, depends on specific usage", metavar="NAME")

#parser.add_option("-f", "--in-file", dest="in_file", default=None,
#                  help="Input file name", metavar="INFILE")
parser.add_option("-f", "--in-file", dest="in_file", default=None,
                  action="append",
                  help="Input file names", metavar="INFILE")

parser.add_option("-s", "--add-suffix", dest="suffix", default=None,
                  action="append",
                  help="Suffixes for hists in each input file, order must match the order of -f options")

parser.add_option("--add-prefix", dest="prefix", default=None,
                  action="append",
                  help="Prefix to be prepended to each hist name")

parser.add_option("-o", "--out-file", dest="out_file", default=None,
                  help="Output file name")

parser.add_option("-p", "--plot-hist", dest="hist_name", default=None,
                  help="Histogram name to plot", metavar="HISTNAME")

parser.add_option("-l", "--list-hist", dest="hist_pattern",
                  default=None,
                  help="List histograms with pattern in name",
                  metavar="HISTNAME")

parser.add_option("--merge-ewk", dest="merge_ewk",
                  default=False,
                  action="store_true",
                  help="Merge electroweak histos into one")

parser.add_option("--merge-top", dest="merge_top",
                  default=False,
                  action="store_true",
                  help="Merge electroweak histos into one")

parser.add_option("--strip-suffix", dest="strip_suffix",
                  default=False,
                  action="store_true",
                  help="Strip hist names of all starting with underscore")

parser.add_option("-d", "--dump-hists", dest="dump_hists",
                  action="store_true",
                  default=False,
                  help="dump all hists in a file")

parser.add_option("-v", "--dump-variables", dest="variables",
                  default=False,
                  action="store_true",
                  help="Print out available variables")

parser.add_option("-m", "--merge-files", dest="merge",
                  default=False,
                  action="store_true",
                  help="Merge and rename histograms from multiple files into one")

parser.add_option("--clear-overflow", dest="clear_overflow",
                  default=False,
                  action="store_true",
                  help="Set all under- and overflow bins to zero")

parser.add_option("--add-pseudodata", dest="add_pseudodata",
                  default=False,
                  action="store_true",
                  help="Add background-only pseudodata")

parser.add_option("--projection-x", dest="projection_x",
                  default=None,
                  action="store_true",
                  help="Project all 2D histogram onto X.")

parser.add_option("--projection-y", dest="projection_y",
                  default=None,
                  action="store_true",
                  help="Project all 2D histogram onto Y.")

parser.add_option("-r", "--reduce-input", dest="reduced_filename",
                  default=None,
                  help="Save some of the input histograms in a separate file",
                  metavar="REDUCE")

parser.add_option("--value", dest="value", default=None,
                  help="Value, depends on the context")

parser.add_option("--error", dest="error", default=None,
                  help="Error, depends on the context")

parser.add_option("--max-err-sig", dest="max_err_sig", default=10.0,
                  help="Error, depends on the context")

parser.add_option("--mass", dest="mass", default=None,
                  help="Invariant mass")

parser.add_option("--scale-factor", dest="scale_factor", default=1.0,
                  help="Scale factor")

parser.add_option("--channel", dest="channel", default=None,
                  help="Channel: ejets or mujets")

parser.add_option("--rename-hists", dest="rename_hists", default=None,
                  help="Specify histogram renaming scheme, e.g, hist1=hist_one:hist_e=histEnergy...")

parser.add_option("--rebin-ngroup", dest="rebin_ngroup", default=None,
                  help="How many adjacent bins to merge")

parser.add_option("--rebin-low", dest="rebin_low", default=None,
                  help="Low range bound for rebinned histogram")

parser.add_option("--rebin-high", dest="rebin_high", default=None,
                  help="High range bound for rebinned histogram")

parser.add_option("--combine-bins", dest="combine_bins",
                  default=None,
                  help="Call combine bins action, specify method: brown, brown2")

parser.add_option("--hist-factory", dest="hist_factory",
                  default=None,
                  action="store_true",
                  help="Run HistFactory")

parser.add_option("--toys-pvalue", dest="toys_pvalue",
                  default=None,
                  action="store_true",
                  help="Compute p-value distribution for a set of toy experiments")

parser.add_option("--toys", dest="toys",
                  default=None,
                  help="Run specified type of a toy study")

parser.add_option("--make-plots", dest="make_plots",
                  default=False,
                  action="store_true",
                  help="Create additional cross check plots")

parser.add_option("--scale-bin-error", dest="scale_bin_error",
                  default=None,
                  help="Multiply error in every histogram bin by the given factor")

parser.add_option("--fit", dest="fit",
                  default=None,
                  help="Run a fit using workspace")

parser.add_option("--bin-map", dest="bin_map",
                  default=None,
                  help="Study bin map")

parser.add_option("--powheg", dest="powheg",
                  default=False,
                  action="store_true",
                  help="Create powheg shape plus and minus templates")





print _legend, 'parsing command line options...',
(options, args) = parser.parse_args()
print 'done'

# single input file for backwards compatibility
if options.in_file:
    infile = options.in_file[0]
else:
    infile = None

name = options.name

doTest       = False
doPlotHist   = False
doDumpHists  = False
doListHists  = False
doDumpVariables = False
doReduce     = False
doInspectWorkspace = False
doMerge = False
doAddHist = False
doToys       = False

# define which plots to make based on params
if options.test:
    doTest = True

elif options.hist_name:
    doPlotHist = True

elif options.dump_hists:
    doDumpHists = True

elif options.hist_pattern:
    doListHists = True

elif options.variables:
    doDumpVariables = True

elif options.reduced_filename:
    doReduce = True

elif options.action and options.action == "ws_inspect":
    doInspectWorkspace = True

elif options.merge:
    doMerge = True

elif options.action and options.action == "add_hist":
    doAddHist = True

elif options.action and options.action == "toys":
    doToys = True

########################################
#
# ---- main
#

legend = '[tprime]:'

ds = TprimeData(821.0)

if doTest:
    print 'TEST'
    #ds.generate_hists(options.test)
    ds.load_all_hists(infile, merge_ewk=options.merge_ewk)
    _map={}
    _map['Ewk_ht35:fitMass'] =  392
    _map['Top_ht35:fitMass'] = 1685
    ds.CreatePseudoData('pseudoData', _map)
    ds.list(options.hist_pattern)

if (options.combine_bins) or (options.action and options.action == "combine_bins"):

    from combine_bins import *
    prefix = ''
    if options.prefix:
        prefix = options.prefix[0]

    CombineBins(options.mass, options.scale_factor,
                options.error, infile, options.out_file,
                prefix,
                method = options.combine_bins,
                make_plots = options.make_plots,
                maxErrSig = options.max_err_sig,
                channel = options.channel)

if doToys:
    print legend, 'beginning a toy study of possible model bias...'
    import tprime_toys as toys
    toys.run(ds)

if doPlotHist:
    ds.load_all_hists(infile, merge_ewk=options.merge_ewk)
    ds.hists[options.hist_name].Draw('H0')
    raw_input('press <enter> to continue...')
    
if doDumpHists:
    ds.load_all_hists(infile, merge_ewk=options.merge_ewk)
    ds.processes()
    #ds.list_cat('Top')
    #ds.list_process('Wjets')

if doListHists:
    ds.load_all_hists(infile, merge_ewk=options.merge_ewk)
    ds.list(options.hist_pattern)

if doDumpVariables:
    ds.load_all_hists(infile, merge_ewk=options.merge_ewk)
    ds.variables()

if doReduce:
    ds.load_all_hists(infile, merge_ewk=options.merge_ewk)
    ds.reduce(options.reduced_filename)

if doInspectWorkspace:
    if infile == None:
        print 'No input file specified, exiting...'
        sys.exit(-1)
    if options.name == None:
        print 'No object name specified, exiting...'
        sys.exit(-1)
    from tprime_ws import *
    ws_inspect(infile, options.name)


if doMerge:
    #
    # Usage:
    #
    #   Merge histograms, zero overflow, create 1D projections:
    #     ./tprime.py -m -n ejets_merge --clear-overflow --projection-x
    #

    legend = '[tprime merge]:'

    merge_ewk = options.merge_ewk
    merge_top = options.merge_top
    if merge_ewk:
        print legend, 'will merge electroweak templates into one'
    else:
        print legend, 'will NOT merge electroweak templates'
        print legend, 'call with --merge-ewk if desired'

    if merge_top:
        print legend, 'will merge top templates into one'
    else:
        print legend, 'will NOT merge top templates'
        print legend, 'call with --merge-top if desired'

    prefix = ''
    if options.prefix:
        prefix = options.prefix[0]

    # use command line options only
    if len(options.in_file)>0 and options.out_file:
        i_suff = 0
        for name in options.in_file:
            if options.suffix:
                _suffix = options.suffix[i_suff]
            else:
                _suffix = ''

            print legend, name, _suffix
            ds.load_all_hists(name,
                              merge_ewk=merge_ewk,
                              merge_top=merge_top,
                              strip_suffix = options.strip_suffix,
                              suffix = _suffix,
                              prefix = prefix,
                              projection_x = options.projection_x,
                              projection_y = options.projection_y,
                              rebin_ngroup = options.rebin_ngroup,
                              rebin_low = options.rebin_low,
                              rebin_high = options.rebin_high,
                              scale_bin_error = options.scale_bin_error)
            i_suff += 1

        # add pseudo data if requested
        if options.add_pseudodata:
            _map={}
            _map['ewk'] =  380
            _map['top'] = 1766
            ds.CreatePseudoData('DATA', _map)
            
        ds.SaveAllHists(options.out_file)
            
    if name == 'mujets_merge':
        ds.load_all_hists('data/mujets/5jets/v2/mujets_1889ipb_2D_v2.root', merge_ewk=merge_ewk)
        ds.load_all_hists('data/mujets/5jets/v2/mujets_1889ipb_2D_v2_JES_minus.root',"","_JESdown", merge_ewk=merge_ewk)
        ds.load_all_hists('data/mujets/5jets/v2/mujets_1889ipb_2D_v2_JES_plus.root',"","_JESup", merge_ewk=merge_ewk)
        ds.SaveAllHists('data/mujets/tprime_mujets_2D_1889ipb_5j_v2.root')

    if name == 'mujets':
        ds.LoadAllHists('tprime/data/mujets_305/statres_305ipb_v0.root')
        ds.LoadAllHists('tprime/data/mujets_305/statres_305ipb_v0_jes_m5.root',"","_JESdown")
        ds.LoadAllHists('tprime/data/mujets_305/statres_305ipb_v0_jes_p5.root',"","_JESup")
        ds.SaveAllHists('tprime/data/mujets_305/mujets_merged_305pb_v0_rebinned.root')
    elif name == 'mujets2':
        ds.LoadAllHists('tprime/data/mujets_305/statres_305ipb_v0.root', low = 100, high = 550, binwidth = 10, setOverflow = False)
        ds.LoadAllHists('tprime/data/mujets_305/statres_305ipb_v0_jes_m5.root',"","_JESdown", low = 100, high = 550, binwidth = 10, setOverflow = False)
        ds.LoadAllHists('tprime/data/mujets_305/statres_305ipb_v0_jes_p5.root',"","_JESup", low = 100, high = 550, binwidth = 10, setOverflow = False)
        ds.SaveAllHists('tprime/data/mujets_305/mujets_merged_305pb_v0_rebinned_100_550.root')

        

    elif name == 'ejets_merge':
        ds.load_all_hists('data/templates3.56invfb/5jets/ht_alljetsAbove35/ht35_vs_fitMass.root', merge_ewk=merge_ewk)
        ds.load_all_hists('data/templates3.56invfb/5jets/ht_alljetsAbove35/ht35_vs_fitMass_JES095.root',"","_JESdown", merge_ewk=merge_ewk)
        ds.load_all_hists('data/templates3.56invfb/5jets/ht_alljetsAbove35/ht35_vs_fitMass_JES105.root',"","_JESup", merge_ewk=merge_ewk)
        ds.SaveAllHists('data/ejets_3560/tprime_ejets_2D_3560ipb_ht35_14nov2011.root')

    elif name == 'ejets_merge_4jets':
        ds.load_all_hists('data/templates3.56invfb/5jets/ht_4jetsAfterfit/ht4jetsAfterFit_vs_fitMass.root', merge_ewk=merge_ewk)
        ds.load_all_hists('data/templates3.56invfb/5jets/ht_4jetsAfterfit/ht4jetsAfterFit_vs_fitMass_JES095.root',"","_JESdown", merge_ewk=merge_ewk)
        ds.load_all_hists('data/templates3.56invfb/5jets/ht_4jetsAfterfit/ht4jetsAfterFit_vs_fitMass_JES105.root',"","_JESup", merge_ewk=merge_ewk)
        ds.SaveAllHists('data/ejets_3560/tprime_ejets_2D_3560ipb_ht4_14nov2011.root')

            
    elif name == 'ejets134':
        ds.LoadAllHists('tprime/data/ejets_134/andrew_ele_134invpb_15jun2011.root', low = 100, high = 550, binwidth = 18)
        ds.SaveAllHists('tprime/data/ejets_134/ejets_merged_134invpb_20jun2011.root')

    # wprime stuff
    if name == 'wprime':
        ds.load_all_hists('data/Wprime_LimitInput_1BTag_wp1000L_InvariantMass_Jet1Jet2W_mu_Ge2Jet_100.root','','_L', merge_ewk=merge_ewk)
        ds.load_all_hists('data/Wprime_LimitInput_1BTag_wp1000R_InvariantMass_Jet1Jet2W_mu_Ge2Jet_100.root','','_R', merge_ewk=merge_ewk)
        ds.load_all_hists('data/Wprime_LimitInput_1BTag_wp1000RL_InvariantMass_Jet1Jet2W_mu_Ge2Jet_100.root','','_RL', merge_ewk=merge_ewk)
        ds.SaveAllHists('data/wprime.root')

if doAddHist:
    #
    # Usage:
    #
    #   Create and add new histograms to file. Create the file if needed.
    #   Currently used for creating inputs for a counting experiment HistFactory channel
    #
    #     ./tprime.py -a add_hist -f tprime_dilepton.root -n data --value 3.5 --error 1.2
    #

    import math
    import add_hist

    _values=[]
    _errors=[]

    if infile == None:
        print _legend, 'no file name specified, use -f file.root'
        print _legend, 'exiting...'
        sys.exit()
    else:
        _file_name = infile

    if options.name == None:
        print _legend, 'no histogram name specified, use -n name'
        print _legend, 'exiting...'
        sys.exit()
    else:
        _hist_name = options.name
        
    if options.value == None:
        print _legend, 'no histogram bin value specified, use --value value'
        print _legend, 'exiting...'
        sys.exit()
    else:
        #_values.clear()
        _values.append(options.value)

    #_errors.clear()
    
    if options.error == None:
        print _legend, 'no histogram bin error specified, using default sqrt(value)'
        print _legend, 'exiting...'
        _errors.append(math.sqrt(_bin_value))
    else:
        _errors.append(options.error)

    add_hist.add_new_hist(_file_name, _hist_name,
                          _values, _errors)


if options.rename_hists:
    if len(options.in_file)>0 and options.out_file:
        for name in options.in_file:
            ds.load_all_hists(name)

    ds.RenameHistograms(options.rename_hists)
    ds.SaveAllHists(options.out_file)



if options.hist_factory:
    import hist_factory as hf
    hf.run( mass = options.mass,
            in_file = options.in_file )



if options.toys_pvalue or options.toys:
    import tprime_toys as tt
    par = {}
    if options.toys:
        par['study'] = options.toys.strip()
    pvalue = tt.RunToy(par)
    print legend, 'p-value from a toy:', pvalue


if options.fit:
    if options.fit == 'mike':
        import tprime_fit as tf
        tf.Fit()
    else:
        import tprime_fit_gena as tf
        tf.Fit(options)
    

if options.bin_map:
    import tprime_map as tm
    tm.CountBins(options)


if options.powheg:
    import tprime_powheg as tp
    tp.CreateTemplates(options)
