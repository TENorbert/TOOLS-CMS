/*************************************************
  This script adds plots from TTrees from many 
  files into the same histograms, scaling each, 
  and saves histograms into a new file.

  Very customizable.

  Michael B. Anderson
  Feb 19, 2009
*************************************************/


#include "TH1F.h"
#include <vector>
#include <cmath>

void ttreesToHistograms() {
//********************************************************************
//****                      Variables                           ****//
cout << "Loading variables into vectors..." << endl;

vector<TString> fileName;
fileName.push_back( "rootfiles0/PhotonJetPt15_Summer09.root"  );//file0
fileName.push_back( "rootfiles0/PhotonJetPt30_Summer09.root"  );//file1
fileName.push_back( "rootfiles0/PhotonJetPt80_Summer09.root"  );//file2
fileName.push_back( "rootfiles0/PhotonJetPt170_Summer09.root" );//file3
fileName.push_back( "rootfiles0/PhotonJetPt300_Summer09.root" );//file4
fileName.push_back( "rootfiles0/PhotonJetPt470_Summer09.root" );//file5
fileName.push_back( "rootfiles0/PhotonJetPt800_Summer09.root" );//file6

TString treeName = "TreePhotonJet";

TString outputFileName = "PhotonJetHists-2009-09-02-matchesReco.root";


//*********************************
//**** Set Scale
// The following 4 number set the scale
// example:
//   scale = (integrated luminosity (1/pb))*(cross section (pb))*(filter eff)/(events analyzed)
float invLuminosityToScaleTo = 200; // in pb-1

vector<float> crossSection;
crossSection.push_back( 2.887E5 -3.222E4 );  // in pb
crossSection.push_back( 3.222E4 -1.010E3 );
crossSection.push_back( 1.010E3 -5.143E1 );
crossSection.push_back( 5.143E1 -4.193E0 );
crossSection.push_back( 4.193E0 -4.515E-1 );
crossSection.push_back( 4.515E-1 -2.003E-2 );
crossSection.push_back( 2.003E-2 );

vector<float> filterEffeciency;
filterEffeciency.push_back( 1.0 );
filterEffeciency.push_back( 1.0 );
filterEffeciency.push_back( 1.0 );
filterEffeciency.push_back( 1.0 );
filterEffeciency.push_back( 1.0 );
filterEffeciency.push_back( 1.0 );
filterEffeciency.push_back( 1.0 );

vector<float> eventsAnalyzied;
eventsAnalyzied.push_back( 1073270 );
eventsAnalyzied.push_back( 1088546 );
eventsAnalyzied.push_back(  993509 );
eventsAnalyzied.push_back( 1483940 );
eventsAnalyzied.push_back( 1024589 );
eventsAnalyzied.push_back( 1014413 );
eventsAnalyzied.push_back( 1216320 );
// END of setting scale
//*********************************


//*********************************
//****         Set Cuts      ****//
// Variables will be plotted for each "location"
vector<TString> locationCut;
locationCut.push_back( "abs(hardGenPhoton_eta)>1.55&&abs(hardGenPhoton_eta)<2.5" );
locationCut.push_back( "abs(hardGenPhoton_eta)<1.45" );

vector<TString> locationName;
locationName.push_back( "Endcap" );
locationName.push_back( "Barrel" );

// These cuts will be merged into one giant cut, applied to all plots for all files
vector<TString> cuts;
cuts.push_back( "hardGenPhoton_et>15.0&&photon_et>15.0&&photon_matches_hardGen>0.5" );

// These cuts will be applied only to corresponding file
vector<TString> fileCuts;
fileCuts.push_back( "event_genEventScale>15&&event_genEventScale<30"    ); //file0
fileCuts.push_back( "event_genEventScale>30&&event_genEventScale<80"    ); //file1
fileCuts.push_back( "event_genEventScale>80&&event_genEventScale<170"   ); //file2
fileCuts.push_back( "event_genEventScale>170&&event_genEventScale<300"  ); //file3
fileCuts.push_back( "event_genEventScale>300&&event_genEventScale<470"  ); //file4
fileCuts.push_back( "event_genEventScale>470&&event_genEventScale<800"  ); //file5
fileCuts.push_back( "event_genEventScale>800&&event_genEventScale<1400" ); //file6
//**** END of setting cuts
//*********************************


//*********************************
int locationVariablesToPlot[2][2]; // [a][b], a=number of locations, b=2 (for min,max range of variables to plot)
locationVariablesToPlot[0][0] = 16; // Endcap
locationVariablesToPlot[0][1] = 35;
locationVariablesToPlot[1][0] = 16; // Barrel
locationVariablesToPlot[1][1] = 35;
/*locationVariablesToPlot[2][0] = 0;
locationVariablesToPlot[2][1] = 4;
locationVariablesToPlot[3][0] = 0;
locationVariablesToPlot[3][1] = 4;
locationVariablesToPlot[4][0] = 0;
locationVariablesToPlot[4][1] = 4;
locationVariablesToPlot[5][0] = 0;
locationVariablesToPlot[5][1] = 4;
locationVariablesToPlot[6][0] = 0;
locationVariablesToPlot[6][1] = 4;
locationVariablesToPlot[7][0] = 0;
locationVariablesToPlot[7][1] = 4;*/

// Variables you want plotted
vector<TString> variableToPlot;
// --- the following require gen level info
variableToPlot.push_back( "hardGenPhoton_et"  );  // 0
variableToPlot.push_back( "hardGenPhoton_eta" );
variableToPlot.push_back( "hardGenPhoton_phi" );
variableToPlot.push_back( "fmod(hardGenPhoton_phi+3.141592,20.0*3.141592/180.0)-10.0*3.141592/180.0" );
variableToPlot.push_back( "abs(hardGenPhoton_eta)" ); // 4
variableToPlot.push_back( "(recPhoton.energy-hardGenPhoton_energy)/hardGenPhoton_energy" );
variableToPlot.push_back( "(recPhoton.energy-hardGenPhoton_energy)/hardGenPhoton_energy:hardGenPhoton_energy"   );
variableToPlot.push_back( "(recPhoton.energy-hardGenPhoton_energy)/hardGenPhoton_energy:abs(hardGenPhoton_eta)" );
variableToPlot.push_back( "(recPhoton.energy-hardGenPhoton_energy)/hardGenPhoton_energy:hardGenPhoton_phiMod"   );
variableToPlot.push_back( "photon_hadronicOverEm:hardGenPhoton_et"       );
variableToPlot.push_back( "photon_hadronicOverEm:abs(hardGenPhoton_eta)" ); // 10
variableToPlot.push_back( "photon_hadronicOverEm:hardGenPhoton_phiMod"   );
variableToPlot.push_back( "photon_eta-hardGenPhoton_eta" );
variableToPlot.push_back( "photon_eta-hardGenPhoton_eta:hardGenPhoton_et" );
variableToPlot.push_back( "photon_eta-hardGenPhoton_eta:abs(hardGenPhoton_eta)" );
variableToPlot.push_back( "deltaPhiGenRecPhoton" );  // 15
// --- the following require only rec photons
variableToPlot.push_back( "photon_et"  );         // 16
variableToPlot.push_back( "photon_eta" );
variableToPlot.push_back( "photon_phi" );
variableToPlot.push_back( "fmod(photon_phi+3.141592,20.0*3.141592/180.0)-10.0*3.141592/180.0" );
variableToPlot.push_back( "abs(photon_eta)" );    // 20
variableToPlot.push_back( "photon_r9" );
variableToPlot.push_back( "photon_ecalRecHitSumEtConeDR03" );
variableToPlot.push_back( "photon_hcalTowerSumEtConeDR03"  );
variableToPlot.push_back( "photon_trkSumPtSolidConeDR03"   );
variableToPlot.push_back( "photon_trkSumPtHollowConeDR03"  ); //25
variableToPlot.push_back( "photon_nTrkSolidConeDR03"  );
variableToPlot.push_back( "photon_nTrkHollowConeDR03" );
variableToPlot.push_back( "photon_hadronicOverEm"     );
variableToPlot.push_back( "photon_r2x5" );
variableToPlot.push_back( "photon_ecalRecHitSumEtConeDR03/photon_et" ); // 30
variableToPlot.push_back( "photon_hcalTowerSumEtConeDR03/photon_et"  );
variableToPlot.push_back( "photon_trkSumPtSolidConeDR03/photon_et"   );
variableToPlot.push_back( "photon_trkSumPtHollowConeDR03/photon_et"  );
// --- the following require jets
/*variableToPlot.push_back( "calcDeltaPhi(photon_phi,jet_phi)"  );
variableToPlot.push_back( "calcDeltaPhi(photon_phi,jet2_phi)" ); // 35
variableToPlot.push_back( "calcDeltaPhi(jet_phi,jet2_phi)"    );*/
variableToPlot.push_back( "(photon_et-jet_et)/photon_et"      );
variableToPlot.push_back( "jet2_et/photon_et"                 );

// Histograms for the above variables
vector<TH1*> histogram;
// --- the following require gen level info
histogram.push_back( new TH1F("photonGenEt",     "Photon E_{T} ;E_{T} (GeV);entries per 15 GeV",  50,  0, 750)  );  // 0
histogram.push_back( new TH1F("photonGenEta",    "Photon #eta ;#eta;entries per 0.1 bin",   61, -3.05,   3.05) );
histogram.push_back( new TH1F("photonGenPhi",    "Photon #phi ;#phi;entries per bin",   62, (-1.-1./30.)*TMath::Pi(), (1.+1./30.)*TMath::Pi()) );
histogram.push_back( new TH1F("photonGenPhiMod", "Photon #phi_{mod} ", 42, (-1.-1./20)*0.1745329, (1.+1./20.)*0.1745329) );
histogram.push_back( new TH1F("photonGenAbsEta", "Photon |#eta| ", 51, 0.00,   2.55) );
histogram.push_back( new TH1F("photonDeltaE",    "(E(#gamma_{rec})-E(#gamma_{gen}))/E(#gamma_{gen}) ", 50, -0.8, 0.3) );
histogram.push_back( new TH2F("photonDeltaE_vs_E","(E(#gamma_{rec})-E(#gamma_{gen}))/E(#gamma_{gen}) vs E(#gamma_{gen}) ", 50, 0, 3000, 50, -0.8, 0.3) );
histogram.push_back( new TH2F("photonDeltaE_vs_AbsEta","(E(#gamma_{rec})-E(#gamma_{gen}))/E(#gamma_{gen}) vs |#eta(#gamma_{gen}|) ", 51, 0.0, 2.5, 50, -0.8, 0.3) );
histogram.push_back( new TH2F("photonDeltaE_vs_PhiMod","(E(#gamma_{rec})-E(#gamma_{gen}))/E(#gamma_{gen}) vs #phi_{mod}(#gamma_{gen}) ", 42, (-1.-1./20)*0.1745329, (1.+1./20.)*0.1745329, 50, -0.9, 0.2) );
histogram.push_back( new TH2F("photonHoverE_vs_Et",     "H/E vs E_{T}(#gamma_{gen}) ", 50, 0, 1000, 50, 0.0, 0.2) );
histogram.push_back( new TH2F("photonHoverE_vs_AbsEta", "H/E vs |#eta(#gamma_{gen})| ", 51, 0.0, 2.5, 50, 0.0, 0.2) );
histogram.push_back( new TH2F("photonHoverE_vs_PhiMod", "H/E vs #phi_{mod}(#gamma_{gen}) ", 42, (-1.-1./20)*0.1745329, (1.+1./20.)*0.1745329, 50, 0.0, 0.2) );
histogram.push_back( new TH1F("photonDeltaEta", "#Delta#eta(#gamma_{rec},#gamma_{gen}) ;#Delta#eta(#gamma_{rec},#gamma_{gen});entries/bin", 41, -0.01, 0.01) );
histogram.push_back( new TH2F("photonDeltaEta_vs_Et",    "#Delta#eta(#gamma_{rec},#gamma_{gen}) vs E_{T}(#gamma_{gen}) ", 50, 0, 1000, 41, -0.1, 0.1) );
histogram.push_back( new TH2F("photonDeltaEta_vs_AbsEta","#Delta#eta(#gamma_{rec},#gamma_{gen}) vs #eta(#gamma_{gen})", 51, 0.0, 2.55, 41, -0.1, 0.1) );
histogram.push_back( new TH1F("photonDeltaPhi",          "#Delta#phi(#gamma_{rec},#gamma_{gen}) ;#Delta#phi(#gamma_{rec},#gamma_{gen});entries/bin", 41, 0.0, 0.01) ); // 15
// --- the following require only rec photons
histogram.push_back( new TH1F("photonEt",        "Photon E_{T} ;E_{T} (GeV);entries per 15 GeV", 50,  0,    750   ) ); // 16
histogram.push_back( new TH1F("photonEta",       "Photon #eta ;#eta;entries per 0.1"           , 61, -3.05,   3.05) );
histogram.push_back( new TH1F("photonPhi",       "Photon #phi ;#phi;entries per bin"           , 62, (-1.-1./30.)*TMath::Pi(), (1.+1./30.)*TMath::Pi()) );
histogram.push_back( new TH1F("photonPhiMod",    "Photon #phi_{mod} "                          , 42, (-1.-1./20)*0.1745329, (1.+1./20.)*0.1745329) );
histogram.push_back( new TH1F("photonAbsEta",    "Photon |#eta| "                              , 51, 0.00,  2.55) );  // 20
histogram.push_back( new TH1F("photonR9",        "R9 = E(3x3) / E(SuperCluster)  ;R9;entries/bin"   , 50, 0.6, 1.0) );
histogram.push_back( new TH1F("photonEcalIso",   "#SigmaEcal Rec Hit E_{T} in Hollow #DeltaR cone " , 50, 0  , 15) );
histogram.push_back( new TH1F("photonHcalIso",   "#SigmaHcal Rec Hit E_{T} in Hollow #DeltaR cone " , 50, 0  , 15) );
histogram.push_back( new TH1F("photonTrackSolidIso",     "#Sigmatrack p_{T} in Solid #DeltaR cone " , 50, 0  , 15) );
histogram.push_back( new TH1F("photonTrackHollowIso",    "#Sigmatrack p{T} in Hollow #DeltaR cone " , 50, 0  , 15) );  // 25
histogram.push_back( new TH1F("photonTrackCountSolid",   "Number of tracks in Solid #DeltaR cone ;Number of Tracks;entries/bin" , 25, -0.5, 24.5) );
histogram.push_back( new TH1F("photonTrackCountHollow",  "Number of tracks in Hollow #DeltaR cone ;Number of Tracks;entries/bin", 25, -0.5, 24.5) );
histogram.push_back( new TH1F("photonHoverE",            "Hadronic / EM ", 50, 0.0, 0.2) );
histogram.push_back( new TH1F("photonScSeedE2x5over5x5", "E2x5/E5x5  "   , 50, 0.6, 1.0) );
histogram.push_back( new TH1F("photonEcalIsoOverE",        "#SigmaEcal Rec Hit E_{T} in #DeltaR cone / Photon E_{T} " , 50, -0.1, 1.0) ); // 30
histogram.push_back( new TH1F("photonHcalIsoOverE",        "#SigmaHcal Rec Hit E_{T} in #DeltaR cone / Photon E_{T} " , 50, -0.1, 1.0) );
histogram.push_back( new TH1F("photonTrackSolidIsoOverE" , "#SigmaTrack p_{T} in #DeltaR cone / Photon E_{T} "        , 50, -0.1, 1.0) );
histogram.push_back( new TH1F("photonTrackHollowIsoOverE", "#SigmaTrack p_{T} in Hollow #DeltaR cone / Photon E_{T} " , 50, -0.1, 1.0) );
// --- the following require jets
/*histogram.push_back( new TH1F("h_deltaPhi_photon_jet", "#Delta#phi between Highest E_{T} #gamma and jet;#Delta#phi(#gamma,1^{st} jet)"               , 50, 0, 3.1415926) );
histogram.push_back( new TH1F("h_deltaPhi_photon_jet2","#Delta#phi between Highest E_{T} #gamma and 2^{nd} highest jet;#Delta#phi(#gamma,2^{nd} jet)", 50, 0, 3.1415926) );
histogram.push_back( new TH1F("h_deltaPhi_jet_jet2"  , "#Delta#phi between Highest E_{T} jet and 2^{nd} jet;#Delta#phi(1^{st} jet,2^{nd} jet)"       , 50, 0, 3.1415926) );*/
histogram.push_back( new TH1F("h_deltaEt_photon_jet" , "(E_{T}(#gamma)-E_{T}(jet))/E_{T}(#gamma) when #Delta#phi(#gamma,1^{st} jet) > 2.8;#DeltaE_{T}(#gamma,1^{st} jet)/E_{T}(#gamma)", 20, -1.0, 1.0) );
histogram.push_back( new TH1F("h_jet2_etOverPhotonEt", "E_{T}(2^{nd} highest jet) / E_{T}(#gamma);E_{T}(2^{nd} Jet)/E_{T}(#gamma)", 20, 0.0, 4.0) );
//****                  END of Variables                        ****//
//********************************************************************




//********************************************************************
//****                Main part of Program                      ****//

 // Human error checking
 if (variableToPlot.size() != histogram.size() ) {
   cout << "Should have equal entries in histogram and variableToPlot vector." << endl;
   return;
 }
 if (fileName.size() > crossSection.size() ) {
   cout << "Should have equal entries in fileName and crossSection vetor." << endl;
   return;
 }
 if (fileName.size() > fileCuts.size() ) {
   cout << "Should have equal entries in fileName and fileCuts vector." << endl;
   return;
 }

 // Combine all the cuts into one
 cout << endl << "Cuts that will be applied to everything: " << endl << "  ";
 TCut allCuts = "";
 for (int i =0; i<cuts.size(); i++) {
   allCuts += cuts[i];
   if (i>0) cout << "&&";
   cout << "(" << cuts[i] << ")";
 }
 cout << endl << endl;


 // Open the files & set their scales
 cout << endl << "Histograms will be scaled to " << invLuminosityToScaleTo << "pb-1 " << endl;
 cout << "Looking for TTree named \"" << treeName << "\" in files..." << endl;
 vector<float> fileScale;
 TList *fileList = new TList();
 for (int i=0; i < fileName.size(); i++) {

   TFile* currentFile = TFile::Open(fileName[i]);
   fileList->Add(currentFile);
   float currentScale = crossSection[i]*invLuminosityToScaleTo*filterEffeciency[i]/eventsAnalyzied[i];
   fileScale.push_back( currentScale );

   // Display entries in that file's TTree
   TTree* tree;
   currentFile->GetObject(treeName, tree);
   cout << "file" << i <<": " << fileName[i] << " contains " << tree->GetEntries(allCuts) << " entries, and will be scaled by " << 
currentScale << endl;
 }
 cout << endl << endl;


 //Create output file
 TFile *outputFile = TFile::Open( outputFileName, "RECREATE" );


 //************************************************************
 //                 Core of the Script                       //
 // Loop over locations
 for (int l=0; l<locationName.size(); l++) {
   TString currentLocation = locationName[l];
   TCut currentCuts = allCuts;
   currentCuts += locationCut[l];
   cout << "Creating plots for " << currentLocation << ", " << locationCut[l] << endl;
  
   // Loop over variables to plot
   for (int i=0; i<variableToPlot.size(); i++) {
     // should we plot this variable for this location?
     if (i<locationVariablesToPlot[l][0] || i>locationVariablesToPlot[l][1]) continue;

     TString currentHistType  = histogram[i]->IsA()->GetName();
     TString currentHistName  = TString(histogram[i]->GetName())  + "_" + currentLocation;
     TString currentHistTitle = TString(histogram[i]->GetTitle()) + "(" + currentLocation + ")";
     cout << "  " << variableToPlot[i] << " >> " << currentHistName;
     TString currentHistDrawOpt;
     if (currentHistType=="TH2F") {
       currentHistDrawOpt="goffbox";
     } else {
       currentHistDrawOpt="egoff";
     }
     TH1* currentHist = (TH1*)histogram[i]->Clone(currentHistName);  // Creates clone with name currentHistName
     currentHist->Sumw2(); // store errors
     currentHist->SetTitle(currentHistTitle);
     //cout << " from file";

     // Plot from the first file
     int f = 0;
     //cout << f;
     TTree *tree;
     TFile *current_file = (TFile*)fileList->First();
     current_file->cd();
     current_file->GetObject(treeName, tree);
     tree->Draw(variableToPlot[i]+">>"+currentHistName,currentCuts+TCut(fileCuts[f]),currentHistDrawOpt);
     currentHist->Scale(fileScale[f]);
     f++;

     // Loop over files
     current_file = (TFile*)fileList->After( current_file );
     while ( current_file ) {
       current_file->cd();
       //cout << ", file" << f;
       current_file->GetObject(treeName, tree);

       TString tempHistName = currentHistName+"Temp";
       TH1* tempHist = (TH1*)currentHist->Clone(tempHistName);
       tree->Draw(variableToPlot[i]+">>"+tempHistName,currentCuts+TCut(fileCuts[f]),currentHistDrawOpt);
       tempHist->Scale(fileScale[f]);
       currentHist->Add(tempHist);
       tempHist->Delete();

       current_file = (TFile*)fileList->After( current_file );
       f++;
     } // End of loop over files

     outputFile->cd();
     currentHist->Write();
     cout << endl;
   } // End of loop over variabls to plot
 } // End of loop over locations
 //                  END of Core of Script                   //
 //************************************************************

 cout << endl;
 cout << "Wrote file " << outputFileName << endl;
 cout << endl;
 outputFile->Close();
}
