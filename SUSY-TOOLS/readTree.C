#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"

/** Run this example as:
 * .x readTree.C++
 *
**/

void readTree()
{
  //TFile *f = new TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/RECO/gmsb180_2000.root");
  TFile *f = new TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/RECO/gjet_470-800.root");
  TTree *tr = (TTree*)f->Get("DPAnalysis");
  Int_t nJets, nPhotons;
  Float_t met;
  tr->SetBranchAddress("nPhotons",    &nPhotons);
  tr->SetBranchAddress("nJets",       &nJets); 
  tr->SetBranchAddress("met",            &met);
  TH1F *njets = new TH1F("njets", "Number of Jets in Event", 20, 0, 20);
  TH1F *nphos = new TH1F("nphos", "Number of #gamma s in Event", 10, 0, 10);
  TH1F *ev_met = new TH1F("ev_met", "Missing Energy(GeV)", 200, 0, 4000);
 
 // Read and Fill histograms
 Long64_t nentries = tr->GetEntries();
 for (Long64_t i = 0; i < nentries; i++){
      tr->GetEntry(i);
      njets->Fill(nJets);
      nphos->Fill(nPhotons);
      ev_met->Fill(met);

      }
 
 TFile *Of = new TFile("/data/whybee0b/user/norbert/GMSBAnalysis/MC2012/OUT-PUT-FILE/GammaJet-MC_470-800.root","recreate");
 Of->cd();

  njets->GetXaxis()->SetTitle("Njets");
  njets->GetYaxis()->SetTitle("Events");
  njets->Draw();
  njets->Write();

  nphos->GetXaxis()->SetTitle("NPhotons");
  nphos->GetYaxis()->SetTitle("Events");
  nphos->Draw();
  nphos->Write();
      
  ev_met->GetXaxis()->SetTitle("MET[GeV]");
  ev_met->GetYaxis()->SetTitle("Events");
  ev_met->Draw();
  ev_met->Write();
  
  Of->Close();
}

