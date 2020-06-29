#ifndef ptfe_runAction_h
#define ptfe_runAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class ptfe_eventAction;
class G4Run;
class G4GenericMessenger;

class ptfe_runAction : public G4UserRunAction
{
  public:
    ptfe_runAction();
    virtual ~ptfe_runAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    ptfe_eventAction* fEventAction;
    G4int fRunSeed;
};

#endif