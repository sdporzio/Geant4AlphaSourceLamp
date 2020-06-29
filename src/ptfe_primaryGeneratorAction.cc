#include "ptfe_primaryGeneratorAction.hh"
#include "ptfe_detectorConstruction.hh"

#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"


ptfe_primaryGeneratorAction::ptfe_primaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleSource(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
}

ptfe_primaryGeneratorAction::~ptfe_primaryGeneratorAction(){}

// Generate primary particles
void ptfe_primaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

  auto surface_solid = G4SolidStore::GetInstance()->GetSolid("Active");
  G4ThreeVector disc_origin = G4PhysicalVolumeStore::GetInstance()->GetVolume("Disc")->GetTranslation();
  G4ThreeVector active_origin = G4PhysicalVolumeStore::GetInstance()->GetVolume("Active")->GetTranslation();
  G4ThreeVector part_orig = disc_origin+active_origin;

  G4bool isOutside = true;
  G4double x0,y0;
  while(isOutside)
  {
    x0 = (G4UniformRand()*14.5) - 14.5/2.;
    y0 = (G4UniformRand()*14.5) - 14.5/2.;
    isOutside = (surface_solid->Inside(G4ThreeVector(x0,y0,0))==0);
  }


  G4String particleName;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* alpha
    = particleTable->FindParticle(particleName="alpha");
  fParticleGun->SetParticleDefinition(alpha);
  fParticleGun->SetParticleEnergy(5.48*MeV);

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,0)+part_orig);
    // fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-50*mm));


  // Isotropic
  // G4double cosT = -1.0 + G4UniformRand()*2.0; // ISOTROPIC
  G4double cosT = G4UniformRand()*1.0; // ONLY RIGHT SIDE
  G4double phi = G4UniformRand()*M_PI*2.;
  G4double sinT = sqrt(1-cosT*cosT);
  G4ThreeVector direction(sinT*sin(phi),sinT*cos(phi),cosT);
  fParticleGun->SetParticleMomentumDirection(direction);

  // Straight
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  
  fParticleGun->GeneratePrimaryVertex(event);
};
