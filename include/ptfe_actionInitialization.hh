#ifndef ptfe_actionInitialization_h
#define ptfe_actionInitialization_h 1
#include "G4VUserActionInitialization.hh"

class ptfe_actionInitialization : public G4VUserActionInitialization
{
  public:
    ptfe_actionInitialization();
    virtual ~ptfe_actionInitialization();
    virtual void Build() const;
  };

#endif