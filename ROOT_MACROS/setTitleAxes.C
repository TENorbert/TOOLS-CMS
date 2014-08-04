/* Created by Jeff Klukas
   
   Example of usage in root:
   .X setTitleAxes(hist1, "p_{T} of Reconstructed Muons", "p_{T}")
   This would change the x-axis title of hist1 to pt and the y axis to 
   "Number of Events per Bin", while the main title would be "pt of
   Reconstructed muons". 
   
   This method is implemented for 1-D and 2-D histograms.
*/



void setTitleAxes(TH1 &*hist, TString title, TString XaxisName, 
                  TString YaxisName = "Number of Events per Bin", 
                  double YaxisOffset = 1.2) {
   hist->SetTitle(title);
   hist->GetXaxis()->SetTitle(XaxisName);
   hist->GetYaxis()->SetTitle(YaxisName);
   hist->GetYaxis()->SetTitleOffset(YaxisOffset);
}

void setTitleAxes(TH2 &*hist, TString title, TString XaxisName, 
                  TString YaxisName = "Number of Events per Bin", 
                  double XaxisOffset = 1.2, double YaxisOffset = 1.2) {
   hist->SetTitle(title);
   hist->GetXaxis()->SetTitle(XaxisName);
   hist->GetYaxis()->SetTitle(YaxisName);
   hist->GetXaxis()->SetTitle(XaxisName);
   hist->GetYaxis()->SetTitle(YaxisName);
   hist->GetXaxis()->SetTitleOffset(XaxisOffset);
   hist->GetYaxis()->SetTitleOffset(YaxisOffset);
}
