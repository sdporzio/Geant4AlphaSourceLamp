#ifndef ptfe_eventAction_h
#define ptfe_eventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class G4Event;
class ptfe_runAction;
class ptfe_anaTrack;

class ptfe_eventAction : public G4UserEventAction
{
  public:
    ptfe_eventAction(ptfe_runAction* runAction);
    virtual ~ptfe_eventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEnDep(G4String volumeName, G4double edep)
    {
      fEnDep_vector[volumeName].push_back(edep);
      fEnDep[volumeName] += edep;
    }

    
    // Too lazy to put it in private and write methods
    std::map<G4int,ptfe_anaTrack> fAnaTrack;


  private:
    ptfe_runAction* fRunAction;
    std::map<G4String,std::vector<G4double>> fEnDep_vector;
    std::map<G4String,G4double> fEnDep; // MeV?
    G4long fEventSeedIndex, fEventSeed1, fEventSeed2;
    time_t startTime, previousTime;
};

#endif
