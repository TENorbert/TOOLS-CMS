#ifndef ZRAPIDITY_STANDARD_INCLUDED
#define ZRAPIDITY_STANDARD_INCLUDED

#include "TText.h"
#include "TLatex.h"
#include "TDatime.h"
#include <vector>
#include <iostream>
static bool dotime=true;
static const char* y_xaxis_label="y_{ee}";

static const char* qt_xaxis_label="p_{T,ee} [GeV/c]";
static const char* qt_xaxis_ll_label="p_{T} [GeV/c]";

TH1* zpt_rebinForPlot(TH1* original) {
  char name[125];
  sprintf(name,"%s_4plot",original->GetName());
  int    pt_bins=19;
  double pt_binning[]={ 0.7, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 30, 40, 50, 70, 90, 110, 150, 190, 250, 600, 3500 };
  TH1* rebinned=new TH1F(name,name,pt_bins,pt_binning);

  for(int i=1;i<=original->GetNbinsX();i++){
    rebinned->SetBinContent(i,original->GetBinContent(i));
    rebinned->SetBinError(i,original->GetBinError(i));
  }
  return rebinned;
}
TH1* z_rebinForComp(TH1* original,int vars,int zdef,int doWgt=1) {
  if(vars>11)return original;
  char name[125];
  sprintf(name,"%s_4plot",original->GetName());
  int    pt_bins[20];
 std::vector<double*> pt_binning; 
 TH1* rebinned;
 if(zdef!=9){
  pt_bins[0]=22;//at
  pt_bins[1]=15;//18;//7;//21;//ps
  pt_bins[2]=20;//pt
  pt_bins[3]=26;//y
  pt_bins[4]=28;//eta1
  pt_bins[5]=28;//eta2
  pt_bins[6]=28;//etae
  pt_bins[7]=10;//phi1
 pt_bins[8]=10;//ph2
  pt_bins[9]=10;//phie
  pt_bins[10]=16;//pt1
  pt_bins[11]=16;//pt2
 
 
  
  double bins1[30]={0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 21, 24, 27, 35, 40, 45, 60,100, 150};
  pt_binning.push_back(bins1);
  double bins2[30]={0.0001, 0.008,  0.016,  0.024,  0.034,  0.045,  0.057,   0.072,   0.091,   0.114,   0.145,   0.189,    0.258,   0.391,  0.524,   1};//{0.0001,0.005,0.01,0.016,0.023,0.031,0.04,0.05,0.07,0.1,0.15,0.2,0.28,0.36,0.46,0.66,0.88,1};//{0.001,0.005,0.01,0.02,0.03,0.05,0.07,0.1,0.15,0.2,0.28,0.36,0.46,0.66,0.88,1};//{0,0.02,0.04, 0.1,0.2,0.4,0.6,1};//ps
  // double bins2[30]={0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.22, 0.26, 0.3, 0.35, 0.45, 0.6, 0.8, 1};
   pt_binning.push_back(bins2);
  double bins3[30]={0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 26, 32, 38, 50, 70, 100, 150, 250, 400};
 pt_binning.push_back(bins3);
  double bins4[30]={-5, -3.7, -3.3, -3.2, -3.1, -3.0, -2.9, -2.8, -2.7, -2.6, -2.5, -2.4, -2.3, -2.0, 0.0, 2.0, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.3, 3.6, 5.0};
 pt_binning.push_back(bins4);
  double bins5[30]={-5, -4.6, -4, -3.8, -3.6, -3.4, -3.2, -3.0, -2.6, -2.4, -2.2, -2.0, -1.8, -1.4, 0.0, 1.4, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.6, 5};
 pt_binning.push_back(bins5);
  double bins6[30]={-5, -4.6, -4, -3.8, -3.6, -3.4, -3.2, -3.0, -2.6, -2.4, -2.2, -2.0, -1.8, -1.4, 0.0, 1.4, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.6, 5};
 pt_binning.push_back(bins6);
  double bins7[30]={-5, -4.6, -4, -3.8, -3.6, -3.4, -3.2, -3.0, -2.6, -2.4, -2.2, -2.0, -1.8, -1.4, 0.0, 1.4, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.6, 5};
 pt_binning.push_back(bins7);
  double bins8[30]={-3.14,-2.512,-1.884,-1.256,-0.628,0.0,6.268,1.256,1.884,2.512,3.14};
 pt_binning.push_back(bins8);
  double bins9[30]={-3.14,-2.512,-1.884,-1.256,-0.628,0.0,6.268,1.256,1.884,2.512,3.14};
 pt_binning.push_back(bins9);
  double bins10[30]={-3.14,-2.512,-1.884,-1.256,-0.628,0.0,6.268,1.256,1.884,2.512,3.14};
 pt_binning.push_back(bins10);
  double bins11[30]={0, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 46, 60, 110, 400};
 pt_binning.push_back(bins11);
  double bins12[30]={0, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 46, 60, 110, 400};
  pt_binning.push_back(bins12);
  rebinned=new TH1F(name,name,pt_bins[vars],pt_binning[vars]);
 }else{return original;}
  for(int i=1;i<=original->GetNbinsX();i++){
    int jbin=rebinned->GetXaxis()->FindBin(original->GetBinCenter(i));
    rebinned->SetBinContent(jbin,rebinned->GetBinContent(jbin)+original->GetBinContent(i));
    rebinned->SetBinError(jbin,sqrt(pow(rebinned->GetBinError(jbin),2)+pow(original->GetBinError(i),2)));
   
  }
  if(doWgt==1){ 
    for(int i=1;i<=rebinned->GetNbinsX();i++){
      double width=rebinned->GetXaxis()->GetBinWidth(i);
      double wgt=0.005*rebinned->GetBinContent(i)*1.0/width;
      rebinned->SetBinContent(i,wgt);
      rebinned->SetBinError(i,0.005*rebinned->GetBinError(i)/width);
    }//for i
  }//if dowgt
  return rebinned;
}


TH2F* z_rebinForPhiStar2D(TH2F* original, int zdef) {

  char name[125];
  sprintf(name,"%s_4plot2D",original->GetName());
  int    pt_bins;
  double* pt_binning; 


  if(zdef==0){
    pt_bins=15;
    double pt_binning1[]={0.0001, 0.008,  0.016,  0.024,  0.034,  0.045,  0.057,   0.072,   0.091,   0.114,   0.145,   0.189,    0.258,   0.391,  0.524,   1};
    pt_binning=pt_binning1;
  }else if(zdef==1){
    pt_bins=15;
    double pt_binning1[]={0.0001, 0.008,  0.016,  0.024,  0.034,  0.045,  0.057,   0.072,   0.091,   0.114,   0.145,   0.189,    0.258,   0.391,  0.524,   1};
    pt_binning=pt_binning1;
  }else if (zdef==2){
    pt_bins=15;
    double pt_binning1[]={0.0001, 0.008,  0.016,  0.024,  0.034,  0.045,  0.057,   0.072,   0.091,   0.114,   0.145,   0.189,    0.258,   0.391,  0.524,   1};
    pt_binning=pt_binning1;
  }else{

    return original;
  }



  TH2F* rebinned=new TH2F(name,name,pt_bins,pt_binning,pt_bins,pt_binning);

  for(int i=1;i<=original->GetNbinsX();i++){
    for(int j=1;j<=original->GetNbinsY();j++){
      rebinned->Fill(original->GetXaxis()->GetBinCenter(i),original->GetYaxis()->GetBinCenter(j),original->GetBinContent(i,j));
      // rebinned->SetBinError(i,j,sqrt(rebinned->GetBinContent(i)));
    } 
  }


  // printf("name %s bins %i var %i\n",name,rebinned->GetNbinsX());
  return rebinned;



}


TH2* z_rebinForComp2D(TH2* original,int vars, int zdef) {
  if(vars>11)return original;
  char name[125];
  sprintf(name,"%s_4plot2D",original->GetName());
  int    pt_bins[20];
std::vector<double*> pt_binning; 

 if(zdef!=9){
  pt_bins[0]=22;//at

  pt_bins[1]=15;//17;//7;//21;//ps

  pt_bins[2]=20;//pt
  pt_bins[3]=26;//y
  pt_bins[4]=28;//eta1
  pt_bins[5]=28;//eta2
  pt_bins[6]=28;//etae
  pt_bins[7]=10;//phi1
  pt_bins[8]=10;//ph2
  pt_bins[9]=10;//phie
  pt_bins[10]=16;//pt1
  pt_bins[11]=16;//pt2
  // pt_bins[12]=120;//mass
   double bins1[30]={0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 21, 24, 27, 35, 40, 45, 60,100, 150};
  pt_binning.push_back(bins1);
  //  double bins2[30]={0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.22, 0.26, 0.3, 0.35, 0.45, 0.6, 0.8, 1};


  // double bins2[30]={0,0.02,0.04, 0.1,0.2,0.4,0.6,1};
  double bins2[30]={0.0001, 0.008,  0.016,  0.024,  0.034,  0.045,  0.057,   0.072,   0.091,   0.114,   0.145,   0.189,    0.258,   0.391,  0.524,   1};//{0.0001,0.005,0.01,0.016,0.023,0.031,0.04,0.05,0.07,0.1,0.15,0.2,0.28,0.36,0.46,0.66,0.88,1};
  //double bins2[30]={0.0001,0.005,0.01,0.02,0.03,0.05,0.07,0.1,0.15,0.2,0.28,0.36,0.46,0.66,0.88,1};


   pt_binning.push_back(bins2);
  double bins3[30]={0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 26, 32, 38, 50, 70, 100, 150, 250, 400};
 pt_binning.push_back(bins3);
  double bins4[30]={-5, -3.7, -3.3, -3.2, -3.1, -3.0, -2.9, -2.8, -2.7, -2.6, -2.5, -2.4, -2.3, -2.0, 0.0, 2.0, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.3, 3.6, 5.0};
 pt_binning.push_back(bins4);
  double bins5[30]={-5, -4.6, -4, -3.8, -3.6, -3.4, -3.2, -3.0, -2.6, -2.4, -2.2, -2.0, -1.8, -1.4, 0.0, 1.4, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.6, 5};
 pt_binning.push_back(bins5);
  double bins6[30]={-5, -4.6, -4, -3.8, -3.6, -3.4, -3.2, -3.0, -2.6, -2.4, -2.2, -2.0, -1.8, -1.4, 0.0, 1.4, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.6, 5};
 pt_binning.push_back(bins6);
  double bins7[30]={-5, -4.6, -4, -3.8, -3.6, -3.4, -3.2, -3.0, -2.6, -2.4, -2.2, -2.0, -1.8, -1.4, 0.0, 1.4, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.6, 5};
 pt_binning.push_back(bins7);
  double bins8[30]={-3.14,-2.512,-1.884,-1.256,-0.628,0.0,6.268,1.256,1.884,2.512,3.14};
 pt_binning.push_back(bins8);
  double bins9[30]={-3.14,-2.512,-1.884,-1.256,-0.628,0.0,6.268,1.256,1.884,2.512,3.14};
 pt_binning.push_back(bins9);
  double bins10[30]={-3.14,-2.512,-1.884,-1.256,-0.628,0.0,6.268,1.256,1.884,2.512,3.14};
 pt_binning.push_back(bins10);
  double bins11[30]={0, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 46, 60, 110, 400};
 pt_binning.push_back(bins11);
  double bins12[30]={0, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 46, 60, 110, 400};
  pt_binning.push_back(bins12);
 }else{return original;}

 
  TH2* rebinned=new TH2F(name,name,pt_bins[vars],pt_binning[vars],60,60,120);

  for(int i=1;i<=original->GetNbinsX();i++){
    for(int j=1;j<=original->GetNbinsY();j++){
      rebinned->Fill(original->GetXaxis()->GetBinCenter(i),original->GetYaxis()->GetBinCenter(j),original->GetBinContent(i,j));
      // rebinned->SetBinError(i,j,sqrt(rebinned->GetBinContent(i)));
    } 
  }

 //  for(int j=1;j<=rebinned->GetNbinsY();j++){
//     for(int i=1;i<=rebinned->GetNbinsX();i++){
//       double width=rebinned->GetXaxis()->GetBinWidth(i);
//       double wgt=0.005*rebinned->GetBinContent(i,j)*1.0/width;
//       rebinned->SetBinContent(i,j,wgt);
//     }}


  printf("name %s bins %i var %i\n",name,rebinned->GetNbinsX(),vars);
  return rebinned;
}




void zrap_colors() {
  const Int_t NRGBs = 2;
  const Int_t NCont = 30;
  
  Double_t stops[NRGBs] = { 0.00, 1.0 };
  Double_t red[NRGBs]   = { 1.00, 0.0 };
  Double_t green[NRGBs] = { 1.00, 0.0 };
  Double_t blue[NRGBs]  = { 1.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}


void zrap_Prelim(double px, double py, double tx=-1, double ty=-1) {
  const char* time_;
  TDatime mytime;
  time_ = mytime.AsString();

  TText *plabel = new TText();
  plabel-> SetNDC();
  plabel -> SetTextFont(42);
  plabel -> SetTextColor(1);
  plabel -> SetTextSize(0.040);
  plabel -> SetTextAlign(22);
  plabel -> SetTextAngle(0);

  
  //Then for each plot, pick a nice spot and draw
  plabel -> DrawText(px, py, "CMS Internal"); // 'ala VBTF'
  if (tx>=0 && ty>=0 && dotime) {
    TText *tlabel = new TText();
    tlabel-> SetNDC();
    tlabel -> SetTextFont(42);
    tlabel -> SetTextColor(1);
    tlabel -> SetTextSize(0.02);
    tlabel -> SetTextAlign(22);
    tlabel -> SetTextAngle(0);
    
    tlabel -> DrawText(tx, ty, Form("%s",time_));
  }
}
void zrap_UNPrelim(double px, double py, double tx=-1, double ty=-1) {
  const char* time_;
  TDatime mytime;
  time_ = mytime.AsString();

  TText *plabel = new TText();
  plabel-> SetNDC();
  plabel -> SetTextFont(42);
  plabel -> SetTextColor(1);
  plabel -> SetTextSize(0.040);
  plabel -> SetTextAlign(22);
  plabel -> SetTextAngle(0);

  
  //Then for each plot, pick a nice spot and draw
  plabel -> DrawText(px, py, "CMS Unofficial"); // 'ala VBTF'
  if (tx>=0 && ty>=0 && dotime) {
    TText *tlabel = new TText();
    tlabel-> SetNDC();
    tlabel -> SetTextFont(42);
    tlabel -> SetTextColor(1);
    tlabel -> SetTextSize(0.02);
    tlabel -> SetTextAlign(22);
    tlabel -> SetTextAngle(0);
    
    tlabel -> DrawText(tx, ty, Form("%s",time_));
  }
}



void cleanTitle(double px, double py, int val) {
  char text[40];
  sprintf(text,"#int Ldt=%d pb^{-1} at #sqrt{s}=7 TeV",val);

  TLatex *plabel = new TLatex(px,py,text);
  plabel-> SetNDC();
  plabel -> SetTextFont(42);
  plabel -> SetTextColor(1);
  plabel -> SetTextSize(0.035);
  plabel -> SetTextAlign(22);
  plabel -> SetTextAngle(0);


  //Then for each plot, pick a nice spot and draw
  plabel -> Draw();

}


void sim_label(double px, double py) {
  char text[40];
  sprintf(text,"Simulation");

  TLatex *plabel = new TLatex(px,py,text);
  plabel-> SetNDC();
  plabel -> SetTextFont(42);
  plabel -> SetTextColor(1);
  plabel -> SetTextSize(0.035);
  plabel -> SetTextAlign(22);
  plabel -> SetTextAngle(0);


  //Then for each plot, pick a nice spot and draw
  plabel -> Draw();

}

void zrap_Lumi(double px, double py, int val) {
  char text[40];
  sprintf(text,"#int Ldt=%d pb^{-1} at #sqrt{s}=7 TeV",val);

  TLatex *plabel = new TLatex(px,py,text);
  plabel-> SetNDC();
  plabel -> SetTextFont(42);
  plabel -> SetTextColor(1);
  plabel -> SetTextSize(0.035);
  plabel -> SetTextAlign(22);
  plabel -> SetTextAngle(0);


  //Then for each plot, pick a nice spot and draw
  plabel -> Draw();

}

void zqt_Cut(double px, double py, const char* lept="l",double sizef=1.0) {
  char text[40];
  sprintf(text,"|#eta_{%s}|<2.1, p_{T,%s}>20 GeV/c",lept,lept);

  TLatex *plabel = new TLatex(px,py,text);
  plabel-> SetNDC();
  plabel -> SetTextFont(42);
  plabel -> SetTextColor(kBlue);
  plabel -> SetTextSize(0.045*sizef);
  plabel -> SetTextAlign(22);
  plabel -> SetTextAngle(0);


  //Then for each plot, pick a nice spot and draw
  plabel -> Draw();

}
#endif // ZRAPIDITY_STANDARD_INCLUDED
