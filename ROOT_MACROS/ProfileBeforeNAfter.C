/*************************************************
  Plots 2 profile plots onto same canvas before
  and after a specific cut.

  Should be used with my rootlogon.C


  Example Use:
    ProfileBeforeNAfter(TreePhotonAndJet,"(recPhtn.et-genParticle1.et)/genParticle1.et:genParticle1.et","recPhtn.isLoosePhoton>0","recPhtn.isTightPhoton>0&&recPhtn.r9>0.93","Loose #gamma","Tight #gamma","(RecEt-GenEt)/GenEt","E_{T} (GeV)")

  Michael B. Anderson
  June 30, 2008
*************************************************/

void ProfileBeforeNAfter(TTree* aTree,
                         TString thingToPlot,
                         TString cuts1,
                         TString cuts2,
                         TString label1="", 
                         TString label2="",
                         TString title="",
                         TString xAxisLabel="") {

  int events1 = aTree->Draw(thingToPlot,cuts1,"box");
  htemp->ProfileX()->Draw();
  h1=(TH1*)htemp_pfx->Clone();

  int events2 = aTree->Draw(thingToPlot,cuts2,"box");
  htemp->ProfileX()->Draw();
  h2=(TH1*)htemp_pfx->Clone();

  if (title == "") {
    h1->SetTitle(thingToPlot);
  } else {
    h1->SetTitle(title);
  }
  if (xAxisLabel != "") h1->GetXaxis()->SetTitle(xAxisLabel);

  cout << "Plotting " << thingToPlot << endl;
  cout << events1 << " events pass " << cuts1 << endl;
  cout << events2 << " events pass " << cuts2 << endl;
  cout << "y mean1 = " << h1->GetMean(2) << endl;
  cout << "y mean2 = " << h2->GetMean(2) << endl;

  // This is a thing loaded in the rootlogon.C
  Plot2hists1D(h1,h2,label1,label2);

}
