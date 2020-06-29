#include "ptfe_runAction.hh"
#include "ptfe_detectorConstruction.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "g4analysis.hh"


ptfe_runAction::ptfe_runAction() : G4UserRunAction()
{

  // Create the root manager
  auto analysisManager = G4Analysis::ManagerInstance("root");
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  G4cout << "Using " << analysisManager->GetFileName() << G4endl;


  // Default settings
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("ptfe");

  // Create the metadata branch
  analysisManager->CreateNtuple("metadata", "Metadata");              //ntuple-ID 0
  analysisManager->CreateNtupleIColumn("seed_run");
  analysisManager->FinishNtuple();

  // Create the data branch
  analysisManager->CreateNtuple("event_data", "EventData");             //ntuple-ID 1
  analysisManager->CreateNtupleIColumn("event_number");
  analysisManager->CreateNtupleIColumn("seed_event_index");
  analysisManager->CreateNtupleIColumn("seed_event1");
  analysisManager->CreateNtupleIColumn("seed_event2");

  // Create the tracks branch
  analysisManager->CreateNtuple("tracks_data", "TracksData");             //ntuple-ID 2
  analysisManager->CreateNtupleIColumn("event_number");
  analysisManager->CreateNtupleSColumn("particle_name");
  analysisManager->CreateNtupleIColumn("particle_pdg");
  analysisManager->CreateNtupleIColumn("tracks_id");
  analysisManager->CreateNtupleIColumn("parent_id");
  // Coordinates start
  analysisManager->CreateNtupleDColumn("xStart");
  analysisManager->CreateNtupleDColumn("yStart");
  analysisManager->CreateNtupleDColumn("zStart");
  // Momentum start
  analysisManager->CreateNtupleDColumn("pxStart");
  analysisManager->CreateNtupleDColumn("pyStart");
  analysisManager->CreateNtupleDColumn("pzStart");
  // Time-energy start
  analysisManager->CreateNtupleDColumn("eStart");
  analysisManager->CreateNtupleDColumn("kStart");
  analysisManager->CreateNtupleDColumn("tStart");
  // Coordinates end
  analysisManager->CreateNtupleDColumn("xEnd");
  analysisManager->CreateNtupleDColumn("yEnd");
  analysisManager->CreateNtupleDColumn("zEnd");
  // Momentum end
  analysisManager->CreateNtupleDColumn("pxEnd");
  analysisManager->CreateNtupleDColumn("pyEnd");
  analysisManager->CreateNtupleDColumn("pzEnd");
  // Time-energy end
  analysisManager->CreateNtupleDColumn("eEnd");
  analysisManager->CreateNtupleDColumn("kEnd");
  analysisManager->CreateNtupleDColumn("tEnd");
  // Volume information
  analysisManager->CreateNtupleSColumn("volumeStart");
  analysisManager->CreateNtupleSColumn("volumeEnd");
  // Energy deposited
  analysisManager->CreateNtupleDColumn("distTravelled");
  analysisManager->CreateNtupleDColumn("enDeposited_tot");
  // Collection status
  analysisManager->CreateNtupleIColumn("enteredCollection");
  analysisManager->CreateNtupleIColumn("exitedCollection");

  analysisManager->FinishNtuple();
}

ptfe_runAction::~ptfe_runAction()
{}

void ptfe_runAction::BeginOfRunAction(const G4Run*)
{
  // Create ROOT file
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
}     

void ptfe_runAction::EndOfRunAction(const G4Run* run)
{
  // Get the detector (for metadata purposes)
  const ptfe_detectorConstruction* myDetector
      = static_cast<const ptfe_detectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // Save metadata to the ROOT file
  auto analysisManager = G4AnalysisManager::Instance(); G4int i = 0;
  analysisManager->FillNtupleIColumn(0,i,CLHEP::HepRandom::getTheSeed()); i+=1;
  analysisManager->AddNtupleRow(0);
  analysisManager->Write();
  analysisManager->CloseFile();
}