#include "Histogramming.h"


//constructor
Histogramming::Histogramming(){

}
//Destructor
Histogramming::~Histogramming(){

}

//Now Methods
TH1D* Histogramming::RebinHist( TH1D* h1, string newHistoName, double minBC ){
     // accumuate bin information
     vector<double> xV ;
     vector<double> yV ;
     double aveBC = 0 ;
     double sumBC = 0 ;
     double rbin_ = 0 ;
     for ( int i= 1 ; i<= h1->GetNbinsX() ; i++ ) {

         double bc_ = h1->GetBinContent(i) ;
         double x_ = h1->GetBinCenter(i) ;
         double bc1_ = ( i == h1->GetNbinsX() ) ? minBC+1 : h1->GetBinContent(i+1) ;
         //cout<<" x: "<< x_ <<" y:"<< bc_ ;
         if ( aveBC < minBC ) {
            sumBC += bc_ ;
            rbin_ += 1. ;
            aveBC = sumBC / rbin_ ;
            if ( (aveBC > minBC && bc1_ > minBC) || i == h1->GetNbinsX() ) {
               xV.push_back( x_ ) ;
               yV.push_back( aveBC );
               //cout<<" aveBC: "<< aveBC<<" from sum:"<< sumBC<<"/"<<rbin_ ;
               sumBC = 0 ;
               rbin_ = 0 ;
            }
            aveBC = 0 ;
         }
         //cout<<" "<<endl ;
     }
     //cout<<" ====================== "<<endl ;
     // refill the histogram
     TH1D* h2 = (TH1D*) h1->Clone() ;
     h2->SetName( newHistoName.c_str() ) ;
     int ii = 0 ;
     for ( int i= 1 ; i<= h1->GetNbinsX() ; i++ ) {
         h2->SetBinContent(i, 0 ) ;
         for ( size_t j= ii ; j< xV.size(); j++ ) {
              double x_ = h2->GetBinCenter(i) ;
              if ( x_ <= xV[j] ) {
                 h2->SetBinContent(i, yV[j] ) ;
                 ii = j ;
                 //cout<<" x: "<< x_ <<" xB:"<< xV[j]<<" y: "<< yV[j] <<endl ;
                 break ;
              }
         }
     }
     return h2 ;

// Rebin 2
TH1D* Histogramming::Rebin( TH1D* Ih, int rebval )
{
 Ih->Rebin(rebval);
return Ih; 
}

// TTree
RooWorkspace* Histogramming::Hist2Ntuple( TH1D* Ih, TFile* If ) 
{
  Float_t ecalt; 
  TH1D* hs = (TH1D*)If->Get(Ih);
  RooWorkspace* ws = new RooWorkspace();
  TNtuple * ntuple = new TNtuple("Ntuple","Unbinned Input Varaible", "ecalt");
  //TTree * tree = new TTree("Tree","Unbinned Input Varaible");
  //TBranch * br1= tree->Branch("ecalt",&Float_t, "ecalt/F"); 
  //Int_t NumEntries = (Int_t)hs->GetEntries();
  Int_t Nxbins     = (Int_t)hs->GetNbinsX();
  //for (Int_t entry = 0; entry <= NumEnetries; entry++){
  for (Int_t entry = 0; entry <= Nxbins; entry++){
        ecalt = hs->GetBincontent(entry);
        ntuple->Fill(ecalt);
       }
  //tree->Print();
  ntuple->Print();

  ntuple->Write("",TObject::kOverwrite); // save new version only
//  tree->Write("",TObject::kOverwrite); // save new version only
return ws;
}

