#include "ptfe_materials.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"


ptfe_materials::ptfe_materials(){}
ptfe_materials::~ptfe_materials(){}

G4Material* ptfe_materials::MakeGasXe()
{
  G4double xeTemperature = 266.48 * kelvin; // Room temperature (20 C)
  G4double xePressure = 1.1 * bar; // From Claudio's thesis
  G4double xeDensity = 0.006374 * g / cm3; // Room temperature xenon density

  G4Isotope* Xe124 = new G4Isotope("Xe124", 54, 124, 123.9058942 * g / mole);
  G4Isotope* Xe126 = new G4Isotope("Xe126", 54, 126, 125.904281 * g / mole);
  G4Isotope* Xe128 = new G4Isotope("Xe128", 54, 128, 127.9035312 * g / mole);
  G4Isotope* Xe129 = new G4Isotope("Xe129", 54, 129, 128.9047801 * g / mole);
  G4Isotope* Xe130 = new G4Isotope("Xe130", 54, 130, 129.9035094 * g / mole);
  G4Isotope* Xe131 = new G4Isotope("Xe131", 54, 131, 130.905072 * g / mole);
  G4Isotope* Xe132 = new G4Isotope("Xe132", 54, 132, 131.904144 * g / mole);
  G4Isotope* Xe134 = new G4Isotope("Xe134", 54, 134, 133.905395 * g / mole);
  G4Isotope* Xe136 = new G4Isotope("Xe136", 54, 136, 135.907214 * g / mole);

  G4Element* natXe = new G4Element("Natural Xe", "natXe", 9);
    natXe->AddIsotope(Xe124, 0.09 * perCent);
    natXe->AddIsotope(Xe126, 0.09 * perCent);
    natXe->AddIsotope(Xe128, 1.92 * perCent);
    natXe->AddIsotope(Xe129, 26.44 * perCent);
    natXe->AddIsotope(Xe130, 4.08 * perCent);
    natXe->AddIsotope(Xe131, 21.18 * perCent);
    natXe->AddIsotope(Xe132, 26.89 * perCent);
    natXe->AddIsotope(Xe134, 10.44 * perCent);
    natXe->AddIsotope(Xe136, 8.87 * perCent);

  G4Material* gasXe = new G4Material("gasXe", xeDensity, 1, kStateGas,
    xeTemperature, xePressure);
    gasXe->AddElement(natXe, 1);
    
  return gasXe;
}



G4Material* ptfe_materials::MakeTeflon()
{
  G4Isotope* C12 = new G4Isotope("C12", 6, 12, 12. * g / mole);
  G4Isotope* C13 = new G4Isotope("C13", 6, 13, 13.003354826 * g / mole);
  G4Isotope* F19 = new G4Isotope("F19", 9, 19, 18.99840322 * g / mole);

  G4Element* natC = new G4Element("Natural C", "natC", 2);
    natC->AddIsotope(C12, 98.93 * perCent);
    natC->AddIsotope(C13, 1.07 * perCent);

  G4Element* natF = new G4Element("Natural F", "natF", 1);
    natF->AddIsotope(F19, 100. * perCent);

  G4Material* teflon = new G4Material("teflon", 2.16 * g / cm3, 2);
    teflon->AddElement(natC, 1);
    teflon->AddElement(natF, 2);

  return teflon;
}

G4Material* ptfe_materials::MakeCopper()
{
  G4Isotope* Cu63 = new G4Isotope("Cu63", 29, 63, 62.92959898 * g / mole);
  G4Isotope* Cu65 = new G4Isotope("Cu65", 29, 65, 64.9277929 * g / mole);

  G4Element* natCu = new G4Element("Natural Cu", "natCu", 2);
    natCu->AddIsotope(Cu63, 69.17 * perCent);
    natCu->AddIsotope(Cu65, 30.83 * perCent);

  G4Material* copper = new G4Material("copper", 8.920 * g / cm3, 1);
    copper->AddElement(natCu, 1);

  return copper;
}