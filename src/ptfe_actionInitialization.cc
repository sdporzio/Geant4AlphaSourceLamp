#include "ptfe_actionInitialization.hh"
#include "ptfe_primaryGeneratorAction.hh"
#include "ptfe_runAction.hh"
#include "ptfe_eventAction.hh"
#include "ptfe_steppingAction.hh"

#include "G4ParticleGun.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"

// Construct/Deconstructor
ptfe_actionInitialization::ptfe_actionInitialization() : G4VUserActionInitialization()
{};
ptfe_actionInitialization::~ptfe_actionInitialization(){};

void ptfe_actionInitialization::Build() const
{
  SetUserAction(new ptfe_primaryGeneratorAction);

  ptfe_runAction* runAction = new ptfe_runAction;
  SetUserAction(runAction);

  ptfe_eventAction* eventAction = new ptfe_eventAction(runAction);
  SetUserAction(eventAction);

  ptfe_steppingAction* steppingAction = new ptfe_steppingAction(eventAction);
  SetUserAction(steppingAction);
}
