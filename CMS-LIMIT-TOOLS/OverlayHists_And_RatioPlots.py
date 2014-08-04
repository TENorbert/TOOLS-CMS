#/usr/bin/env python
"""Overlay the histograms from several root files with identical structure"""
__version__ = "1.0"

## Created by Jeff Klukas (klukas@wisc.edu), November 2009
## Updated February 2010

######## Import python libraries #############################################

import sys
import optparse
import shutil
import os
import re

## If we actually plan to do something other than show the help menu, 
## import the PyROOT package
if '-h' not in sys.argv and len(sys.argv) > 1:
    import ROOT
    # ROOT parses options when the first ROOT command is called, so we must
    # add '-b' before that to get batch mode, but we must immediately remove
    # it to avoid interference with option parsing for this script.
    sys.argv.append('-b')
    ROOT.gErrorIgnoreLevel = ROOT.kWarning
    sys.argv.remove('-b')



######## Feel free to change these style options to suit you  ################

def add_style_options(options):
    """Define a set of global variables storing style information, etc."""
    GetColor = ROOT.TColor.GetColor
    options.colors = [
        ## a default set of contrasting colors the author happens to like
        GetColor( 82, 124, 219), # blue
        GetColor(212,  58, 143), # red
        GetColor(231, 139,  77), # orange
        GetColor(145,  83, 207), # purple
        GetColor(114, 173, 117), # green
        GetColor( 67,  77,  83), # dark grey
        ]
    options.marker_styles = [
        ## some of the more clear markers in root
         3, # asterisk
         4, # circle 
         5, # x
        25, # square
        26, # triangle
        27, # diamond
        28, # cross
        30, # five-pointed star
        ]
    return options



######## Define classes and generators #######################################

class RootFile:
    """A wrapper for TFiles, allowing quick access to the name and Get."""
    def __init__(self, file_name):
        self.name = file_name[0:-5]
        self.file = ROOT.TFile(file_name, "read")
        if self.file.IsZombie():
            print "Error opening %s, exiting..." % file_name
            sys.exit(1)
    def Get(self, object_name):
        return self.file.Get(object_name)

def counter_generator():
    """Incremement the counter used to number plots."""
    k = 0
    while True:
        k += 1
        yield k
next_counter = counter_generator().next



######## These functions are the meat of this program #########################

#### A recursive function to drill down through directories
def process_directory(path, files):
    """Loop through all histograms in the directory and plot them."""
    dir_to_make = "%s/%s" % (options.plot_dir, path)
    if not os.path.exists(dir_to_make):
        os.mkdir(dir_to_make)
    keys = files[0].file.GetDirectory(path).GetListOfKeys()
    key = keys[0]
    while key:
        obj = key.ReadObj()
        key = keys.After(key)
        new_path = "%s/%s" % (path, obj.GetName())
        if obj.IsA().InheritsFrom("TDirectory"):
            process_directory(new_path, files)
        #### If obj is a desired histogram, process it
        if (options.regex.search(new_path) and
            obj.IsA().InheritsFrom("TH1") and
            not obj.IsA().InheritsFrom("TH2") and
            not obj.IsA().InheritsFrom("TH3")):
            process_hist(path, new_path, files, obj)


#### This is where all the plotting actually happens
def process_hist(path, new_path, files, obj):
    """Overlay all the instances of this plot and apply the options."""
    counter = next_counter() # used for page numbers
    name = obj.GetName()
    hist = files[0].file.GetDirectory(path).Get(name)
    title = hist.GetTitle()
    x_title = hist.GetXaxis().GetTitle()
    y_title = hist.GetYaxis().GetTitle()
    if options.normalize or (options.sticky and "Norm" in name):
        y_title = "Fraction of Events in Bin"
    if options.normalize_to_file:
        file_name = files[int(options.normalize_to_file) - 1].name
        y_title = "Events Normalized to %s" % file_name
    hists = []
    #### Apply options to hist from each file
    for i, file in enumerate(files):
        hist = file.file.GetDirectory(path).Get(name)
        if not hist: continue
        hist.SetTitle(file.name)
        color = options.colors[i % len(options.colors)]
        hist.SetLineColor(color)
#         if options.fill:
#             r, g, b = plot_colors_rgb[i % len(colors)]
#             #fill_color = ROOT.TColor.GetColor(r * 1.2, g * 1.2, b * 1.2)
#             fill_color = color
#             hist.SetFillColor(fill_color)
#             hist.SetFillStyle(1001)
#             print "Hist ", hist.GetFillColor()
        if options.markers:
            hist.SetMarkerColor(color)
            hist.SetMarkerStyle(marker_styles[i])
        else:
            hist.SetMarkerSize(0)
        if options.overflow or (options.sticky and "Overflow" in name):
            nbins = hist.GetNbinsX()
            overflow = hist.GetBinContent(nbins + 1)
            hist.AddBinContent(nbins, overflow)
        if options.underflow or (options.sticky and "Underflow" in name):
            underflow = hist.GetBinContent(0)
            hist.AddBinContent(1, underflow)
        if options.normalize or (options.sticky and "Norm" in name):
            integral = hist.Integral()
            if integral: hist.Scale(1. / integral)
        hists.append(hist)
    if options.normalize_to_file:
        integral = hists[int(options.normalize_to_file) - 1].Integral()
        if integral:
            for hist in hists:
                hist.Scale(hist.Integral() / integral)
    #### Combine hists in a THStack and draw
    pads = [canvas]
    stack = ROOT.THStack("st%.3i" % int(counter), title)
    legend_height = 0.04 * len(files) + 0.02
    legend = ROOT.TLegend(0.65, 0.89 - legend_height, 0.87, 0.89)
    for hist in hists:
        stack.Add(hist)
        legend.AddEntry(hist)
    stack.Draw(options.opt)
    stack.GetXaxis().SetTitle(x_title)
    stack.GetYaxis().SetTitle(y_title)
    if options.ratio or (options.sticky and "Ratio" in name):
        pads, stack, stack_ratio = add_ratio_plot(hists, stack, counter)
        pads[1].cd()
        stack_ratio.Draw(options.opt)
        pads[0].cd()
    pads[0].SetLogx(options.logx or (options.sticky and "Logx" in name))
    pads[0].SetLogy(options.logy or (options.sticky and "Logy" in name))
    stack.Draw(options.opt)
    if options.numbering:
        display_page_number(counter)
    if options.efficiency or (options.sticky and "Eff" in name):
        stack.Draw(options.opt + "e")
        stack.SetMaximum(1.)
        stack.SetMinimum(0.)
    if options.overflow or (options.sticky and "Overflow" in name):
        display_overflow(stack, hist)
    if options.underflow or (options.sticky and "Underflow" in name):
        display_underflow(stack, hist)
    legend.Draw()
    save_plot(stack, options.plot_dir, path, name, counter)



######## Define some supporting functions #####################################

def save_plot(stack, plot_dir, path, name, counter):
    """Save the canvas to the output format defined by --ext."""
    output_file_name = "%s/%s/%s.%s" % (plot_dir, path, name, options.ext)
    canvas.SaveAs(output_file_name)
    if options.ext == "pdf":
        numbered_pdf_name = "%.3i.pdf" % counter
        shutil.copy(output_file_name, numbered_pdf_name)
    report_progress(counter, 1)

def report_progress(counter, divisor):
    """Print the current number of finished plots."""
    if counter % divisor == 0:
        print "\r%i plots written to %s" % (counter, options.output),
        sys.stdout.flush()

def merge_pdf():
    """Merge together all the produced plots into one pdf file."""
    print "Writing merged pdf..."
    os.system("gs -q -dBATCH -dNOPAUSE -sDEVICE=pdfwrite "
              "-dAutoRotatePages=/All "
              "-sOutputFile=%s.pdf " % options.output +
              "[0-9][0-9][0-9].pdf")
    os.system("rm [0-9]*.pdf")

def display_page_number(page_number):
    """Add a page number to the top corner of the canvas."""
    page_text = ROOT.TText()
    page_text.SetTextSize(0.03)
    page_text.SetTextAlign(33)
    page_text.DrawTextNDC(0.97, 0.985, "%i" % page_number)

def display_overflow(stack, hist):
    """Add the overflow to the last bin and print 'Overflow' on the bin."""
    nbins = hist.GetNbinsX()
    x = 0.5 * (hist.GetBinLowEdge(nbins) +
               hist.GetBinLowEdge(nbins + 1))
    y = stack.GetMinimum("nostack")
    display_bin_text(x, y, nbins, "Overflow")

def display_underflow(stack, hist):
    """Add the underflow to the first bin and print 'Underflow' on the bin."""
    nbins = hist.GetNbinsX()
    x = 0.5 * (hist.GetBinLowEdge(1) +
               hist.GetBinLowEdge(2))
    y = stack.GetMinimum("nostack")
    display_bin_text(x, y, nbins, "Underflow")

def display_bin_text(x, y, nbins, text):
    """Overlay TEXT on this bin."""
    bin_text = ROOT.TText()
    bin_text.SetTextSize(min(1. / nbins, 0.04))
    bin_text.SetTextAlign(12)
    bin_text.SetTextAngle(90)
    bin_text.SetTextColor(13)
    bin_text.SetTextFont(42)
    bin_text.DrawText(x, y, text)

def add_ratio_plot(hists, stack, counter):
    """Divide canvas into two parts, and plot the ratio on the bottom."""
    ## Both pads are set to the full canvas size to maintain font sizes
    ## Fill style 4000 used to ensure pad transparency because of this
    div = 0.3 # portion of canvas to use for ratio plot
    margins = [ROOT.gStyle.GetPadTopMargin(), ROOT.gStyle.GetPadBottomMargin()]
    useable_height = 1 - (margins[0] + margins[1])
    canvas.Clear()
    pad = ROOT.TPad("mainPad", "mainPad", 0., 0., 1., 1.)
    pad.SetFillStyle(4000)
    pad.Draw()
    pad.SetBottomMargin(margins[1] + div * useable_height)
    pad_ratio = ROOT.TPad("ratioPad", "ratioPad", 0., 0., 1., 1.);
    pad_ratio.SetFillStyle(4000)
    pad_ratio.Draw()
    pad_ratio.SetTopMargin(margins[0] + (1 - div) * useable_height)
    pad.cd()
    stack.Draw()
    stack_ratio = ROOT.THStack("stRatio%.3i" % int(counter),
                               ";%s;Ratio" % stack.GetXaxis().GetTitle())
    for hist in hists[1:]:
        ratio_hist = hist.Clone()
        ratio_hist.Divide(hists[0])
        stack_ratio.Add(ratio_hist)
    stack_ratio.Draw()
    stack_ratio.GetYaxis().SetNdivisions(507) # Avoids crowded labels
    stack.GetXaxis().SetBinLabel(1, "") # Don't show numbers below top plot
    stack.GetXaxis().SetTitle("")
    if stack.GetYaxis().GetTitle() == "":
        stack.GetYaxis().SetTitle("Content")
    # Avoid overlap of y-axis numbers by supressing zero
    if stack.GetMinimum() / stack.GetMaximum() < 0.25:
        stack.SetMinimum(stack.GetMaximum() / 10000)
    return [pad, pad_ratio], stack, stack_ratio



######## Define the main program #############################################

def main():
    usage="""usage: %prog [options] file1.root file2.root file3.root ...

    function: overlays corresponding histograms from several files, dumping the
          images into an identical directory structure in the local directory
          and also merging all images into a single file (if output is pdf);
          most style options can be controlled from your rootlogon.C macro"""
    
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-e', '--ext', default="pdf", 
                      help="choose an output extension; default is pdf")
    parser.add_option('-o', '--opt', default="nostack p H",
                      help="pass OPT to the Draw command; default is "
                      "'nostack p H', add 'e' for error bars")
    parser.add_option('-m', '--markers', action="store_true", default=False,
                      help="add markers to histograms")
    parser.add_option('-s', '--sticky', action="store_true", default=False,
                      help="enable name-based special plotting options "
                      "(see below)")
#     parser.add_option('-f', '--fill', action="store_true", default=False,
#                       help="Fill histograms with a color")
    parser.add_option('--output', default="overlaidHists", metavar="NAME",
                      help="name of output directory; default is 'overlaidHists'")
    parser.add_option('--numbering', action="store_true", default=False,
                      help="add a page number in the upper right of each plot")
    parser.add_option('--match', default="", metavar="REGEX",
                      help="only make plots for paths containing the specified "
                      "regular expression (use '.*' for wildcard)")
    parser.add_option('--normalize-to-file', default="", metavar="FILENUM",
                      help="normalize to the FILENUMth file")
    group1 = optparse.OptionGroup(
        parser,
        "special plotting options",
        "Use the command line options given below to apply changes to all "
        "plots.  If you only wish to apply an option to a specific plot, "
        "you can use '-s' "
        "to turn on sticky keywords (such as 'Norm').  Any plot that includes "
        "the given keyword in its ROOT name will have the option applied "
        "regardless of its presence or absence on the command line."
        )
    group1.add_option('-n', '--normalize', action="store_true", default=False,
                      help="'Norm': area normalize the histograms")
    group1.add_option('--efficiency', action="store_true", default=False,
                      help="'Eff' : force y axis scale to run from 0 to 1")
    group1.add_option('--logx', action="store_true", default=False,
                      help="'Logx': force log scale for x axis")
    group1.add_option('--logy', action="store_true", default=False,
                      help="'Logy': force log scale for y axis")
    group1.add_option('--overflow', action="store_true", default=False,
                      help="'Overflow' : display overflow content in "
                      "highest bin")
    group1.add_option('--underflow', action="store_true", default=False,
                      help="'Underflow': display underflow content in "
                      "lowest bin")
    group1.add_option('--ratio', action="store_true", default=False,
                      help="'Ratio': display a ratio plot below the normal "
                      "plot")
    parser.add_option_group(group1)
    global options
    options, arguments = parser.parse_args()
    options.plot_dir = "%s/%s" % (os.path.abspath('.'), options.output)
    options.regex = re.compile(options.match)
    files = [RootFile(filename) for filename in arguments]
    ## if no arguments provided, just display the help message
    if len(files) == 0:
        parser.print_help()
        sys.exit(0)
    ## add style options and create the canvas
    options = add_style_options(options)
    global canvas
    canvas = ROOT.TCanvas()
    ## here, we decend into the files to start plotting
    process_directory("", files)
    print ""
    if options.ext == "pdf":
        merge_pdf()


if __name__ == "__main__":
    main()

