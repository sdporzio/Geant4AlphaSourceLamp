#ifndef ptfe_physicsList_h
#define ptfe_physicsList_h 1

#include "G4VModularPhysicsList.hh"

class ptfe_physicsList: public G4VModularPhysicsList
{
public:
  ptfe_physicsList();
  virtual ~ptfe_physicsList();

  virtual void SetCuts();
};


#endif