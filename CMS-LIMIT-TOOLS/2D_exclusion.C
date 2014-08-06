#include "TColor.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGaxis.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPolyLine.h"
#include "TROOT.h"
//#include "tdrstyle.C"

void rootlogon();

void rootlogon() {
   gROOT->SetStyle("Plain");
   gStyle->SetPalette(1);
   gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
   gStyle->SetOptFit(111110);
   gStyle->SetOptFile(1);
   gStyle->SetMarkerStyle(20);
   gStyle->SetMarkerSize(2.);
   gStyle->SetMarkerColor(1);
   gStyle->SetTitleOffset(1.20,"Y");

    //define high def color palette
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    cout << "loading TDR style and setting as default" << endl;
     gROOT->ProcessLine(".L tdrstyle.C");
     // setTDRStyle();

}


std::string get_sqrtText() {

   char label_sqrt_text[150];
  
    sprintf( label_sqrt_text, "#sqrt{s} = 7 TeV");
    std::string returnString(label_sqrt_text);

  return returnString;

}


TPaveText* get_labelCMS( int legendQuadrant = 0 , std::string year="2011", bool sim=false, std::string run = "RUN2011A") {

  if( legendQuadrant!=0 && legendQuadrant!=1 && legendQuadrant!=2 && legendQuadrant!=3 ) {
    std::cout << "WARNING! Legend quadrant '" << legendQuadrant << "' not yet implemented for CMS label. Using 2." << std::endl;
    legendQuadrant = 2;
  }

  float x1, y1, x2, y2;
  if( legendQuadrant==1 ) {
    x1 = 0.63;
    y1 = 0.83;
    x2 = 0.8;
    y2 = 0.87;
  } else if( legendQuadrant==2 ) {
    x1 =  0.25;
    y1 = 0.83;
    x2 =  0.42;
    y2 = 0.87;
  } else if( legendQuadrant==3 ) {
    x1 = 0.25;
    y1 = 0.2;
    x2 = 0.42;
    y2 = 0.24;
  } else if( legendQuadrant==0 ) {
    x1 = 0.175;
    y1 = 0.953;
    x2 = 0.6;
    y2 = 0.975;
  }

  
  TPaveText* cmslabel = new TPaveText( x1, y1, x2, y2, "brNDC" );
  cmslabel->SetFillColor(kWhite);
  cmslabel->SetTextSize(0.038);
  if( legendQuadrant==0 ) cmslabel->SetTextAlign(11);
  cmslabel->SetTextSize(0.038);
  cmslabel->SetTextFont(42);
 
    std::string leftText;
   
    if(year == "2010")  leftText = "CMS Preliminary 2010, 34 pb^{-1}";
    if (sim)  leftText = "CMS Simulation 2011";
    else {
      if(year == "2011" && run == "RUN2011A")  leftText = "CMS Preliminary RUN2011A 2.034 fb^{-1}";
      if(year == "2011" && run == "RUN2011B")  leftText = "CMS Preliminary 2011, 2.516 fb^{-1}";
      if(year == "2011" && run == "ALL")  leftText = "CMS Preliminary 2011, 4.86 fb^{-1}";
      if(year == "May2011")leftText = "CMS Preliminary 2011, 858.4 pb^{-1}";

    }
    cmslabel->AddText(leftText.c_str());
    return cmslabel;

}




TPaveText* get_labelSqrt( int legendQuadrant ) {

  if( legendQuadrant!=0 && legendQuadrant!=1 && legendQuadrant!=2 && legendQuadrant!=3 ) {
    std::cout << "WARNING! Legend quadrant '" << legendQuadrant << "' not yet implemented for Sqrt label. Using 2." << std::endl;
    legendQuadrant = 2;
  }


  float x1, y1, x2, y2;
  if( legendQuadrant==1 ) {
    x1 = 0.63;
    y1 = 0.78;
    x2 = 0.8;
    y2 = 0.82;
  } else if( legendQuadrant==2 ) {
    x1 = 0.25;
    y1 = 0.78;
    x2 = 0.42;
    y2 = 0.82;
  } else if( legendQuadrant==3 ) {
    x1 = 0.25;
    y1 = 0.16;
    x2 = 0.42;
    y2 = 0.2;
  } else if( legendQuadrant==0 ) {
    x1 = 0.67;
    y1 = 0.953;
    x2 = 0.86;
    y2 = 0.975;
  }


  TPaveText* label_sqrt = new TPaveText(x1,y1,x2,y2, "brNDC");
  label_sqrt->SetFillColor(kWhite);
  label_sqrt->SetTextSize(0.038);
  label_sqrt->SetTextFont(42);
  label_sqrt->SetTextAlign(31); // align right
  label_sqrt->AddText("#sqrt{s} = 7 TeV");
  return label_sqrt;

}





void make2D(){


TPaveText* label_cms = get_labelCMS(0,"2011", false, "ALL");
  TPaveText* label_sqrt = get_labelSqrt(0);
  

  //**************************observed cms*******************************//
  Double_t obs_lim_cms[16]={100., 145., 157., 179., 192., 216., 221., 218., 218., 221., 216., 192., 179., 157., 145., 100.};

  Double_t ctau[16]={0., 0., 0., 0., 0., 0., 0., 0., 1., 250., 500., 1000., 2000., 4000., 6000., 6000.};

  TGraph* obs_exclusion_cms;
  obs_exclusion_cms = new TGraph(16, obs_lim_cms,ctau);
  obs_exclusion_cms->SetFillColor(5);
  obs_exclusion_cms->SetLineColor(0);
  obs_exclusion_cms->SetLineWidth(0);




//**************************expecteded cms*******************************//
  Double_t exp_lim_cms[16]={100., 148., 160., 182., 199., 218., 221., 221., 221., 221., 218., 199., 182., 160., 148., 100.};

 

  TGraph* exp_exclusion_cms;
  exp_exclusion_cms = new TGraph(16, exp_lim_cms,ctau );
//  exp_exclusion_cms->SetFillStyle(3022);
//  exp_exclusion_cms->SetFillColor(kOrange+7);
//  exp_exclusion_cms->SetLineColor(kOrange+7);
  exp_exclusion_cms->SetLineWidth(1);
  exp_exclusion_cms->SetLineStyle(2);



//**************************observed cdf*******************************//
  Double_t obs_lim_cdf[6]={100., 146., 149.5, 149.5, 146., 100.};
  Double_t lifetime_cdf[6]={0., 0., 0., 300., 600., 600.};


  TGraph* obs_exclusion_cdf;
  obs_exclusion_cdf = new TGraph(6, obs_lim_cdf,lifetime_cdf );
  obs_exclusion_cdf->SetFillColor(29);
  obs_exclusion_cdf->SetFillStyle(3144);
  obs_exclusion_cdf->SetLineColor(0);
  obs_exclusion_cdf->SetLineWidth(0);


//**************************observed D0*******************************//
  Double_t obs_lim_d0[4]={100., 175., 175., 100.};
  Double_t lifetime_d0[4]={0., 0., 0.9, 0.9};

 

  TGraph* obs_exclusion_d0;
  obs_exclusion_d0 = new TGraph(4, obs_lim_d0,lifetime_d0 );
  obs_exclusion_d0->SetFillColor(9);
  obs_exclusion_d0->SetLineColor(0);
  obs_exclusion_d0->SetLineWidth(0);
//  obs_exclusion_d0->SetFillStyle(3144);



//**************************observed conversion*******************************//
  Double_t obs_lim_conv[4]={100., 140., 140., 100.};
  Double_t lifetime_conv[4]={0., 0., 30., 30.};

 

  TGraph* obs_exclusion_conv;
  obs_exclusion_conv = new TGraph(4, obs_lim_conv,lifetime_conv );
  obs_exclusion_conv->SetFillColor(46);
  obs_exclusion_conv->SetLineColor(0);
  obs_exclusion_conv->SetLineWidth(0);
  obs_exclusion_conv->SetFillStyle(3001);

  
  

  TCanvas* c0 = new TCanvas("exclusion limit", "exclusion limit", 1);
  c0->cd();
  // c0->SetLogy();
  //c0->SetGridx();
  //c0->SetGridy();


    TPad * pad2 = new TPad("pad2", "pad2",0.01,0.0001,0.99,0.99);

    pad2->Draw("AH");
    pad2->SetBorderSize(0.);
    pad2->cd();
    /* 
    TH2F *h3 = new TH2F("h3","Axes",100,100,300,100,0,100);
    h3->SetLineColor(kWhite);   
    h3->SetLineWidth(0.);   
    h3->Draw("L");
    */
  
 

    TPad * pad1 = new TPad("pad1", "pad1",0.01,0.002,0.99,0.99);
    
    pad1->SetLogy();
    pad1->Draw();
    pad1->cd();


    
    TH2F *h2 = new TH2F("h","Axes",100,100,250,100,0.7,1E8);
    h2->GetXaxis()->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
    h2->GetYaxis()->SetTitle("c#tau_{#tilde{#chi^{0}_{1}}} [mm]");
    h2->GetXaxis()->SetTitleSize(0.048);
    h2->GetXaxis()->SetTitleOffset(1.24);
    h2->GetYaxis()->SetTitleSize(0.048);
    h2->GetYaxis()->SetTitleOffset(1.6);
    
    h2->Draw("");
    obs_exclusion_cms->Draw("LF");
    obs_exclusion_cdf->Draw("LFsame");
 //   obs_exclusion_conv->Draw("LFsame");
    obs_exclusion_d0->Draw("LFsame");
    exp_exclusion_cms->Draw("Lsame");
    h2->Draw("sameaxis");



    TPaveText *pt = new TPaveText(0.45,0.3,0.65,0.55,"NDC");
   pt = new TPaveText(0.4551193,0.2499166,0.6555865,0.3643191,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(5);
   pt->SetTextAlign(12);
   pt->SetTextFont(42);
   pt->SetTextColor(kGray+2);
    pt->AddText("#font[42]{SPS8}"); 
    pt->AddText("#font[42]{GMSB #tilde{#chi}^{0}_{1}#rightarrow #gamma #tilde{G}}");
   pt->AddText("#font[42]{M_{m} = 2#Lambda, tan(#beta)=15}");
   pt->AddText("#font[42]{N_{m} = 1, #mu > 0}");

    TLegend* leg = new TLegend(0.25,0.55,0.8,0.92);
    leg->SetFillStyle(0); leg->SetBorderSize(0); 
    leg->SetFillColor(0);
    
    label_cms->Draw("same");
    label_sqrt->Draw("same");
    pt->Draw("same");

 
    leg ->SetTextFont(42);
    leg->AddEntry(exp_exclusion_cms, "CMS expected with #gamma + Jets (4.86 fb^{-1})", "l");
    leg->AddEntry(obs_exclusion_cms, "CMS observed with #gamma + Jets (4.86 fb^{-1})", "f");
//    leg->AddEntry(obs_exclusion_conv,"CMS with #gamma (conversions) + #slash{E}_{T} + Jets (2.1 fb^{-1})","f");
    leg->AddEntry(obs_exclusion_cdf,"CDF with #gamma #gamma + #slash{E}_{T} + Jets (2.6 fb^{-1})","f"); 
    leg->AddEntry(obs_exclusion_d0,"D#slash{O} with prompt #gamma #gamma + #slash{E}_{T} (6.3 fb^{-1})","f");
  
    leg->Draw("same");

   c0->SaveAs("./PAS_PLOT/2D_exclusion.png");
   c0->SaveAs("./PAS_PLOT/2D_exclusion.pdf");


}
