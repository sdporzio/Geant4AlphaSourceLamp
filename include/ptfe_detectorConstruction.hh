#ifndef ptfe_detectorConstruction_h
#define ptfe_detectorConstruction_h 1
#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4GenericMessenger;
class G4VSolid;
class ptfe_surfaceGenerator;

// Construction class for the detector
class ptfe_detectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ptfe_detectorConstruction();
    virtual ~ptfe_detectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4double GetContaminationDepth() const { return fContaminationDepth; }
    G4bool DoOnSurface() const { return fOnSurface; }
    ptfe_surfaceGenerator* GetSG() const { return fSg; }

  private:
    G4GenericMessenger* fMessenger; // Generic messenger class
    G4double fContaminationDepth; // How deep in the surface does Po210 go
    G4bool fOnSurface; // Whether particle should be simulated EXACTLY on surface
    G4int fSurfaceType; // Surface type
    ptfe_surfaceGenerator* fSg;

};

#endif