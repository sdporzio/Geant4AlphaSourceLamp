#ifndef ptfe_primaryGeneratorAction_h
#define ptfe_primaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"


class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

class ptfe_primaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ptfe_primaryGeneratorAction();
    virtual ~ptfe_primaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);

    const G4GeneralParticleSource* GetParticleSource() const { return fParticleSource; }
  
  private:
    G4GeneralParticleSource* fParticleSource;
    G4ParticleGun* fParticleGun;
    G4ParticleGun* fParticleGun1;
    G4ParticleGun* fParticleGun2;

};

#endif