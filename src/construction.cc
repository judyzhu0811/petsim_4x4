#include "construction.hh"
#include "G4VisAttributes.hh"   // <<< needed for SetVisAttributes()
#include "detector.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh" 

MyDetectorConstruction::MyDetectorConstruction()
{
}

MyDetectorConstruction::~MyDetectorConstruction()
{
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();
	const G4int iNbEntries = 3; //Number of entries used in the optical arrays

	//================================== elements ===================================
	G4Element* Xe = new G4Element("Xenon", "Xe", 54., 131.293 * g / mole);
	G4Element* H = new G4Element("Hydrogen", "H", 1., 1.0079 * g / mole);
	G4Element* C = new G4Element("Carbon", "C", 6., 12.011 * g / mole);
	G4Element* N = new G4Element("Nitrogen", "N", 7., 14.007 * g / mole);
	G4Element* O = new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
	G4Element* F = new G4Element("Fluorine", "F", 9., 18.998 * g / mole);
	G4Element* Al = new G4Element("Aluminium", "Al", 13., 26.982 * g / mole);
	G4Element* Si = new G4Element("Silicon", "Si", 14., 28.086 * g / mole);
	G4Element* Cr = new G4Element("Chromium", "Cr", 24., 51.996 * g / mole);
	G4Element* Mn = new G4Element("Manganese", "Mn", 25., 54.938 * g / mole);
	G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.85 * g / mole);
	G4Element* Ni = new G4Element("Nickel", "Ni", 28., 58.693 * g / mole);
	G4Element* Cu = new G4Element("Copper", "Cu", 29., 63.546 * g / mole);
	G4Element* Pb = new G4Element("Lead", "Pb", 82., 207.2 * g / mole);
	G4Element* Sn = new G4Element("Tin", "Sn", 50., 120.0 * g / mole);
	G4Element* B = nist->FindOrBuildElement("B");
	G4Element* Gd = nist->FindOrBuildElement("Gd");

	G4Element* Ca = new G4Element("Calcium", "Ca", 20., 40.078 * g / mole);

	G4Element* Mg = new G4Element("Mg", "Mg", 12., 24.3050 * g / mole);
	G4Element* K = new G4Element("K", "K", 19., 39.0983 * g / mole);
	G4Element* Na = new G4Element("Na", "Na", 11., 22.989769 * g / mole);
	G4Element* P = new G4Element("P", "P", 15., 30.973762 * g / mole);
	G4Element* S = new G4Element("S", "S", 16., 32.065 * g / mole);
	G4Element* Ti = new G4Element("Ti", "Ti", 22., 47.867 * g / mole);

	G4Element* Au = new G4Element("Au", "Au", 79., 196.96657 * g / mole);
	//================================== materials ==================================

	//----------------------------------- air ---------------------------------------
	G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");

	//----------------------------------- water -------------------------------------
	G4Material *Water = new G4Material("Water", 1. * g / cm3, 2, kStateLiquid);
	Water->AddElement(H, 2);
	Water->AddElement(O, 1);

	//----------------------------------- vacuum ------------------------------------
	G4Material *Vacuum = new G4Material("Vacuum", 1.e-20 * g / cm3, 2, kStateGas);
	Vacuum->AddElement(N, 0.755);
	Vacuum->AddElement(O, 0.245);
	
	//------------------------------------ teflon -----------------------------------
	G4Material *Teflon = new G4Material("Teflon", 2.2 * g / cm3, 2, kStateSolid);
	Teflon->AddElement(C, 0.240183);
	Teflon->AddElement(F, 0.759817);

	G4double pdTeflonPhotonMomentum[iNbEntries] = {6.91 * eV, 6.98 * eV,
                                                 7.05 * eV};
	G4double pdTeflonRefractiveIndex[iNbEntries] = {1.63, 1.61, 1.58};
	G4double pdTeflonReflectivity[iNbEntries] = {0.99, 0.99, 0.99};
	G4double pdTeflonSpecularLobe[iNbEntries] = {0.01, 0.01, 0.01};
	G4double pdTeflonSpecularSpike[iNbEntries] = {0.01, 0.01, 0.01};
	G4double pdTeflonBackscatter[iNbEntries] = {0.01, 0.01, 0.01};
	G4double pdTeflonEfficiency[iNbEntries] = {1.0, 1.0, 1.0};
	G4double pdTeflonAbsorbtionLength[iNbEntries] = {10 * m, 10 * m,
													10 * m};
	G4MaterialPropertiesTable *pTeflonPropertiesTable =
		new G4MaterialPropertiesTable();
	pTeflonPropertiesTable->AddProperty("RINDEX", pdTeflonPhotonMomentum,
										pdTeflonRefractiveIndex, iNbEntries);
	pTeflonPropertiesTable->AddProperty("REFLECTIVITY", pdTeflonPhotonMomentum,
										pdTeflonReflectivity, iNbEntries);
	pTeflonPropertiesTable->AddProperty("ABSLENGTH", pdTeflonPhotonMomentum,
										pdTeflonAbsorbtionLength, iNbEntries);
	pTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT",
										pdTeflonPhotonMomentum,
										pdTeflonSpecularLobe, iNbEntries);
	pTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT",
										pdTeflonPhotonMomentum,
										pdTeflonSpecularSpike, iNbEntries);
	pTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT",
										pdTeflonPhotonMomentum,
										pdTeflonBackscatter, iNbEntries);
	pTeflonPropertiesTable->AddProperty("EFFICIENCY", pdTeflonPhotonMomentum,
										pdTeflonEfficiency, iNbEntries);
	
	//==== Default optics if no OpticalSurface is given
	Teflon->SetMaterialPropertiesTable(pTeflonPropertiesTable);
	G4double pdTeflonSufraceTransmittance[iNbEntries] = {1e-12,1e-12,1e-12}; // 1e-12 because Geant4 is stupid
	//==== LXe Teflon ====
	G4OpticalSurface *pLXeTeflonOpticalSurface = new G4OpticalSurface("LXeTeflonOpticalSurface", 
		unified, ground, dielectric_dielectric, 0.1);

	G4MaterialPropertiesTable *pLXeTeflonPropertiesTable =
		new G4MaterialPropertiesTable();
	pLXeTeflonPropertiesTable->AddProperty("RINDEX", 
		pTeflonPropertiesTable->GetProperty("RINDEX"));
	pLXeTeflonPropertiesTable->AddProperty("REFLECTIVITY",
		pTeflonPropertiesTable->GetProperty("REFLECTIVITY"));
	pLXeTeflonPropertiesTable->AddProperty("ABSLENGTH", 
		pTeflonPropertiesTable->GetProperty("ABSLENGTH"));
	pLXeTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT", 
		pTeflonPropertiesTable->GetProperty("SPECULARLOBECONSTANT"));
	pLXeTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", 
		pTeflonPropertiesTable->GetProperty("SPECULARSPIKECONSTANT"));
	pLXeTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT", 
		pTeflonPropertiesTable->GetProperty("BACKSCATTERCONSTANT"));
	pLXeTeflonPropertiesTable->AddProperty("EFFICIENCY", 
		pTeflonPropertiesTable->GetProperty("EFFICIENCY"));
	pLXeTeflonPropertiesTable->AddProperty("TRANSMITTANCE", pdTeflonPhotonMomentum, 
											pdTeflonSufraceTransmittance, iNbEntries);  
	pLXeTeflonOpticalSurface->SetMaterialPropertiesTable(
		pLXeTeflonPropertiesTable);

	//----------------------------------- quartz ------------------------------------
	// ref: http://www.sciner.com/Opticsland/FS.htm
	G4Material *Quartz = new G4Material("Quartz", 2.201 * g / cm3, 2, kStateSolid,
										168.15 * kelvin, 1.5 * atmosphere);
	Quartz->AddElement(Si, 1);
	Quartz->AddElement(O, 2);

	// Optical properties Quartz
	const G4int iNbEntriesMatch = 5;
	G4double pdQuartzPhotonMomentum[iNbEntriesMatch] = {
		1. * eV, 6.9 * eV, 6.91 * eV, 6.98 * eV,
		7.05 * eV};  // SERENA: changed  2.*eV to 1.*eV otherwise it gets stuck
					// "Out of Range - Attempt to retrieve information below
					// range!"
	G4double pdQuartzRefractiveIndex[iNbEntriesMatch] = {1.50, 1.50, 1.50, 1.56,
														1.60};
	G4double pdQuartzAbsorbtionLength[iNbEntriesMatch] = {30 * m, 30 * m, 30 * m,
															30 * m, 30 * m};
	G4MaterialPropertiesTable *pQuartzPropertiesTable =
		new G4MaterialPropertiesTable();

	/*
	// Masatoshi Kobayashi for ID PMT photocathode:  For debug. Set same RINDEX with LXe to ignore reflection etc between PMT Window and LXe.
	pQuartzPropertiesTable->AddProperty("RINDEX", pdLXePhotonMomentum,
										pdLXeRefractiveIndex, iNbEntriesMatch);
	*/


	pQuartzPropertiesTable->AddProperty("RINDEX", pdQuartzPhotonMomentum,
										pdQuartzRefractiveIndex, iNbEntriesMatch);
	pQuartzPropertiesTable->AddProperty("ABSLENGTH", pdQuartzPhotonMomentum,
										pdQuartzAbsorbtionLength,
										iNbEntriesMatch);
	Quartz->SetMaterialPropertiesTable(pQuartzPropertiesTable);


	/**********Stainless Steel**********/
	// stainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

	// G4Material *stainlessSteel =
	// 	new G4Material("SS304LSteel", 8.00 * g / cm3, 5, kStateSolid);
	// stainlessSteel->AddElement(Fe, 0.65);
	// stainlessSteel->AddElement(Cr, 0.20);
	// stainlessSteel->AddElement(Ni, 0.12);
	// stainlessSteel->AddElement(Mn, 0.02);
	// stainlessSteel->AddElement(Si, 0.01);

	// G4double pdLXeSSPhotonMomentum[iNbEntries] = {6.91 * eV, 6.98 * eV,
	// 												7.05 * eV};
	// G4double pdLXeSSReflectivity[iNbEntries] = {0.05, 0.05, 0.05};
	// G4MaterialPropertiesTable *pLXeSSPropertiesTable = 
	// 			new G4MaterialPropertiesTable();
	// pLXeSSPropertiesTable->AddProperty("REFLECTIVITY",
	// 		pdLXeSSPhotonMomentum, pdLXeSSReflectivity, iNbEntries);
	
	// pLXeSSSurface = new G4OpticalSurface("LXeSSSurface", 
	// 	unified, polished, dielectric_metal, 0.1);   
	// pLXeSSSurface->SetMaterialPropertiesTable(pLXeSSPropertiesTable);

	/**********Gold**********/

	// G4Material *Gold = new G4Material("Au", 19.3 * g / cm3, 1, kStateSolid);
	// Gold->AddElement(Au, 1.);

	// G4double pdLXeAuPhotonMomentum[iNbEntries] = {6.91 * eV, 6.98 * eV,
	// 												7.05 * eV};
	// G4double pdLXeAuReflectivity[iNbEntries] = {0.23, 0.23, 0.23};
	// G4MaterialPropertiesTable *pLXeAuPropertiesTable = 
	// 			new G4MaterialPropertiesTable();
	// pLXeAuPropertiesTable->AddProperty("REFLECTIVITY",
	// 		pdLXeAuPhotonMomentum, pdLXeAuReflectivity, iNbEntries);
	
	// pLXeAuSurface = new G4OpticalSurface("LXeAuSurface", 
	// 	unified, polished, dielectric_metal, 0.1);   
	// pLXeAuSurface->SetMaterialPropertiesTable(pLXeAuPropertiesTable);

	/**********Liquid Xenon**********/
	G4Material *LXe = new G4Material("LXe", 2.85 * g / cm3, 1, kStateLiquid, 177.05 * kelvin, 1.8 * atmosphere);
	LXe->AddElement(Xe, 1);

	G4double pdLXePhotonMomentum[iNbEntries] = { 6.91 * eV, 6.98 * eV, 7.05 * eV };
	G4double pdLXeScintillation[iNbEntries] = { 0.1,     1.0,     0.1 };
	G4double pdLXeRefractiveIndex[iNbEntries] = { 1.63,    1.61,    1.58 };
	G4double pdLXeAbsorbtionLength[iNbEntries] = { 5000. * cm, 5000. * cm, 5000. * cm };
	G4double pdLXeScatteringLength[iNbEntries] = { 30. * cm,  30. * cm,  30. * cm };

	G4MaterialPropertiesTable* pLXePropertiesTable = new G4MaterialPropertiesTable();

	pLXePropertiesTable->AddProperty("SCINTILLATIONCOMPONENT1", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries);
	pLXePropertiesTable->AddProperty("SCINTILLATIONCOMPONENT2", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries);
	pLXePropertiesTable->AddProperty("RINDEX", pdLXePhotonMomentum, pdLXeRefractiveIndex, iNbEntries);
	pLXePropertiesTable->AddProperty("ABSLENGTH", pdLXePhotonMomentum, pdLXeAbsorbtionLength, iNbEntries);
	pLXePropertiesTable->AddProperty("RAYLEIGH", pdLXePhotonMomentum, pdLXeScatteringLength, iNbEntries);

	pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 40. / keV);
	pLXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 1.0);
	pLXePropertiesTable->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3. * ns);
	pLXePropertiesTable->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 27. * ns);
	pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD1", 1.0);

	LXe->SetMaterialPropertiesTable(pLXePropertiesTable);

	//-------------------------------- gaseous xenon --------------------------------
	// G4Material *GXe = new G4Material("GXe", 0.005887 * g / cm3, 1, kStateGas, 173.15 * kelvin, 1.5 * atmosphere);
	// GXe->AddElement(Xe, 1);

	// G4double pdGXePhotonMomentum[iNbEntries] = { 6.91 * eV, 6.98 * eV, 7.05 * eV };
	// G4double pdGXeScintillation[iNbEntries] = { 0.1,     1.0,     0.1 };
	// G4double pdGXeRefractiveIndex[iNbEntries] = { 1.00,    1.00,    1.00 };
	// G4double pdGXeAbsorbtionLength[iNbEntries] = { 100 * m,   100 * m,   100 * m };
	// G4double pdGXeScatteringLength[iNbEntries] = { 100 * m,   100 * m,   100 * m };

	// G4MaterialPropertiesTable* pGXePropertiesTable = new G4MaterialPropertiesTable();

	//pGXePropertiesTable->AddProperty("FASTCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries);
	//pGXePropertiesTable->AddProperty("SLOWCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries);
	//pGXePropertiesTable->AddProperty("RINDEX", pdGXePhotonMomentum, pdGXeRefractiveIndex, iNbEntries);
	//pGXePropertiesTable->AddProperty("ABSLENGTH", pdGXePhotonMomentum, pdGXeAbsorbtionLength, iNbEntries);
	//pGXePropertiesTable->AddProperty("RAYLEIGH", pdGXePhotonMomentum, pdGXeScatteringLength, iNbEntries);

	//pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0. / (keV));
	//pGXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 0);
	//pGXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3. * ns);
	//pGXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27. * ns);
	//pGXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0);

	//GXe->SetMaterialPropertiesTable(pGXePropertiesTable);

    G4Material *worldMat = Vacuum;
    
    G4Box *solidWorld = new G4Box("solidWorld", 0.05*m, 0.05*m, 0.05*m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

	//teflon shell in PET cell
	G4Box *solidTeflonShell = new G4Box("teflonShell", 0.035*m, 0.035*m, 0.025*m);
	G4LogicalVolume *logicTeflonShell = new G4LogicalVolume(solidTeflonShell, Teflon, "logicTeflon");
	new G4LogicalSkinSurface(
    "LXeTeflonSkin",
    logicTeflonShell,
    pLXeTeflonOpticalSurface);
	G4VPhysicalVolume *physTeflonShell = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTeflonShell, "physTeflonShell", logicWorld, false, 0, true);

	//liquid xenon in PET cell
	G4double shrinkXY = 1*mm; // shrink on x and y
G4double shrinkZ  = 1*mm; // shrink on z

G4Box *solidLiquidXenon = new G4Box(
    "liquidXenon",
    0.035*m - shrinkXY,
    0.035*m - shrinkXY,
    0.025*m - shrinkZ
);
G4LogicalVolume *logicLiquidXenon = new G4LogicalVolume(solidLiquidXenon, LXe, "logicLiquidXenon");
G4VPhysicalVolume *physLiquidXenon = new G4PVPlacement(
    0, G4ThreeVector(0., 0., 0.*m), logicLiquidXenon, "physLiquidXenon", logicTeflonShell, false, 0, true
);
	// Create solid detector and logical volume
G4Box *solidDetector = new G4Box("solidDetector", 0.0075*m, 0.0075*m, 0.001*m);
G4Material* Silicon = nist->FindOrBuildMaterial("G4_Si");
G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector, Silicon, "logicDetector");

// --- Cathode grid parameters ---
G4Material* metalMaterial = nist->FindOrBuildMaterial("G4_Al");

// --- Cathode optical surface ---
G4double metalPhotonEnergy[iNbEntries] = {6.91*eV, 6.98*eV, 7.05*eV};
G4double metalReflectivity[iNbEntries] = {0.85, 0.85, 0.85};
G4double metalEfficiency[iNbEntries] = {0.0, 0.0, 0.0};
G4MaterialPropertiesTable* metalMPT = new G4MaterialPropertiesTable();
metalMPT->AddProperty("REFLECTIVITY", metalPhotonEnergy, metalReflectivity, iNbEntries);
metalMPT->AddProperty("EFFICIENCY", metalPhotonEnergy, metalEfficiency, iNbEntries);

G4OpticalSurface* cathodeOpticalSurface = new G4OpticalSurface("CathodeSurface",
    unified, polished, dielectric_metal);
cathodeOpticalSurface->SetMaterialPropertiesTable(metalMPT);

// --- Cathode grid dimensions ---
G4double cathodeHalfXY = 35*mm;          // half-size of cathode
G4int nSquares = 10;                      // 10×10 squares
G4int nWires = nSquares + 1;              // 11 wires along X and Y
G4double wireWidth = 1*micrometer;        // wire width
G4double wireThickness = 0.01*mm;         // wire thickness along Z
G4double squareSize = 6*mm;               // size of each square
G4double gap = 0.89*mm;                   // gap between squares
G4double edgeMargin = 0.995*mm;           // distance from edge to first wire

// distance between wire centers
G4double wireSpacing = squareSize + gap;

// Logical volume for cathode frame (thin placeholder along Z)
G4Box* solidCathodeFrame = new G4Box("cathodeFrame", cathodeHalfXY, cathodeHalfXY, wireThickness/2.);
G4LogicalVolume* logicCathodeFrame = new G4LogicalVolume(solidCathodeFrame, metalMaterial, "logicCathodeFrame");

// --- Single horizontal wire (along X axis) ---
G4Box* solidWireX = new G4Box("wireX", cathodeHalfXY, wireWidth/2., wireThickness/2.);
G4LogicalVolume* logicWireX = new G4LogicalVolume(solidWireX, metalMaterial, "logicWireX");

// Place horizontal wires
for(G4int i=0; i<nWires; i++){
    G4double y = -cathodeHalfXY + edgeMargin + i*wireSpacing;
    new G4PVPlacement(0, G4ThreeVector(0., y, 0.), logicWireX, "physWireX", logicCathodeFrame, false, i, true);
}

// --- Single vertical wire (along Y axis) ---
G4Box* solidWireY = new G4Box("wireY", wireWidth/2., cathodeHalfXY, wireThickness/2.);
G4LogicalVolume* logicWireY = new G4LogicalVolume(solidWireY, metalMaterial, "logicWireY");

// Place vertical wires
for(G4int j=0; j<nWires; j++){
    G4double x = -cathodeHalfXY + edgeMargin + j*wireSpacing;
    new G4PVPlacement(0, G4ThreeVector(x, 0., 0.), logicWireY, "physWireY", logicCathodeFrame, false, j, true);
}

// --- Place cathode in world ---
G4double cathodeZ = -0.024*m;
new G4PVPlacement(0, G4ThreeVector(0., 0., cathodeZ), logicCathodeFrame, "physCathode", logicTeflonShell, false, 0, true);

// Attach optical surface
new G4LogicalSkinSurface("CathodeSurfaceSkin", logicCathodeFrame, cathodeOpticalSurface);

//----------------------------------- Quartz Anode Plate --------------------------------
G4double anodeHalfXY = 34*mm;   // 68 mm / 2
G4double anodeHalfZ  = 0.25*mm; // 0.5 mm / 2

// Quartz material logical volume
G4Box* solidAnodeQuartz = new G4Box("anodeQuartz", anodeHalfXY, anodeHalfXY, anodeHalfZ);
G4LogicalVolume* logicAnodeQuartz = new G4LogicalVolume(solidAnodeQuartz, Quartz, "logicAnodeQuartz");

// --- Visual attributes ---
G4VisAttributes* quartzVis = new G4VisAttributes(G4Colour(0.5,0.5,1.0,0.3)); // semi-transparent blue
quartzVis->SetForceSolid(true);
logicAnodeQuartz->SetVisAttributes(quartzVis);

// --- Place the quartz anode inside the Teflon shell; 5 mm above top of sipm ---
G4double sipmBottom = 0.024*m - 0.001*m;
G4double anodeZ = sipmBottom - 5*mm - anodeHalfZ;

new G4PVPlacement(0, G4ThreeVector(0.,0.,anodeZ),
                  logicAnodeQuartz, "physAnodeQuartz",
                  logicTeflonShell, false, 0, true);

// --- Optical surface for wires (shared top/bottom) ---
G4OpticalSurface* anodeOpticalSurface = new G4OpticalSurface("AnodeSurface",
    unified, polished, dielectric_metal);
anodeOpticalSurface->SetMaterialPropertiesTable(metalMPT);

// --- Wire parameters ---
G4double anodeWireWidth       = 5*micrometer;   // along Y
G4double anodeWireThickness   = 1*micrometer;   // along Z
G4double anodeTopWireSpacing  = 1*mm;           // center-to-center along Y
G4double anodeVerticalOffset  = 0.5*mm;         // top wires above bottom wires

G4int nTopWires = static_cast<G4int>(68*mm / anodeTopWireSpacing) + 1;  // 69 wires
G4int nBottomWires = nTopWires;

// --- Logical volumes for wires ---
G4Box* solidTopWire = new G4Box("topWire", anodeHalfXY, anodeWireWidth/2., anodeWireThickness/2.);
G4LogicalVolume* logicTopWire = new G4LogicalVolume(solidTopWire, metalMaterial, "logicTopWire");
new G4LogicalSkinSurface("TopWireSurface", logicTopWire, anodeOpticalSurface);

G4Box* solidBottomWire = new G4Box("bottomWire", anodeHalfXY, anodeWireWidth/2., anodeWireThickness/2.);
G4LogicalVolume* logicBottomWire = new G4LogicalVolume(solidBottomWire, metalMaterial, "logicBottomWire");
new G4LogicalSkinSurface("BottomWireSurface", logicBottomWire, anodeOpticalSurface);

// --- Placement ---
// Bottom wires (at bottom surface of quartz)
G4double zBottom = -anodeHalfZ + anodeWireThickness/2.;
for(G4int i=0; i<nBottomWires; i++){
    G4double y = -anodeHalfXY + i*anodeTopWireSpacing;
    new G4PVPlacement(0, G4ThreeVector(0., y, zBottom), logicBottomWire,
                      "physBottomWire", logicAnodeQuartz, false, i, true);
}

// Top wires (0.5 mm above bottom wires; closer to SiPMs)
G4double zTop = anodeHalfZ - anodeWireThickness/2.;

for(G4int i=0; i<nTopWires; i++){
    G4double y = -anodeHalfXY + i*anodeTopWireSpacing;
    new G4PVPlacement(0, G4ThreeVector(0., y, zTop), logicTopWire,
                      "physTopWire", logicAnodeQuartz, false, i, true);
}


// 4x4 SiPM grid
G4int gridSize = 4;
G4double spacing = 0.01625*m;
G4double startPos = -spacing*(gridSize-1)/2;  // start at negative offset

for (G4int i = 0; i < gridSize; i++) {
    for (G4int j = 0; j < gridSize; j++) {
        G4ThreeVector pos(
            startPos + i*spacing,  // x
            startPos + j*spacing,  // y
            0.024*m               // z
        );
        new G4PVPlacement(
            0, pos,
            logicDetector,
            "physDetector",
            logicTeflonShell,
            false,
            i*gridSize + j,
            true
        );
    }
}
ConstructSDandField(); 
    
return physWorld; 
} 

void MyDetectorConstruction::ConstructSDandField()
{
    // Create the sensitive detector
    Detector* sensDet = new Detector("SensitiveDetector");

    // Register it
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);

    // Attach it to all logical volumes named "logicDetector"
    G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
    for (auto lv : *lvStore) {
        if (lv->GetName() == "logicDetector") {
            lv->SetSensitiveDetector(sensDet);
        }
    }

    // Now all SiPMs should be linked, no need to return anything
}

