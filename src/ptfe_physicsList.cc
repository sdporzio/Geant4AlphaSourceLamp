#include "ptfe_physicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "PhysListEmStandardNR.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsGS.hh"

#include "G4IonPhysics.hh"


ptfe_physicsList::ptfe_physicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new PhysListEmStandardNR());
  // RegisterPhysics(new G4EmStandardPhysics());
  // RegisterPhysics(new G4EmLivermorePhysics());
  // RegisterPhysics(new G4EmLowEPPhysics());
  // RegisterPhysics(new G4EmStandardPhysicsSS());
  // RegisterPhysics(new G4EmStandardPhysicsWVI());
  // RegisterPhysics(new G4EmStandardPhysicsGS());
  // RegisterPhysics(new G4IonPhysics());


}

ptfe_physicsList::~ptfe_physicsList(){}

void ptfe_physicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}
