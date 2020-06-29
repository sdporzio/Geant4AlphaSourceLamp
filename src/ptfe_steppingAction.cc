#include "ptfe_steppingAction.hh"
#include "ptfe_eventAction.hh"
#include "ptfe_detectorConstruction.hh"
#include "ptfe_anaTrack.hh"


#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4StepPoint.hh"
#include "globals.hh"


ptfe_steppingAction::ptfe_steppingAction(ptfe_eventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

ptfe_steppingAction::~ptfe_steppingAction()
{}

void ptfe_steppingAction::UserSteppingAction(const G4Step* step)
{
  // Get the volume associated with the step
  G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
  // Get the track associated with the step
  G4Track* track = step->GetTrack();
  G4int track_id = track->GetTrackID();
  G4int parent_id = track->GetParentID();
  G4String particle_name = track->GetParticleDefinition()->GetParticleName();
  // Take also prepoint and postpoint for good measure
  G4StepPoint* prepoint = step->GetPreStepPoint();
  G4StepPoint* postpoint = step->GetPostStepPoint();

  // We're only interested in stuff that makes it to the collection volume
    G4double edep = step->GetTotalEnergyDeposit()/CLHEP::keV;

    // Check if the anaTrack class has ever seen this track before
    // If it doesn't exist yet, create it and initialise it
  if((fEventAction->fAnaTrack.count(track_id))==0)
  {
    ptfe_anaTrack thisTrack;
    fEventAction->fAnaTrack[track_id] = thisTrack;
    fEventAction->fAnaTrack[track_id].trackId = track_id;
    fEventAction->fAnaTrack[track_id].enteredCollection = false;
    fEventAction->fAnaTrack[track_id].exitedCollection = false;

    fEventAction->fAnaTrack[track_id].name = track->GetParticleDefinition()->GetParticleName();
    fEventAction->fAnaTrack[track_id].pdg = track->GetParticleDefinition()->GetPDGEncoding();
    fEventAction->fAnaTrack[track_id].parentId = track->GetParentID();
    // GET STARTING QUANTITIES
    // Get first point coordinates
    fEventAction->fAnaTrack[track_id].xStart = prepoint->GetPosition()[0]/CLHEP::mm;
    fEventAction->fAnaTrack[track_id].yStart = prepoint->GetPosition()[1]/CLHEP::mm;
    fEventAction->fAnaTrack[track_id].zStart = prepoint->GetPosition()[2]/CLHEP::mm;
    fEventAction->fAnaTrack[track_id].tStart = prepoint->GetLocalTime()/CLHEP::ns;
    // Get first point momenta
    fEventAction->fAnaTrack[track_id].pxStart = prepoint->GetMomentum()[0];
    fEventAction->fAnaTrack[track_id].pyStart = prepoint->GetMomentum()[1];
    fEventAction->fAnaTrack[track_id].pzStart = prepoint->GetMomentum()[2];
    // Get first point energy
    fEventAction->fAnaTrack[track_id].eStart = prepoint->GetTotalEnergy()/CLHEP::MeV;
    fEventAction->fAnaTrack[track_id].kStart = prepoint->GetKineticEnergy()/CLHEP::MeV;
    // Get first point volume
    fEventAction->fAnaTrack[track_id].volumeStart = prepoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
    // ALL THE SAME BUT FOR END POINT
    fEventAction->fAnaTrack[track_id].xEnd = postpoint->GetPosition()[0]/CLHEP::mm;
    fEventAction->fAnaTrack[track_id].yEnd = postpoint->GetPosition()[1]/CLHEP::mm;
    fEventAction->fAnaTrack[track_id].zEnd = postpoint->GetPosition()[2]/CLHEP::mm;
    fEventAction->fAnaTrack[track_id].tEnd = postpoint->GetLocalTime()/CLHEP::ns;
    // Get last point momenta
    fEventAction->fAnaTrack[track_id].pxEnd = postpoint->GetMomentum()[0];
    fEventAction->fAnaTrack[track_id].pyEnd = postpoint->GetMomentum()[1];
    fEventAction->fAnaTrack[track_id].pzEnd = postpoint->GetMomentum()[2];
    // Get last point energy
    fEventAction->fAnaTrack[track_id].eEnd = postpoint->GetTotalEnergy()/CLHEP::MeV;
    fEventAction->fAnaTrack[track_id].kEnd = postpoint->GetKineticEnergy()/CLHEP::MeV;
    // Get last point volume (CAREFUL, for some reason, sometimes unassigned)
    if(!(postpoint->GetPhysicalVolume())) fEventAction->fAnaTrack[track_id].volumeStart = "Unassigned";
    else fEventAction->fAnaTrack[track_id].volumeEnd = postpoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
    // Keep adding energy deposited and travelled length
    fEventAction->fAnaTrack[track_id].enDeposited_tot = 0;
    fEventAction->fAnaTrack[track_id].distTravelled = 0;
    fEventAction->fAnaTrack[track_id].enDeposited_tot += edep;
    fEventAction->fAnaTrack[track_id].distTravelled += step->GetStepLength()/CLHEP::mm;

  }
  // if not, just update the end quantities
  else
  {
    // Check if particle made it to collection
    if(volumeName=="Collection") fEventAction->fAnaTrack[track_id].enteredCollection = true;
    // At this point check if particle previously made it to collection volume but now exited it
    if (fEventAction->fAnaTrack[track_id].enteredCollection==true & volumeName!="Collection") fEventAction->fAnaTrack[track_id].exitedCollection = true;

    // Record every step UNLESS the particle has exited collection
    if(!fEventAction->fAnaTrack[track_id].exitedCollection)
    {  
      fEventAction->fAnaTrack[track_id].xEnd = postpoint->GetPosition()[0]/CLHEP::mm;
      fEventAction->fAnaTrack[track_id].yEnd = postpoint->GetPosition()[1]/CLHEP::mm;
      fEventAction->fAnaTrack[track_id].zEnd = postpoint->GetPosition()[2]/CLHEP::mm;
      fEventAction->fAnaTrack[track_id].tEnd = postpoint->GetLocalTime()/CLHEP::ns;
      // Get last point momenta
      fEventAction->fAnaTrack[track_id].pxEnd = postpoint->GetMomentum()[0];
      fEventAction->fAnaTrack[track_id].pyEnd = postpoint->GetMomentum()[1];
      fEventAction->fAnaTrack[track_id].pzEnd = postpoint->GetMomentum()[2];
      // Get last point energy
      fEventAction->fAnaTrack[track_id].eEnd = postpoint->GetTotalEnergy()/CLHEP::MeV;
      fEventAction->fAnaTrack[track_id].kEnd = postpoint->GetKineticEnergy()/CLHEP::MeV;
      // Get last point volume (CAREFUL, for some reason, sometimes unassigned)
      if(!(postpoint->GetPhysicalVolume())) fEventAction->fAnaTrack[track_id].volumeStart = "Unassigned";
      else fEventAction->fAnaTrack[track_id].volumeEnd = postpoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
      // Keep adding energy deposited and travelled length
      fEventAction->fAnaTrack[track_id].enDeposited_tot += edep;
      fEventAction->fAnaTrack[track_id].distTravelled += step->GetStepLength()/CLHEP::mm;
    }    
  }
}
