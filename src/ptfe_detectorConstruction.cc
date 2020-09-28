#include "ptfe_detectorConstruction.hh"
#include "ptfe_materials.hh"

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SubtractionSolid.hh"

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Orb.hh"

#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Material.hh"
#include "g4analysis.hh"

#include <cmath>
#include <iostream>

// Construct/Deconstructor
ptfe_detectorConstruction::ptfe_detectorConstruction() : G4VUserDetectorConstruction(),
  fMessenger(nullptr)
{
  fMessenger = new G4GenericMessenger(this,"/mg4/","");
  fMessenger->DeclareProperty("holeType",fHoleType,"");
  fMessenger->DeclareProperty("holeWidth",fHoleWidth,"");
  fMessenger->DeclareProperty("holeOffset",fHoleOffset,"");
}
ptfe_detectorConstruction::~ptfe_detectorConstruction()
{
  delete fMessenger;
}

// Construct function
G4VPhysicalVolume* ptfe_detectorConstruction::Construct()
{
  // HARD-CODED SETTINGS
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = false;

  // MATERIALS
  G4NistManager* nist = G4NistManager::Instance();
  ptfe_materials* materials = new ptfe_materials();
  G4Material* ptfe = materials->MakeTeflon();
  G4Material* glass = nist->FindOrBuildMaterial("G4_GLASS_LEAD");
  G4Material* alum = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  G4Material* xenon = materials->MakeGasXe();

  // SIZES
  G4double disc_diameter = 12*mm;
  G4double disc_thickness = 0.5*mm;
  G4double disc_distanceFromChamberWall = 1*mm;
  G4double active_diameter = 8*mm;
  G4double active_thickness = 5*um;
  G4double ptfe_diameter = 34.7*mm;
  G4double ptfe_thickness = 20*mm;
  G4double smallHole_diameter = fHoleWidth*mm;

  G4double curvature_radius = 50*mm;

  G4double mirror_diameter = 25.4*mm; // 1 inch
  G4double mirror_centerThickness = 6*mm;
  G4double mirror_edgeThickness = 7.6*mm; // Arbitrary, will be carved out
  G4double collection_thickness = 30*mm;

  G4double mirrorPtfe_offset = ptfe_thickness*0.5-mirror_edgeThickness*0.5;


  // WORLD
  // World is five times bigger than surface
  G4double world_sizeXY = 50*cm;
  G4double world_sizeZ = 50*cm;
  // Build the world
  G4Box* solidWorld =    
    new G4Box("World",                                           //name
       world_sizeXY*0.5, world_sizeXY*0.5, world_sizeZ*0.5);     //size   
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,             //solid
                        xenon,           //material
                        "World");               //name                         
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //logical volume
                      "World",               //name
                      0,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Give it a sensible colour
  G4VisAttributes* worldColour= new G4VisAttributes(G4Colour(0.2,0.2,0.9,0));
  logicWorld->SetVisAttributes(worldColour);

  // MIRROR CURVATURE
  G4Orb* solidCurvature = 
    new G4Orb("Orb",curvature_radius);

  // ALUMINUM SOURCE DISC
  G4Tubs* solidDisc =    
    new G4Tubs("Disc",                                           //name
       0,
       disc_diameter*0.5,
       disc_thickness*0.5,
       0,
       M_PI*2.);     //size   
  G4LogicalVolume* logicDisc =                         
    new G4LogicalVolume(solidDisc,             //solid
                        alum,           //material
                        "Disc");               //name                         
  G4VPhysicalVolume* physDisc = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0, -1*(ptfe_diameter*0.5-disc_diameter*0.5) + disc_distanceFromChamberWall, -1*(ptfe_thickness*0.5+disc_thickness*0.5)+mirrorPtfe_offset),       //at (0,0,0)
                      logicDisc,            //logical volume
                      "Disc",               //name
                      logicWorld,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Give it a sensible colour
  G4VisAttributes* discColour= new G4VisAttributes(G4Colour(0.75,0.25,0.25,1));
  logicDisc->SetVisAttributes(discColour);
  // ALUMINUM SOURCE ACTIVE DISC
  G4Tubs* solidActive =    
    new G4Tubs("Active",                                           //name
       0,
       active_diameter*0.5,
       active_thickness*0.5,
       0,
       M_PI*2.);     //size   
  G4LogicalVolume* logicActive =                         
    new G4LogicalVolume(solidActive,             //solid
                        alum,           //material
                        "Active");               //name                         
  G4VPhysicalVolume* physActive = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,disc_thickness*0.5),       //at (0,0,0)
                      logicActive,            //logical volume
                      "Active",               //name
                      logicDisc,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Give it a sensible colour
  G4VisAttributes* activeColour= new G4VisAttributes(G4Colour(0.25,0.05,0.05,1));
  logicActive->SetVisAttributes(activeColour);    

  // MIRROR
  G4Tubs* solidMirror_straight =    
    new G4Tubs("Mirror_straight",                                           //name
       0,
       mirror_diameter*0.5,
       mirror_edgeThickness*0.5,
       0,
       M_PI*2.);     //size
  G4VSolid* solidMirror = 
    new G4SubtractionSolid("Mirror",
      solidMirror_straight,
      solidCurvature,
      0,
      G4ThreeVector(0,0,curvature_radius - mirror_edgeThickness/2. + mirror_centerThickness));

  G4LogicalVolume* logicMirror =                         
    new G4LogicalVolume(solidMirror,             //solid
                        glass,           //material
                        "Mirror");               //name                         
  G4VPhysicalVolume* physMirror = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicMirror,            //logical volume
                      "Mirror",               //name
                      logicWorld,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Give it a sensible colour
  G4VisAttributes* mirrorColour= new G4VisAttributes(G4Colour(0.25,0.25,0.75,1));
  logicMirror->SetVisAttributes(mirrorColour);  

  // PTFE
  G4Tubs* solidPtfe_full =    
    new G4Tubs("PTFE_full",                                           //name
       0.0*mm,
       ptfe_diameter*0.5,
       ptfe_thickness*0.5,
       0,
       M_PI*2.);     //size
  G4Tubs* solidPtfe_hole =    
    new G4Tubs("PTFE_hole",                                           //name
       0.0*mm,
       mirror_diameter*0.5,
       ptfe_thickness,
       0,
       M_PI*2.);     //size
  G4Tubs* solidPtfe_smallHole =    
    new G4Tubs("PTFE_smallHole",                                           //name
       0.0*mm,
       smallHole_diameter*0.5,
       ptfe_thickness*0.75,
       0,
       M_PI*2.);     //size
  G4VSolid* solidPtfe_ring = 
    new G4SubtractionSolid("PTFE_ring",
      solidPtfe_full,
      solidPtfe_hole,
      0,
      G4ThreeVector());
  // MAKE ONE HOLE OR CLUSTER OF HOLES
  G4VSolid* solidPtfe_straight;
  if (fHoleType==0)
  {
    solidPtfe_straight = 
    new G4SubtractionSolid("PTFE_straight",
      solidPtfe_ring,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(0,-1*(mirror_diameter*0.5+smallHole_diameter*0.5+0.5*mm),0)); 
  }
  else if (fHoleType==1)
  {
    // Determine the whole set of coordinates
    double mir_rad = mirror_diameter*0.5;
    double hole_rad = smallHole_diameter*0.5;
    double gap = 0.5;
    double y0 = 0. - mir_rad - gap - hole_rad;
    double yp1 = y0 - hole_rad - gap - hole_rad;
    double ym1 = y0 + gap;
    double x0 = 0.;
    double xp1 = x0 + hole_rad + gap + hole_rad;
    double xp2 = xp1 + hole_rad + gap + hole_rad;
    double xp3 = xp2 + hole_rad + gap + hole_rad;
    double xm1 = -xp1;
    double xm2 = -xp2;
    double xm3 = -xp3;

    G4VSolid* solidPtfe_straight_interm;
    // FIRST ROW (Y0)
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_ring,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(x0,y0,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xm2,y0,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xm1,y0,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xp1,y0,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xp2,y0,0.));
    // SECOND ROW (YP1)
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(x0,yp1,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xm1,yp1,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xp1,yp1,0.));
    // THIRD ROW, WINGS (YM1)
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xm3,ym1,0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xp3,ym1,0.));
    // END, ATTACH TO SOLID
    solidPtfe_straight = solidPtfe_straight_interm;
  }

  else if (fHoleType==2)
  {
    std::map<int,float> xs;
    std::map<int,float> ys;
    G4VSolid* solidPtfe_straight_interm;

    xs[-4] = -6.;
    xs[-3] = -4.5;
    xs[-2] = -3.;
    xs[-1] = -1.5;
    xs[0] = 0;
    xs[1] = 1.5;
    xs[2] = 3.;
    xs[3] = 4.5;
    xs[4] = 6.;

    ys[0] = -15.2;
    ys[1] = -14.7;
    ys[2] = -14.2;
    ys[3] = -13.7;
    ys[4] = -13.2;
    ys[5] = -12.7;
    ys[6] = -12.2;

  
    // FIRST ROW (Y0)
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_ring,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[0],ys[0],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[1],ys[0],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[-1],ys[0],0.));

    // SECOND ROW
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[0],ys[3],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[1],ys[3],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[-1],ys[3],0.));

    // FIRST WING
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[2],ys[1],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[2],ys[4],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[-2],ys[1],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[-2],ys[4],0.));

    // // SECOND WING
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[3],ys[2],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[3],ys[5],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[-3],ys[2],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[-3],ys[5],0.));

    // // THIRD WING
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[4],ys[3],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[4],ys[6],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[-4],ys[3],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[-4],ys[6],0.));

    // END, ATTACH TO SOLID
    solidPtfe_straight = solidPtfe_straight_interm;
  }

  else if (fHoleType==3)
  {
    std::map<int,float> xs;
    std::map<int,float> ys;
    G4VSolid* solidPtfe_straight_interm;

    double y_offset = fHoleOffset;


    xs[-4] = -6.;
    xs[-3] = -4.5;
    xs[-2] = -3.;
    xs[-1] = -1.5;
    xs[0] = 0;
    xs[1] = 1.5;
    xs[2] = 3.;
    xs[3] = 4.5;
    xs[4] = 6.;

    ys[0] = -15.2 + y_offset;
    ys[1] = -14.7 + y_offset;
    ys[2] = -14.2 + y_offset;
    ys[3] = -13.7 + y_offset;
    ys[4] = -13.2 + y_offset;
    ys[5] = -12.7 + y_offset;
    ys[6] = -12.2 + y_offset;

  
    // SECOND ROW
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_ring,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[0],ys[3],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[1],ys[3],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[-1],ys[3],0.));

    // FIRST WING
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[2],ys[4],0.));
    solidPtfe_straight_interm = 
    new G4SubtractionSolid("PTFE_straight_interm",
      solidPtfe_straight_interm,
      solidPtfe_smallHole,
      0,
      G4ThreeVector(xs[-2],ys[4],0.));

    // // SECOND WING
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[3],ys[5],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[-3],ys[5],0.));

    // // THIRD WING
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[4],ys[6],0.));
    // solidPtfe_straight_interm = 
    // new G4SubtractionSolid("PTFE_straight_interm",
    //   solidPtfe_straight_interm,
    //   solidPtfe_smallHole,
    //   0,
    //   G4ThreeVector(xs[-4],ys[6],0.));

    // END, ATTACH TO SOLID
    solidPtfe_straight = solidPtfe_straight_interm;
  }


  G4VSolid* solidPtfe = 
    new G4SubtractionSolid("PTFE",
      solidPtfe_straight,
      solidCurvature,
      0,
      G4ThreeVector(0,0,curvature_radius-ptfe_thickness*0.5+mirror_centerThickness));
  G4LogicalVolume* logicPtfe =                         
    new G4LogicalVolume(solidPtfe,             //solid
                        ptfe,           //material
                        "PTFE");               //name                         
  G4VPhysicalVolume* physPtfe = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,mirrorPtfe_offset),       //at (0,0,0)
                      logicPtfe,            //logical volume
                      "PTFE",               //name
                      logicWorld,           //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Give it a sensible colour
  G4VisAttributes* ptfeColour= new G4VisAttributes(G4Colour(0.75,0.75,0.75,1));
  logicPtfe->SetVisAttributes(ptfeColour);  

  G4Tubs* solidCollection =    
    new G4Tubs("Collection",                                           //name
       0,
       ptfe_diameter*0.5,
       collection_thickness*0.5, 
       0,
       M_PI*2.);     //size   
  G4LogicalVolume* logicCollection =                         
    new G4LogicalVolume(solidCollection,             //solid
                        xenon,           //material
                        "Collection");               //name                         
  G4VPhysicalVolume* physCollection = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,collection_thickness*0.5+ptfe_thickness*0.5 - 3.5*mm),       //at (0,0,0)
                      logicCollection,            //logical volume
                      "Collection",               //name
                      logicWorld,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
// Give it a sensible colour
  G4VisAttributes* collectionColour= new G4VisAttributes(G4Colour(0.05,0.05,0.75,0.4));
  logicCollection->SetVisAttributes(collectionColour);  



  return physWorld;
}
