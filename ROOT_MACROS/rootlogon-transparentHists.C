{

// Rename this to rootlogon.C and place in your working directory.
// This is based on the tdrStyle used for the CMS P-TDR
// The major change is that when you call hist->SaveAs("hist.pdf") using this
// rootlogon, the output has no background fill, so in presentations and the 
// like, you can do things like putting a drop shadow on the graph, which
// can greatly improve aesthetics.

// The relevant lines for transparency are marked as "Changed -- JEK"
// They mostly involve setting various colors to "-1"

gSystem->Load("libFWCoreFWLite.so"); 
AutoLibraryLoader::enable();
gSystem->Load("libDataFormatsFWLite.so");
gROOT->ProcessLine("namespace edm {typedef edm::Wrapper<vector<float> > Wrapper<vector<float,allocator<float> > >; }");
gROOT->ProcessLine("namespace edm {typedef edm::Wrapper<vector<double> > Wrapper<vector<double,allocator<double> > >; }");

TStyle *jeffStyle = new TStyle("jeffStyle","Used by Jeff Klukas, Adapted from Style for P-TDR");

cout << "TDR Style initialized" << endl;

// For the canvas:
jeffStyle->SetCanvasBorderMode(0);
jeffStyle->SetCanvasColor(-1);                    // Changed --JEK
jeffStyle->SetCanvasDefH(600); //Height of canvas
jeffStyle->SetCanvasDefW(600); //Width of canvas
jeffStyle->SetCanvasDefX(0);   //POsition on screen
jeffStyle->SetCanvasDefY(0);

// For the Pad:
jeffStyle->SetPadBorderMode(0);
// jeffStyle->SetPadBorderSize(Width_t size = 1);
jeffStyle->SetPadColor(-1);                       // Changed -- JEK
jeffStyle->SetPadGridX(false);
jeffStyle->SetPadGridY(false);
jeffStyle->SetGridColor(0);
jeffStyle->SetGridStyle(3);
jeffStyle->SetGridWidth(1);

// For the frame:
jeffStyle->SetFrameBorderMode(0);
jeffStyle->SetFrameBorderSize(1);
jeffStyle->SetFrameFillColor(-1);                 // Changed -- JEK
jeffStyle->SetFrameFillStyle(0);
jeffStyle->SetFrameLineColor(1);
jeffStyle->SetFrameLineStyle(1);
jeffStyle->SetFrameLineWidth(1);

// For the histo:
jeffStyle->SetHistFillColor(-1);                  // Changed -- JEK
// jeffStyle->SetHistFillStyle(0);
jeffStyle->SetHistLineColor(1);
jeffStyle->SetHistLineStyle(0);
jeffStyle->SetHistLineWidth(1);
// jeffStyle->SetLegoInnerR(Float_t rad = 0.5);
// jeffStyle->SetNumberContours(Int_t number = 20);

jeffStyle->SetEndErrorSize(2);
//jeffStyle->SetErrorX(0.);
  
jeffStyle->SetMarkerStyle(20); // Kind of dot used for points
jeffStyle->SetMarkerSize(0.5); 

//For the fit/function:
jeffStyle->SetOptFit(1);
jeffStyle->SetFitFormat("5.4g");
jeffStyle->SetFuncColor(2);
jeffStyle->SetFuncStyle(1);
jeffStyle->SetFuncWidth(1);

//For the date:
jeffStyle->SetOptDate(0);
// jeffStyle->SetDateX(Float_t x = 0.01);
// jeffStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
jeffStyle->SetOptFile(0);
jeffStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
jeffStyle->SetStatColor(kWhite);
jeffStyle->SetStatFont(42);
jeffStyle->SetStatFontSize(0.025);
jeffStyle->SetStatTextColor(1);
jeffStyle->SetStatFormat("6.4g");
jeffStyle->SetStatBorderSize(1);
jeffStyle->SetStatH(0.1);
jeffStyle->SetStatW(0.15);
// jeffStyle->SetStatStyle(Style_t style = 1001);
// jeffStyle->SetStatX(Float_t x = 0);
// jeffStyle->SetStatY(Float_t y = 0);

// Margins:
jeffStyle->SetPadTopMargin(0.15);
jeffStyle->SetPadBottomMargin(0.13);
jeffStyle->SetPadLeftMargin(0.13);
jeffStyle->SetPadRightMargin(0.15);

// For the Global title:

//  jeffStyle->SetOptTitle(0);
jeffStyle->SetTitleFont(42);
jeffStyle->SetTitleSize(0.05);
jeffStyle->SetTitleColor(1);
jeffStyle->SetTitleTextColor(1);
jeffStyle->SetTitleFillColor(-1);                // Changed -- JEK
jeffStyle->SetTitleFontSize(0.05);
// jeffStyle->SetTitleH(0); // Set the height of the title box
// jeffStyle->SetTitleW(0); // Set the width of the title box
// jeffStyle->SetTitleX(0); // Set the position of the title box
// jeffStyle->SetTitleY(0.985); // Set the position of the title box
// jeffStyle->SetTitleStyle(Style_t style = 1001);
// jeffStyle->SetTitleBorderSize(2);

// For the axis titles:

jeffStyle->SetTitleColor(1, "XYZ");
jeffStyle->SetTitleFont(42, "XYZ");
jeffStyle->SetTitleSize(0.05, "XYZ");             //
// jeffStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
// jeffStyle->SetTitleYSize(Float_t size = 0.02);
jeffStyle->SetTitleXOffset(1.08);
//   jeffStyle->SetTitleYOffset(1.5);
jeffStyle->SetTitleOffset(1.2, "Y"); // Another way to set the Offset

// For the axis labels:

jeffStyle->SetLabelColor(1, "XYZ");
jeffStyle->SetLabelFont(42, "XYZ");
jeffStyle->SetLabelOffset(0.007, "XYZ");
jeffStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

jeffStyle->SetAxisColor(1, "XYZ");
jeffStyle->SetStripDecimals(kTRUE);
jeffStyle->SetTickLength(0.03, "XYZ");
jeffStyle->SetNdivisions(510, "XYZ");
jeffStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
jeffStyle->SetPadTickY(1);

// Change for log plots:
jeffStyle->SetOptLogx(0);
jeffStyle->SetOptLogy(0);
jeffStyle->SetOptLogz(0);

jeffStyle->SetPalette(1,0);

//                                       Additions -- JEK
jeffStyle->SetFillColor(-1);
jeffStyle->SetFillStyle(4000);
jeffStyle->SetStatStyle(0);
jeffStyle->SetTitleStyle(0);
jeffStyle->SetCanvasBorderSize(0);
jeffStyle->SetFrameBorderSize(0);
jeffStyle->SetLegendBorderSize(0);
jeffStyle->SetStatBorderSize(0);
jeffStyle->SetTitleBorderSize(0);

gROOT->ForceStyle(); 

// Postscript options:
// jeffStyle->SetPaperSize(15.,15.);
// jeffStyle->SetLineScalePS(Float_t scale = 3);
// jeffStyle->SetLineStyleString(Int_t i, const char* text);
// jeffStyle->SetHeaderPS(const char* header);
// jeffStyle->SetTitlePS(const char* pstitle);

// jeffStyle->SetBarOffset(Float_t baroff = 0.5);
// jeffStyle->SetBarWidth(Float_t barwidth = 0.5);
// jeffStyle->SetPaintTextFormat(const char* format = "g");
// jeffStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
// jeffStyle->SetTimeOffset(Double_t toffset);
// jeffStyle->SetHistMinimumZero(kTRUE);

jeffStyle->cd();

}

