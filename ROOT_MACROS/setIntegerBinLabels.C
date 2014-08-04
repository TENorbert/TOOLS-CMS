// For histograms plotting discrete numbers (nJets, nHits, etc.), it makes 
// more sense to have discrete bin labels rather than letting ROOT set up
// an axis with intermediate values, tick marks, etc.

// This script will set integers as the bin labels, assigning startingValue
// to the first bin (which will usually be either 0 or 1).

void setIntegerBinLabels( TH1F* h, int startingValue = 0 ) 
{
  // Draw with graphics off to make sure X axis exists
  h->Draw("goff"); 
  int nBins = h->GetNbinsX();
  for ( int i = 0; i < nBins; i++ )
    h->GetXaxis()->SetBinLabel( i + 1, Form( "%i", i + startingValue ) ); 
}
