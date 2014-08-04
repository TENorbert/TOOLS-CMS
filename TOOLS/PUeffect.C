// This Tool actually Just Overlays Different Histograms
// @ La TEN

#include  "TH1.h"
#include  "TFile.h"
#include  "TLegend.h"
#include  "string.h"
#include  "TCanvas.h"
#include  "TProfile.h"
#include  "TStyle.h"

using namespace std;
// Define Fxn Here!



void PUeffect()
{


 gStyle->SetOptStat(0);

  TFile *f1 = new TFile("met50Cut_LessThan3Jets_DP_All_Trig_NoJetID.root");
  TFile *f2 = new TFile("met50Cut_MoreThan3Jets_DP_All_Trig_NoJetID.root");
  TFile *f3 = new TFile("metcut50_ALLJetsgmsbSum12cL140tau2000_DP_All_Trig_NoJetID.root");
   

//Ecal Iso
TProfile *EcalIso1 = (TProfile*)f1->Get("NeutKin/my.ecalIso");
TProfile *EcalIso2 = (TProfile*)f2->Get("NeutKin/my.ecalIso");
TProfile *EcalIso3 = (TProfile*)f3->Get("NeutKin/my.ecalIso");

//HcalIso
TProfile *HcalIso1 = (TProfile*)f1->Get("NeutKin/my.hcalIso");
TProfile *HcalIso2 = (TProfile*)f2->Get("NeutKin/my.hcalIso");
TProfile *HcalIso3 = (TProfile*)f3->Get("NeutKin/my.hcalIso");

//TrkIso
TProfile *TrkIso1 =  (TProfile*)f1->Get("NeutKin/my.trkIso");
TProfile *TrkIso2 =  (TProfile*)f2->Get("NeutKin/my.trkIso");
TProfile *TrkIso3 =  (TProfile*)f3->Get("NeutKin/my.trkIso");

//2012 HcalIso
TProfile *nHcalIso1 = (TProfile*)f1->Get("NeutKin/my.HcalIso");
TProfile *nHcalIso2 = (TProfile*)f2->Get("NeutKin/my.HcalIso");
TProfile *nHcalIso3 = (TProfile*)f3->Get("NeutKin/my.HcalIso");

//sMinor
TProfile *sminor1 = (TProfile*)f1->Get("NeutKin/my.sminor");
TProfile *sminor2 = (TProfile*)f2->Get("NeutKin/my.sminor");
TProfile *sminor3 = (TProfile*)f3->Get("NeutKin/my.sminor");

//EcalTiming Vs PU
TH1D* photVtx1 = (TH1D*)f1->Get("NeutKin/my.tV1");
TH1D* photVtx2 = (TH1D*)f1->Get("NeutKin/my.tV2");
TH1D* photVtx3 = (TH1D*)f1->Get("NeutKin/my.tV3");
TH1D* photVtx4 = (TH1D*)f1->Get("NeutKin/my.tV4");
TH1D* photVtx5 = (TH1D*)f1->Get("NeutKin/my.tV5");
TH1D* photVtx6 = (TH1D*)f1->Get("NeutKin/my.tV6");
TH1D* photVtx7 = (TH1D*)f1->Get("NeutKin/my.tV7");
//TH1D* photVtx8 = (TH1D*)f1->Get("NeutKin/my.tV8");
//TH1D* photVtx9 = (TH1D*)f1->Get("NeutKin/my.tV9");
//TH1D* photVtx10 = (TH1D*)f1->Get("NeutKin/my.tV10");
//TH1D* photVtx11 = (TH1D*)f1->Get("NeutKin/my.tV11");

// W AVe SC Time
TH1D* photVtx12 = (TH1D*)f1->Get("NeutKin/my.tV12");
TH1D* photVtx13 = (TH1D*)f1->Get("NeutKin/my.tV13");
TH1D* photVtx14 = (TH1D*)f1->Get("NeutKin/my.tV14");
TH1D* photVtx15 = (TH1D*)f1->Get("NeutKin/my.tV15");
TH1D* photVtx16 = (TH1D*)f1->Get("NeutKin/my.tV16");
TH1D* photVtx17 = (TH1D*)f1->Get("NeutKin/my.tV17");
TH1D* photVtx18 = (TH1D*)f1->Get("NeutKin/my.tV18");
//TH1D* photVtx19 = (TH1D*)f1->Get("NeutKin/my.tV19");
//TH1D* photVtx20 = (TH1D*)f1->Get("NeutKin/my.tV20");
//TH1D* photVtx21 = (TH1D*)f1->Get("NeutKin/my.tV21");
//TH1D* photVtx22 = (TH1D*)f1->Get("NeutKin/my.tV22");

// W AVe Seed BC Time
TH1D* phoWAvSBCtime13 = (TH1D*)f1->Get("NeutKin/my.py13");
TH1D* phoWAvSBCtime14 = (TH1D*)f1->Get("NeutKin/my.py14");
TH1D* phoWAvSBCtime15 = (TH1D*)f1->Get("NeutKin/my.py15");
TH1D* phoWAvSBCtime16 = (TH1D*)f1->Get("NeutKin/my.py16");
TH1D* phoWAvSBCtime17 = (TH1D*)f1->Get("NeutKin/my.py17");
TH1D* phoWAvSBCtime18 = (TH1D*)f1->Get("NeutKin/my.py18");
TH1D* phoWAvSBCtime19 = (TH1D*)f1->Get("NeutKin/my.py19");

//Set Colors
EcalIso1->SetLineColor(6);
EcalIso2->SetLineColor(9);
EcalIso3->SetLineColor(12);

HcalIso1->SetLineColor(6);
HcalIso2->SetLineColor(9);
HcalIso3->SetLineColor(12);

TrkIso1->SetLineColor(6);
TrkIso2->SetLineColor(9);
TrkIso3->SetLineColor(12);

nHcalIso1->SetLineColor(6);
nHcalIso2->SetLineColor(9);
nHcalIso3->SetLineColor(12);

sminor1->SetLineColor(6);
sminor2->SetLineColor(9);
sminor3->SetLineColor(12);

// W Ave Seed  BC Time
photVtx1->SetLineColor(1);
photVtx2->SetLineColor(2);
photVtx3->SetLineColor(3);
photVtx4->SetLineColor(4);
photVtx5->SetLineColor(5);
photVtx6->SetLineColor(6);
photVtx7->SetLineColor(7);
//photVtx8->SetLineColor(8);
//photVtx9->SetLineColor(9);
//photVtx10->SetLineColor(10);
//photVtx11->SetLineColor(11);

// W Ave SC Time
photVtx12->SetLineColor(1);
photVtx13->SetLineColor(2);
photVtx14->SetLineColor(3);
photVtx15->SetLineColor(4);
photVtx16->SetLineColor(5);
photVtx17->SetLineColor(6);
photVtx18->SetLineColor(7);
//photVtx19->SetLineColor(8);
//photVtx20->SetLineColor(9);
//photVtx21->SetLineColor(10);
//photVtx22->SetLineColor(11);


// W Ave Seed BC Time
phoWAvSBCtime13->SetLineColor(2);
phoWAvSBCtime14->SetLineColor(3);
phoWAvSBCtime15->SetLineColor(4);
phoWAvSBCtime16->SetLineColor(5);
phoWAvSBCtime17->SetLineColor(6);
phoWAvSBCtime18->SetLineColor(7);
phoWAvSBCtime19->SetLineColor(8);

//Set Marker Style
EcalIso1->SetMarkerStyle(6);
EcalIso2->SetMarkerStyle(9);
EcalIso3->SetMarkerStyle(12);

HcalIso1->SetMarkerStyle(6);
HcalIso2->SetMarkerStyle(9);
HcalIso3->SetMarkerStyle(12);

TrkIso1->SetMarkerStyle(6);
TrkIso2->SetMarkerStyle(9);
TrkIso3->SetMarkerStyle(12);

nHcalIso1->SetMarkerStyle(6);
nHcalIso2->SetMarkerStyle(9);
nHcalIso3->SetMarkerStyle(12);

sminor1->SetMarkerStyle(6);
sminor2->SetMarkerStyle(9);
sminor3->SetMarkerStyle(12);

// Scale Histograms
if(EcalIso1->Integral()!=0) EcalIso1->Scale(1/EcalIso1->Integral());
if(EcalIso2->Integral()!=0) EcalIso2->Scale(1/EcalIso2->Integral());
if(EcalIso3->Integral()!=0) EcalIso3->Scale(1/EcalIso3->Integral());

if(HcalIso1->Integral()!=0) HcalIso1->Scale(1/HcalIso1->Integral());
if(HcalIso2->Integral()!=0) HcalIso2->Scale(1/HcalIso2->Integral());
if(HcalIso3->Integral()!=0) HcalIso3->Scale(1/HcalIso3->Integral());

if(TrkIso1->Integral()!=0) TrkIso1->Scale(1/TrkIso1->Integral());
if(TrkIso2->Integral()!=0) TrkIso2->Scale(1/TrkIso2->Integral());
if(TrkIso3->Integral()!=0) TrkIso3->Scale(1/TrkIso3->Integral());

if(nHcalIso1->Integral()!=0) nHcalIso1->Scale(1/nHcalIso1->Integral());
if(nHcalIso2->Integral()!=0) nHcalIso2->Scale(1/nHcalIso2->Integral());
if(nHcalIso3->Integral()!=0) nHcalIso3->Scale(1/nHcalIso3->Integral());

if(sminor1->Integral()!=0) sminor1->Scale(1/sminor1->Integral());
if(sminor2->Integral()!=0) sminor2->Scale(1/sminor2->Integral());
if(sminor3->Integral()!=0) sminor3->Scale(1/sminor3->Integral());

if(photVtx1->Integral()!=0) photVtx1->Scale(1/photVtx1->Integral());
if(photVtx2->Integral()!=0) photVtx2->Scale(1/photVtx2->Integral());
if(photVtx3->Integral()!=0) photVtx3->Scale(1/photVtx3->Integral());
if(photVtx4->Integral()!=0) photVtx4->Scale(1/photVtx4->Integral());
if(photVtx5->Integral()!=0) photVtx5->Scale(1/photVtx5->Integral());
if(photVtx6->Integral()!=0) photVtx6->Scale(1/photVtx6->Integral());
if(photVtx7->Integral()!=0) photVtx7->Scale(1/photVtx7->Integral());
//if(photVtx8->Integral()!=0) photVtx8->Scale(1/photVtx8->Integral());
//if(photVtx9->Integral()!=0) photVtx9->Scale(1/photVtx9->Integral());
//if(photVtx10->Integral()!=0) photVtx10->Scale(1/photVtx10->Integral());
//if(photVtx11->Integral()!=0) photVtx11->Scale(1/photVtx11->Integral());


// W AVe SC Time
if(photVtx12->Integral()!=0) photVtx12->Scale(1/photVtx12->Integral());
if(photVtx13->Integral()!=0) photVtx13->Scale(1/photVtx13->Integral());
if(photVtx14->Integral()!=0) photVtx14->Scale(1/photVtx14->Integral());
if(photVtx15->Integral()!=0) photVtx15->Scale(1/photVtx15->Integral());
if(photVtx16->Integral()!=0) photVtx16->Scale(1/photVtx16->Integral());
if(photVtx17->Integral()!=0) photVtx17->Scale(1/photVtx17->Integral());
if(photVtx18->Integral()!=0) photVtx18->Scale(1/photVtx18->Integral());
//if(photVtx19->Integral()!=0) photVtx19->Scale(1/photVtx19->Integral());
//if(photVtx20->Integral()!=0) photVtx20->Scale(1/photVtx20->Integral());
//if(photVtx21->Integral()!=0) photVtx21->Scale(1/photVtx21->Integral());
//if(photVtx22->Integral()!=0) photVtx22->Scale(1/photVtx22->Integral());

// W AVe seed BC Time
if(phoWAvSBCtime13->Integral()!=0) phoWAvSBCtime13->Scale(1/phoWAvSBCtime13->Integral());
if(phoWAvSBCtime14->Integral()!=0) phoWAvSBCtime14->Scale(1/phoWAvSBCtime14->Integral());
if(phoWAvSBCtime15->Integral()!=0) phoWAvSBCtime15->Scale(1/phoWAvSBCtime15->Integral());
if(phoWAvSBCtime16->Integral()!=0) phoWAvSBCtime16->Scale(1/phoWAvSBCtime16->Integral());
if(phoWAvSBCtime17->Integral()!=0) phoWAvSBCtime17->Scale(1/phoWAvSBCtime17->Integral());
if(phoWAvSBCtime18->Integral()!=0) phoWAvSBCtime18->Scale(1/phoWAvSBCtime18->Integral());
if(phoWAvSBCtime19->Integral()!=0) phoWAvSBCtime19->Scale(1/phoWAvSBCtime19->Integral());


//Label Axis
EcalIso1->GetXaxis()->SetTitle("nVtx");
EcalIso1->GetYaxis()->SetTitle("EcalIso");
EcalIso1->SetTitle("EcalIso Vs PU");
HcalIso1->GetXaxis()->SetTitle("nVtx");
HcalIso1->GetYaxis()->SetTitle("HcalIso");
HcalIso1->SetTitle("HcalIso Vs PU");
TrkIso1->GetXaxis()->SetTitle("nVtx");
TrkIso1->GetYaxis()->SetTitle("TrkIso");
TrkIso1->SetTitle("TrackIso Vs PU");
nHcalIso1->GetXaxis()->SetTitle("nVtx");
nHcalIso1->GetYaxis()->SetTitle("2012 HcalIso");
nHcalIso1->SetTitle("2012 HcalIso");
sminor1->GetXaxis()->SetTitle("nVtx");
sminor1->GetYaxis()->SetTitle("sMinor");
sminor1->SetTitle("SMinor Vs PU");


photVtx6->GetXaxis()->SetTitle("Photon Time[ns]");
photVtx6->GetYaxis()->SetTitle("No. Event X 10^3");
photVtx6->Clear("");
photVtx6->SetTitle("Photon(Seed XTal) Time");

photVtx17->GetXaxis()->SetTitle("Photon Time[ns]");
photVtx17->GetYaxis()->SetTitle("No. Event X 10^3");
photVtx17->Clear("");
photVtx17->SetTitle("Photon(W. Ave SC) Time");

phoWAvSBCtime19->GetXaxis()->SetTitle("Photon Time[ns]");
phoWAvSBCtime19->GetYaxis()->SetTitle("No. Event X 10^3");
phoWAvSBCtime19->Clear("");
phoWAvSBCtime19->SetTitle("Photon(W. Ave Seed BC) Time");
//Draw Plots
//Draw Plots
TCanvas *c1 = new TCanvas("c1","EcalIso", 800, 800);
c1->cd();
EcalIso1->Draw();
EcalIso2->Draw("Sames");
EcalIso3->Draw("Sames");

TLegend* lg1 = new TLegend(0.14, 0.70, 0.47, 0.90);
lg1->SetBorderSize(1);
lg1->SetFillColor(0);
lg1->AddEntry(EcalIso1, "< 3Jets + VLIso#gamma","l");
lg1->AddEntry(EcalIso2, ">=3Jets + VLIso#gamma","l");
lg1->AddEntry(EcalIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l");
lg1->SetTextSize(0.018);
lg1->SetHeader("Samples");
lg1->Draw();
c1->SetLogy();

TCanvas *c2 = new TCanvas("c2","HcalIso", 800, 800);
c2->cd();
HcalIso1->Draw();
HcalIso2->Draw("Sames");
HcalIso3->Draw("Sames");

TLegend* lg2 = new TLegend(0.14, 0.70, 0.47, 0.90);
lg2->SetBorderSize(1);
lg2->SetFillColor(0);
lg2->AddEntry(HcalIso1, "< 3Jets + VLIso#gamma","l");
lg2->AddEntry(HcalIso2, ">=3Jets + VLIso#gamma","l");
lg2->AddEntry(HcalIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l");
lg2->SetTextSize(0.018);
lg2->SetHeader("Samples");
lg2->Draw();
c2->SetLogy();

TCanvas *c3 = new TCanvas("c3","TrkIso", 800, 800);
c3->cd();
TrkIso1->Draw();
TrkIso2->Draw("Sames");
TrkIso3->Draw("Sames");

TLegend* lg3 = new TLegend(0.14, 0.70, 0.47, 0.90);
lg3->SetBorderSize(1);
lg3->SetFillColor(0);
lg3->AddEntry(TrkIso1, "< 3Jets + VLIso#gamma","l");
lg3->AddEntry(TrkIso2, ">=3Jets + VLIso#gamma","l");
lg3->AddEntry(TrkIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l");
lg3->SetTextSize(0.018);
lg3->SetHeader("Samples");
lg3->Draw();
c3->SetLogy();

TCanvas *c4 = new TCanvas("c4","nHcalIso", 800, 800);
c4->cd();
nHcalIso1->Draw();
nHcalIso2->Draw("Sames");
nHcalIso3->Draw("Sames");

TLegend* lg4 = new TLegend(0.14, 0.70, 0.47, 0.90);
lg4->SetBorderSize(1);
lg4->SetFillColor(0);
lg4->AddEntry(nHcalIso1, "< 3Jets + VLIso#gamma","l");
lg4->AddEntry(nHcalIso2, ">=3Jets + VLIso#gamma","l");
lg4->AddEntry(nHcalIso3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l");
lg4->SetTextSize(0.018);
lg4->SetHeader("Samples");
lg4->Draw();
c4->SetLogy();

TCanvas *c5 = new TCanvas("c5","sminor", 800, 800);
c5->cd();
sminor1->Draw();
sminor2->Draw("Sames");
sminor3->Draw("Sames");

TLegend* lg5 = new TLegend(0.14, 0.70, 0.47, 0.90);
lg5->SetBorderSize(1);
lg5->SetFillColor(0);
lg5->AddEntry(sminor1, "< 3Jets + VLIso#gamma","l");
lg5->AddEntry(sminor2, ">=3Jets + VLIso#gamma","l");
lg5->AddEntry(sminor3,"GMSB-c#tau=2000mm-#Lambda=140TeV","l");
lg5->SetTextSize(0.018);
lg5->SetHeader("Samples");
lg5->Draw();
c5->SetLogy();

TCanvas *c6 = new TCanvas("c6","photonTimeVsPU", 800, 800);
c6->cd();
photVtx6->Draw();
photVtx2->Draw("Sames");
photVtx3->Draw("Sames");
photVtx4->Draw("Sames");
photVtx5->Draw("Sames");
photVtx1->Draw("Sames");
photVtx7->Draw("Sames");
//photVtx8->Draw("Sames");
//photVtx9->Draw("Sames");
//photVtx10->Draw("Sames");
//photVtx11->Draw("Sames");

TLegend* lg6 = new TLegend(0.25, 0.65, 0.45, 0.90);
lg6->SetBorderSize(1);
lg6->SetFillColor(0);
lg6->AddEntry(photVtx1,"0<= nVtx <=7","l");
lg6->AddEntry(photVtx2,"7 <= nVtx <=14","l");
lg6->AddEntry(photVtx3,"14 < nVtx <=21","l");
lg6->AddEntry(photVtx4,"21 < nVtx <=28","l");
lg6->AddEntry(photVtx5,"28 < nVtx <=35","l");
lg6->AddEntry(photVtx6,"35 < nVtx <=42","l");
lg6->AddEntry(photVtx7,"42 < nVtx <=49","l");
//lg6->AddEntry(photVtx8,"35 < nVtx <=40","l");
//lg6->AddEntry(photVtx9,"40 < nVtx <=45","l");
//lg6->AddEntry(photVtx10,"45 < nVtx <=50","l");
//lg6->AddEntry(photVtx11,"50 < nVtx <=55","l");
lg6->SetTextSize(0.018);
lg6->SetHeader("NVertices");
lg6->Draw();
c6->SetLogy();

TCanvas *c7 = new TCanvas("c7","photonSCTimeVsPU", 800, 800);
c7->cd();
photVtx17->Draw("");
photVtx13->Draw("Sames");
photVtx14->Draw("Sames");
photVtx15->Draw("Sames");
photVtx16->Draw("Sames");
photVtx12->Draw("Sames");
photVtx18->Draw("Sames");
//photVtx19->Draw("Sames");
//photVtx20->Draw("Sames");
//photVtx21->Draw("Sames");
//photVtx22->Draw("Sames");

TLegend* lg7 = new TLegend(0.25, 0.65, 0.45, 0.90);
lg7->SetBorderSize(1);
lg7->SetFillColor(0);
lg7->AddEntry(photVtx12,"0<= nVtx <=7","l");
lg7->AddEntry(photVtx13,"7<= nVtx <=14","l");
lg7->AddEntry(photVtx14,"14 < nVtx <=21","l");
lg7->AddEntry(photVtx15,"21 < nVtx <=28","l");
lg7->AddEntry(photVtx16,"28 < nVtx <=35","l");
lg7->AddEntry(photVtx17,"35 < nVtx <=42","l");
lg7->AddEntry(photVtx18,"42 < nVtx <=49","l");
//lg7->AddEntry(photVtx19,"35 < nVtx <=40","l");
//lg7->AddEntry(photVtx20,"40 < nVtx <=45","l");
//lg7->AddEntry(photVtx21,"45 < nVtx <=50","l");
//lg7->AddEntry(photVtx22,"50 < nVtx <=55","l");
lg7->SetTextSize(0.018);
lg7->SetHeader("NVertices");
lg7->Draw();
c7->SetLogy();

TCanvas *c8 = new TCanvas("c8","photonSeedBCTimeVsPU", 800, 800);
c8->cd();
phoWAvSBCtime19->Draw("");
phoWAvSBCtime14->Draw("Sames");
phoWAvSBCtime15->Draw("Sames");
phoWAvSBCtime16->Draw("Sames");
phoWAvSBCtime17->Draw("Sames");
phoWAvSBCtime18->Draw("Sames");
phoWAvSBCtime13->Draw("Sames");

TLegend* lg8 = new TLegend(0.25, 0.65, 0.42, 0.90);
lg8->SetBorderSize(1);
lg8->SetFillColor(0);
lg8->AddEntry(phoWAvSBCtime13,"45 < nVtx <=55","l");
lg8->AddEntry(phoWAvSBCtime14,"35 < nVtx <=45","l");
lg8->AddEntry(phoWAvSBCtime15,"24 < nVtx <=35","l");
lg8->AddEntry(phoWAvSBCtime16,"18 < nVtx <=24","l");
lg8->AddEntry(phoWAvSBCtime17,"12 < nVtx <=18","l");
lg8->AddEntry(phoWAvSBCtime18,"6 < nVtx <=12","l");
lg8->AddEntry(phoWAvSBCtime19,"0 <= nVtx <=6","l");
lg8->SetTextSize(0.018);
lg8->SetHeader("NVertices");
lg8->Draw();
c8->SetLogy();
//Save Plots
c1->SaveAs("EcalIso.png");
c2->SaveAs("HcalIso.png");
c3->SaveAs("TrackIso.png");
c4->SaveAs("NewHcalIso.png");
c5->SaveAs("SMinor.png");
c6->SaveAs("PhotonSeedTimeVsPU.png");
c7->SaveAs("PhotonWAVeSCTimeVsPU.png");
c8->SaveAs("PhotonWAVeSeedBCTimeVsPU.png");


}









