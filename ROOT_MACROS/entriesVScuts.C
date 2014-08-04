/*************************************************
  This script lists the number of entries left
  in a TTree after each additional cut.

  Michael B. Anderson
  Feb 19, 2009
*************************************************/
{
  //************************************************
  // Variables
  TFile *file1 = new TFile("PhotonJetPt15_realPhotons.root");

  TTree *tree1;
  file1->GetObject("TreePhotonMatched",tree1);

  vector<TString> cuts;
  cuts.push_back( "recPhoton.et>20&&abs(recPhoton.eta)<2.5&&recPhoton.hadronicOverEm<0.1" );
  cuts.push_back( "recPhoton.hadronicOverEm<0.005" );
  cuts.push_back( "recPhoton.r9>0.8" );
  cuts.push_back( "recPhoton.isolationHcalRecHit<15" );
  cuts.push_back( "recPhoton.isolationEcalRecHit<15" );
  cuts.push_back( "recPhoton.SolidTrkCone<5" );
  cuts.push_back( "((abs(recPhoton.eta)<1.48&&recPhoton.isEBGap<1)||abs(recPhoton.eta)>1.55)" );
  //************************************************


  //************************************************
  cout << "File:\t" << file1.GetName() << endl;
  cout << "Tree:\t" << tree1.GetName() << endl;
  int initialEntries = tree1->GetEntries();
  cout << initialEntries << " entries at start." << endl << endl;

  cout << "Entries left after each progressive cut:" << endl;
  TCut currentCuts = "";
  for (int i =0; i<cuts.size(); i++) {
    currentCuts += cuts[i];
    int currentEntries = tree1->GetEntries(currentCuts);

    // Print number of entries passing cuts
    float precentLeft  = (100.0 * currentEntries) / initialEntries;
    printf("%i\t%4.1f\t%s \n",currentEntries,precentLeft,cuts[i].Data());
  }
}
