///
//
// Importing Data from Hist or Tree
// Binned and unbinned into workspace
//
//
////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAbsData.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TRandom.h"
using namespace RooFit ;

//#define bkghist  "NZMass seed time"
#define bkghist  "histo_exp_bkgtime"
#define datahist "EBEB/rh_dataTime"
#define sighist  "EBEB/sig_dataTime"
#define TreeName  "Tree"


//Useful Fxns
TH1* makeTH1() ;
TTree* makeTTree() ;
void ReadnAdd2Ws( RooWorkspace* w, RooDataSet* data);
void makeworspace(RooDataSet * data, RooDataHist* sighist, RooDataHist* bkghist);


void ImportDataToWorkspace()

{
///
// importing ROot histograms ///
///
  // TFile *bgf = new TFile("Bkg_dataSet.root","READ");
   TFile *bgf = new TFile("Bkg_dataSetABCD_DataSet.root","READ");
// TFile *dataf = new TFile("Inputfile.root","READ");
// TFile *sigf = new TFile("Inputfile.root","READ");
   TH1F* bkgh = (TH1F*)bgf->Get(bkghist);
   if(!bkgh) exit(1);
// TH1* datah = (TH1*)dataf->Get(datahist);
// TH1* sigh = (TH1*)sigf->Get(sighist);


 RooRealVar t("t","ECAL Time", -3.0, 10.0,"ns");
 
 RooRealVar mean("mean", "mean", 0.00, "Time[ns]");
 RooRealVar width("width", "#sigma", 0.9, "Time[ns]");
 mean.setConstant(kTRUE);
 width.setConstant(kTRUE);
 // Build simple bkg model
 RooRealVar ctau("ctau", "c#tau", -0.90, -0.0, 5.0, "ns");
 ctau.setConstant(kFALSE);
 RooExponential datamodel("datamodel", "datamodel", t, ctau);
 RooGaussian signalmodel("signalmodel", "signalmodel", t, mean, width);
 
 //Import from Binned Histograms
 RooDataHist bkg("bkg", "Bkg Binned Hist", t, Import(*bkgh) );
 //RooDataHist data("data", "Data Binned Hist", t, Import(*datah) );
 //RooDataHist sig("sig", "Sig Binned Hist", t, Import(*sigh) );
 
 //Generate Toy Signal
 //Binnned:
 RooDataHist* data = datamodel.generateBinned(RooArgSet(t), bkgh->Integral());
 RooDataHist* sig =  signalmodel.generateBinned(RooArgSet(t), bkgh->Integral());
 // Unbinned
 //RooDataSet data = bkgmodel.generate(RooArgSet(t), bkgh->Integral());
 //RooDataSet sig =  signalmodel.generate(RooArgSet(t), bkgh->Integral());
 

// Plot Histograms
 RooPlot* frame = t.frame(Range(0, 5),Bins(10),Title("Binned Bkg with Poisson Error Bars") );
 bkg.plotOn(frame,Binning(7));
 
 data->plotOn(frame,MarkerColor(kRed),LineColor(kRed));
 //datamodel.plotOn(frame,);
 sig->plotOn(frame,MarkerColor(kBlue),LineColor(kBlue));
 
 RooPlot* frame2 = t.frame(Range(0,5),Title("Binned Bkg With internal errors"));
 bkg.plotOn(frame2,DataError(RooAbsData::SumW2));
 data->plotOn(frame2,DataError(RooAbsData::SumW2),MarkerColor(kRed),LineColor(kRed));
 sig->plotOn(frame2,DataError(RooAbsData::SumW2),MarkerColor(kBlue),LineColor(kBlue));


// Importing from a Tree( Unbinned DataSet )
//  TFile* trfile = new TFile("TreeFile.root","READ");
//  TTree * tr = (TTree*)trfile->Get(TreeName);

  // Do Rondom for now!
  TTree * tr = makeTTree() ;
  RooRealVar time("time", "ECAL Time", -3.0, 10.0, "ns");
  
  RooDataSet dset("dset","dataset", RooArgSet(time), Import(*tr));
 // Print N Events in dataset
  //dset.Print("v");

  RooPlot*frame3 = time.frame(Range(0,5),Title("Unbinned Data in Default frame Binning"));
  dset.plotOn(frame3,MarkerColor(kBlue),LineColor(kBlue));

  RooPlot*frame4 = time.frame(Range(0,5),Title("Unbinned Data in Custom Binning"));
  dset.plotOn(frame4,Binning(7),MarkerColor(kRed),LineColor(kRed));


  // Unbinned DataSet from Binned Histogram:
  RooDataSet *dd = dset.emptyClone() ;
  for (Int_t i=0 ; i< bkg.numEntries() ; i++) {
       //dd->addFast(*bkg.get(i),bkg.weight()) ;
         dd->add(*bkg.get(i),bkg.weight()) ;
       //dd->add(*bkg.get(i)) ;
  }  

  dd->Print();

  RooPlot*frame1 = time.frame(Range(0,5),Title("Unbinned Bkg Estimation"));
  dd->plotOn(frame1,Binning(7),MarkerColor(kGreen),LineColor(kGreen));
  
  // Seperate Binned BKG
  RooPlot* frameb = t.frame(Range(0,5),Title("Bkg Estimate From Collision"));
  bkg.plotOn(frameb,DataError(RooAbsData::SumW2));
//Now Draw all frames on one canvas
  TCanvas *c = new TCanvas("c", "Importing (un)Binned Data", 900, 900 );
  c->Divide(2,2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;
  c->cd(3) ; gPad->SetLeftMargin(0.15) ; frame3->GetYaxis()->SetTitleOffset(1.4) ; frame3->Draw() ;
  c->cd(4) ; gPad->SetLeftMargin(0.15) ; frame4->GetYaxis()->SetTitleOffset(1.4) ; frame4->Draw() ;


  TCanvas *cv = new TCanvas("cv", "Ubinned Bkg Estimation From Binned Hist", 900, 900 );
  cv->cd() ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw() ;

  TCanvas *cb = new TCanvas("cb", "Importing  Binned  Bkg Data", 900, 900 );
  cb->cd() ; gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
 
  TCanvas *cbin = new TCanvas("cbin", "Importing  Binned  Bkg Data", 900, 900 );
  cbin->cd() ; gPad->SetLeftMargin(0.15) ; frameb->GetYaxis()->SetTitleOffset(1.4) ; frameb->Draw() ;
  // Save Histograms into Ws
   makeworspace(dd,sig,data );

}


TH1* makeTH1() 
{
  // Create ROOT TH1 filled with a Gaussian distribution

  TH1D* hh = new TH1D("hh","hh",25,-3.0,10.0) ;
  for (int i=0 ; i<100 ; i++) {
    hh->Fill(gRandom->Gaus(0,3)) ;
  }
  return hh ;
}


TTree* makeTTree() 
{
  // Create ROOT TTree filled with a Gaussian distribution in x and a uniform distribution in y

  TTree* tree = new TTree("tree","tree") ;
  Float_t* time = new Float_t ;
//  Double_t* py = new Double_t ;
  tree->Branch("t",time,"t/F") ;
 // tree->Branch("y",py,"y/D") ;
  for (int i=0 ; i<100 ; i++) {
    *time = gRandom->Gaus(0,1) ;
    //*py = gRandom->Uniform()*30 - 15 ;
    tree->Fill() ;
  }
  return tree ;
}

//ReadWorkspace
void ReadnAdd2Ws( RooWorkspace* w, RooDataSet* data)
{

 w->import(*data);
 //RooRealVar* x = w->var("x") ;
 //RooAbsPdf* model = w->pdf("model") ;
 RooDataSet* data = w->data("data") ;
 w->Print();
}

// Make workspace
void makeworspace(RooDataSet * data, RooDataHist* sighist, RooDataHist* bkghist)
{
 // Create a new empty workspace
    RooWorkspace *w = new RooWorkspace("w","workspace") ;
   // Import model and all its components into the workspace
   // w->import(model) ;
   // Import data into the workspace
    w->import(*data) ;
    w->import(*sighist) ;
    w->import(*bkghist) ;
    w->Print() ;
    w->writeToFile("N2ggPlusMet.root") ;
    gDirectory->Add(w) ;
}
