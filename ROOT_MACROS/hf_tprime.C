//////////////////////////////////////////////////////////////////////////
//
// tprime.C  version 0.2
//
// Routine for statistical inference for a tprime analysis
//
//
// Gena Kukartsev
//
// May 2011: first version
// 

/*
The code should be compiled in ROOT (5.28.00c or higher):

root -l

.L hf_tprime.C+

Usage:
       limit = limit(param, ...);

Inputs:
       param          - Parameter


Description goes here
*/

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "TFile.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TUnixSystem.h"
#include "TH1D.h"
#include "TStopwatch.h"

#include "RooRandom.h"
#include "RooWorkspace.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooFitResult.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProposalHelper.h"
#include "RooStats/TestStatistic.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/ToyMCSampler.h"
//#include "RooStats/FrequentistCalculator.h"
//#include "RooStats/HypoTestResult.h"
#include "RooStats/HistFactory/LinInterpVar.h"
//#include "RooStats/SequentialProposal.h"

//#include "lorenzo_moneta_HypoTestInvDemo_16jun2011.C"
//#include "lorenzo_HypoTestInvDemo_20jun2011.C"
//#include "roostats_HypoTestInvDemo.C"
#include "StandardHypoTestInvDemo.C"

using namespace RooFit;
using namespace RooStats;

// ---> user interface ---------------------------------------------

Int_t limit( std::string channel = "ejets", // ejets, mujets, combined
	     std::string mode = "observed",    // obsereved, expected
	     std::string method = "asymcls",       // mcmc, plr, cls, asymcls
	     double peak = 450,                // resonance mass
	     std::string suffix = "",          // suffix for output file names
	     Int_t ntoys = 10000,                  // number of pseudoexperiments for expected limit
	     Int_t npoints = 100,                // points to scan
	     Double_t poimin =  0.,           // scan lower range
	     Double_t poimax = 10.,           // scan upper range
	     Int_t mcmc_iter = 0,         // number of MCMC iterations
	     Int_t mcmc_burnin = 0,          // number of MCMC burn in steps to be discarded
	     std::string inputdir = "");       // directory with workspace files

void inspect( void );
void run_mcmc( std::string channel = "combined", std::string mode = "observed" );
void run_frequentist( std::string channel = "combined" );
void hf_tprime(void){} // dummy

// ---> implementation below ---------------------------------------

class Tprime{
  //
  // The class combines multiple channel analyses. A workspace is created
  // with the combined model, data, model config. The class can also call
  // interval calculation routines
  //

public:

  Tprime();
  ~Tprime();
  
  Int_t LoadWorkspace(std::string filename,
		      std::string ws_name);

  RooStats::ModelConfig * GetModelConfig( std::string mcName = "ModelConfig" );
  RooWorkspace * GetWorkspace( void ){return pWs;};
  RooStats::ModelConfig * SetBModel( void );

  Int_t SetParameterPoints( std::string sbModelName,
			   std::string bModelName );

  RooAbsData * GetWorkspaceData( std::string datasetName ); // return dataset from workspace and set data to it
  RooAbsData * GetPseudoData( void ); // generate and return pseudo-data

  Int_t PrintToFile( std::string filename,
		     std::vector<Double_t> entries,
		     std::string outputLegend = "Some numbers");

  Int_t RunCls( std::string channel, // ejets, mujets, combined
		std::string mode,    // observed, expected
		double peak,         // resonance mass
		std::string suffix,  // suffix for output file names
		Int_t ntoys,         // number of pseudoexperiments for expected limit
		std::string method,  // asymtotic cls or standard cls
		Int_t npoints,       // points to scan
		Double_t poimin,     // scan lower range
		Double_t poimax,     // scan upper range
		std::string inputdir
		); // input dir name

  Int_t RunMcmc( std::string channel = "combined", // Bayesian MCMC calculation
		 std::string mode = "observed",
		 double peak = 350,
		 std::string suffix = "",
		 Int_t ntoys = 1,
  		 Int_t mcmc_iter = 100000,
		 Int_t mcmc_burnin = 100,
		 std::string inputdir = "");

  Int_t RunPlr( std::string channel = "combined",  // Profile likelihood ratio calculation
		std::string mode = "observed",
		double peak = 350,
		std::string suffix = "",
		Int_t ntoys = 1,
		std::string inputdir = "" );

  LikelihoodInterval * GetPlrInterval( double conf_level );

  MCMCInterval * GetMcmcInterval(double conf_level,
				 int n_iter,
				 int n_burn,
				 double left_side_tail_fraction,
				 int n_bins);

  MCMCInterval * GetMcmcInterval(ModelConfig mc,
				 double conf_level,
				 int n_iter,
				 int n_burn,
				 double left_side_tail_fraction,
				 int n_bins);
  void makeMcmcPosteriorPlot( std::string filename );
  double printMcmcUpperLimit( double peak, std::string filename = "" );


  //RooStats::HypoTestResult * GetFrequentistHypoTest();
  //void GetFrequentistLimit( void );





private:

  Double_t GetPoiUpper(std::string channel, Double_t peak);

  Double_t GetRandom( std::string pdf, std::string var );

  RooWorkspace * pWs;

  // roostats calculators results
  LikelihoodInterval * pPlrInt;
  MCMCInterval * mcInt;

  // test statistic and toy sampler
  RooStats::TestStatistic * pTestStatistic;
  RooStats::ToyMCSampler * pToyMcSampler;

  // current dataset, may be observed data or pseudo-data
  RooAbsData * data;

  // random numbers
  TRandom3 r;
};


Tprime::Tprime(){

  std::string legend = "[Tprime::Tprime]: ";
  pWs = 0;
  pPlrInt = 0;
  mcInt = 0;
  data = 0;
  pTestStatistic = 0;
  pToyMcSampler = 0;

  // set random seed
  r.SetSeed();
  UInt_t _seed = r.GetSeed();
  UInt_t _pid = gSystem->GetPid();
  std::cout << legend << "Random seed: " << _seed << std::endl;
  std::cout << legend << "PID: " << _pid << std::endl;
  _seed = 31*_seed+_pid;
  //_seed = 1437;
  std::cout << legend << "Random seed: " << _seed << std::endl;
  r.SetSeed(_seed);

  // set RooFit random seed (it has a private copy)
  RooRandom::randomGenerator()->SetSeed(_seed);
}


Tprime::~Tprime(){
  delete pWs;
  delete pPlrInt;
  delete mcInt;
  delete data;
  delete pTestStatistic;
  delete pToyMcSampler;
}



Double_t Tprime::GetPoiUpper(std::string channel, Double_t peak){
  //
  // Estimate a good value for the upper boundary of the range of POI
  //

  Double_t result = 5.0;

  std::cout << "[Tprime]: " << channel << ", mass = " << peak << std::endl;

  return result;

}



Int_t Tprime::LoadWorkspace(std::string filename,
			    std::string ws_name){
  //
  // load a workspace from a file
  // a copy of the workspace is kept as a class member,
  // the input file is immediately closed
  //
  std::string legend = "[Tprime::LoadWorkspace]: ";

  TFile * infile = new TFile(filename.c_str(), "read");

  std::cout << legend << "loading workspace " << ws_name << std::endl;

  //pWs = infile->Get(ws_name.c_str())->Print();

  pWs = (RooWorkspace *)infile->Get(ws_name.c_str())->Clone();
  if (!pWs){
    std::cout << legend << "workspace " << ws_name 
	      << " not found" << std::endl;
    return -1;
  }

  delete infile;
  
  SetBModel();

  GetWorkspaceData("obsData");

  SetParameterPoints("ModelConfig", "BModel");

  return 0;
}


RooStats::ModelConfig * Tprime::GetModelConfig( std::string mcName ){
  //
  // Return a pointer to the ModelConfig in the loaded
  // workspace, or 0 if no workspace is loaded.
  // User does NOT take ownership.
  //

  if (pWs){
    RooStats::ModelConfig * _mc = (RooStats::ModelConfig *)pWs->obj(mcName.c_str());
    _mc -> SetWorkspace(*pWs);
    //_mc->Print();
    //_mc->GetWorkspace()->Print();
    return _mc;
  }
  else return 0;
}



RooStats::ModelConfig * Tprime::SetBModel( void ){
  //
  // Define model config and parameter snapshot to describe the b model.
  // Import to workspace.
  //

  std::string legend = "[Tprime::SetBModel]: ";

  // full signal+background model
  //RooStats::ModelConfig * pSBModel = (RooStats::ModelConfig *)pWs->genobj("ModelConfig");
  
  // let's make the b model (bg-only) from the alt model (s+b) with xsec=0
  //RooStats::ModelConfig * pBModel = 
  //  new RooStats::ModelConfig(*(RooStats::ModelConfig *)pWs->genobj("ModelConfig"));
  RooStats::ModelConfig * _sbModel = (RooStats::ModelConfig *)pWs->genobj("ModelConfig");
  RooStats::ModelConfig * pBModel = _sbModel->Clone("BModel");

  //pBModel->SetName("BModel");

  pBModel->SetWorkspace(*pWs);

  //pBModel->SetParametersOfInterest(RooArgSet());
  pWs->import(*pBModel);

  // set POI to the b model value and take snapshot
  RooRealVar * pPoi = (RooRealVar *)pBModel->GetParametersOfInterest()->first();
  pPoi->setVal(0.0);
  pBModel->SetSnapshot(*pPoi);

  pBModel->Print();

  return pBModel;
}



Int_t Tprime::SetParameterPoints( std::string sbModelName,
				 std::string bModelName ){
  //
  // Fit the data with S+B model.
  // Make a snapshot of the S+B parameter point.
  // Profile with POI=0.
  // Make a snapshot of the B parameter point
  // (B model is the S+B model with POI=0
  //

  Double_t poi_value_for_b_model = 0.0;

  // get S+B model config from workspace
  RooStats::ModelConfig * pSbModel = (RooStats::ModelConfig *)pWs->obj(sbModelName.c_str());
  pSbModel->SetWorkspace(*pWs);

  // get parameter of interest set
  const RooArgSet * poi = pSbModel->GetParametersOfInterest();

  // get B model config from workspace
  RooStats::ModelConfig * pBModel = (RooStats::ModelConfig *)pWs->obj(bModelName.c_str());
  pBModel->SetWorkspace(*pWs);

  // make sure that data has been loaded
  if (!data) return -1;

  // find parameter point for global maximum with the S+B model,
  // with conditional MLEs for nuisance parameters
  // and save the parameter point snapshot in the Workspace
  RooAbsReal * nll = pSbModel->GetPdf()->createNLL(*data);
  RooAbsReal * profile = nll->createProfile(RooArgSet());
  profile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
  RooArgSet * poiAndNuisance = new RooArgSet();
  if(pSbModel->GetNuisanceParameters())
    poiAndNuisance->add(*pSbModel->GetNuisanceParameters());
  poiAndNuisance->add(*pSbModel->GetParametersOfInterest());
  pWs->defineSet("SPlusBModelParameters", *poiAndNuisance);
  pWs->saveSnapshot("SPlusBFitParameters",*poiAndNuisance);
  pSbModel->SetSnapshot(*poi);
  RooArgSet * sbModelFitParams = (RooArgSet *)poiAndNuisance->snapshot();
  cout << "\nWill save these parameter points that correspond to the fit to data" << endl;
  sbModelFitParams->Print("v");
  delete profile;
  delete nll;
  delete poiAndNuisance;
  delete sbModelFitParams;

  //

  // Find a parameter point for generating pseudo-data
  // with the background-only data.
  // Save the parameter point snapshot in the Workspace
  nll = pBModel->GetPdf()->createNLL(*data);
  profile = nll->createProfile(*poi);
  ((RooRealVar *)poi->first())->setVal(poi_value_for_b_model);
  profile->getVal(); // this will do fit and set nuisance parameters to profiled values
  poiAndNuisance = new RooArgSet();
  if(pBModel->GetNuisanceParameters())
    poiAndNuisance->add(*pBModel->GetNuisanceParameters());
  poiAndNuisance->add(*pBModel->GetParametersOfInterest());
  pWs->defineSet("parameterPointToGenerateData", *poiAndNuisance);
  pWs->saveSnapshot("parametersToGenerateData",*poiAndNuisance);
  pBModel->SetSnapshot(*poi);
  RooArgSet * paramsToGenerateData = (RooArgSet *)poiAndNuisance->snapshot();
  cout << "\nShould use these parameter points to generate pseudo data for bkg only" << endl;
  paramsToGenerateData->Print("v");
  delete profile;
  delete nll;
  delete poiAndNuisance;
  delete paramsToGenerateData;

  return 0;
}



RooAbsData * Tprime::GetWorkspaceData( std::string datasetName ){
  //
  // Return a pointer to a dataset from workspace.
  // Class member pointer data set to point to the dataset.
  // Caller does not take ownership.
  //
  
  //RooArgSet * pPoiSet = pMc
  delete data;
  data = (RooAbsData *)pWs->data(datasetName.c_str())->Clone();

  return data;
}



RooAbsData * Tprime::GetPseudoData( void ){
  //
  // Generate pseudo data, return a pointer.
  // Class member pointer data set to point to the dataset.
  // Caller does not take ownership.
  //
  
  static int n_toys = 0;

  // legend for printouts
  std::string legend = "[Tprime::GetPseudoData()]: ";

  delete data;

  // We will use ToyMCSampler to generate pseudo-data (and test statistic, eventually)
  // We are responsible for randomizing nuisances and global observables,
  // ToyMCSampler only generates observables (as of ROOT 5.30.00-rc1 and before)
  
  // MC sampler and test statistic
  if(n_toys == 0){ // on first entry
    // get B model config from workspace
    RooStats::ModelConfig * pBModel = (RooStats::ModelConfig *)pWs->obj("BModel");
    pBModel->SetWorkspace(*pWs);

    // get parameter of interest set
    //const RooArgSet * poi = pSbModel->GetParametersOfInterest();

    //RooStats::TestStatistic * pTestStatistic = new RooStats::ProfileLikelihoodTestStat(*pBModel->GetPdf());  
    //RooStats::ToyMCSampler toymcs(*pTestStatistic, 1);
    pTestStatistic = new RooStats::ProfileLikelihoodTestStat(*pBModel->GetPdf());  
    pToyMcSampler = new RooStats::ToyMCSampler(*pTestStatistic, 1);
    pToyMcSampler->SetPdf(*pBModel->GetPdf());
    pToyMcSampler->SetObservables(*pBModel->GetObservables());
    pToyMcSampler->SetParametersForTestStat(*pBModel->GetParametersOfInterest()); // just POI
    pToyMcSampler->SetGlobalObservables(*pBModel->GetGlobalObservables());
  }

  // load parameter point
  pWs->loadSnapshot("parametersToGenerateData");

  RooArgSet dummySet;
  data = pToyMcSampler->GenerateToyData(dummySet);
  std::cout << legend << "generated the following background-only pseudo-data:" << std::endl;
  data->Print();

  // count number of generated toys
  ++n_toys;

  return data;
}



Int_t Tprime::PrintToFile( std::string filename,
			   std::vector<Double_t> entries,
			   std::string outputLegend){
  //
  // print out numbers from the vector to file, append if the file exists
  //

  if (filename.size()!=0){
    
    std::ofstream aFile;

    // append to file if exists
    aFile.open(filename.c_str(), std::ios_base::app);

    if (aFile.tellp() < 1){
      aFile << outputLegend << std::endl;
      aFile << "#------------------------------------------------------------" << std::endl;
    }

    char buf[1024];
    std::string str_buf = "";
    for (std::vector<Double_t>::const_iterator num = entries.begin();
	 num != entries.end();
	 ++num){
      sprintf(buf, "%15.6f", *num);
      str_buf.append(buf);
    }

    
    aFile << str_buf << std::endl;

    // close outfile here so it is safe even if subsequent iterations crash
    aFile.close();

  }

  return 0;
}



MCMCInterval * Tprime::GetMcmcInterval(double conf_level,
				       int n_iter,
				       int n_burn,
				       double left_side_tail_fraction,
				       int n_bins){
  //
  // Bayesian MCMC calculation using the ModelConfig from the workspace
  // Want an efficient proposal function, so derive it from covariance
  // matrix of fit
  //

  return GetMcmcInterval(*GetModelConfig(),
			 conf_level,
			 n_iter,
			 n_burn,
			 left_side_tail_fraction,
			 n_bins);
}


  
MCMCInterval * Tprime::GetMcmcInterval(ModelConfig mc,
				       double conf_level,
				       int n_iter,
				       int n_burn,
				       double left_side_tail_fraction,
				       int n_bins){
  //
  // Bayesian MCMC calculation using arbitrary ModelConfig
  // Want an efficient proposal function, so derive it from covariance
  // matrix of fit
  //
  
  RooAbsData * _data = data;
  //RooAbsData * _data = pWs->data("obsData");
  //RooStats::ModelConfig * _mc = (RooStats::ModelConfig *)pWs->genobj("ModelConfig");
  RooStats::ModelConfig * _mc = GetModelConfig();
  _mc->Print();

  //RooFitResult * fit = pWs->pdf("model_tprime")->fitTo(*_data,Save());
  RooFitResult * fit = _mc->GetPdf()->fitTo(*_data,Save());
  ProposalHelper ph;
  ph.SetVariables((RooArgSet&)fit->floatParsFinal());
  ph.SetCovMatrix(fit->covarianceMatrix());
  ph.SetUpdateProposalParameters(kTRUE); // auto-create mean vars and add mappings
  ph.SetCacheSize(100);
  ProposalFunction * pf = ph.GetProposalFunction();

  //delete pf;
  //pf = new SequentialProposal();
  
  MCMCCalculator mcmc( *_data, mc );
  mcmc.SetConfidenceLevel(conf_level);
  mcmc.SetNumIters(n_iter);          // Metropolis-Hastings algorithm iterations
  mcmc.SetProposalFunction(*pf);
  mcmc.SetNumBurnInSteps(n_burn); // first N steps to be ignored as burn-in
  mcmc.SetLeftSideTailFraction(left_side_tail_fraction);
  mcmc.SetNumBins(n_bins);
  
  //mcInt = mcmc.GetInterval();
  try {
    mcInt = mcmc.GetInterval();
  } catch ( std::length_error &ex) {
    mcInt = 0;
  }

  //std::cout << "!!!!!!!!!!!!!! interval" << std::endl;
  if (mcInt == 0) std::cout << "No interval found!" << std::endl;

  delete fit;
  delete pf;

  return mcInt;
}



LikelihoodInterval * Tprime::GetPlrInterval( double conf_level ){
  //
  // Profile likelihood ratio interval calculations
  //

  delete pPlrInt;
  
  RooStats::ModelConfig * _mc = GetModelConfig();
  _mc->Print();

  ProfileLikelihoodCalculator plc(*data, *_mc);
  plc.SetConfidenceLevel(conf_level);
  pPlrInt = plc.GetInterval();

  return pPlrInt;
}



void Tprime::makeMcmcPosteriorPlot( std::string filename ){
  
  if (mcInt){
    TCanvas c1("c1");
    MCMCIntervalPlot plot(*mcInt);
    plot.Draw();
    c1.SaveAs(filename.c_str());
  }
  
  return;
}



double Tprime::printMcmcUpperLimit( double peak, std::string filename ){
  //
  // print out the upper limit on the first Parameter of Interest
  //

  RooStats::ModelConfig * _mc = (RooStats::ModelConfig *)pWs->genobj("ModelConfig");

  RooRealVar * firstPOI = (RooRealVar*) _mc->GetParametersOfInterest()->first();
  double _limit = mcInt->UpperLimit(*firstPOI);
  cout << "\n95% upper limit on " <<firstPOI->GetName()<<" is : "<<
    _limit <<endl;

  if (filename.size()!=0){
    
    std::ofstream aFile;

    // append to file if exists
    aFile.open(filename.c_str(), std::ios_base::app);

    char buf[1024];
    sprintf(buf, "%7.1f   %7.6f", peak, _limit);

    aFile << buf << std::endl;

    // close outfile here so it is safe even if subsequent iterations crash
    aFile.close();

  }

  return _limit;
}


Double_t Tprime::GetRandom( std::string pdf, std::string var ){
  //
  // generates a random number using a pdf in the workspace
  //
  
  // generate a dataset with one entry
  RooDataSet * _ds = pWs->pdf(pdf.c_str())->generate(*pWs->var(var.c_str()), 1);

  Double_t _result = ((RooRealVar *)(_ds->get(0)->first()))->getVal();
  delete _ds;

  return _result;
}



/*
RooStats::HypoTestResult * Tprime::GetFrequentistHypoTest(){

  RooAbsData * pObsData = pWs->data("obsData");
  RooStats::ModelConfig * pSBModel = (RooStats::ModelConfig *)pWs->obj("ModelConfig");

  RooRealVar * pPoi = (RooRealVar *)pSBModel->GetParametersOfInterest()->first();

  pSBModel->SetWorkspace(*pWs);
  //std::cout << "SBModel snapshot
  pSBModel->SetSnapshot(*pPoi);

  RooStats::ModelConfig * pBModel = (RooStats::ModelConfig *)pWs->obj("BModel");
  pBModel->SetWorkspace(*pWs);

  // set POI to the b model value and take snapshot
  pPoi->setVal(0.0);
  pBModel->SetSnapshot(*pPoi);
  //pBModel->LoadSnapshot();

  RooStats::TestStatSampler * pSampler = 0;

  FrequentistCalculator * pFreqCalc = new FrequentistCalculator(*pObsData,
								*pSBModel,
								*pBModel,
								pSampler);

  RooStats::HypoTestResult * pHypoTestResult = pFreqCalc->GetHypoTest();

  return pHypoTestResult;
}



void Tprime::GetFrequentistLimit( void ){

  RooAbsData * pObsData = pWs->data("obsData");

  RooStats::ModelConfig * pSBModel = (RooStats::ModelConfig *)pWs->obj("ModelConfig");
  pSBModel->SetWorkspace(*pWs);
  //pSBModel->SetName("SBModel");

  RooRealVar * pPoi = (RooRealVar *)pSBModel->GetParametersOfInterest()->first();

  //std::cout << "SBModel snapshot
  pSBModel->GetPdf()->fitTo(*pWs->data("obsData"), Save());
  pSBModel->SetSnapshot(*pPoi);

  RooStats::ModelConfig * pBModel = (RooStats::ModelConfig *)pWs->obj("BModel");
  pBModel->SetWorkspace(*pWs);

  // set POI to the b model value and take snapshot
  pPoi->setVal(0.0);
  pBModel->SetSnapshot(*pPoi);
  //pBModel->LoadSnapshot();

  // temp: save the amended workspace to file
  pWs->SaveAs("tprime_model.root");

  pWs->Print();

  HypoTestInvDemo( "tprime_model.root",
		   "tprime_e",
		   "ModelConfig",
		   "BModel",
		   "obsData",                  
		   0,  // calculator type 
		   0,  // test stat type
		   5, // npoints
		   100, // ntoys
		   true  // useCls
		   );

  return;
}
*/


Int_t Tprime::RunCls( std::string channel, // ejets, mujets, combined
		      std::string mode,    // observed, expected
		      double peak,         // resonance mass
		      std::string suffix,  // suffix for output file names
		      Int_t ntoys,         // number of pseudoexperiments for expected limit
		      std::string method,  // method asymcls or cls standard
		      Int_t npoints,       // points to scan
		      Double_t poimin,     // scan upper range
		      Double_t poimax,     // scan upper range
		      std::string inputdir ){ // input dir name
  //
  // CLs calculation
  //

  //RooStats::ProfileLikelihoodTestStat::setReuseNLL(kTRUE) ; // activate use of setData()
  //RooStats::ToyMCSampler::setUseMultiGen(kTRUE) ;// activate use of prepareMultiGen() 

  std::string legend = "[tprime::RunCls()]: ";

  // print out inputs
  std::cout << legend << std::endl;
  std::cout << legend << "Input parameters specified. Some of them are not used and defaults are entered" << std::endl;
  std::cout << legend << "------------------------------" << std::endl;
  std::cout << legend << "channel: " << channel << std::endl;
  std::cout << legend << "mode: " << mode << std::endl;
  std::cout << legend << "input directory: " << inputdir << std::endl;
  std::cout << legend << "resonance peak mass: " << peak << std::endl;
  std::cout << legend << "suffix: ->" << suffix << "<-" << std::endl;
  std::cout << legend << "number of pseudo-experiments: "<< ntoys << std::endl;
  std::cout << legend << std::endl;

  // compose the workspace file name
  char buf[1024];
  sprintf(buf, "%sresults_%04.0f/tprime_%s_tprimeCrossSection_model.root", inputdir.c_str(), peak, channel.c_str());
  std::string _file = buf;
  std::cout << legend << "guessed name of the file with the workspace: >" << _file << "<" << std::endl;

  if (suffix.find("merge_")==std::string::npos){
    
    //load workspace
    LoadWorkspace(_file, channel);
    
    // fix POI range if needed
    std::cout << std::endl << legend << "fixing POI range to [0-10]" << std::endl;
    pWs->var("xsec")->setRange(poimin,poimax);
    
  }

  // timer
  TStopwatch t;
  t.Start();
  

  // FIXME: experimental: try adaptive range
  
  if (suffix.find("merge_")==std::string::npos){
    GetPlrInterval(0.95);
    Double_t upper_limit = pPlrInt->UpperLimit( *pWs->var("xsec") );
    // FIXME: this only works for limits ~1!!! May become 0 - dangerous!
    poimax = ((double)(int)(4.0 * upper_limit*100.0))/100.0; // round to ~1% precision to avoid splitting points in batch
  }
  
  int calcType = 1; // calculator type, 0-freq, 1-hybrid, 2-asymototic cls
  if( method.find("asymcls") != std::string::npos ){
    calcType = 2;
  }
  std::string suff = suffix;
  if(suffix.find('asymcls') != std::string::npos){
    suff = "";
  }

  std::vector<Double_t> lim = 
    StandardHypoTestInvDemo( _file.c_str(), //filename
			     channel.c_str(),//pWs, //ws name const char
			     "ModelConfig",
			     "BModel",
			     "obsData",
			     calcType, 
			     3, // test statistic, 0-lep, 1-tevatron, 2-PL, 3-PL 1-sided
			     true, // useCls
			     npoints, // npoints in the scan
			     poimin, // poimin: use default is poimin >= poimax
			     poimax, // poimax
			     ntoys,// ntoys
			     false, //bool //use number counting 
			     0,//nuispriorname  
			     suff.c_str(), //suffix name
			     peak
			     ); 
  
  std::string _outfile = "tprime_limit_cls_" + channel + "_" + mode + "_" + suffix + ".ascii";
  PrintToFile(_outfile, lim, 
	      "# mass observed_limit  expected_m2    expected_m1   expected_med    expected_p1    expected_p2");

  t.Stop();
  t.Print();

  return 0;
}



Int_t Tprime::RunMcmc( std::string channel, // ejets, mujets, combined
		       std::string mode,    // observed, expected
		       double peak,         // resonance mass
		       std::string suffix,  // suffix for output file names
		       Int_t ntoys,         // number of pseudoexperiments for expected limit
		       Int_t mcmc_iter,     // number of MCMC iterations
		       Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
		       std::string inputdir // input dir name
		       ){
  //
  // Bayesian MCMC calculation
  //

  std::string legend = "[tprime::RunMcmc()]: ";

  // print out inputs
  std::cout << legend << std::endl;
  std::cout << legend << "Input parameters specified. Some of them are not used and defaults are entered" << std::endl;
  std::cout << legend << "------------------------------" << std::endl;
  std::cout << legend << "channel: " << channel << std::endl;
  std::cout << legend << "mode: " << mode << std::endl;
  std::cout << legend << "input directory: " << inputdir << std::endl;
  std::cout << legend << "resonance peak mass: " << peak << std::endl;
  std::cout << legend << "suffix: ->" << suffix << "<-" << std::endl;
  std::cout << legend << "number of pseudo-experiments: "<< ntoys << std::endl;
  std::cout << legend << std::endl;
  std::cout << legend << "Bayesian MCMC parameters" << std::endl;
  std::cout << legend << "------------------------------" << std::endl;
  std::cout << legend << "number of iterations: " << mcmc_iter << std::endl;
  std::cout << legend << "number of burn-in steps to discard: " << mcmc_burnin << std::endl;
  std::cout << legend << std::endl;

  // compose the workspace file name
  char buf[1024];
  sprintf(buf, "%sresults_%04.0f/tprime_%s_tprimeCrossSection_model.root", inputdir.c_str(), peak, channel.c_str());
  std::string _file = buf;
  std::cout << legend << "guessed name of the file with the workspace: >" << _file << "<" << std::endl;

  //load workspace
  LoadWorkspace(_file, channel);

  // change POI range
  double poiUpper = GetPoiUpper(channel, peak);
  std::cout << legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
  pWs->var("xsec")->setRange(0.0, poiUpper);
  
  // timer
  TStopwatch t;
  t.Start();

  int pe_counter = 0;
  std::vector<Double_t> _limits;
  while (pe_counter < ntoys){
    
    // for mass limit, add k-factor systematics to the nsig systematics
    // FIXME: this is a correlated part of the uncertainty!!!
    //  - different uncertainties for graviton and Z' models
    if ( mode.find("mass_") != std::string::npos ){
      
      std::cout << legend << std::endl;
      std::cout << legend << "this a mass limit calculation," << std::endl;
      std::cout << legend << "I would add k-factor uncertainty to the nsig uncertainty" << std::endl;
      std::cout << legend << "I would do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << legend << "Not implemented yet " << std::endl;
      std::cout << legend << std::endl;

      //Double_t kfactor_err = GetKfactorUncertainty(peak, mode);

      //double nsig_kappa = ws->var("nsig_kappa_dimuon")->getVal();
      //nsig_kappa = sqrt(nsig_kappa*nsig_kappa + kfactor_err*kfactor_err);
      //ws->var("nsig_kappa_dimuon")->setVal(nsig_kappa);

      //ntoys = 1;
      
    }
    
    else if ( mode.find("expected") != std::string::npos ){
      
      std::cout << legend << std::endl;
      std::cout << legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << legend << "for the expected limit estimate" << std::endl;
      std::cout << legend << std::endl;

      // prepare PE data
      GetPseudoData();

    }
    
    else{ //  "regular" observed limit
      
      std::cout << legend << std::endl;
      std::cout << legend << "calculating an observed limit..." << std::endl;
      std::cout << legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << legend << std::endl;

      GetWorkspaceData("obsData");
    
      //ntoys = 1;
    }

    mcInt = GetMcmcInterval(0.95,        // conf level
			    mcmc_iter,   // number of iterations
			    mcmc_burnin, // number of burn-in to discard
			    0.0,         // left side tail fraction, 0 for upper limit
			    100);        // number of bins in posterior, only for plotting

    ++pe_counter;

    if (!mcInt){
      continue;
    }
    else{
      
      std::string _outfile = "tprime_"+channel+"_xsec_mcmc_limit_" + suffix + ".ascii";
      printMcmcUpperLimit( peak, _outfile );
      
      // limits for averaging/medianing
      RooStats::ModelConfig * pSbModel = GetModelConfig("ModelConfig");
      RooRealVar * firstPOI = (RooRealVar*) pSbModel->GetParametersOfInterest()->first();
      double _limit = mcInt->UpperLimit(*firstPOI);
      _limits.push_back(_limit);
      
    } // end of valid mcInt block
    
  } // end of while

  // write median limit to a file
  if (_limits.size() > 0){
    Double_t _median_limit = TMath::Median(_limits.size(), &_limits[0]);
    std::vector<Double_t> _mass_limit;
    _mass_limit.push_back(peak);
    _mass_limit.push_back(_median_limit);
    std::string _outfile = "tprime_"+channel+"_xsec_mcmc_median_limit_" + suffix + ".ascii";
    PrintToFile(_outfile, _mass_limit, "#  mass         median_limit");
  }

  std::string _outfile = "tprime_"+channel+"_xsec_mcmc_posterior_" + suffix + ".pdf";
  makeMcmcPosteriorPlot( _outfile );

  // timer
  t.Print();

  return 0;
}


  
Int_t Tprime::RunPlr( std::string channel, // ejets, mujets, combined
		      std::string mode,    // observed, expected
		      double peak,         // resonance mass
		      std::string suffix,  // suffix for output file names
		      Int_t ntoys,         // number of pseudoexperiments for expected limit
		      std::string inputdir ){ // input dir name
  //
  // Profile likelihood ratio calculation
  //

  //RooStats::ProfileLikelihoodTestStat::setReuseNLL(kTRUE) ; // activate use of setData()
  //RooStats::ToyMCSampler::setUseMultiGen(kTRUE) ;// activate use of prepareMultiGen() 

  std::string legend = "[tprime::RunPlr()]: ";

  // print out inputs
  std::cout << legend << std::endl;
  std::cout << legend << "Input parameters specified. Some of them are not used and defaults are entered" << std::endl;
  std::cout << legend << "------------------------------" << std::endl;
  std::cout << legend << "channel: " << channel << std::endl;
  std::cout << legend << "mode: " << mode << std::endl;
  std::cout << legend << "input directory: " << inputdir << std::endl;
  std::cout << legend << "resonance peak mass: " << peak << std::endl;
  std::cout << legend << "suffix: ->" << suffix << "<-" << std::endl;
  std::cout << legend << "number of pseudo-experiments: "<< ntoys << std::endl;
  std::cout << legend << std::endl;

  // compose the workspace file name
  char buf[1024];
  sprintf(buf, "%sresults_%04.0f/tprime_%s_tprimeCrossSection_model.root", inputdir.c_str(), peak, channel.c_str());
  std::string _file = buf;
  std::cout << legend << "guessed name of the file with the workspace: >" << _file << "<" << std::endl;

  //load workspace
  LoadWorkspace(_file, channel);

  // fix POI range if needed
  std::cout << std::endl << legend << "fixing POI range to [0-10]" << std::endl;
  pWs->var("xsec")->setRange(0,10);

  // timer
  TStopwatch t;
  t.Start();

  int pe_counter = 0;
  while (pe_counter < ntoys){
    
    // set data
    if (mode.find("observed") != std::string::npos ){

      std::cout << legend << std::endl;
      std::cout << legend << "calculating an observed limit..." << std::endl;
      std::cout << legend << std::endl;

      GetWorkspaceData("obsData");
      ntoys = 1;

      //RooStats::ModelConfig * _mc = GetModaelConfig();
      //_mc->GetPdf()->fitTo(*data, Save());

    }
    else if (mode.find("expected") != std::string::npos ){ // expected interval
      
      std::cout << legend << std::endl;
      std::cout << legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << legend << "for the expected limit estimate" << std::endl;
      std::cout << legend << std::endl;
      
      GetPseudoData();
      //GetWorkspaceData("obsData");
      
    } // end of expected interval 
    
    GetPlrInterval(0.95);
    
    // query intervals
    RooFit::MsgLevel msglevel = RooMsgService::instance().globalKillBelow();
    RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
    RooStats::ModelConfig * _mc = GetModelConfig();
    RooRealVar * _poi = (RooRealVar *)_mc->GetParametersOfInterest()->first();
    Double_t upper_limit = pPlrInt->UpperLimit( *_poi );
    RooMsgService::instance().setGlobalKillBelow(msglevel);

    std::cout << legend << "Profile Likelihood upper limit = " << upper_limit << std::endl; 
      
    std::string _outfile = "tprime_limit_plr_" + channel + "_" + mode + "_" + suffix + ".ascii";
    std::vector<Double_t> results;
    results.push_back(peak);
    results.push_back(upper_limit);
    PrintToFile( _outfile, results, "#          peak    upper_limit" );

    ++pe_counter;
  } // end of while toy loop

  // stop timer and print time used
  t.Stop();
  t.Print();

  // make plot
  TCanvas c1("c1");
  LikelihoodIntervalPlot * pPlrPlot = new LikelihoodIntervalPlot(pPlrInt);
  pPlrPlot->Draw();
  std::string plot_file_name = "tprime_limit_" + channel + "_" + mode + "_" + suffix + ".pdf";
  c1.SaveAs(plot_file_name.c_str());
  delete pPlrPlot;

  return 0;
}



void run_mcmc( std::string channel, std::string mode ){
  //
  // this function implements the interval calculation for the tprime analysis
  //

  Tprime tp;

  //load workspace
  std::string _file = "results/tprime_"+channel+"_tprimeCrossSection_model.root";
  tp.LoadWorkspace(_file, channel);

  // set data
  if (mode.find("observed") != std::string::npos ){
    tp.GetWorkspaceData("obsData");
  }
  else{ // expected interval

  }

  tp.GetMcmcInterval(0.95,
		     10000,
		     5,
		     0,
		     100);

  std::string suffix = "prototype";

  std::string _outfile = channel+"_" + suffix + ".ascii";
  tp.printMcmcUpperLimit( 300.0, _outfile );

  _outfile = channel+"_mcmc_posterior_" + suffix + ".pdf";
  tp.makeMcmcPosteriorPlot( _outfile );
}



void run_frequentist( std::string channel ){
  //
  // this function implements the interval calculation for the tprime analysis
  //

  Tprime tp;

  //load workspace
  std::string _file = "results/tprime_"+channel+"_tprimeCrossSection_model.root";
  tp.LoadWorkspace(_file, channel);

  //RooStats::HypoTestResult * pHypoTestResult = tp.GetFrequentistHypoTest();
  //pHypoTestResult -> Print();
  //delete pHypoTestResult;

  //tp.GetFrequentistLimit();

  return;
}



void inspect( void ){
  //
  // this function implements the interval calculation for the tprime analysis
  //

  Tprime tp;

  //load workspace
  //tp.LoadWorkspace("results/tprime_tprime_tprimeCrossSection_model.root", "tprime");
  tp.LoadWorkspace("results/tprime_combined_tprimeCrossSection_model.root", "combined");

  tp.GetWorkspace()->var("xsec")->Print();
  
}



Int_t limit( std::string channel, // ejets, mujets, combined
	     std::string mode,    // observed, expected
	     std::string method,  // mcmc, plr, cls, asymcls
	     double peak,         // resonance mass
	     std::string suffix,  // suffix for output file names
	     Int_t ntoys,         // number of pseudoexperiments for expected limit
	     Int_t npoints,    // number points for CLs scan
	     Double_t poimin,  // lower scan range
	     Double_t poimax,  // upper scan range
	     Int_t mcmc_iter,     // number of MCMC iterations
	     Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
	     std::string inputdir){ // directory with workspace files
  
  Int_t outcome = 0; // return this, 0 is ok

  std::string legend = "[tprime]: "; // prefix for printouts

  // print out inputs
  std::cout << legend << std::endl;
  std::cout << legend << "Input parameters specified. Some of them are not used and defaults are entered" << std::endl;
  std::cout << legend << "------------------------------" << std::endl;
  std::cout << legend << "channel: " << channel << std::endl;
  std::cout << legend << "mode: " << mode << std::endl;
  std::cout << legend << "method: " << method << std::endl;
  std::cout << legend << "input directory: " << inputdir << std::endl;
  std::cout << legend << "resonance peak mass: " << peak << std::endl;
  std::cout << legend << "suffix: ->" << suffix << "<-" << std::endl;
  std::cout << legend << "number of pseudo-experiments: "<< ntoys << std::endl;
  std::cout << legend << std::endl;
  std::cout << legend << "Bayesian MCMC parameters" << std::endl;
  std::cout << legend << "------------------------------" << std::endl;
  std::cout << legend << "number of iterations: " << mcmc_iter << std::endl;
  std::cout << legend << "number of burn-in steps to discard: " << mcmc_burnin << std::endl;
  std::cout << legend << std::endl;

  Tprime manager; // instance of the class that does all the work

  // CLs calculation
  if (method.find("cls") != std::string::npos ){
    manager.RunCls( channel,
		    mode,
		    peak,
		    suffix,
		    ntoys,
		    method,
		    npoints,
		    poimin,
		    poimax,
		    inputdir );
  }
  // Bayesian MCMC calculation
  if (method.find("mcmc") != std::string::npos ){
    manager.RunMcmc( channel,
		     mode,
		     peak,
		     suffix,
		     ntoys,
		     mcmc_iter,
		     mcmc_burnin,
		     inputdir );
  }
  // Profile likelihood ratio calculation
  
  if (method.find("plr") != std::string::npos ){
    manager.RunPlr( channel,
		    mode,
		    peak,
		    suffix,
		    ntoys,
		    inputdir );
  }
  
  return outcome;
}
