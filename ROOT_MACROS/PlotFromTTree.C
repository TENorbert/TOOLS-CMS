/*************************************************
  Drawing plots directly fron TTree objects can
  give you unexpected axis endpoints and
  binning. So I wrote this to make histograms
  that will be easier to compare.

  In current directory, 
  rootlogon.C must contain, for example:

    G__loadfile("/afs/hep.wisc.edu/home/mbanderson/ROOT_Macros/PlotFromTTree.C");
    TFile *file1 = new TFile("aFile.root");
    TCanvas *c1 = new TCanvas("c1","testCanvas",640,480);
    TH1F *h1 = new TH1F("h1","Blah",20,-5,5);



  Example commands typed 
  at ROOT prompt:
    PlotFromTTree("TreeRec", "recPhtn.phi", "", -4.0, 4.0, 25)
    PlotFromTTree("TreeRec","abs(recPhtn.phi-recJet.phi)","",2.0,4.0,50,"#Delta#phi")

  with this format:
    PlotFromTTree("Tree Name","What To Plot","Cuts to Make", xMin, xMax, bins, "x-axis title", "y-axis title")

  
  Example to draw two things 
  onto same plot:

    root [0] PlotFromTTree("TreePhotonAndJet","recJet2.pt","",0,300,100)
    root [1] PlotFromTTree("TreePhotonAndJet","recJet1.pt","",0,300,100,"","","same")

      OR

    root [0] PlotFromTTree("TreePhotonAndJet","recJet1.pt","recJet2.pt","","",0,300,100)

  Michael B. Anderson
  May 27, 2008
*************************************************/

void PlotFromTTree( TString treeName,
                    TString plotVar,
                    TString cuts,
                    Double_t xMin, 
                    Double_t xMax, 
                    int numOfBins, 
                    TString XaxisTitle = "", 
                    TString YaxisTitle = "",
                    TString drawOption = "")
{
  // *******************************************
  // Variables

  // cutNames will be put into the image filename
  // so certain characters must be removed
  TString cutNames = filenameFilter(cuts);

  // ASSUMES file1 is opened in the rootlogon.C
  TTree* treeAny = file1->Get(treeName);  // Grab the ntuple from the ROOT file
  // see if the TTree was found
  if ( !treeAny ) {
    cout << "Error: " << file1->GetName() << " does not contain TTree named " << treeName << endl;
    return;
  }

  // This will be the filename for the output image
  gSystem->MakeDirectory("Images");
  TString outputName("Images/"+treeName+"-"+plotVar+cutNames+".gif");

  Int_t LogY = 0;                  // Y-axis, No log = 0, Log = 1
  Double_t YaxisTitleOffset = 1.4; // Set to 1.4 if y-axis  title is overlapping with labels
                                   // 0 otherwise
  
  // End of Variables
  // *******************************************


  // Created in rootlogon.C
  TString histName  = h1->GetName();           // Grab the name of the histogram
  TString histName2 = h2->GetName();

  if (drawOption.CompareTo("same") == 0 ) {
    h2->SetBins(numOfBins,xMin,xMax);           // Set the Number of Bins, Xmin, and Xmax
    treeAny->Draw(plotVar+" >> "+histName2,cuts,"goff");     // Draw to it
    h2->SetLineColor(kRed);                       // Set color for line.
    h2->Draw("same");                          // Draw the histogram.
  } else {

    h1->SetBins(numOfBins,xMin,xMax);           // Set the Number of Bins, Xmin, and Xmax
    h1->SetNameTitle(histName,treeName+"-"+plotVar); // Set the Name and Title

    treeAny->Draw(plotVar+" >> "+histName,cuts);     // Draw to it

    //h1->GetYaxis()->SetRangeUser(ymin,ymax);     // Rescale Y-Axis.

    h1->SetLineColor(kBlue);                       // Set color for line.
    //h1->SetStats(kFALSE);                        // Turn the stats box off.

    h1->GetXaxis()->SetTitle(XaxisTitle);          // Give titles to the
    h1->GetYaxis()->SetTitle(YaxisTitle);          // X and Y axis.

    h1->Draw();                          // Draw the histogram.

  }
  h1->GetYaxis()->SetTitleOffset(YaxisTitleOffset);

  c1->SetLogy(LogY);       // Set the axis to a log or not-log scale
  c1->Print(outputName);   // Save the histogram to a file
}


// Plot 2 things onto the same plot
void PlotFromTTree( TString treeName,
                    TString plotVar1,
                    TString plotVar2,
                    TString cuts1="",
                    TString cuts2="",
                    Double_t xMin = 0,
                    Double_t xMax = 0,
                    int numOfBins = 1,
                    TString XaxisTitle = "",
                    TString YaxisTitle = "")
{
  // *******************************************
  // Variables                            

  // cutNames will be put into the image filename    
  // so certain characters must be removed      
  TString cutNames1 = filenameFilter(cuts1);
  TString cutNames2 = filenameFilter(cuts2);

  // ASSUMES file1 is opened in the rootlogon.C             
  TTree* treeAny = file1->Get(treeName);  // Grab the ntuple from the ROOT file
  // see if the TTree was found                             
  if ( !treeAny ) {
    cout << "Error: " << file1->GetName() << " does not contain TTree named " << treeName << endl;
    return;
  }

  // This will be the filename for the output image                                  
  gSystem->MakeDirectory("Images");
  TString outputName("Images/"+treeName+"-"+plotVar1+"-"+plotVar2+cutNames1+cutNames2+".gif");

  Int_t LogY = 0;                  // Y-axis, No log = 0, Log = 1          
  Double_t YaxisTitleOffset = 1.4; // Set to 1.4 if y-axis  title is overlapping with labels
                                   // 0 otherwise  

  // End of Variables
  // *******************************************
  


  if (xMin == xMax) {
    echo << xMin << " " << xMax << endl;
    // User did not give us a value, so determine one
    
    // Draw to a temp histogram to see default bins and xlo and xhi
    treeAny->Draw(plotVar1+" >> htemp",cuts1,"goff");     // Draw to it

    numOfBins = htemp->GetNbinsX();
    xMin = htemp->GetXaxis()->GetXmin();
    XMax = htemp->GetXaxis()->GetXmax();

    htemp->Delete();
  }

  // Set the bin information
  h1->SetBins(numOfBins,xMin,xMax);           // Set the Number of Bins, Xmin, and Xmax  
  TString histName = h1->GetName();           // Grab the name of the histogram
  h1->SetNameTitle(histName,treeName); // Set the Name and Title

  // Draw variable 1 to histogram 1
  treeAny->Draw(plotVar1+" >> "+histName,cuts1,"goff");     // Draw to it

  // Draw variable 2 to histogram 2
  h2->SetBins(numOfBins,xMin,xMax);

  h2->SetLineColor(kBlue);
  treeAny->Draw(plotVar2+" >> h2",cuts2,"goff");     // Draw to it

  // Set the color
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);

  // Get rid of Stats box
  h1->SetStats(kFALSE);

  h1->GetXaxis()->SetTitle(XaxisTitle);          // Give titles to the
  h1->GetYaxis()->SetTitle(YaxisTitle);          // X and Y axis.

  h1->Draw();
  h2->Draw("same");

  // ****************
  // Draw Info Label
  TString hist1Label = plotVar1;
  TString hist2Label = plotVar2;
  TLegend *infoBox = new TLegend(0.74, 0.74, 0.98, 0.98, "");
  if ( (hist1Label=="") || (hist2Label=="") ) {
    infoBox->AddEntry(h1,hist1Name,"L");
    infoBox->AddEntry(h2,hist2Name,"L");
  } else {
    // If the plot variables were the same,
    // then the user is probably comparing before & after a cut
    if ( hist1Label.CompareTo(hist2Label) == 0 ) {
      hist1Label = "before";
      hist2Label = "after";
      h1->SetNameTitle(histName,"New cut(s) = "+cuts2); // Set the Name and Title
      cout << h1->GetEntries() << " entries before with cuts = " << cuts1 << endl;
      cout << h2->GetEntries() << " entries after  with cuts = " << cuts2 << endl;
    }
    infoBox->AddEntry(h1,hist1Label,"L");
    infoBox->AddEntry(h2,hist2Label,"L");
  }
  infoBox->Draw();
  // ****************

  c1->Print(outputName);   // Save the histogram to a file
}


// This is for 2D Plots
void PlotFromTTree( TString treeName,
                    TString plotVarX,
                    TString plotVarY,
                    TString cuts,
                    Double_t xMin,
                    Double_t xMax,
                    Double_t yMin,
                    Double_t yMax,
                    int numOfBinsX,
                    int numOfBinsY,
                    TString XaxisTitle = "",
                    TString YaxisTitle = "")
{
  // *******************************************
  // Variables
                    
  // cutNames will be put into the image filename
  // so certain characters must be removed
  TString cutNames = filenameFilter(cuts);
                    
  // ASSUMES file1 is opened in the rootlogon.C
  TTree* treeAny = file1->Get(treeName);  // Grab the ntuple from the ROOT file
  // see if the TTree was found
  if ( !treeAny ) {
    cout << "Error: " << file1->GetName() << " does not contain TTree named " << treeName << endl;
    return;
  }

  // This will be the filename for the output image
  gSystem->MakeDirectory("Images");
  TString outputName("Images/"+treeName+"-"+plotVarX+"-"+plotVarY+cutNames+".gif");
  
  Double_t YaxisTitleOffset = 1.4; // Set to 1.4 if y-axis  title is overlapping with labels
                                   // 0 otherwise
   
  // End of Variables
  // *******************************************

  h2d->SetBins(numOfBinsX,xMin,xMax,numOfBinsY,yMin,yMax); // Set the Number of Bins, Xmin, and Xmax
  TString histName = h2d->GetName();           // Grab the name of the histogram
  h2d->SetNameTitle(histName,treeName+"-"+plotVarX+"-VS-"+plotVarY); // Set the Name and Title

  h2d->SetOption("BOX");
  treeAny->Draw(plotVarY+":"+plotVarX+" >> "+histName,cuts);     // Draw to it
  
  
  h2d->SetLineColor(kBlue);                       // Set color for line.
  //h2d->SetStats(kFALSE);                        // Turn the stats box off.

  h2d->GetXaxis()->SetTitle(XaxisTitle);          // Give titles to the
  h2d->GetYaxis()->SetTitle(YaxisTitle);          // X and Y axis.
                    
  h2d->Draw();                                    // Draw the histogram.
  h2d->GetYaxis()->SetTitleOffset(YaxisTitleOffset);

  c1->Print(outputName);   // Save the histogram to a file
}


TString filenameFilter(TString originalString) {

  if (originalString == "" ) return originalString;

  TString newString = originalString;
  newString.ReplaceAll("-","S");  // Subtraction
  newString.ReplaceAll("||","-"); // OR
  newString.ReplaceAll("&&","-"); // AND
  newString.ReplaceAll("<","LT"); // Less Than
  newString.ReplaceAll(">","GT"); // Greater Than

  newString = "-"+newString;

  return newString;
} 
