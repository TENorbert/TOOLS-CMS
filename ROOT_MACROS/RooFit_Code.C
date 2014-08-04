void fit_Zmumu_noiso(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    gStyle->SetEndErrorSize(0);
    gSystem->Load("libRooFit"); 
    using namespace RooFit;

    TFile *f = new TFile("./Histograms_FDR2.root");
    TH1F *h1 = (TH1F*)h_Z_noiso_mumu_m->Clone("h1");
    
    RooRealVar x("x","M(#mu#mu)",0,200.,"GeV");
    RooDataHist data("data","dataset with ZMass",x,h1);

    // make signal function
    RooRealVar Mean("Mean","Mean",90.85,50,150,"GeV");
    RooRealVar Sigma("Sigma","Sigma",7.39,0,50,"GeV");
    RooBreitWigner Sig("Sig", "Signal pdf", x, Mean, Sigma);
    //RooGaussian Sig("Sig", "Signal pdf", x, Mean, Sigma);

    Mean.setConstant(kTRUE);
    Sigma.setConstant(kTRUE);
    
    // make background function
    RooRealVar p1("p1","coeff #1", 100, -1000., 1000.);
    RooRealVar p2("p2","coeff #2", 10, -100., 100.);
    RooRealVar p3("p3","coeff #3", 10, -1000., 1000.);
    RooRealVar p4("p4","coeff #4", 10, -1000., 1000.);
    RooRealVar p5("p5","coeff #5", 10, -1000., 1000.);
    //RooPolynomial Bkg("bkg","bkgd pdf", x, RooArgList(p1,p2,p3,p4,p5));
    RooPolynomial Bkg("bkg","bkgd pdf", x, RooArgList(p1));
    
    // we are interested in getting the number of signal/background events
    RooRealVar Nsig("Nsig","signal fraction",500,0.,10000.);
    RooRealVar Nbkg("Nbkg","background fraction",200,0.,10000.);
        //define the model
    RooAddPdf model("model","signal+bkgd",RooArgList(Sig,Bkg),RooArgList(Nsig,Nbkg));

    //fit (notice use of Extended(true) - this is so that we can get nsig/nbkg
    RooFitResult *fitres = model.fitTo(data,Extended(kTRUE),Hesse(kFALSE),Minos(kFALSE),PrintLevel(0),Save(kTRUE),Range(50,150),Extended() );
    
    // make new frame and plot histogram and fit results on it 
    RooPlot* xframe = x.frame();
    data.plotOn(xframe,MarkerColor(kBlue), LineColor(kBlue),  Binning(100));

    model.plotOn(xframe,Components(Bkg),LineStyle(kDashed),LineColor(2),Range(50,150));
    model.plotOn(xframe,LineColor(2));
    //model.paramOn(xframe, Parameters( RooArgSet(Mean,Sigma)), Format("NE"), Layout(0.52,0.89,0.89));
    
    xframe->SetTitle("");
    xframe->SetXTitle("M(#mu#mu) [GeV]");
    xframe->GetXaxis()->SetNdivisions(505);
	xframe->GetYaxis()->SetNdivisions(505);
    xframe->GetYaxis()->SetTitleOffset(1.2);
    xframe->GetXaxis()->CenterTitle(1);
	xframe->GetYaxis()->CenterTitle(1);
    xframe->SetMinimum(0);
    xframe->GetXaxis()->SetRange(26,70);
    xframe->Draw();

    TLegend *leg = new TLegend(0.65,0.80,0.89,0.89);
    leg->SetHeader("No Isolation Requirement");
    leg->SetFillColor(10);
    leg->SetLineColor(10);
	leg->Draw("same");
    
    // print the results of the fit   
    fitres->Print(); 

    // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
    cout<<" Fit chi square/dof = "<<xframe->chiSquare(7)<<endl; 

    x.setRange("signal",70,110) ;
    RooAbsReal* fracSig = Sig.createIntegral(x, x, "signal") ;
    Double_t nsig_frac  = Nsig.getVal() * fracSig->getVal();
    RooAbsReal* fracBkg = Bkg.createIntegral(x, x, "signal") ;
    Double_t nbkg_frac  = Nbkg.getVal() * fracBkg->getVal();

    cout<< " Signal = " << nsig_frac << endl;
    cout<< " BKGD = " << nbkg_frac << endl;
}

void fit_Zmumu_etcone20_1(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    gStyle->SetEndErrorSize(0);
    gSystem->Load("libRooFit"); 
    using namespace RooFit;

    TFile *f = new TFile("./Histograms_FDR2.root");
    TH1F *h1 = (TH1F*)h_Z_etcone_1_mumu_m->Clone("h1");
    
    RooRealVar x("x","M(#mu#mu)",0,200.,"GeV");
    RooDataHist data("data","dataset with ZMass",x,h1);

    // make signal function
    RooRealVar Mean("Mean","Mean",90.85,50,150,"GeV");
    RooRealVar Sigma("Sigma","Sigma",7.39,0,50,"GeV");
    RooBreitWigner Sig("Sig", "Signal pdf", x, Mean, Sigma);

    Mean.setConstant(kTRUE);
    Sigma.setConstant(kTRUE);
    
    // make background function
    //RooRealVar p0("p0","coeff #0", 0.1, -100., 100.);
    RooRealVar p1("p1","coeff #1", 0., -1000., 1000.);
    RooRealVar p2("p2","coeff #2", 0., -1000., 1000.);
    RooPolynomial Bkg("bkg","bkgd pdf", x, RooArgList(p1,p2));

    // we are interested in getting the number of signal/background events
    RooRealVar Nsig("Nsig","signal fraction",10,0.,10000.);
    RooRealVar Nbkg("Nbkg","background fraction",10,0.,10000.);

    //define the model
    RooAddPdf model("model","signal+bkgd",RooArgList(Sig,Bkg),RooArgList(Nsig,Nbkg));

    //fit (notice use of Extended(true) - this is so that we can get nsig/nbkg
    RooFitResult *fitres = model.fitTo(data,Extended(kTRUE),Hesse(kFALSE),Minos(kFALSE),PrintLevel(0),Save(kTRUE),Range(50,150),Extended() );
    
    // make new frame and plot histogram and fit results on it 
    RooPlot* xframe = x.frame();
    data.plotOn(xframe,MarkerColor(kBlue), LineColor(kBlue),  Binning(100));

    model.plotOn(xframe,Components(Bkg),LineStyle(kDashed),LineColor(2),Range(50,150));
    model.plotOn(xframe,LineColor(2));
    //model.paramOn(xframe, Parameters( RooArgSet(Mean,Sigma)), Format("NE"), Layout(0.52,0.89,0.89));
    
    xframe->SetTitle("");
    xframe->SetXTitle("M(#mu#mu) [GeV]");
    xframe->GetXaxis()->SetNdivisions(505);
	xframe->GetYaxis()->SetNdivisions(505);
    xframe->GetYaxis()->SetTitleOffset(1.2);
    xframe->GetXaxis()->CenterTitle(1);
	xframe->GetYaxis()->CenterTitle(1);
    xframe->SetMinimum(0);
    xframe->GetXaxis()->SetRange(26,70);
    xframe->Draw();

    TLegend *leg = new TLegend(0.65,0.80,0.89,0.89);
    leg->SetHeader("At least one #mu : E_{T}cone20(#mu) < 6 GeV");
    leg->SetFillColor(10);
    leg->SetLineColor(10);
	leg->Draw("same");
    
    // print the results of the fit   
    fitres->Print(); 

    // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
    cout<<" Fit chi square/dof = "<<xframe->chiSquare(7)<<endl; 

    x.setRange("signal",70,110) ;
    RooAbsReal* fracSig = Sig.createIntegral(x, x, "signal") ;
    Double_t nsig_frac  = Nsig.getVal() * fracSig->getVal();
    RooAbsReal* fracBkg = Bkg.createIntegral(x, x, "signal") ;
    Double_t nbkg_frac  = Nbkg.getVal() * fracBkg->getVal();

    cout<< " Signal = " << nsig_frac << endl;
    cout<< " BKGD = " << nbkg_frac << endl;
}

void fit_Zmumu_etcone20(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    gStyle->SetEndErrorSize(0);
    gSystem->Load("libRooFit"); 
    using namespace RooFit;

    TFile *f = new TFile("./Histograms_FDR2.root");
    TH1F *h1 = (TH1F*)h_Z_etcone_mumu_m->Clone("h1");
    
    RooRealVar x("x","M(#mu#mu)",0,200.,"GeV");
    RooDataHist data("data","dataset with ZMass",x,h1);

    // make signal function
    RooRealVar Mean("Mean","Mean",90.85,50,150,"GeV");
    RooRealVar Sigma("Sigma","Sigma",7.39,0,50,"GeV");
    RooBreitWigner Sig("Sig", "Signal pdf", x, Mean, Sigma);

    Mean.setConstant(kTRUE);
    Sigma.setConstant(kTRUE);
    
    // make background function
    //RooRealVar p0("p0","coeff #0", 0.1, -100., 100.);
    RooRealVar p1("p1","coeff #1", 0, -1000., 1000.);
    RooRealVar p2("p2","coeff #2", 0, -1000., 1000.);
    RooPolynomial Bkg("bkg","bkgd pdf",x,RooArgList(p1,p2));

    // we are interested in getting the number of signal/background events
    RooRealVar Nsig("Nsig","signal fraction",10,0.,10000.);
    RooRealVar Nbkg("Nbkg","background fraction",10,0.,10000.);

    //define the model
    RooAddPdf model("model","signal+bkgd",RooArgList(Sig,Bkg),RooArgList(Nsig,Nbkg));

    //fit (notice use of Extended(true) - this is so that we can get nsig/nbkg
    RooFitResult *fitres = model.fitTo(data,Extended(kTRUE),Hesse(kFALSE),Minos(kFALSE),PrintLevel(0),Save(kTRUE),Range(50,150) ,Extended() );

    // make new frame and plot histogram and fit results on it 
    RooPlot* xframe = x.frame();
    data.plotOn(xframe,MarkerColor(kBlue), LineColor(kBlue),  Binning(100));

    model.plotOn(xframe,Components(Bkg),LineStyle(kDashed),LineColor(2),Range(50,150));
    model.plotOn(xframe,LineColor(2));
    //model.paramOn(xframe, Parameters( RooArgSet(Mean,Sigma)), Format("NE"), Layout(0.52,0.89,0.89));
    
    xframe->SetTitle("");
    xframe->SetXTitle("M(#mu#mu) [GeV]");
    xframe->GetXaxis()->SetNdivisions(505);
	xframe->GetYaxis()->SetNdivisions(505);
    xframe->GetYaxis()->SetTitleOffset(1.2);
    xframe->GetXaxis()->CenterTitle(1);
	xframe->GetYaxis()->CenterTitle(1);
    xframe->SetMinimum(0);
    xframe->GetXaxis()->SetRange(26,70);
    xframe->Draw();

    TLegend *leg = new TLegend(0.65,0.80,0.89,0.89);
    leg->SetHeader("Both #mu : E_{T}cone20(#mu) < 6 GeV");
    leg->SetFillColor(10);
    leg->SetLineColor(10);
	leg->Draw("same");
    
    // print the results of the fit   
    fitres->Print(); 

    // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
    cout<<" Fit chi square/dof = "<<xframe->chiSquare(7)<<endl; 

    x.setRange("signal",70,110) ;
    RooAbsReal* fracSig = Sig.createIntegral(x, x, "signal") ;
    Double_t nsig_frac  = Nsig.getVal() * fracSig->getVal();
    RooAbsReal* fracBkg = Bkg.createIntegral(x, x, "signal") ;
    Double_t nbkg_frac  = Nbkg.getVal() * fracBkg->getVal();

    cout<< " Signal = " << nsig_frac << endl;
    cout<< " BKGD = " << nbkg_frac << endl;
}

void fit_Zmumu_psi_1(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    gStyle->SetEndErrorSize(0);
    gSystem->Load("libRooFit"); 
    using namespace RooFit;

    TFile *f = new TFile("./Histograms_FDR2.root");
    TH1F *h1 = (TH1F*)h_Z_psi_1_mumu_m->Clone("h1");
    
    RooRealVar x("x","M(#mu#mu)",0,200.,"GeV");
    RooDataHist data("data","dataset with ZMass",x,h1);

    // make signal function
    RooRealVar Mean("Mean","Mean",90.85,50,150,"GeV");
    RooRealVar Sigma("Sigma","Sigma",7.39,0,50,"GeV");
    RooBreitWigner Sig("Sig", "Signal pdf", x, Mean, Sigma);

    Mean.setConstant(kTRUE);
    Sigma.setConstant(kTRUE);

    // make background function
    RooRealVar p1("p1","coeff #1", 0, -1000., 1000.);
    RooRealVar p2("p2","coeff #2", 0, -1000., 1000.);
    RooPolynomial Bkg("bkg","bkgd pdf",x,RooArgList(p1,p2));

    // we are interested in getting the number of signal/background events
    RooRealVar Nsig("Nsig","signal fraction",10, 0.,10000.);
    RooRealVar Nbkg("Nbkg","background fraction",10 ,0.,10000.);

    //define the model
    RooAddPdf model("model","signal+bkgd",RooArgList(Sig,Bkg),RooArgList(Nsig,Nbkg));

    //fit (notice use of Extended(true) - this is so that we can get nsig/nbkg
    RooFitResult *fitres = model.fitTo(data,Extended(kTRUE),Hesse(kFALSE),Minos(kFALSE),PrintLevel(0),Save(kTRUE),Range(50,150));

    // make new frame and plot histogram and fit results on it 
    RooPlot* xframe = x.frame();
    data.plotOn(xframe,MarkerColor(kBlue), LineColor(kBlue),  Binning(100));

    model.plotOn(xframe,Components(Bkg),LineStyle(kDashed),LineColor(2),Range(50,150));
    model.plotOn(xframe,LineColor(2));
    //model.paramOn(xframe, Parameters( RooArgSet(Mean,Sigma)), Format("NE"), Layout(0.52,0.89,0.89));
    
    xframe->SetTitle("");
    xframe->SetXTitle("M(#mu#mu) [GeV]");
    xframe->GetXaxis()->SetNdivisions(505);
	xframe->GetYaxis()->SetNdivisions(505);
    xframe->GetYaxis()->SetTitleOffset(1.2);
    xframe->GetXaxis()->CenterTitle(1);
	xframe->GetYaxis()->CenterTitle(1);
    xframe->SetMinimum(0);
    xframe->GetXaxis()->SetRange(26,70);
    xframe->Draw();

    TLegend *leg = new TLegend(0.65,0.80,0.89,0.89);
    leg->SetHeader("At least one #mu : #Psi(#mu, jet) < 0.5");
    leg->SetFillColor(10);
    leg->SetLineColor(10);
	leg->Draw("same");
    
    // print the results of the fit   
    fitres->Print(); 

    // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
    cout<<" Fit chi square/dof = "<<xframe->chiSquare(7)<<endl; 

    x.setRange("signal",70,110) ;
    RooAbsReal* fracSig = Sig.createIntegral(x, x, "signal") ;
    Double_t nsig_frac  = Nsig.getVal() * fracSig->getVal();
    RooAbsReal* fracBkg = Bkg.createIntegral(x, x, "signal") ;
    Double_t nbkg_frac  = Nbkg.getVal() * fracBkg->getVal();

    cout<< " Signal = " << nsig_frac << endl;
    cout<< " BKGD = " << nbkg_frac << endl;
}


void fit_Zmumu_psi(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    gStyle->SetEndErrorSize(0);
    gSystem->Load("libRooFit"); 
    using namespace RooFit;

    TFile *f = new TFile("./Histograms_FDR2.root");
    TH1F *h1 = (TH1F*)h_Z_psi_mumu_m->Clone("h1");
    
    RooRealVar x("x","M(#mu#mu)",0,200.,"GeV");
    RooDataHist data("data","dataset with ZMass",x,h1);

    // make signal function
    RooRealVar Mean("Mean","Mean",90.85,50,150,"GeV");
    RooRealVar Sigma("Sigma","Sigma",7.39,0,50,"GeV");
    RooBreitWigner Sig("Sig", "Signal pdf", x, Mean, Sigma);

    Mean.setConstant(kTRUE);
    Sigma.setConstant(kTRUE);

    // make background function
    RooRealVar p1("p1","coeff #1", 0., -1000., 1000.);
    RooRealVar p2("p2","coeff #2", 0., -1000., 1000.);
    //RooPolynomial Bkg("bkg","bkgd pdf", x, RooArgList(p1,p2));
    RooPolynomial Bkg("bkg","bkgd pdf", x, RooArgList(p1));

    // we are interested in getting the number of signal/background events
    RooRealVar Nsig("Nsig","signal fraction",10,0.,10000.);
    RooRealVar Nbkg("Nbkg","background fraction",10,0.,10000.);

    //define the model
    RooAddPdf model("model","signal+bkgd",RooArgList(Sig,Bkg),RooArgList(Nsig,Nbkg));

    //fit (notice use of Extended(true) - this is so that we can get nsig/nbkg
    RooFitResult *fitres = model.fitTo(data,Extended(kTRUE),Hesse(kFALSE),Minos(kFALSE),PrintLevel(0),Save(kTRUE),Range(50,150));

    // make new frame and plot histogram and fit results on it 
    RooPlot* xframe = x.frame();
    data.plotOn(xframe,MarkerColor(kBlue), LineColor(kBlue),  Binning(100));

    model.plotOn(xframe,Components(Bkg),LineStyle(kDashed),LineColor(2),Range(50,150));
    model.plotOn(xframe,LineColor(2));
    //model.paramOn(xframe, Parameters( RooArgSet(Mean,Sigma)), Format("NE"), Layout(0.52,0.89,0.89));
    
    xframe->SetTitle("");
    xframe->SetXTitle("M(#mu#mu) [GeV]");
    xframe->GetXaxis()->SetNdivisions(505);
	xframe->GetYaxis()->SetNdivisions(505);
    xframe->GetYaxis()->SetTitleOffset(1.2);
    xframe->GetXaxis()->CenterTitle(1);
	xframe->GetYaxis()->CenterTitle(1);
    xframe->SetMinimum(0);
    xframe->GetXaxis()->SetRange(26,70);
    xframe->Draw();

    TLegend *leg = new TLegend(0.65,0.80,0.89,0.89);
    leg->SetHeader("Both #mu : #Psi(#mu, jet) < 0.5");
    leg->SetFillColor(10);
    leg->SetLineColor(10);
	leg->Draw("same");
    
    // print the results of the fit   
    fitres->Print(); 

    // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
    cout<<" Fit chi square/dof = "<<xframe->chiSquare(7)<<endl; 

    x.setRange("signal",70,110) ;
    RooAbsReal* fracSig = Sig.createIntegral(x, x, "signal") ;
    Double_t nsig_frac  = Nsig.getVal() * fracSig->getVal();
    RooAbsReal* fracBkg = Bkg.createIntegral(x, x, "signal") ;
    Double_t nbkg_frac  = Nbkg.getVal() * fracBkg->getVal();

    cout<< " Signal = " << nsig_frac << endl;
    cout<< " BKGD = " << nbkg_frac << endl;
}

void W_mt(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1  = (TH1F*)h_W_psi_munu_mt->Clone("h1");
    TH1F *h2  = (TH1F*)h_W_etcone_munu_mt->Clone("h2");
    
    h1->Rebin(5);
    h2->Rebin(5);

    h1->SetXTitle("M_{T}(#mu#nu) [GeV]");
	h1->SetYTitle("Entries /5GeV");
	h1->SetTitle("");
	h1->GetXaxis()->SetNdivisions(510);
	h1->GetYaxis()->SetNdivisions(505);
    h1->GetYaxis()->SetLabelSize(0.03);	
    h1->GetYaxis()->SetTitleOffset(1.2);	
	
    h1->SetLineWidth(2);	
	h1->SetLineColor(4);
    h1->SetMarkerStyle(20);
    h1->SetMarkerColor(4);
    h1->SetMarkerSize(1.2);
    h2->SetLineWidth(2);	
	h2->SetLineColor(2);
	h2->SetFillColor(2);
	h2->SetFillStyle(3004);
	h1->GetXaxis()->CenterTitle(1);
	h1->GetYaxis()->CenterTitle(1);
    h1->GetXaxis()->SetRange(5,28);
    
    h1->Draw("e");
    h2->Draw("ehsame");
   
    TLegend *leg = new TLegend(0.63,0.75,0.89,0.89);
    leg->AddEntry(h1,"#Psi(#mu, jet) < 0.5", "lp");
    leg->AddEntry(h2,"E_{T}cone20(#mu) < 6 GeV", "lp");
	leg->SetFillColor(10);
	leg->SetLineColor(10);
	leg->Draw("same");
}

void W_mt_sub(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1  = (TH1F*)h_W_psi_munu_mt->Clone("h1");
    TH1F *h2  = (TH1F*)h_W_etcone_munu_mt->Clone("h2");
    TH1F *h22  = (TH1F*)h_W_etcone_munu_mt->Clone("h22");
    TH1F *h3  = (TH1F*)h_W_etcone_b_jet_munu_mt_1_psi05->Clone("h3");
    
    h1->Rebin(5);
    h22->Rebin(5);
    h2->Rebin(5);
    h3->Rebin(5);

    h2->Add(h3, -1);

    h1->SetXTitle("M_{T}(#mu#nu) [GeV]");
	h1->SetYTitle("Entries /5GeV");
	h1->SetTitle("");
	h1->GetXaxis()->SetNdivisions(510);
	h1->GetYaxis()->SetNdivisions(505);
    h1->GetYaxis()->SetLabelSize(0.03);	
    h1->GetYaxis()->SetTitleOffset(1.2);	
	
    h1->SetLineWidth(2);	
	h1->SetLineColor(4);
    h1->SetMarkerStyle(20);
    h1->SetMarkerColor(4);
    h1->SetMarkerSize(1.2);
    h2->SetLineWidth(3);	
    h22->SetLineWidth(3);	
	h2->SetLineColor(2);
	h22->SetLineColor(1);
	h2->SetFillColor(2);
	h22->SetFillColor(1);
	h2->SetFillStyle(3004);
	h22->SetFillStyle(3004);
	h22->SetLineStyle(2);
	h1->GetXaxis()->CenterTitle(1);
	h1->GetYaxis()->CenterTitle(1);
    h1->GetXaxis()->SetRange(5,28);
    
    h1->Draw("e");
    h1->Draw("esame");
    h22->Draw("ehsame");
    h2->Draw("ehsame");
   
    TLegend *leg = new TLegend(0.63,0.75,0.89,0.89);
    leg->AddEntry(h1,"#Psi(#mu, jet) < 0.5", "lp");
    leg->AddEntry(h2,"E_{T}cone20(#mu) < 6 GeV", "lp");
    leg->AddEntry(h22,"E_{T}cone20(#mu) < 6 GeV", "lp");
	leg->SetFillColor(10);
	leg->SetLineColor(10);
	leg->Draw("same");

    TLegend *leg2 = new TLegend(0.63,0.75,0.89,0.89);
    leg2->SetHeader("has jet around, #Psi(#mu, jet) > 0.5");
	leg2->SetFillColor(10);
	leg2->SetLineColor(10);
	leg2->Draw("same");

}

void W_etcone_b_jet_muon_dR(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1  = (TH1F*)h_W_etcone_b_jet_muon_dR_1->Clone("h1");
    
    h1->Rebin(10);

    h1->SetXTitle("#DeltaR(#mu, jet)");
	h1->SetYTitle("Entries");
	h1->SetTitle("");
	h1->GetXaxis()->SetNdivisions(510);
	h1->GetYaxis()->SetNdivisions(505);
    h1->GetYaxis()->SetLabelSize(0.03);	
    h1->GetYaxis()->SetTitleOffset(1.2);	
	
    h1->SetLineWidth(3);	
	h1->SetLineColor(4);
    h1->SetFillStyle(3004);
    h1->SetFillColor(4);
    h1->SetMarkerSize(1.2);
	h1->GetXaxis()->CenterTitle(1);
	h1->GetYaxis()->CenterTitle(1);
    h1->GetXaxis()->SetRange(0,60);
    
    h1->Draw();
   
    TLegend *leg2 = new TLegend(0.63,0.75,0.89,0.89);
    leg2->SetHeader("M_{T}(#mu, #nu) : 40 ~ 60 GeV");
	leg2->SetFillColor(10);
	leg2->SetLineColor(10);
	leg2->Draw("same");

}

void W_etcone_b_jet_muon_psi(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1  = (TH1F*)h_W_etcone_b_jet_muon_psi_1->Clone("h1");
    
    h1->SetXTitle("#Psi [#mu, jet]");
	h1->SetYTitle("Entries");
	h1->SetTitle("");
	h1->GetXaxis()->SetNdivisions(510);
	h1->GetYaxis()->SetNdivisions(505);
    h1->GetYaxis()->SetLabelSize(0.03);	
    h1->GetYaxis()->SetTitleOffset(1.2);	
	
    h1->SetLineWidth(3);	
	h1->SetLineColor(4);
    h1->SetFillStyle(3004);
    h1->SetFillColor(4);
    h1->SetMarkerSize(1.2);
	h1->GetXaxis()->CenterTitle(1);
	h1->GetYaxis()->CenterTitle(1);
    
    h1->Draw();
   
    TLegend *leg2 = new TLegend(0.63,0.75,0.89,0.89);
    leg2->SetHeader("M_{T}(#mu, #nu) : 40 ~ 60 GeV");
	leg2->SetFillColor(10);
	leg2->SetLineColor(10);
	leg2->Draw("same");

}


void Ratio_Num_W_diff_iso(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 700, 700);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1   = (TH1F*)h_W_psi_Muon_pt_2->Clone("h1");
    TH1F *h2   = (TH1F*)h_W_etcone_Muon_pt_2->Clone("h2");
    TFile *b  = new TFile("Histograms_SMTT.root");
    TH1F *h11   = (TH1F*)h_W_psi_Muon_pt_2->Clone("h11");
    TH1F *h22   = (TH1F*)h_W_etcone_Muon_pt_2->Clone("h22");
    
    h1  ->Rebin(10);
    h2  ->Rebin(10);
    h11 ->Rebin(10);
    h22 ->Rebin(10);
    
    double s11 = h1->Integral(3, 3);
    double s12 = h2->Integral(3, 3);
    double s21 = h1->Integral(4, 4);
    double s22 = h2->Integral(4, 4);
    double s31 = h1->Integral(5, 6);
    double s32 = h2->Integral(5, 6);
    double s41 = h1->Integral(7, 8);
    double s42 = h2->Integral(7, 8);
    double s51 = h1->Integral(9, 50);
    double s52 = h2->Integral(9, 50);
    
    double ss11 = h11->Integral(3, 3);
    double ss12 = h22->Integral(3, 3);
    double ss21 = h11->Integral(4, 4);
    double ss22 = h22->Integral(4, 4);
    double ss31 = h11->Integral(5, 6);
    double ss32 = h22->Integral(5, 6);
    double ss41 = h11->Integral(7, 8);
    double ss42 = h22->Integral(7, 8);
    double ss51 = h11->Integral(9, 50);
    double ss52 = h22->Integral(9, 50);

    cout<< s11 << " +/- " << sqrt(s11)  << "    "<< s12 << " +/- " << sqrt(s12)  <<  endl;
    cout<< s21 << " +/- " << sqrt(s21)  << "    "<< s22 << " +/- " << sqrt(s22)  <<  endl;
    cout<< s31 << " +/- " << sqrt(s31)  <<  "    "<< s32 <<  " +/- " << sqrt(s32)  << endl;
    cout<< s41 << " +/- " << sqrt(s41)  <<  "    "<< s42 << " +/- " << sqrt(s42)  <<  endl;
    cout<< s51 << " +/- " << sqrt(s51)  <<  "    "<< s52 <<  " +/- " << sqrt(s52)  << endl;

    Float_t x[5]  = { 25, 35, 50, 70, 100 };
    Float_t ex[5] = {  0,  0,  0,  0,   0 };
    Float_t y1[5] = { 
                       s11/s12, 
                       s21/s22, 
                       s31/s32, 
                       s41/s42, 
                       s51/s52 
                    };
    Float_t ey1[5] = { 
                        (s11/s12)*sqrt( 1/s11 + 1/s12 ),
                        (s21/s22)*sqrt( 1/s21 + 1/s22 ),
                        (s31/s32)*sqrt( 1/s31 + 1/s32 ),
                        (s41/s42)*sqrt( 1/s41 + 1/s42 ),
                        (s51/s52)*sqrt( 1/s51 + 1/s52 ),
    };

    Float_t y2[5] = { 
                       ss11/ss12, 
                       ss21/ss22, 
                       ss31/ss32, 
                       ss41/ss42, 
                       ss51/ss52 
                    };
    Float_t ey2[5] = { 
                       (ss11/ss12)*sqrt( 1/ss11 + 1/ss12 ),
                       (ss21/ss22)*sqrt( 1/ss21 + 1/ss22 ),
                       (ss31/ss32)*sqrt( 1/ss31 + 1/ss32 ),
                       (ss41/ss42)*sqrt( 1/ss41 + 1/ss42 ),
                       (ss51/ss52)*sqrt( 1/ss51 + 1/ss52 ),
    };

     
    gStyle->SetEndErrorSize(0); 
    TGraphErrors *gr1 = new TGraphErrors(5,x, y1, ex, ey1);
    TGraphErrors *gr2 = new TGraphErrors(5,x, y2, ex, ey2);
    
    gr1->GetXaxis()->SetTitle("P_{T}(#mu) [GeV]");
    gr1->GetYaxis()->SetTitle("N(W)_{#Psi} / N(W)_{E_{T}cone20}");
    gr1->SetTitle("");
    gr1->GetXaxis()->CenterTitle(1);   
    gr1->GetYaxis()->CenterTitle(1);   
    gr1->GetXaxis()->SetNdivisions(505);
    gr1->GetYaxis()->SetNdivisions(505); 
    gr1->GetYaxis()->SetLabelSize(0.04);	
    gr1->GetYaxis()->SetTitleOffset(1.);

    TAxis *xaxis = gr1->GetXaxis();
	xaxis->SetLimits(20,105);
	
    gr1->SetLineWidth(3);	
    gr1->SetLineColor(4);
    gr1->SetMarkerColor(4);
    gr1->SetMarkerStyle(21);
    gr1->SetMarkerSize(1.2);
	
    gr2->SetLineWidth(3);	
    gr2->SetLineColor(2);
    gr2->SetMarkerColor(2);
    gr2->SetMarkerStyle(24);
    gr2->SetMarkerSize(1.2);
    
	gr1->Draw("PLA");
	//gr2->Draw("PLsame");

    TLegend *leg = new TLegend(0.30,0.70,0.89,0.89);
    leg->SetHeader("In mass window 40 ~ 100 GeV");
    //leg->AddEntry(gr1,"FDR2", "pl");
    //leg->AddEntry(gr2,"SM t#bar{t}", "pl");
	leg->SetFillColor(10);
	leg->SetLineColor(10);
	leg->Draw("same");

    TLine *line2 = new TLine(20, 1, 105, 1);
    line2 -> SetLineWidth(3);
    line2 -> SetLineColor(1);
    line2 -> SetLineStyle(2);
    line2->Draw("same");
}

void Top_m(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 600, 600);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1  = (TH1F*)h_Top_psi_m->Clone("h1");
    TH1F *h2  = (TH1F*)h_Top_etcone_m->Clone("h2");
    
    h1->Rebin(10);
    h2->Rebin(10);

    h1->SetXTitle("M(top) [GeV]");
	h1->SetYTitle("Entries /10GeV");
	h1->SetTitle("");
	h1->GetXaxis()->SetNdivisions(505);
	h1->GetYaxis()->SetNdivisions(505);
    h1->GetYaxis()->SetLabelSize(0.03);	
    h1->GetYaxis()->SetTitleOffset(1.2);	
	
    h1->SetLineWidth(2);	
	h1->SetLineColor(4);
    h1->SetMarkerStyle(20);
    h1->SetMarkerColor(4);
    h1->SetMarkerSize(1.2);
    h2->SetLineWidth(2);	
	h2->SetLineColor(2);
	h2->SetFillColor(2);
	h2->SetFillStyle(3004);
	h1->GetXaxis()->CenterTitle(1);
	h1->GetYaxis()->CenterTitle(1);
    h1->GetXaxis()->SetRange(0,50);
   
    h1->SetMaximum(90);
    h1->Draw("e");
    h2->Draw("hesame");
   
    TLegend *leg = new TLegend(0.63,0.75,0.89,0.89);
    leg->AddEntry(h1," #Psi(#mu, jet) < 0.5 ", "lp");
    leg->AddEntry(h2," E_{T}cone20(#mu) < 6 GeV ", "lp");
	leg->SetFillColor(10);
	leg->SetLineColor(10);
	leg->Draw("same");
}

void Ratio_Num_Top_diff_iso(){
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadColor(0);
    gStyle->SetPalette(1);
    c1 = new TCanvas("HF1", "Histos1", 0, 0, 700, 700);

    TFile *a  = new TFile("Histograms_FDR2.root");
    TH1F *h1   = (TH1F*)h_Top_psi_Muon_pt_2->Clone("h1");
    TH1F *h2   = (TH1F*)h_Top_etcone_Muon_pt_2->Clone("h2");
    TFile *b  = new TFile("Histograms_SMTT.root");
    TH1F *h11   = (TH1F*)h_Top_psi_Muon_pt_2->Clone("h11");
    TH1F *h22   = (TH1F*)h_Top_etcone_Muon_pt_2->Clone("h22");
    
    h1  ->Rebin(10);
    h2  ->Rebin(10);
    h11 ->Rebin(10);
    h22 ->Rebin(10);
    
    double s11 = h1->Integral(3, 3);
    double s12 = h2->Integral(3, 3);
    double s21 = h1->Integral(4, 4);
    double s22 = h2->Integral(4, 4);
    double s31 = h1->Integral(5, 6);
    double s32 = h2->Integral(5, 6);
    double s41 = h1->Integral(7, 8);
    double s42 = h2->Integral(7, 8);
    double s51 = h1->Integral(9, 50);
    double s52 = h2->Integral(9, 50);
    
    double ss11 = h11->Integral(3, 3);
    double ss12 = h22->Integral(3, 3);
    double ss21 = h11->Integral(4, 4);
    double ss22 = h22->Integral(4, 4);
    double ss31 = h11->Integral(5, 6);
    double ss32 = h22->Integral(5, 6);
    double ss41 = h11->Integral(7, 8);
    double ss42 = h22->Integral(7, 8);
    double ss51 = h11->Integral(9, 50);
    double ss52 = h22->Integral(9, 50);

    cout<< s11 << " +/- " << sqrt(s11)  << "    "<< s12 << " +/- " << sqrt(s12)  <<  endl;
    cout<< s21 << " +/- " << sqrt(s21)  << "    "<< s22 << " +/- " << sqrt(s22)  <<  endl;
    cout<< s31 << " +/- " << sqrt(s31)  <<  "    "<< s32 <<  " +/- " << sqrt(s32)  << endl;
    cout<< s41 << " +/- " << sqrt(s41)  <<  "    "<< s42 << " +/- " << sqrt(s42)  <<  endl;
    cout<< s51 << " +/- " << sqrt(s51)  <<  "    "<< s52 <<  " +/- " << sqrt(s52)  << endl;


    Float_t x[5]  = { 25, 35, 50, 70, 100 };
    Float_t ex[5] = {  0,  0,  0,  0,   0 };
    Float_t y1[5] = { 
                       s11/s12, 
                       s21/s22, 
                       s31/s32, 
                       s41/s42, 
                       s51/s52 
                    };
    Float_t ey1[5] = { 
                        (s11/s12)*sqrt( 1/s11 + 1/s12 ),
                        (s21/s22)*sqrt( 1/s21 + 1/s22 ),
                        (s31/s32)*sqrt( 1/s31 + 1/s32 ),
                        (s41/s42)*sqrt( 1/s41 + 1/s42 ),
                        (s51/s52)*sqrt( 1/s51 + 1/s52 ),
    };

    Float_t y2[5] = { 
                       ss11/ss12, 
                       ss21/ss22, 
                       ss31/ss32, 
                       ss41/ss42, 
                       ss51/ss52 
                    };
    Float_t ey2[5] = { 
                       (ss11/ss12)*sqrt( 1/ss11 + 1/ss12 ),
                       (ss21/ss22)*sqrt( 1/ss21 + 1/ss22 ),
                       (ss31/ss32)*sqrt( 1/ss31 + 1/ss32 ),
                       (ss41/ss42)*sqrt( 1/ss41 + 1/ss42 ),
                       (ss51/ss52)*sqrt( 1/ss51 + 1/ss52 ),
    };

     
    gStyle->SetEndErrorSize(0); 
    TGraphErrors *gr1 = new TGraphErrors(5,x, y1, ex, ey1);
    TGraphErrors *gr2 = new TGraphErrors(5,x, y2, ex, ey2);
    
    gr1->GetXaxis()->SetTitle("P_{T}(#mu) [GeV]");
    gr1->GetYaxis()->SetTitle("N(top)_{#Psi} / N(top)_{E_{T}cone20}");
    gr1->SetTitle("");
    gr1->GetXaxis()->CenterTitle(1);   
    gr1->GetYaxis()->CenterTitle(1);   
    gr1->GetXaxis()->SetNdivisions(505);
    gr1->GetYaxis()->SetNdivisions(505); 
    gr1->GetYaxis()->SetLabelSize(0.04);	
    gr1->GetYaxis()->SetTitleOffset(1.);

    TAxis *xaxis = gr1->GetXaxis();
	xaxis->SetLimits(20,105);
	
    gr1->SetLineWidth(3);	
    gr1->SetLineColor(4);
    gr1->SetMarkerColor(4);
    gr1->SetMarkerStyle(21);
    gr1->SetMarkerSize(1.2);
	
    gr2->SetLineWidth(3);	
    gr2->SetLineColor(2);
    gr2->SetMarkerColor(2);
    gr2->SetMarkerStyle(24);
    gr2->SetMarkerSize(1.2);
    
	gr1->Draw("PLA");
	//gr2->Draw("PLsame");

    TLegend *leg = new TLegend(0.30,0.70,0.89,0.89);
    leg->SetHeader("In mass window 130 ~ 210 GeV");
    //leg->AddEntry(gr1,"FDR2", "pl");
    //leg->AddEntry(gr2,"SM t#bar{t}", "pl");
	leg->SetFillColor(10);
	leg->SetLineColor(10);
	leg->Draw("same");

    TLine *line2 = new TLine(20, 1, 105, 1);
    line2 -> SetLineWidth(3);
    line2 -> SetLineColor(1);
    line2 -> SetLineStyle(2);
    line2->Draw("same");
}

void cal(){

   //Double_t ss = sqrt( ( (2*2*1530.6*1530.6) / ((1530.6+1549.4)*(1530.6+1549.4)*(1530.6+1549.4)*(1530.6+1549.4)) )*44.2*44.2 + ( 2/(1530.6+1549.4) - (2*1530.6) / ( (1530.6+1549.4)*(1530.6+1549.4) ) )*( 2/(1530.6+1549.4) - (2*1530.6) / ( (1530.6+1549.4)*(1530.6+1549.4) ) )*41.9*41.9              );

   //Double_t ss = sqrt(      ( (2*2*1561.0*1561.0) / ((1561.0+1602.4)*(1561.0+1602.4)*(1561.0+1602.4)*(1561.0+1602.4)) )*43.9*43.9            +          ( 2/(1561.0+1602.4) - ((2*1561.0) / ( (1561.0+1602.4)*(1561.0+1602.4) )) )*( 2/(1561.0+1602.4) -  ((2*1561.0) / ( (1561.0+1602.4)*(1561.0+1602.4) )) )*42.1*42.1              );
   
  
 Double_t ss = sqrt(      ( (2*2*1530.6*1530.6) / ((1530.6+1549.4)*(1530.6+1549.4)*(1530.6+1549.4)*(1530.6+1549.4)) )*(44.2*44.2 + 41.9*41.9)            +          ( 2/(1530.6+1549.4) - ((2*2*1530.6) / ( (1530.6+1549.4)*(1530.6+1549.4) )) )*( 2/(1530.6+1549.4) -  ((2*2*1530.6) / ( (1530.6+1549.4)*(1530.6+1549.4) )) )*41.9*41.9              );

  // Double_t ss = sqrt(      ( (2*2*1561.0*1561.0) / ((1561.0+1602.4)*(1561.0+1602.4)*(1561.0+1602.4)*(1561.0+1602.4)) )*(43.9*43.9 + 42.1*42.1)            +          ( 2/(1561.0+1602.4) - ((2*2*1561.0) / ( (1561.0+1602.4)*(1561.0+1602.4) )) )*( 2/(1561.0+1602.4) -  ((2*2*1561.0) / ( (1561.0+1602.4)*(1561.0+1602.4) )) )*42.1*42.1              );
   
   
   cout<< ss <<endl;

}

