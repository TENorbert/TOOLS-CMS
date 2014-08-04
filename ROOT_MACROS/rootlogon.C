{

gSystem->Load("libFWCoreFWLite.so");
AutoLibraryLoader::enable();

TStyle *gStyle= new TStyle("CMS","CMS approved plots style");

// use plain black on white colors
gStyle->SetFrameBorderMode(0);
gStyle->SetCanvasBorderMode(0);
gStyle->SetPadBorderMode(0);
gStyle->SetPadColor(0);
gStyle->SetCanvasColor(0);
gStyle->SetStatColor(0);
gStyle->SetFillColor(0);

// set the paper & margin sizes
gStyle->SetPaperSize(20,26);
gStyle->SetPadTopMargin(0.05);
gStyle->SetPadRightMargin(0.05);
gStyle->SetPadBottomMargin(0.16);
gStyle->SetPadLeftMargin(0.12);

// use large Times-Roman fonts
gStyle->SetTextFont(132);
gStyle->SetTextSize(0.08);
gStyle->SetLabelFont(132,"x");
gStyle->SetLabelFont(132,"y");
gStyle->SetLabelFont(132,"z");
gStyle->SetLabelSize(0.05,"x");
gStyle->SetTitleSize(0.06,"x");
gStyle->SetLabelSize(0.05,"y");
gStyle->SetTitleSize(0.06,"y");
gStyle->SetLabelSize(0.05,"z");
gStyle->SetTitleSize(0.06,"z");

// use bold lines and markers
gStyle->SetMarkerStyle(20);
gStyle->SetHistLineWidth(1.85);
//gStyle->SetHistLineWidth(3.85);
gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

// get rid of X error bars and y error bar caps
gStyle->SetErrorX(0.001);

// do not display any of the standard histogram decorations
gStyle->SetOptTitle(0);
gStyle->SetOptStat(0);
gStyle->SetOptFit(0);

// put tick marks on top and RHS of plots
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);

gROOT->SetStyle("Plain");
//gStyle->SetOptStat(1100);
gStyle->SetOptStat(1);
gStyle->SetOptFit(1111);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
}
