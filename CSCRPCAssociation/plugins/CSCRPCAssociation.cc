// -*- C++ -*-
//
// Package:    CSCRPCAssociation/CSCRPCAssociation
// Class:      CSCRPCAssociation
//
/**\class CSCRPCAssociation CSCRPCAssociation.cc CSCRPCAssociation/CSCRPCAssociation/plugins/CSCRPCAssociation.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mauricio Thiel
//         Created:  Sun, 06 Jun 2021 21:01:33 GMT
//
//

// system include files
#include <memory>

// user include files
// Joao
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include <set>
#include <map>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "DataFormats/CSCRecHit/interface/CSCRangeMapAccessor.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHit.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "Geometry/RPCGeometry/interface/RPCChamber.h"
#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"
#include "Geometry/CSCGeometry/interface/CSCChamberSpecs.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "Geometry/CommonTopologies/interface/RectangularStripTopology.h"
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"
#include "RecoLocalMuon/RPCRecHit/src/CSCStationIndex.h"
#include "RecoLocalMuon/RPCRecHit/src/CSCObjectMap.h"

///Rumi
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
// Joao
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

//using reco::TrackCollection;

class CSCRPCAssociation : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit CSCRPCAssociation(const edm::ParameterSet&);
  ~CSCRPCAssociation();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  // Calling the collections
  edm::EDGetTokenT<RPCRecHitCollection> rpc_token;
  edm::EDGetTokenT<CSCSegmentCollection> cscseg_token;
  edm::Handle<RPCRecHitCollection> pRPCrecHits;
  edm::Handle<CSCSegmentCollection> cscsegments;
///Rumi
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> cscCorrDigis_token;
  edm::Handle<CSCCorrelatedLCTDigiCollection> cscCorrDigis;

// Joao
  edm::EDGetTokenT<RPCDigiCollection> rpcDigis_token;
  edm::Handle<RPCDigiCollection> rpcDigis;
  edm::EDGetTokenT<CSCWireDigiCollection> wiresDigis_token;
  edm::Handle<CSCWireDigiCollection> wiresDigis;
  edm::EDGetTokenT<CSCStripDigiCollection> stripsDigis_token;
  edm::Handle<CSCStripDigiCollection> stripsDigis;

  TH2F *CSCWG_RPCrolls_station4;
  TH2F *CSCWG_RPCrolls_station3;
  TH2F *CSCWG_RPCrolls_station2;
  TH2F *CSCWG_RPCrolls_station1_2;
  TH2F *CSCWG_RPCrolls_station1_3;
  TH2F *CSCWG_etaMidRolls_station4;
  TH2F *CSCWG_etaMidRolls_station3;
  TH2F *CSCWG_etaMidRolls_station2;
  TH2F *CSCWG_etaMidRolls_station1_2;
  TH2F *CSCWG_etaMidRolls_station1_3;

  TH2F *CSCWG_etaMidStrips_station4;
  TH2F *CSCWG_etaMidStrips_station3;
  TH2F *CSCWG_etaMidStrips_station2;
  TH2F *CSCWG_etaMidStrips_station1_2;
  TH2F *CSCWG_etaMidStrips_station1_3;

  TH2F *CSCWGWireDigis_RPCrolls_station4;
  TH2F *CSCWGWireDigis_RPCrolls_station3;
  TH2F *CSCWGWireDigis_RPCrolls_station2;
  TH2F *CSCWGWireDigis_RPCrolls_station1_2;
  TH2F *CSCWGWireDigis_RPCrolls_station1_3;
  TH2F *CSCWGWireDigis_etaMidRolls_station4;
  TH2F *CSCWGWireDigis_etaMidRolls_station3;
  TH2F *CSCWGWireDigis_etaMidRolls_station2;
  TH2F *CSCWGWireDigis_etaMidRolls_station1_2;
  TH2F *CSCWGWireDigis_etaMidRolls_station1_3;
  // One histogram per ring to get the rolls occupancy
  TH1F *Rolls_RE_1_2;
  TH1F *Rolls_RE_1_3;
  TH1F *Rolls_RE_2_2;
  TH1F *Rolls_RE_2_3;
  TH1F *Rolls_RE_3_2;
  TH1F *Rolls_RE_3_3;
  TH1F *Rolls_RE_4_2;
  TH1F *Rolls_RE_4_3;

  TH2F *WireGroups;
  TH2F *KeyWireGroups;

  TH1F *ETA_ME_1_2;
  TH1F *ETA_ME_1_3;
  TH1F *ETA_ME_2_2;
  TH1F *ETA_ME_3_2;
  TH1F *ETA_ME_4_2;

  TH1F *ETAstrip_ME_1_2;
  TH1F *ETAstrip_ME_1_3;
  TH1F *ETAstrip_ME_2_2;
  TH1F *ETAstrip_ME_3_2;
  TH1F *ETAstrip_ME_4_2;

  TH1F * CFEB_CSC_occupancy_station4;
  TH1F * CFEB_CSC_occupancy_station3;
  TH1F * CFEB_CSC_occupancy_station2;
  TH1F * CFEB_CSC_occupancy_station1_3;
  TH1F * CFEB_CSC_occupancy_station1_2;

  TH1F *RPCStrips;

  TH2F *RPCStrips_CFEB_station4_2;
  TH2F *RPCStrips_CFEB_station4_3;
  TH2F *RPCStrips_CFEB_station3_2;
  TH2F *RPCStrips_CFEB_station3_3;
  TH2F *RPCStrips_CFEB_station2_2;
  TH2F *RPCStrips_CFEB_station2_3;
  TH2F *RPCStrips_CFEB_station1_2;
  TH2F *RPCStrips_CFEB_station1_3;

  TH2F *CSCStrips_CFEB_station4;
  TH2F *CSCStrips_CFEB_station3;
  TH2F *CSCStrips_CFEB_station2;
  TH2F *CSCStrips_CFEB_station1_2;
  TH2F *CSCStrips_CFEB_station1_3;

  // ----------member data ---------------------------
//  edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//  edm::ESGetToken<SetupData, SetupRecord> setupToken_;
#endif
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CSCRPCAssociation::CSCRPCAssociation(const edm::ParameterSet& iConfig) // Taking the collections names from the config file
      : rpc_token(consumes<RPCRecHitCollection>(iConfig.getParameter<edm::InputTag>("rpcRecHitTag")))
      , cscseg_token(consumes<CSCSegmentCollection>(iConfig.getParameter<edm::InputTag>("cscSegTag")))
//Rumi
      , cscCorrDigis_token(consumes<CSCCorrelatedLCTDigiCollection>(iConfig.getParameter<edm::InputTag>("cscCorrDigisTag")))
// Joao
      , rpcDigis_token(consumes<RPCDigiCollection>(iConfig.getParameter<edm::InputTag>("rpcDigisTag")))
      , wiresDigis_token(consumes<CSCWireDigiCollection>(iConfig.getParameter<edm::InputTag>("wiresDigisTag")))
      , stripsDigis_token(consumes<CSCStripDigiCollection>(iConfig.getParameter<edm::InputTag>("stripsDigisTag")))
{
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//  setupDataToken_ = esConsumes<SetupData, SetupRecord>();
#endif
  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  CSCWG_RPCrolls_station4 = fs->make<TH2F>("CSCWG_RPCrolls_station4", "CSCWireGroup vs RPCRoll (station = 4); CSCWireGroups ME4/2",66,-1,65,8,0,8);
  CSCWG_RPCrolls_station2 = fs->make<TH2F>("CSCWG_RPCrolls_station2", "CSCWireGroup vs RPCRoll (station = 2); CSCWireGroups ME2/2",66,-1,65,8,0,8);
  CSCWG_RPCrolls_station3 = fs->make<TH2F>("CSCWG_RPCrolls_station3", "CSCWireGroup vs RPCRoll (station = 3); CSCWireGroups ME3/2",66,-1,65,8,0,8);
  CSCWG_RPCrolls_station1_2 = fs->make<TH2F>("CSCWG_RPCrolls_station1_2", "CSCWireGroup vs RPCRoll (station = 1); CSCWireGroups ME1/2",66,-1,65,8,0,8);
  CSCWG_RPCrolls_station1_3 = fs->make<TH2F>("CSCWG_RPCrolls_station1_3", "CSCWireGroup vs RPCRoll (station = 1); CSCWireGroups ME1/3",66,-1,65,8,0,8);
  CSCWG_etaMidRolls_station2 = fs->make<TH2F>("CSCWG_etaMidRolls_station2", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 2); CSCWireGroups ME2/2;|#eta|_{midRPCRoll}",66,-1,65,10,0.9,1.9);
  CSCWG_etaMidRolls_station3 = fs->make<TH2F>("CSCWG_etaMidRolls_station3", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 3); CSCWireGroups ME3/2;|#eta|_{midRPCRoll}",66,-1,65,10,0.9,1.9);
  CSCWG_etaMidRolls_station4 = fs->make<TH2F>("CSCWG_etaMidRolls_station4", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 4); CSCWireGroups ME4/2;|#eta|_{midRPCRoll}",66,-1,65,10,0.9,1.9);
  CSCWG_etaMidRolls_station1_2 = fs->make<TH2F>("CSCWG_etaMidRolls_station1_2", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 1); CSCWireGroups ME1/2;|#eta|_{midRPCRoll}",66,-1,65,10,0.9,1.9);
  CSCWG_etaMidRolls_station1_3 = fs->make<TH2F>("CSCWG_etaMidRolls_station1_3", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 1); CSCWireGroups ME1/3;|#eta|_{midRPCRoll}",66,-1,65,10,0.9,1.9);

  CSCWGWireDigis_RPCrolls_station4 = fs->make<TH2F>("CSCWGWireDigis_RPCrolls_station4", "CSCWireGroup vs RPCRoll (station = 4); CSCWireGroups ME4/2",114,-1,113,8,0,8);
  CSCWGWireDigis_RPCrolls_station2 = fs->make<TH2F>("CSCWGWireDigis_RPCrolls_station2", "CSCWireGroup vs RPCRoll (station = 2); CSCWireGroups ME2/2",114,-1,113,8,0,8);
  CSCWGWireDigis_RPCrolls_station3 = fs->make<TH2F>("CSCWGWireDigis_RPCrolls_station3", "CSCWireGroup vs RPCRoll (station = 3); CSCWireGroups ME3/2",114,-1,113,8,0,8);
  CSCWGWireDigis_RPCrolls_station1_2 = fs->make<TH2F>("CSCWGWireDigis_RPCrolls_station1_2", "CSCWireGroup vs RPCRoll (station = 1); CSCWireGroups ME1/2",114,-1,113,8,0,8);
  CSCWGWireDigis_RPCrolls_station1_3 = fs->make<TH2F>("CSCWGWireDigis_RPCrolls_station1_3", "CSCWireGroup vs RPCRoll (station = 1); CSCWireGroups ME1/3",114,-1,113,8,0,8);
  CSCWGWireDigis_etaMidRolls_station2 = fs->make<TH2F>("CSCWGWireDigis_etaMidRolls_station2", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 2); CSCWireGroups ME2/2;|#eta|_{midRPCRoll}",114,-1,113,10,0.9,1.9);
  CSCWGWireDigis_etaMidRolls_station3 = fs->make<TH2F>("CSCWGWireDigis_etaMidRolls_station3", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 3); CSCWireGroups ME3/2;|#eta|_{midRPCRoll}",114,-1,113,10,0.9,1.9);
  CSCWGWireDigis_etaMidRolls_station4 = fs->make<TH2F>("CSCWGWireDigis_etaMidRolls_station4", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 4); CSCWireGroups ME4/2;|#eta|_{midRPCRoll}",114,-1,113,10,0.9,1.9);
  CSCWGWireDigis_etaMidRolls_station1_2 = fs->make<TH2F>("CSCWGWireDigis_etaMidRolls_station1_2", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 1); CSCWireGroups ME1/2;|#eta|_{midRPCRoll}",114,-1,113,10,0.9,1.9);
  CSCWGWireDigis_etaMidRolls_station1_3 = fs->make<TH2F>("CSCWGWireDigis_etaMidRolls_station1_3", "CSCWireGroup vs |#eta| of middle of RPCRoll (station = 1); CSCWireGroups ME1/3;|#eta|_{midRPCRoll}",114,-1,113,10,0.9,1.9);


  CSCWG_etaMidStrips_station2 = fs->make<TH2F>("CSCWG_etaMidStrips_station2", "CSCWireGroup vs |#eta| of middle of RPC fired strip (station = 2); CSCWireGroups ME2/2;|#eta|_{midRPCStrip}",114,-1,113,10,0.9,1.9);
  CSCWG_etaMidStrips_station3 = fs->make<TH2F>("CSCWG_etaMidStrips_station3", "CSCWireGroup vs |#eta| of middle of RPC fired strip (station = 3); CSCWireGroups ME3/2;|#eta|_{midRPCStrip}",114,-1,113,10,0.9,1.9);
  CSCWG_etaMidStrips_station4 = fs->make<TH2F>("CSCWG_etaMidStrips_station4", "CSCWireGroup vs |#eta| of middle of RPC fired strip (station = 4); CSCWireGroups ME4/2;|#eta|_{midRPCStrip}",114,-1,113,10,0.9,1.9);
  CSCWG_etaMidStrips_station1_2 = fs->make<TH2F>("CSCWG_etaMidStrips_station1_2", "CSCWireGroup vs |#eta| of middle of RPC fired strip (station = 1); CSCWireGroups ME1/2;|#eta|_{midRPCStrip}",114,-1,113,10,0.9,1.9);
  CSCWG_etaMidStrips_station1_3 = fs->make<TH2F>("CSCWG_etaMidStrips_station1_3", "CSCWireGroup vs |#eta| of middle of RPC fired strip (station = 1); CSCWireGroups ME1/3;|#eta|_{midRPCStrip}",114,-1,113,10,0.9,1.9);


  KeyWireGroups = fs->make<TH2F>("KeyWireGroups", "CSCCorrelatedLCTCollection KeyWireGroup; KeyWireGroup",114,-1,113,7,0,7);
  WireGroups = fs->make<TH2F>("WireGroups", "CSCWireDigiCollection WireGroup; WireGroup",114,-1,113,7,0,7);

  RPCStrips = fs->make<TH1F>("RPCStrips", "RPCDigisCollection Strip; Strip",121,-1,120);

  CSCStrips_CFEB_station4 = fs->make<TH2F>("CSCStrips_CFEB_station4", "CSCStripDigisCollection CFEB x CSCStrip ME4/2; CFEB; CSCStrip",7,-1,6,86,-1,85);
  CSCStrips_CFEB_station3 = fs->make<TH2F>("CSCStrips_CFEB_station3", "CSCStripDigisCollection CFEB x CSCStrip ME3/2; CFEB; CSCStrip",7,-1,6,86,-1,85);
  CSCStrips_CFEB_station2 = fs->make<TH2F>("CSCStrips_CFEB_station2", "CSCStripDigisCollection CFEB x CSCStrip ME2/2; CFEB; CSCStrip",7,-1,6,86,-1,85);
  CSCStrips_CFEB_station1_2 = fs->make<TH2F>("CSCStrips_CFEB_station1_2", "CSCStripDigisCollection CFEB x CSCStrip ME1/2; CFEB; CSCStrip",7,-1,6,86,-1,85);
  CSCStrips_CFEB_station1_3 = fs->make<TH2F>("CSCStrips_CFEB_station1_3", "CSCStripDigisCollection CFEB x CSCStrip ME1/3; CFEB; CSCStrip",7,-1,6,86,-1,85);

  RPCStrips_CFEB_station4_2 = fs->make<TH2F>("RPCStrips_CFEB_station4_2", "RPCStripDigisCollection CFEB x RPCStrip RE4/2; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station4_3 = fs->make<TH2F>("RPCStrips_CFEB_station4_3", "RPCStripDigisCollection CFEB x RPCStrip RE4/3; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station3_2 = fs->make<TH2F>("RPCStrips_CFEB_station3_2", "RPCStripDigisCollection CFEB x RPCStrip RE3/2; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station3_3 = fs->make<TH2F>("RPCStrips_CFEB_station3_3", "RPCStripDigisCollection CFEB x RPCStrip RE3/3; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station2_2 = fs->make<TH2F>("RPCStrips_CFEB_station2_2", "RPCStripDigisCollection CFEB x RPCStrip RE2/2; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station2_3 = fs->make<TH2F>("RPCStrips_CFEB_station2_3", "RPCStripDigisCollection CFEB x RPCStrip RE2/3; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station1_2 = fs->make<TH2F>("RPCStrips_CFEB_station1_2", "RPCStripDigisCollection CFEB x RPCStrip RE1/2; CFEB; RPCStrip",7,-1,6,37,-1,36);
  RPCStrips_CFEB_station1_3 = fs->make<TH2F>("RPCStrips_CFEB_station1_3", "RPCStripDigisCollection CFEB x RPCStrip RE1/3; CFEB; RPCStrip",7,-1,6,37,-1,36);
 
  CFEB_CSC_occupancy_station4 = fs->make<TH1F>("CFEB_CSC_occupancy_station4", "CFEB CSC Occupancy ME4/2; CFEB", 7, -1, 6);
  CFEB_CSC_occupancy_station3 = fs->make<TH1F>("CFEB_CSC_occupancy_station3", "CFEB CSC Occupancy ME3/2; CFEB", 7, -1, 6);
  CFEB_CSC_occupancy_station2 = fs->make<TH1F>("CFEB_CSC_occupancy_station2", "CFEB CSC Occupancy ME2/2; CFEB", 7, -1, 6);
  CFEB_CSC_occupancy_station1_2 = fs->make<TH1F>("CFEB_CSC_occupancy_station1_2", "CFEB CSC Occupancy ME1/2; CFEB", 7, -1, 6);
  CFEB_CSC_occupancy_station1_3 = fs->make<TH1F>("CFEB_CSC_occupancy_station1_3", "CFEB CSC Occupancy ME1/3; CFEB", 7, -1, 6);

  KeyWireGroups->GetYaxis()->SetBinLabel(7," ");
  KeyWireGroups->GetYaxis()->SetBinLabel(6,"ME4/2");
  KeyWireGroups->GetYaxis()->SetBinLabel(5,"ME3/2");
  KeyWireGroups->GetYaxis()->SetBinLabel(4,"ME2/2");
  KeyWireGroups->GetYaxis()->SetBinLabel(3,"ME1/3");
  KeyWireGroups->GetYaxis()->SetBinLabel(2,"ME1/2");
  KeyWireGroups->GetYaxis()->SetBinLabel(1," ");

  WireGroups->GetYaxis()->SetBinLabel(7," ");
  WireGroups->GetYaxis()->SetBinLabel(6,"ME4/2");
  WireGroups->GetYaxis()->SetBinLabel(5,"ME3/2");
  WireGroups->GetYaxis()->SetBinLabel(4,"ME2/2");
  WireGroups->GetYaxis()->SetBinLabel(3,"ME1/3");
  WireGroups->GetYaxis()->SetBinLabel(2,"ME1/2");
  WireGroups->GetYaxis()->SetBinLabel(1," ");

  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(8," ");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(7,"RE4/3 roll A");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(6,"RE4/3 roll B");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(5,"RE4/3 roll C");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(4,"RE4/2 roll A");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(3,"RE4/2 roll B");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(2,"RE4/2 roll C");
  CSCWG_RPCrolls_station4->GetYaxis()->SetBinLabel(1," ");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(8," ");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(7,"RE3/3 roll A");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(6,"RE3/3 roll B");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(5,"RE3/3 roll C");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(4,"RE3/2 roll A");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(3,"RE3/2 roll B");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(2,"RE3/2 roll C");
  CSCWG_RPCrolls_station3->GetYaxis()->SetBinLabel(1," ");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(8," ");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(7,"RE2/3 roll A");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(6,"RE2/3 roll B");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(5,"RE2/3 roll C");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(4,"RE2/2 roll A");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(3,"RE2/2 roll B");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(2,"RE2/2 roll C");
  CSCWG_RPCrolls_station2->GetYaxis()->SetBinLabel(1," ");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(8," ");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(7,"RE1/3 roll A");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(6,"RE1/3 roll B");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(5,"RE1/3 roll C");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(4,"RE1/2 roll A");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(3,"RE1/2 roll B");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(2,"RE1/2 roll C");
  CSCWG_RPCrolls_station1_2->GetYaxis()->SetBinLabel(1," "); 
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(8," ");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(7,"RE1/3 roll A");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(6,"RE1/3 roll B");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(5,"RE1/3 roll C");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(4,"RE1/2 roll A");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(3,"RE1/2 roll B");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(2,"RE1/2 roll C");
  CSCWG_RPCrolls_station1_3->GetYaxis()->SetBinLabel(1," ");

  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(8," ");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(7,"RE4/3 roll A");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(6,"RE4/3 roll B");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(5,"RE4/3 roll C");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(4,"RE4/2 roll A");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(3,"RE4/2 roll B");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(2,"RE4/2 roll C");
  CSCWGWireDigis_RPCrolls_station4->GetYaxis()->SetBinLabel(1," ");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(8," ");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(7,"RE3/3 roll A");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(6,"RE3/3 roll B");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(5,"RE3/3 roll C");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(4,"RE3/2 roll A");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(3,"RE3/2 roll B");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(2,"RE3/2 roll C");
  CSCWGWireDigis_RPCrolls_station3->GetYaxis()->SetBinLabel(1," ");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(8," ");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(7,"RE2/3 roll A");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(6,"RE2/3 roll B");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(5,"RE2/3 roll C");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(4,"RE2/2 roll A");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(3,"RE2/2 roll B");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(2,"RE2/2 roll C");
  CSCWGWireDigis_RPCrolls_station2->GetYaxis()->SetBinLabel(1," ");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(8," ");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(7,"RE1/3 roll A");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(6,"RE1/3 roll B");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(5,"RE1/3 roll C");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(4,"RE1/2 roll A");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(3,"RE1/2 roll B");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(2,"RE1/2 roll C");
  CSCWGWireDigis_RPCrolls_station1_2->GetYaxis()->SetBinLabel(1," ");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(8," ");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(7,"RE1/3 roll A");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(6,"RE1/3 roll B");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(5,"RE1/3 roll C");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(4,"RE1/2 roll A");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(3,"RE1/2 roll B");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(2,"RE1/2 roll C");
  CSCWGWireDigis_RPCrolls_station1_3->GetYaxis()->SetBinLabel(1," ");

  // Rolls hists
  Rolls_RE_1_2 = fs->make<TH1F>("Rolls_RE_1_2", "Rolls occupancy RE1/2; Rolls; Entries",3,1,4);
  Rolls_RE_1_3 = fs->make<TH1F>("Rolls_RE_1_3", "Rolls occupancy RE1/3; Rolls; Entries",3,1,4);
  Rolls_RE_2_2 = fs->make<TH1F>("Rolls_RE_2_2", "Rolls occupancy RE2/2; Rolls; Entries",3,1,4);
  Rolls_RE_2_3 = fs->make<TH1F>("Rolls_RE_2_3", "Rolls occupancy RE2/3; Rolls; Entries",3,1,4);
  Rolls_RE_3_2 = fs->make<TH1F>("Rolls_RE_3_2", "Rolls occupancy RE3/2; Rolls; Entries",3,1,4);
  Rolls_RE_3_3 = fs->make<TH1F>("Rolls_RE_3_3", "Rolls occupancy RE3/3; Rolls; Entries",3,1,4);
  Rolls_RE_4_2 = fs->make<TH1F>("Rolls_RE_4_2", "Rolls occupancy RE4/2; Rolls; Entries",3,1,4);
  Rolls_RE_4_3 = fs->make<TH1F>("Rolls_RE_4_3", "Rolls occupancy RE4/3; Rolls; Entries",3,1,4);
 
  // ETA for ME normalization
  ETA_ME_1_2 = fs->make<TH1F>("ETA_ME_1_2", "ETA of RE1/2; |#eta|; Entries",10,0.9,1.9);
  ETA_ME_1_3 = fs->make<TH1F>("ETA_ME_1_3", "ETA of RE1/3; |#eta|; Entries",10,0.9,1.9);
  ETA_ME_2_2 = fs->make<TH1F>("ETA_ME_2_2", "ETA of RE2/2; |#eta|; Entries",10,0.9,1.9);
  ETA_ME_3_2 = fs->make<TH1F>("ETA_ME_3_2", "ETA of RE3/2; |#eta|; Entries",10,0.9,1.9);
  ETA_ME_4_2 = fs->make<TH1F>("ETA_ME_4_2", "ETA of RE4/2; |#eta|; Entries",10,0.9,1.9);

  // ETA for strip eta normalization
  ETAstrip_ME_1_2 = fs->make<TH1F>("ETAstrip_ME_1_2", "ETA of RE1/2; |#eta|; Entries",10,0.9,1.9);
  ETAstrip_ME_1_3 = fs->make<TH1F>("ETAstrip_ME_1_3", "ETA of RE1/3; |#eta|; Entries",10,0.9,1.9);
  ETAstrip_ME_2_2 = fs->make<TH1F>("ETAstrip_ME_2_2", "ETA of RE2/2; |#eta|; Entries",10,0.9,1.9);
  ETAstrip_ME_3_2 = fs->make<TH1F>("ETAstrip_ME_3_2", "ETA of RE3/2; |#eta|; Entries",10,0.9,1.9);
  ETAstrip_ME_4_2 = fs->make<TH1F>("ETAstrip_ME_4_2", "ETA of RE4/2; |#eta|; Entries",10,0.9,1.9);

}

CSCRPCAssociation::~CSCRPCAssociation() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void CSCRPCAssociation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	using namespace edm;
	
	// Joao
	edm::ESHandle<RPCGeometry> rpcGeo;
	edm::ESHandle<CSCGeometry> cscGeo;

	iSetup.get<MuonGeometryRecord>().get(rpcGeo);
	iSetup.get<MuonGeometryRecord>().get(cscGeo);

	iEvent.getByToken(cscCorrDigis_token, cscCorrDigis);
	iEvent.getByToken(rpc_token, pRPCrecHits);
	iEvent.getByToken(cscseg_token, cscsegments);
	iEvent.getByToken(rpcDigis_token, rpcDigis);
        iEvent.getByToken(wiresDigis_token, wiresDigis);
        iEvent.getByToken(stripsDigis_token, stripsDigis);

	RPCDigiCollection::DigiRangeIterator detUnitIt;
	// Loop on rpcDigis
	for (detUnitIt = rpcDigis->begin(); detUnitIt != rpcDigis->end(); ++detUnitIt) {
	    const RPCDetId id = (*detUnitIt).first; // RPCDetId: set of RPC Digis
            if (id.region()==0)continue; // Exclude barrel region
	    if (dynamic_cast<const RPCRoll*>(rpcGeo->roll(id)) == 0) continue; //joao inserted this if to not crash
            const RPCRoll* roll = dynamic_cast<const RPCRoll*>(rpcGeo->roll(id)); // get roll, if it is not 0
            const RPCDigiCollection::Range range = (*detUnitIt).second; // RPCDigi (pair<bx window, fired strip number>)
            const BoundPlane & RPCSurface = roll->surface();
	    int digisInRoll = 0;
	    // Loop on bx window
            for (RPCDigiCollection::const_iterator digiIt = range.first; digiIt != range.second; ++digiIt) {
	      // Loop on fired strips
  	      for (RPCDigiCollection::const_iterator digiIts = range.second; digiIts != range.first; --digiIts) {
                if (digiIt->strip() < 1 || digiIt->strip() > roll->nstrips()) std::cout << " XXXXXXXXXXXXX Problemt with " << id << std::endl;
                int strip= (*digiIt).strip(); // get strip from global hit
                int strip_= (*digiIts).strip(); // get strip from local hit (smeared values)
                const TrapezoidalStripTopology* top_= dynamic_cast<const TrapezoidalStripTopology*> (&((roll)->topology()));
                GlobalPoint CenterPointRollGlobal = RPCSurface.toGlobal(top_->localPosition(strip));
                GlobalPoint CenterPointRollGlobal_ = RPCSurface.toGlobal(top_->localPosition(strip_));
                float etaMiddleStrip = abs(CenterPointRollGlobal_.eta()); // eta of the middle of the strip
		float etaMiddleRoll = abs(CenterPointRollGlobal.eta());   // eta of the middle of the roll
	        int bxRPC=(*digiIt).bx(); // get bx
		//std::cout << "strip(): " << digiIt->strip() << std::endl;
	        if (bxRPC!=0) continue; // require central bx
	        digisInRoll++;
	        // Get the region, station, roll, chamber, ring, sector and layer info
		int regionRPC = id.region();
	        int stationRPC = id.station();	
	        RPCGeomServ rpcsrv(id);
	        int rollRPC = id.roll();
	        int chamberRPC = rpcsrv.segment();
                int ringRPC = id.ring();
		if (ringRPC==1) continue;
                int sectorRPC = id.sector();
		int layerRPC = id.layer();
		// Exclude iRPCs ring
                if (stationRPC==3 && ringRPC==1) continue;
                if (stationRPC==4 && ringRPC==1) continue;
                int nrollRPC=0;      
		// This is a change of convention of rolls enumeration to be easier of drawing histos
                if (rollRPC==1) nrollRPC=3;
                if (rollRPC==2) nrollRPC=2;
                if (rollRPC==3) nrollRPC=1;
	 	RPCStrips->Fill(strip);
		// Loop on CSCCorrelatedLCTDigis collection
                for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc = cscCorrDigis.product()->begin();
                    csc != cscCorrDigis.product()->end();
                    csc++) {
                    CSCCorrelatedLCTDigiCollection::Range myCscRange = cscCorrDigis.product()->get((*csc).first);
                    int brk=0;
		    for (CSCCorrelatedLCTDigiCollection::const_iterator lct = myCscRange.first; 
			lct != myCscRange.second; 
			lct++) {//loop on bx
			// Get the id, endcap, station, sector, ring, cscId, chamber, layer, keywiregroup
			const CSCDetId idCSC = (*csc).first;
                        int endcap = idCSC.endcap();
                        if (endcap==2) endcap=-1;
                        int station = idCSC.station();
                        if (station==0) continue;
		        int sector = idCSC.triggerSector();
                        int ring = idCSC.ring();
		        if (ring==1) continue;
                	int cscId = idCSC.triggerCscId();
                	int bx = lct->getBX();
		  	bx = bx-8; // Remember that on CSCCorrLCTDigis collection, the central bx is 8
                	if (bx!=0) continue;
		        int bxData = lct->getBXData();
                	int bx0 = lct->getBX0();
                	int chamberId =  idCSC.chamberId();
                	int layer = idCSC.layer();
                	int chamber = idCSC.chamber();
			int wiregroup = lct->getKeyWG();
			if (endcap!=regionRPC || station!=stationRPC || chamber != chamberRPC || bx!=bxRPC) continue; //preliminary matching
			// exclude the ring we do not will use, like ring 1
			if (station==4 || station==3 || station==2) {
			   int varing = ring+1; 
			   if (ringRPC!=ring && ringRPC!=varing) continue;
			}
        		if (station==1 && ringRPC!=ring) continue;
			// Get the keywiregroups
			if (station==1 && ring==2) KeyWireGroups->Fill(wiregroup,1);
                        if (station==1 && ring==3) KeyWireGroups->Fill(wiregroup,2);
                        if (station==2 && ring==2) KeyWireGroups->Fill(wiregroup,3);
                        if (station==3 && ring==2) KeyWireGroups->Fill(wiregroup,4);
                        if (station==4 && ring==2) KeyWireGroups->Fill(wiregroup,5);
		        //std::cout << "CSCCorrLCT INFO: "<< "cscEndCap: " << endcap << "\tcscStation: " << station << "\tcscSector: " << sector << "\tcscChamber: " << chamber << "\tcscLayer: " << layer<< "\tcscRing: " << ring << "wiregroup: " << wiregroup  <<  std::endl; 
			//std::cout << "RPCDigiCol INFO: "<< "rpcRegion: " << regionRPC << "\trpcStation: " << stationRPC << "\trpcSector: " << sectorRPC << "\trpcChamber: " << chamberRPC << "\trpcLayer: " << layerRPC << "\trpcRing: " << ringRPC << "\troll: " << rollRPC << std::endl;
			// Fill the correlation of RPC and CSC chambers 
			if (station==4 && ring==2 && ringRPC==2) {
                           CSCWG_RPCrolls_station4->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWG_etaMidRolls_station4->Fill(wiregroup,etaMiddleRoll);
                           CSCWG_etaMidStrips_station4->Fill(wiregroup,etaMiddleStrip);
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
                        }
                        if (station==4 && ring==2 && ringRPC==3) {
                           CSCWG_RPCrolls_station4->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWG_etaMidRolls_station4->Fill(wiregroup,etaMiddleRoll);
                           CSCWG_etaMidStrips_station4->Fill(wiregroup,etaMiddleStrip);
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
                        }
			if (station==3 && ring==2 && ringRPC==2) {
			   CSCWG_RPCrolls_station3->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
 			   CSCWG_etaMidRolls_station3->Fill(wiregroup,etaMiddleRoll);            
                           CSCWG_etaMidStrips_station3->Fill(wiregroup,etaMiddleStrip);           
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
			}
                        if (station==3 && ring==2 && ringRPC==3) {
                           CSCWG_RPCrolls_station3->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWG_etaMidRolls_station3->Fill(wiregroup,etaMiddleRoll);
                           CSCWG_etaMidStrips_station3->Fill(wiregroup,etaMiddleStrip);
	   	           //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
                        }
                        if (station==2 && ring==2 && ringRPC==2) {
                           CSCWG_RPCrolls_station2->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWG_etaMidRolls_station2->Fill(wiregroup,etaMiddleRoll);
                           CSCWG_etaMidStrips_station2->Fill(wiregroup,etaMiddleStrip);
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
                        }
                        if (station==2 && ring==2 && ringRPC==3) {
			   CSCWG_RPCrolls_station2->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWG_etaMidRolls_station2->Fill(wiregroup,etaMiddleRoll);
			   CSCWG_etaMidStrips_station2->Fill(wiregroup,etaMiddleStrip);
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
			}
                        if (station==1 && ring==2 && ringRPC==2) {
			   CSCWG_RPCrolls_station1_2->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
			   CSCWG_etaMidRolls_station1_2->Fill(wiregroup,etaMiddleRoll);
                           CSCWG_etaMidStrips_station1_2->Fill(wiregroup,etaMiddleStrip);
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
			}
			if (station==1 && ring==3 && ringRPC==3) {
			   CSCWG_RPCrolls_station1_3->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
			   CSCWG_etaMidRolls_station1_3->Fill(wiregroup,etaMiddleRoll);
                           CSCWG_etaMidStrips_station1_3->Fill(wiregroup,etaMiddleStrip);
			   //std::cout << "station: " << station << " ring: " << ringRPC << " roll: " << rollRPC << " eta: " << etaMiddleRoll << std::endl;
			}
			//std::cout << "RPCDigiCol INFO: "<< "rpcRegion: " << regionRPC << "\trpcStation: " << stationRPC << "\trpcSector: " << sectorRPC << "\trpcChamber: " << chamberRPC << "\trpcLayer: " << layerRPC << "\trpcRing: " << ringRPC << "\troll: " << rollRPC << std::endl;
			//std::cout << "CSCCorrLCT INFO: "<< "cscEndCap: " << endcap << "\tcscStation: " << station << "\tcscSector: " << sector << "\tcscChamber: " << chamber << "\tcscLayer: " << layer<< "\tcscRing: " << ring << "wiregroup: " << wiregroup  <<  std::endl;
	  	   brk=1;break;  
		   }
	      if (brk==1) break;
	      }
	      // Loop on strips of CSC to check the matching of CFEBs of CSC and RPC strips
              for (CSCStripDigiCollection::DigiRangeIterator strips=stripsDigis->begin(); strips!=stripsDigis->end(); strips++) {
		   CSCDetId id = (CSCDetId)(*strips).first;
		   // get the endcap, ring, station and chamber info
                   int kEndcap  = id.endcap();
                   int kRing    = id.ring();
                   int kStation = id.station();
                   int kChamber = id.chamber();
                   if (kEndcap==0) continue; // exclude barrel region
                   if (kRing==1) continue;   // exclude ring 1 of CSC
                   if (kEndcap==2) kEndcap=-1; // same convention of RPC
                   std::vector<CSCStripDigi>::const_iterator stripIt = (*strips).second.first;
                   std::vector<CSCStripDigi>::const_iterator lastStrip = (*strips).second.second;
		   int brk =0;
		   for ( ; stripIt != lastStrip; ++stripIt){
                        int CSCstrip = (*stripIt).getStrip();// Get the strip number. counts from 1.
                        int CFEB  = (*stripIt).getCFEB();// Get the CFEB number. Counts from 0.
			//int bx    = stripIt->getBX();
			// Preliminary matching
			if (kEndcap!=regionRPC || kStation!=stationRPC || kChamber != chamberRPC) continue;
                        if (kStation==4 || kStation==3 || kStation==2) {
			   int varkRing=kRing+1; 
			   if (varkRing!=ringRPC && kRing!=ringRPC) continue;
			}
                        if (kStation==1 && ringRPC!=kRing) continue;
			// Fill the correlation of CFEBs and RPC strips
			if (kStation==1 && kRing==2 && ringRPC==2) {
			   CFEB_CSC_occupancy_station1_2->Fill(CFEB); 
			   CSCStrips_CFEB_station1_2->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station1_2->Fill(CFEB,strip);
			}
                        if (kStation==1 && kRing==3 && ringRPC==3) {
			   CFEB_CSC_occupancy_station1_3->Fill(CFEB); 
			   CSCStrips_CFEB_station1_3->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station1_3->Fill(CFEB,strip);
			}
                        if (kStation==2 && kRing==2 && ringRPC==2) {
			   CFEB_CSC_occupancy_station2->Fill(CFEB); 
			   CSCStrips_CFEB_station2->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station2_2->Fill(CFEB,strip);
			}
                        if (kStation==2 && kRing==2 && ringRPC==3) {
			   CFEB_CSC_occupancy_station2->Fill(CFEB); 
			   CSCStrips_CFEB_station2->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station2_3->Fill(CFEB,strip);
			}
                        if (kStation==3 && kRing==2 && ringRPC==2) {
			   CFEB_CSC_occupancy_station3->Fill(CFEB); 
			   CSCStrips_CFEB_station3->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station3_2->Fill(CFEB,strip);
			}
                        if (kStation==3 && kRing==2 && ringRPC==3) {
			   CFEB_CSC_occupancy_station3->Fill(CFEB); 
			   CSCStrips_CFEB_station3->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station3_3->Fill(CFEB,strip);
			}
                        if (kStation==4 && kRing==2 && ringRPC==2) {
			   CFEB_CSC_occupancy_station4->Fill(CFEB); 
			   CSCStrips_CFEB_station4->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station4_2->Fill(CFEB,strip);
			}
                        if (kStation==4 && kRing==2 && ringRPC==3) {
			   CFEB_CSC_occupancy_station4->Fill(CFEB); 
			   CSCStrips_CFEB_station4->Fill(CFEB,CSCstrip);
			   RPCStrips_CFEB_station4_3->Fill(CFEB,strip);
			}
//		        std::cout <<" RPCstrip: " << strip << " RPCstrip_: " << strip_ << " CSCstrip: " << CSCstrip << " CFEB: " << CFEB << std::endl;
		   //brk=1;break;   
		   } 
		//if (brk==1) break;
	        }

//              std::cout<<"---------------------"<< std::endl;	
                // Loop on wires of CSC to check the compatibility with key wire groups
		for (CSCWireDigiCollection::DigiRangeIterator wi=wiresDigis->begin(); wi!=wiresDigis->end(); wi++) {
                   CSCDetId id = (CSCDetId)(*wi).first;
		   // get endcap, ring, station, chamber
                   int kEndcap  = id.endcap();
                   int kRing    = id.ring();
                   int kStation = id.station();
                   int kChamber = id.chamber();
                   if (kEndcap==0) continue; // exclude barrel
		   if (kRing==1) continue;   // exclude ring 1 of CSC
		   if (kEndcap==2) kEndcap=-1;  // change for RPC convention
		   std::vector<CSCWireDigi>::const_iterator wireIt = (*wi).second.first;
      		   std::vector<CSCWireDigi>::const_iterator lastWire = (*wi).second.second;
		   int brk=0;
		   // Loop on wires
                   for ( ; wireIt != lastWire; ++wireIt){
                        int wiregroup = (*wireIt).getWireGroup(); // get wire groups
                   	int bxCSCWG = wireIt->getWireGroupBX();   // get bx
			if (bxCSCWG!=0) continue;     
			// Fill the wire groups
			if (kStation==1 && kRing==2) WireGroups->Fill(wiregroup,1);
			if (kStation==1 && kRing==3) WireGroups->Fill(wiregroup,2);
                        if (kStation==2 && kRing==2) WireGroups->Fill(wiregroup,3);
                        if (kStation==3 && kRing==2) WireGroups->Fill(wiregroup,4);
                        if (kStation==4 && kRing==2) WireGroups->Fill(wiregroup,5);
			// Preliminary matching
			if (kEndcap!=regionRPC && kStation!=stationRPC && kChamber!= chamberRPC && bxCSCWG!=bxRPC) continue;
			if (kStation==4 || kStation==3 || kStation==2) {
			   int varkRing=kRing+1; 
			   if (varkRing!=ringRPC && kRing!=ringRPC) continue;
			}
			if (kStation==1 && ringRPC!=kRing) continue;
			// Fill the correlation
                        if (kStation==4 && kRing==2 && ringRPC==2) {
                           CSCWGWireDigis_RPCrolls_station4->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station4->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==4 && kRing==2 && ringRPC==3) {
                           CSCWGWireDigis_RPCrolls_station4->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station4->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==3 && kRing==2 && ringRPC==2) {
                           CSCWGWireDigis_RPCrolls_station3->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station3->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==3 && kRing==2 && ringRPC==3) {
                           CSCWGWireDigis_RPCrolls_station3->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station3->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==2 && kRing==2 && ringRPC==2) {
                           CSCWGWireDigis_RPCrolls_station2->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station2->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==2 && kRing==2 && ringRPC==3) {
                           CSCWGWireDigis_RPCrolls_station2->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station2->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==1 && kRing==2 && ringRPC==2) {
                           CSCWGWireDigis_RPCrolls_station1_2->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station1_2->Fill(wiregroup,etaMiddleRoll);
                        }
                        if (kStation==1 && kRing==3 && ringRPC==3) {
                           CSCWGWireDigis_RPCrolls_station1_3->Fill(wiregroup,(ringRPC-2)*3+nrollRPC);
                           CSCWGWireDigis_etaMidRolls_station1_3->Fill(wiregroup,etaMiddleRoll);
                        }
                    brk=1;
                    break;   
                    }
	        if (brk==1) break;
                }
                // Get the occupancy of rolls and eta regions                                                         	      
        	if (stationRPC==1 && ringRPC==2) {
		   Rolls_RE_1_2->Fill(nrollRPC);
		   ETA_ME_1_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_1_2->Fill(etaMiddleStrip);
		}
		if (stationRPC==1 && ringRPC==3) {
		   Rolls_RE_1_3->Fill(nrollRPC);
		   ETA_ME_1_3->Fill(etaMiddleRoll);
		   ETAstrip_ME_1_3->Fill(etaMiddleStrip);
		}
		if (stationRPC==2 && ringRPC==2) {
		   Rolls_RE_2_2->Fill(nrollRPC);
		   ETA_ME_2_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_2_2->Fill(etaMiddleStrip);
		}
		if (stationRPC==2 && ringRPC==3) {
		   Rolls_RE_2_3->Fill(nrollRPC);
		   ETA_ME_2_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_2_2->Fill(etaMiddleStrip);
		}
		if (stationRPC==3 && ringRPC==2) {
		   Rolls_RE_3_2->Fill(nrollRPC);
		   ETA_ME_3_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_3_2->Fill(etaMiddleStrip);
		}
		if (stationRPC==3 && ringRPC==3) {
		   Rolls_RE_3_3->Fill(nrollRPC);
		   ETA_ME_3_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_3_2->Fill(etaMiddleStrip);
		}
		if (stationRPC==4 && ringRPC==2) {
		   Rolls_RE_4_2->Fill(nrollRPC);
		   ETA_ME_4_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_4_2->Fill(etaMiddleStrip);
		}
		if (stationRPC==4 && ringRPC==3) {
		   Rolls_RE_4_3->Fill(nrollRPC);
		   ETA_ME_4_2->Fill(etaMiddleRoll);
		   ETAstrip_ME_4_2->Fill(etaMiddleStrip);
		}
	     }
          } 
    }

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  // if the SetupData is always needed
  // auto setup = iSetup.getData(setupToken_);
  // if need the ESHandle to check if the SetupData was there or not
  // auto pSetup = iSetup.getHandle(setupToken_);
#endif
}

// ------------ method called once each job just before starting event loop  ------------
void CSCRPCAssociation::beginJob() {
  // please remove this method if not needed
}

// ------------ method called once each job just after ending the event loop  ------------
void CSCRPCAssociation::endJob() {
  // please remove this method if not needed
  std::cout << "FINISHED" << std::endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void CSCRPCAssociation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCRPCAssociation);
