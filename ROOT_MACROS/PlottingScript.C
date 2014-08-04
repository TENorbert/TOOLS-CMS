{
//************************************************
// Variables

TFile *file1 = new TFile("PhotonJet20-200.root");
TFile *file2 = new TFile("JetET110.root");

TTree *tree1, *tree2;
file1->GetObject("TreePhotonAndJet",tree1);
file2->GetObject("TreePhotonAndJet",tree2);

TH1F* htemp1 = new TH1F("htemp1","blah",20, 0.0, 1.0);
TH1F* htemp2 = new TH1F("htemp2","blah",20, 0.0, 1.0);

TString thingToDraw1 = "recPhtnMtch.r19";
TString thingToDraw2 = thingToDraw1;
TString label1 = "#gamma";
TString label2 = "jet";
TString cutsAll = "recPhtnMtch.et>50&&recPhtnMtch.et<150&&abs(recPhtnMtch.eta)<1.5";
TString title = "R19";
TString titleX = "E(max) / E(3x3)";

TString saveFileName = title+".gif";
TString saveFileLocation = "/afs/cern.ch/user/a/anderson/www/";

//************************************************


// Draw from the TTrees
tree1->Draw(thingToDraw1+" >> htemp1", cutsAll);
tree2->Draw(thingToDraw2+" >> htemp2", cutsAll);

// Scale the histograms
Double_t scale1 = 1/htemp1->Integral();
Double_t scale2 = 1/htemp2->Integral();
htemp1->Scale(scale1);
htemp2->Scale(scale2);

// Set the histogram colors & lines
htemp1->SetLineColor(kRed);
htemp2->SetLineColor(kGreen);
htemp1->SetLineWidth(2);
htemp2->SetLineWidth(2);

// Turn off Stats Box
gStyle->SetOptStat(0);

// Create TStack but will later draw without stacking
THStack *tempStack = new THStack();
tempStack->Add(htemp1);
tempStack->Add(htemp2);

// Draw the histogram and titles
tempStack->Draw();
tempStack->SetTitle(title);
tempStack->GetXaxis()->SetTitle(titleX);
tempStack->Draw("nostack");

// Draw the legend
TLegend *infoBox = new TLegend(0.74, 0.74, 0.98, 0.98, "");
infoBox->AddEntry(htemp1,label1,"L");
infoBox->AddEntry(htemp2,label2,"L");
infoBox->Draw();

// Save the canvas
c1->SaveAs(saveFileLocation+saveFileName);
}
