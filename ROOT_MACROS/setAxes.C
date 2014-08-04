/* Created by Jeff Klukas
   
   Example of usage in root:
   .X setAxes(hist1, "p_{T}")
      This would change the x-axis title of hist1 to pt and the y axis to 
   "Number of Events per Bin". 

   The method is implemented for 1-D and 2-D histograms as well as stacks.
*/



void setAxes(TH1 &*hist, TString XaxisName, TString YaxisName = 
             "Number of Events per Bin", double YaxisOffset = 1.2) {
   hist->GetXaxis()->SetTitle(XaxisName);
   hist->GetYaxis()->SetTitle(YaxisName);
   hist->GetYaxis()->SetTitleOffset(YaxisOffset);
}

void setAxes(TH2 &*hist, TString XaxisName, 
             TString YaxisName = "Number of Events per Bin") {
   hist->GetXaxis()->SetTitle(XaxisName);
   hist->GetYaxis()->SetTitle(YaxisName);
}

void setAxes(THStack &*hist, TString XaxisName, 
             TString YaxisName = "Number of Events per Bin", 
             double YaxisOffset = 1.2) {
   hist->GetXaxis()->SetTitle(XaxisName);
   hist->GetYaxis()->SetTitle(YaxisName);
   hist->GetYaxis()->SetTitleOffset(YaxisOffset);
}
