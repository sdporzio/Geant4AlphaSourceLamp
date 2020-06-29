#ifndef ptfe_steppingAction_h
#define ptfe_steppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class ptfe_eventAction;

class ptfe_steppingAction : public G4UserSteppingAction
{
  public:
    ptfe_steppingAction(ptfe_eventAction* eventAction);
    virtual ~ptfe_steppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    ptfe_eventAction*  fEventAction;
};

#endif