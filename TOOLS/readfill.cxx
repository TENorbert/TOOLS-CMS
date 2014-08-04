#include "Riostream.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TFile.h"
#include "TH1F.h"
#include "TNtuple.h"
#include <vector>

using namespace std;

void readfill(ifstream ifile, TFile* file, TH1F* h, TNtuple* tuple);


// Real Main do now!
int main( )
{
  ifstream ifile; 
  ifile.open("sampledata.txt",ios::in);
  TFile *f = new TFile("Outfile.root","RECREATE");
  TH1F *h1 = new TH1F("h1","x distribution",100,-4,4);
  TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x:y:z");
  
  readfill(ifile, f, h1, ntuple);
  f->Write();
  ifile.close();
  return(0);
}

// Define fxn
void readfill(fstream ifile, TFile* f, TH1F* h1, TNtuple* ntuple)
{
  std::string line;
  float x;
  float y;
  float z;
  int nlines = 0;
  vector<float> vecx;
  vector <float> vecy;
  vector<float> vecz;
  // Now read file
  if(ifile.is_open()){
      string line;
      while (getline(ifile, line))
            {
                 line >> x >> y >> z;
		 vecx.push_back(x);
		 vecy.push_back(y);
		 vecz.push_back(z);
            //  if (!in.good()) break;
                 // if (nlines < 0)  exit(1);//printf("x=%8f, y=%8f, z=%8f\n",x,y,z);
                  h1->Fill(x);
                  ntuple->Fill(x,y,z);
                  nlines++;
            }  
  printf("found %d points\n",nlines);

  ifile.close();

  }else
       cout << "Error opening file " << endl;

}
