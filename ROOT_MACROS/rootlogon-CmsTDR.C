{
// Lines for loading framework lite, probably not needed for you.
/*gSystem->Load("libFWCoreFWLite.so"); 
  AutoLibraryLoader::enable();
  gSystem->Load("libDataFormatsFWLite.so");
  gROOT->ProcessLine("namespace edm {typedef edm::Wrapper<vector<float> > Wrapper<vector<float,allocator<float> > >; }");
  gROOT->ProcessLine("namespace edm {typedef edm::Wrapper<vector<double> > Wrapper<vector<double,allocator<double> > >; }");
*/

  gROOT->LoadMacro("style-CMSTDR.C");
  setTDRStyle()
}
