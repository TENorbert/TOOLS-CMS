
// Include Ecal Stuff Here!
#include "DQM/EcalCommon/interface/Numbers.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

// C++    stuffs
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <vector>

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TChain.h"
#include "TFile.h"
#include "TProfile2D.h"

#define NWL 3 // if you change this, mind to change the Branch declaration of TTree...
#define ETA_NBIN 100
#define ETA_MIN   -3.
#define ETA_MAX   +3.


#define RunB4TS 160147    // 176652  // 160138    // 160133       // 160111 // 159784 //175832  // 175354 //159784 //1611175
#define RunAfTS 178241


const int numEBFed  = 36;
const int maxNumCCUInFed = 68;
const int mxNumXtalInCCU = 25;



// my new histograms
TProfile2D*   CCUAvgTimeEB[numEBFed];

using namespace std;

TFile *savefile; 

struct ntu_xtals {
    int   run;
    int   seq;
    int   fed;
    int   ix;
    int   iy;
    int   iz;
    int   detId;
    int   elecId;
    int   harness;
    int   side;
    float eta;
    float phi;
    float field;
    float alpha;
    int   wl[NWL];
    int   time[NWL];
    int   nevt[NWL];
    int   laser_power[NWL];
    int   vinj[NWL];
    int   nxtal25[NWL];
    int   npn[NWL];
    int   vfe_mode[NWL];
    int   mem_gain[NWL];
    int   numfill[NWL];
    float lumi[NWL];
    float qmax[NWL];
    float qmaxS[NWL];
    float tmax[NWL];
    float apdpnA[NWL];
    float apdpnAS[NWL];
    float apdpnB[NWL];
    float apdpnBS[NWL];
    float apdpnAB[NWL];
    float apdpnABS[NWL];
    float ped[NWL];
    float pedS[NWL];
    float ped_tp[NWL];
    float ped_tpS[NWL];
    float ped_laser[NWL];
    float ped_laserS[NWL];
    float tp[NWL];
    float tpS[NWL];
    float corrwidth[NWL];
    float apdapdA[NWL];
    float apdapdAS[NWL];
    float apdapdB[NWL];
    float apdapdBS[NWL];
    float l_ampli[NWL];
    float l_rise_time[NWL];
    float l_fwhm[NWL];
    float l_width90[NWL];
    float l_width95[NWL];
    float l_prepulse[NWL];
    float l_nphot0[NWL];
    float l_nphot1[NWL];
    float l_nphot2[NWL];
    float l_nphot3[NWL];
    float l_nphot4[NWL];
    float pnA_qmax[NWL];
    float pnA_qmaxS[NWL];
    float pnA_pnpnB[NWL];
    float pnA_pnpnBS[NWL];
    float pnA_corrwidth[NWL];
    float pnA_ped[NWL];
    float pnA_pedS[NWL];
    float pnA_tp_ped[NWL];
    float pnA_tp_pedS[NWL];
    float pnA_l_ped[NWL];
    float pnA_l_pedS[NWL];
    float pnA_tp[NWL];
    float pnA_tpS[NWL];
    float pnB_qmax[NWL];
    float pnB_qmaxS[NWL];
    float pnB_pnpnB[NWL];
    float pnB_pnpnBS[NWL];
    float pnB_corrwidth[NWL];
    float pnB_ped[NWL];
    float pnB_pedS[NWL];
    float pnB_tp_ped[NWL];
    float pnB_tp_pedS[NWL];
    float pnB_l_ped[NWL];
    float pnB_l_pedS[NWL];
    float pnB_tp[NWL];
    float pnB_tpS[NWL];
};

// char * hprofnameEBminus = new  char[18];
// char * hprofnameEBplus = new char[18];
// std::string bufferTitleEBplus;
// std::string bufferTitleEBminus;


// TProfile2D* CCUAvgTimingEBplus[18];   //= {610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627};
// TProfile2D* CCUAvgTimingEBminus[18];   //= {628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645};

TProfile2D* FedAvgTimingEB;
TProfile2D* FedAvgTimingEEP;
TProfile2D* FedAvgTimingEEM;


// TH1F* CCUTimeDiffPlot = new TH1F("CCUTimeDiffPlot","Time Shift Of CCU in EB FED",100, -2.5 , 2.5);
// TH1F* CCUTimeB4TS = new TH1F("CCUTimeB4TS","CCU Time  in EB FED Before TS", 100, -2.5, 2.5);
// TH1F* CCUTimeAFTS = new  TH1F("CCUTimeAFTS","CCU Time in EB FED After TS", 100, -2.5, 2.5);


// Xatl time Vs Amplitude EB and EE
TH2F*   XtaltimeVsAmpEB = new TH2F("XtaltimeVsAmpEB", "Crystal In EB Time Vs Amplitude",  100, 0, 4000,100, -25.0, 25.0 );
TH2F*   XtaltimeVsAmpEE = new TH2F("XtaltimeVsAmpEE", "Crystal In EE Time Vs Amplitude",  100, 0, 4000,100, -25.0, 25.0 );
//  Time of 1d
TH1F*  xtaltimeDistEB = new TH1F("xtaltimeEB","Crystal Time Distrubution",100, -25, 25);
TH1F*  xtaltimeDistEE = new TH1F("xtaltimeEE","Crystal Time Distrubution",100, -25, 25);


// Xtal time Vs Time Measured EB and EE
TH2F*   XtaltimeVsTimeEB = new TH2F("XtaltimeVsTimeEB", "Crystal  In EB Time  Vs Time Measured", 100, 0, 10000000, 100, -25, 25 ); 
TH2F*   XtaltimeVsTimeEE = new TH2F("XtaltimeVsTimeEE", "Crystal  In EE Time  Vs Time Measured", 100, 0, 10000000,100, -25, 25 );
// Xtal time Vs FedId
TH2F*   XtaltimeVsFedIDEB = new TH2F("XtaltimeVsFedIDEB", "Crystal Time[ns] Vs FedId In EB only", 54, 601, 655, 100, -25, 25 );
TH2F*   XtaltimeVsFedIDEE = new TH2F("XtaltimeVsFedIDEE","Crystal Time[ns] Vs FedId In EE Only",  54,  601,  655, 100, -25, 25); 
//Xtal Time Vs Lumi
TH2F*   XtaltimeVsLumiEB = new TH2F("XtaltimeVsLumiEB", "Crystal  In EB Time  Vs Lumi", 100, 0, 4000,100, -25, 25 );
// xtak Time Vs Eta EB   
TH2F*   XtaltimeVsEta = new TH2F("XtaltimeVsEta", "Crystal  In EB Time  Vs Eta", 10, -3, 3,100, -25, 25 );
//xtal Time Vs Run Number
TH2F*   XtaltimeVsRun = new TH2F("XtaltimeVsRun", "Crystal  In EB Time  Vs Run Number", 100, 150000, 190000, 100, -25, 25);


TH1F* EBP1Dtime =  new TH1F("EBP1Dtime","EB-1 Timing [ns]", 100, -25, 25);


TH1F* EBM1Dtime =  new TH1F("EBM1Dtime","EB+1 Timing [ns]", 100, -25, 25);


//******** Function to Write Hists*************//
void writehist()
{
  savefile -> cd();

  TDirectory * TestPlots = savefile->mkdir("LaserTiming");

  TestPlots->cd();


  XtaltimeVsAmpEB->Write();
  XtaltimeVsAmpEE->Write();
  XtaltimeVsTimeEB->Write();
  XtaltimeVsTimeEE->Write();
  XtaltimeVsFedIDEB->Write();
  XtaltimeVsFedIDEE->Write();

  XtaltimeVsLumiEB->Write();
  XtaltimeVsEta->Write();
  XtaltimeVsRun->Write();

  // 1 D timing dist
  xtaltimeDistEB->Write();
  xtaltimeDistEE->Write();


  // EB 1D tests
  EBP1Dtime->Write();
  EBM1Dtime->Write();
  // Timing Check Here
  //CCUTimeDiffPlot->Write();

 // CCUTimeB4TS->Write();

//  CCUTimeAFTS->Write();

// EB All SM

 FedAvgTimingEB->GetXaxis()->SetTitle("i#phi");
 FedAvgTimingEB->GetYaxis()->SetTitle("i#eta");
FedAvgTimingEB->Draw("colztext");
FedAvgTimingEB->Write();

//EE Plus
 FedAvgTimingEEP->GetXaxis()->SetTitle("ix");
  FedAvgTimingEEP->GetYaxis()->SetTitle("iy");
 FedAvgTimingEEP->Draw("colztext");
 FedAvgTimingEEP->Write();
 //EE Minus
  FedAvgTimingEEM->GetXaxis()->SetTitle("ix");
  FedAvgTimingEEM->GetYaxis()->SetTitle("iy");
 FedAvgTimingEEM->Draw("colztext");
 FedAvgTimingEEM->Write();


 
 //****** New Folder to Fill EB CCUs For RunA ******/// 
TDirectory * CCUInEB = savefile->mkdir("CCUInEBFedsTiming");

 CCUInEB->cd();
 
for( int numfed = 0; numfed < numEBFed; numfed++)
{
// CCUAvgTimingEBplus[j]->Write();
 CCUAvgTimeEB[numfed]->GetXaxis()->SetTitle("i#phi");
 CCUAvgTimeEB[numfed]->GetYaxis()->SetTitle("i#eta");

 CCUAvgTimeEB[numfed]->Draw("colztext");
 CCUAvgTimeEB[numfed]->Write(); 
 }

//******New Folder For CCU Hists EB RunB ******//// 
// TDirectory * CCUInEB_RunB = savefile->mkdir("CCUInEBFedsTimeRunB");

//  CCUInEB_RunB->cd();
 
//  for(int SM = 0;; SM < numEBFed; SM++)
//    {
// 	 CCUAvgTimeSM[SM]->GetXaxis()->SetTitle("i#phi");
// 	 CCUAvgTimeSM[SM]->GetYaxis()->SetTitle("i#eta");
// 	 CCUAvgTimeSM[SM]->Write(); 
//    }

 

 
} //  end of fxn Writehist





struct ntu_xtals x;

void init_ttree(TTree * t, struct ntu_xtals * x)
{
    t->SetBranchAddress("run", &x->run);
    t->SetBranchAddress("seq", &x->seq);
    t->SetBranchAddress("fed", &x->fed);
    t->SetBranchAddress("ix", &x->ix);
    t->SetBranchAddress("iy", &x->iy);
    t->SetBranchAddress("iz", &x->iz);
    t->SetBranchAddress("detId", &x->detId);
    t->SetBranchAddress("elecId", &x->elecId);
    t->SetBranchAddress("harness", &x->harness);
    t->SetBranchAddress("side", &x->side);
    t->SetBranchAddress("eta", &x->eta);
    t->SetBranchAddress("phi", &x->phi);
    t->SetBranchAddress("field", &x->field);
    t->SetBranchAddress("alpha", &x->alpha);
    t->SetBranchAddress("wl", &x->wl);
    t->SetBranchAddress("time", &x->time);
    t->SetBranchAddress("nevt", &x->nevt);
    t->SetBranchAddress("laser_power", &x->laser_power);
    t->SetBranchAddress("vinj", &x->vinj);
    t->SetBranchAddress("nxtal25", &x->nxtal25);
    t->SetBranchAddress("npn", &x->npn);
    t->SetBranchAddress("vfe_mode", &x->vfe_mode);
    t->SetBranchAddress("mem_gain", &x->mem_gain);
    t->SetBranchAddress("numfill", &x->numfill);
    t->SetBranchAddress("lumi", &x->lumi);
    t->SetBranchAddress("qmax", &x->qmax);
    t->SetBranchAddress("qmaxS", &x->qmaxS);
    t->SetBranchAddress("tmax", &x->tmax);
    t->SetBranchAddress("apdpnA", &x->apdpnA);
    t->SetBranchAddress("apdpnAS", &x->apdpnAS);
    t->SetBranchAddress("apdpnB", &x->apdpnB);
    t->SetBranchAddress("apdpnBS", &x->apdpnBS);
    t->SetBranchAddress("apdpnAB", &x->apdpnAB);
    t->SetBranchAddress("apdpnABS", &x->apdpnABS);
    t->SetBranchAddress("ped", &x->ped);
    t->SetBranchAddress("pedS", &x->pedS);
    t->SetBranchAddress("ped_tp", &x->ped_tp);
    t->SetBranchAddress("ped_tpS", &x->ped_tpS);
    t->SetBranchAddress("ped_laser", &x->ped_laser);
    t->SetBranchAddress("ped_laserS", &x->ped_laserS);
    t->SetBranchAddress("tp", &x->tp);
    t->SetBranchAddress("tpS", &x->tpS);
    t->SetBranchAddress("corrwidth", &x->corrwidth);
    t->SetBranchAddress("apdapdA", &x->apdapdA);
    t->SetBranchAddress("apdapdAS", &x->apdapdAS);
    t->SetBranchAddress("apdapdB", &x->apdapdB);
    t->SetBranchAddress("apdapdBS", &x->apdapdBS);
    t->SetBranchAddress("l_ampli", &x->l_ampli);
    t->SetBranchAddress("l_rise_time", &x->l_rise_time);
    t->SetBranchAddress("l_fwhm", &x->l_fwhm);
    t->SetBranchAddress("l_width90", &x->l_width90);
    t->SetBranchAddress("l_width95", &x->l_width95);
    t->SetBranchAddress("l_prepulse", &x->l_prepulse);
    t->SetBranchAddress("l_nphot0", &x->l_nphot0);
    t->SetBranchAddress("l_nphot1", &x->l_nphot1);
    t->SetBranchAddress("l_nphot2", &x->l_nphot2);
    t->SetBranchAddress("l_nphot3", &x->l_nphot3);
    t->SetBranchAddress("l_nphot4", &x->l_nphot4);
    t->SetBranchAddress("pnA_qmax", &x->pnA_qmax);
    t->SetBranchAddress("pnA_qmaxS", &x->pnA_qmaxS);
    t->SetBranchAddress("pnA_pnpnB", &x->pnA_pnpnB);
    t->SetBranchAddress("pnA_pnpnBS", &x->pnA_pnpnBS);
    t->SetBranchAddress("pnA_corrwidth", &x->pnA_corrwidth);
    t->SetBranchAddress("pnA_ped", &x->pnA_ped);
    t->SetBranchAddress("pnA_pedS", &x->pnA_pedS);
    t->SetBranchAddress("pnA_tp_ped", &x->pnA_tp_ped);
    t->SetBranchAddress("pnA_tp_pedS", &x->pnA_tp_pedS);
    t->SetBranchAddress("pnA_l_ped", &x->pnA_l_ped);
    t->SetBranchAddress("pnA_l_pedS", &x->pnA_l_pedS);
    t->SetBranchAddress("pnA_tp", &x->pnA_tp);
    t->SetBranchAddress("pnA_tpS", &x->pnA_tpS);
    t->SetBranchAddress("pnB_qmax", &x->pnB_qmax);
    t->SetBranchAddress("pnB_qmaxS", &x->pnB_qmaxS);
    t->SetBranchAddress("pnB_pnpnB", &x->pnB_pnpnB);
    t->SetBranchAddress("pnB_pnpnBS", &x->pnB_pnpnBS);
    t->SetBranchAddress("pnB_corrwidth", &x->pnB_corrwidth);
    t->SetBranchAddress("pnB_ped", &x->pnB_ped);
    t->SetBranchAddress("pnB_pedS", &x->pnB_pedS);
    t->SetBranchAddress("pnB_tp_ped", &x->pnB_tp_ped);
    t->SetBranchAddress("pnB_tp_pedS", &x->pnB_tp_pedS);
    t->SetBranchAddress("pnB_l_ped", &x->pnB_l_ped);
    t->SetBranchAddress("pnB_l_pedS", &x->pnB_l_pedS);
    t->SetBranchAddress("pnB_tp", &x->pnB_tp);
    t->SetBranchAddress("pnB_tpS", &x->pnB_tpS);
}

int ebwl[] = { 440, 800 };
int eewl[] = { 440, 455, 617 };



// Helper functions
int isEB(int ifed)
{
    return (ifed >= 610 && ifed <= 645);
}


std::string ConvertIntToString(int num)
{
  //  using name space std;
  stringstream mystream;
  mystream  << num;

  return mystream.str();
  
}


/////// ***** Fxn Main Begins *********///////
int main(int argc, char ** argv)
{
    int * wl = NULL, nwl = 0;
    TChain * tx = new TChain("x");
      tx->Add("/hdfs/cms/user/norbert/data/Laser/*.root");
	//  tx->Add("/hdfs/cms/user/norbert/data/NewLaserData/ntuple_example/*.root");
	//	tx->Add("/hdfs/cms/user/norbert/data/NewLaserData/cmsecallaser/srv-ecal-laser-13/disk0/ferriff/tree_data/ntuple_example/*.root");



// Declare CCU in EB+ and EB - Hists here!
// CCUAvgTimingEBM1 = new TProfile2D("CCUAvgTimingEBM1","CCU Mean Time EB-1",4,0.-10.0,10.0,17,-85,0);
//  CCUAvgTimingEBM2 = new TProfile2D("CCUAvgTimingEBM2","CCU Mean Time EB-2",4,10.0,30.0,17,-85,0);
//  CCUAvgTimingEBM3 = new TProfile2D("CCUAvgTimingEBM3","CCU Mean Time EB-3",4,30.0,50.0,17,-85,0);
//  CCUAvgTimingEBM4 = new TProfile2D("CCUAvgTimingEBM4","CCU Mean Time EB-4",4,50.0,70.0,17,-85,0);
//  CCUAvgTimingEBM5 = new TProfile2D("CCUAvgTimingEBM5","CCU Mean Time EB-5",4,70.0,90.0,17,-85,0);
//  CCUAvgTimingEBM6 = new TProfile2D("CCUAvgTimingEBM6","CCU Mean Time EB-6",4,90.0,110.0,17,-85,0);
//  CCUAvgTimingEBM7 = new TProfile2D("CCUAvgTimingEBM7","CCU Mean Time EB-7",4,110.0,130.0,17,-85,0);
//  CCUAvgTimingEBM8 = new TProfile2D("CCUAvgTimingEBM8","CCU Mean Time EB-8",4,130.0,150.0,17,-85,0);
//  CCUAvgTimingEBM9 = new TProfile2D("CCUAvgTimingEBM9","CCU Mean Time EB-9",4,150.0,170.0,17,-85,0);
//  CCUAvgTimingEBM10 = new TProfile2D("CCUAvgTimingEBM10","CCU Mean Time EB-10",4,170.0,190.0,17,-85,0);
//  CCUAvgTimingEBM11 = new TProfile2D("CCUAvgTimingEBM11","CCU Mean Time EB-11",4,190.0,210.0,17,-85,0);
//  CCUAvgTimingEBM12 = new TProfile2D("CCUAvgTimingEBM12","CCU Mean Time EB-12",4,210.0,230.0,17,-85,0);
//  CCUAvgTimingEBM13 = new TProfile2D("CCUAvgTimingEBM13","CCU Mean Time EB-13",4,230.0,250.0,17,-85,0);
//  CCUAvgTimingEBM14 = new TProfile2D("CCUAvgTimingEBM14","CCU Mean Time EB-14",4,250.0,270.0,17,-85,0);
//  CCUAvgTimingEBM15 = new TProfile2D("CCUAvgTimingEBM15","CCU Mean Time EB-15",4,270.0,290.0,17,-85,0);
//  CCUAvgTimingEBM16 = new TProfile2D("CCUAvgTimingEBM16","CCU Mean Time EB-16",4,290.0,310.0,17,-85,0);
//  CCUAvgTimingEBM17 = new TProfile2D("CCUAvgTimingEBM17","CCU Mean Time EB-17",4,310.0,330.0,17,-85,0);
//  CCUAvgTimingEBM18 = new TProfile2D("CCUAvgTimingEBM18","CCU Mean Time EB-18",4,330.0,350.0,17,-85,0);

// //EB Plus
//  CCUAvgTimingEBP1 = new TProfile2D("CCUAvgTimingEBP1","CCU Mean Time EB+1",4,-10.0,10.0,17,0,85);
//  CCUAvgTimingEBP2 = new TProfile2D("CCUAvgTimingEBP2","CCU Mean Time EB+2",4, 10.0,30.0,17,0,85);
//  CCUAvgTimingEBP3 = new TProfile2D("CCUAvgTimingEBP3","CCU Mean Time EB+3",4,30.0,50.0,17,0,85);
//  CCUAvgTimingEBP4 = new TProfile2D("CCUAvgTimingEBP4","CCU Mean Time EB+4",4,50.0,70.0,17,0,85);
//  CCUAvgTimingEBP5 = new TProfile2D("CCUAvgTimingEBP5","CCU Mean Time EB+5",4,70.0,90.0,17,0,85);
//  CCUAvgTimingEBP6 = new TProfile2D("CCUAvgTimingEBP6","CCU Mean Time EB+6",4,90.0,110.0,17,0,85);
//  CCUAvgTimingEBP7 = new TProfile2D("CCUAvgTimingEBP7","CCU Mean Time EB+7",4,110.0,130.0,17,0,85);
//  CCUAvgTimingEBP8 = new TProfile2D("CCUAvgTimingEBP8","CCU Mean Time EB+8",4,130.0,150.0,17,0,85);
//  CCUAvgTimingEBP9 = new TProfile2D("CCUAvgTimingEBP9","CCU Mean Time EB+9",4,150.0,170.0,17,0,85);
//  CCUAvgTimingEBP10 = new TProfile2D("CCUAvgTimingEBP10","CCU Mean Time EB+10",4,170.0,190.0,17,0,85);
//  CCUAvgTimingEBP11 = new TProfile2D("CCUAvgTimingEBP11","CCU Mean Time EB+11",4, 190.0,210.0,17,0,85);
//  CCUAvgTimingEBP12 = new TProfile2D("CCUAvgTimingEBP12","CCU Mean Time EB+12",4, 210.0,230.0,17,0,85);
//  CCUAvgTimingEBP13 = new TProfile2D("CCUAvgTimingEBP13","CCU Mean Time EB+13",4, 230.0,250.0,17,0,85);
//  CCUAvgTimingEBP14 = new TProfile2D("CCUAvgTimingEBP14","CCU Mean Time EB+14",4, 250.0,270.0,17,0,85);
//  CCUAvgTimingEBP15 = new TProfile2D("CCUAvgTimingEBP15","CCU Mean Time EB+15",4, 270.0,290.0,17,0,85);
//  CCUAvgTimingEBP16 = new TProfile2D("CCUAvgTimingEBP16","CCU Mean Time EB+16",4, 290.0,310.0,17,0,85);
//  CCUAvgTimingEBP17 = new TProfile2D("CCUAvgTimingEBP17","CCU Mean Time EB+17",4, 310.0,330.0,17,0,85);
//  CCUAvgTimingEBP18 = new TProfile2D("CCUAvgTimingEBP18","CCU Mean Time EB+18",4, 330.0,350.0,17,0,85);



//We Define  EB Feds
for( int hnum = 0; hnum < numEBFed; hnum++)
  {
	std::string fedhistname = "CCU_Mean_Time_EB";

	if(hnum < 18)
	  {
		//fedhistname +="EB";
		fedhistname += ConvertIntToString(( hnum + 610));
		fedhistname +=ConvertIntToString(-hnum);
        CCUAvgTimeEB[hnum] = new TProfile2D(fedhistname.c_str(),fedhistname.c_str(),4,1+hnum*20,1+(hnum+1)*20,17,-85,0);
		
	  }else if (hnum > 17 )
		  {
			//    fedhistname +="EB";
			fedhistname += ConvertIntToString(( hnum + 610));
			fedhistname +=ConvertIntToString(hnum-18);

	       CCUAvgTimeEB[hnum] = new TProfile2D(fedhistname.c_str(),fedhistname.c_str(),4,1+(hnum-18)*20,1+((hnum-18)+1)*20,17,1,86);
		  }
  }
 
//  sprintf(hprofnameEB,"EB %d",hnum);
//  bufferTitleEB.erase();
//  bufferTitleEB = std::string(hprofnameEB)+ std::string("CCU Mean Time[ns]");
//  CCUAvgTimeEB[hnum] = new TProfile2D(hprofnameEB,bufferTitleEB.c_str(),4, 1,21,17,1,86);
 


 // CCUAvgTimingEB[i] = new TProfile2D("CCUAvgTimingEB[i]","CCU Mean Time Distrubution EBP[i]",17, 0,85,4,0,20);

// sprintf(hprofnameEBminus,"EB- %d",hnum);
// bufferTitleEBminus.erase();
// bufferTitleEBminus = std::string(hprofnameEBminus)+std::string(",CCU Mean Time [ns]");
//  CCUAvgTimingEBminus[hnum] = new TProfile2D(hprofnameEBminus,bufferTitleEBminus.c_str(),17, -85,1,4,0,20);
// //CCUAvgTimingEBminus[i] = new TProfile2D("CCUAvgTimingEBminus[i]","CCU Mean Time Distrubution EBM[i]",17, -85,1,4,0,20);
 //   }



//EB All SM
 FedAvgTimingEB = new TProfile2D("FedAvgTimingEB","CCU Mean Time EB", 360, 0., 360., 171, -85,85);

//EE plus
 FedAvgTimingEEP = new TProfile2D("FedAvgTimingEEP","CCU Mean Time EE+", 100, -50., 50., 100, 0, 100);
 //EE minus
 FedAvgTimingEEM = new TProfile2D("FedAvgTimingEEM","CCU Mean Time EE-", 100, -50., 50., 100, -100, 0);



 // Ssave Out Put Files  here!
    savefile = new TFile("EcalLaserTiming_Run160147.root", "recreate");

    init_ttree(tx, &x);
    tx->SetBranchStatus("*",0); //disable all branches
    // only read needed branches
    tx->SetBranchStatus("run",1);
    tx->SetBranchStatus("fed",1);
    tx->SetBranchStatus("seq",1);
    tx->SetBranchStatus("detId",1);
    tx->SetBranchStatus("apdpnAB",1);
    tx->SetBranchStatus("time",1);
    tx->SetBranchStatus("eta",1);
    tx->SetBranchStatus("detId",1);
    tx->SetBranchStatus("elecId",1);
    tx->SetBranchStatus("ix",1);
    tx->SetBranchStatus("iy",1);
    tx->SetBranchStatus("phi",1);
    tx->SetBranchStatus("lumi",1);
    tx->SetBranchStatus("field", 1);
    tx->SetBranchStatus("wl",1);
    tx->SetBranchStatus("qmax",1);
    tx->SetBranchStatus("tmax",1);


	

    int nbytes = -1;
    int ientry = 0;
    int numcrys = 0;

	

// Loop over Crystals Begin Here!  
while ((nbytes = tx->GetEntry(ientry++)) != 0)
 {

///////****** select run before TS RUN A******///////

if(x.run == RunB4TS && x.seq == 0)
  {
	
        

// for Barrel FEDs or SumperModules
 if (isEB(x.fed)) 
{
    
            wl = ebwl;
            nwl = 2;

			
		int ieta = x.ix;
		int iphi = x.iy+10;
        float crytime = x.tmax[0];
		float Xtaltime =(crytime - 5)*25;
		int  EBXtalFed = x.fed;
		int numfed = EBXtalFed - 610;


		if(x.fed ==610)
		  {
			EBM1Dtime->Fill(Xtaltime);
			cout <<  " the Xtal eta is:" << ieta << " Xtal Phi:" << iphi << " Xtal Time is:" << Xtaltime << endl;
		  }

		if (x.fed == 628)
		  {

			cout << "Crystal time is :" << Xtaltime << endl;
			EBP1Dtime->Fill(Xtaltime);
			
		  }
		
    CCUAvgTimeEB[numfed]->Fill(iphi, ieta, Xtaltime); // Fill all EB-FEDs
	
// 		// Fill Only EB- Feds
//    }else if(x.fed > 627)
// 	 {
// // 	    EBDetId detidp(x.detId);
// // 		int ietap = detidp.ietaSM();
// // 		int iphip = detidp.iphiSM()+10;

// 		int ietap = x.ix;
// 		int iphip = x.iy+10;
//         float crytimep = x.tmax[0];
// 		float Xtaltimep = (crytimep-5);//*25;
// 		int  EBXtalFedp = x.fed;

//         int numfedp = EBXtalFedp - 610;
		
		
// 			cout <<" Here is the Issue" <<  numfedp <<   endl;
// 	        CCUAvgTimeEB[numfedp]->Fill(iphip, ietap, Xtaltimep); // Fill all EB+FEDs


// 	 }
//    //
//}
   
        XtaltimeVsAmpEB->Fill(x.qmax[0],x.tmax[0]);
	    XtaltimeVsTimeEB->Fill( x.time[0],x.tmax[0]);
	    XtaltimeVsLumiEB->Fill(x.lumi[0],x.tmax[0]);
	    XtaltimeVsFedIDEB->Fill(x.fed,(x.tmax[0]-5)*25);

	     xtaltimeDistEB->Fill( x.tmax[0]-5);
         FedAvgTimingEB->Fill(x.iy,x.ix,(x.tmax[0]-5)*25);

		 //   Do EE  here!
 }else{
            wl = eewl;
            nwl = 3;


          // Loop over number crystals in EE
			xtaltimeDistEE->Fill( (x.tmax[0]-5)*25);
			XtaltimeVsAmpEE->Fill(x.qmax[0],(x.tmax[0]-5)*25);
	        XtaltimeVsTimeEE->Fill( x.time[0],(x.tmax[0]-5)*25);

			XtaltimeVsFedIDEE->Fill(x.fed,(x.tmax[0]-5)*25);

	    if(x.fed >=601 && x.fed <= 609)
           {
			 FedAvgTimingEEM->Fill(x.ix,x.iy,(x.tmax[0]-5)*25);
     	   }else

	      {
     	      cout << "Plus EE crystals" << endl;
              FedAvgTimingEEP->Fill(x.ix,x.iy,(x.tmax[0]-5)*25);
	      }
	    
   }


          //Fill generally for all sturff.
          XtaltimeVsEta->Fill(x.ix, (x.tmax[0]-5)*25);
          XtaltimeVsRun->Fill( x.run,(x.tmax[0]-5)*25);


//         for (int i = 0; i < nwl; ++i) 
//         {
//           printf("reading WL %d for channel %d in FED %d: APD/PN_AB = %f at time %d\n", x.wl[i], x.detId, x.fed,  x.apdpnAB[i], x.time[i]);
//         }



numcrys +=numcrys;

if( numcrys > 75838) break; 


  }   // end of choice of run and Seq

}// end of Loop over entries( xtals)

    cout << " Write Histograms" << endl;

    writehist();

    savefile->Close();

    return 0;
}



