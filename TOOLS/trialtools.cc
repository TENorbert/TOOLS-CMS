// Working Tool to develop Ideas
//
//
//*********Anomalous Signal Selection**********//

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/DetId/interface/DetId.h"

bool DPntumakerAlyzer::ASignalSCRejecter( reco::SuperClusterRef scref, Handle<EcalRecHitCollection> recHitsEB, Handle<EcalRecHitCollection> recHitsEE, Handle<reco::TrackCollection> tracks ) {

// poorly calib xtals
  int nkpoor = 0 ;
  int nL1flag = 0 ;
  int nswissX = 0 ;

  int nxtal  = 0 ;  // number of BC in SC

// Loop over all Photons
//for(reco::PhotonCollection::const_iterator pho = photons->begin(); pho != photons->end(); pho++) {

// pixel veto
//if ( it->hasPixelSeed() ) continue ;

// Grab Photon SC
//reco::SuperClusterRef SCref = it->superCluster() ;

// If wanted Grab Seed SC
//reco::CaloClusterPtr SCseed = it->superCluster()->seed() ;

// Loop Over all SCref
for ( reco::CaloCluster_iterator clusref = scref->clustersBegin(); clusref  != scref->clustersEnd(); ++clusref) {

// Get only seed BC in SC
     if ( *clusref != scref->seed()  )  continue ;

     reco::BasicCluster seedBC = scref->seed() ;
      
      // get nxtals in SeedBC
       nxtal = seedBC->size() ;
      
      // get Rechits of BC by DetId
       std::vector<DetId> BCrechit = seedBC->getHitsbyDetId() ;
       
	   // Grab Rechits
           const EcalRecHitCollection* rechits = ( it->isEB()) ? recHitsEB.product() : recHitsEE.product() ;

