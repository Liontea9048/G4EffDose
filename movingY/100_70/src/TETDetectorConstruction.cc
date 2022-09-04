//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// TETDetectorConstruction.cc
// \file   MRCP_GEANT4/External/src/TETDetectorConstruction.cc
// \author Sangbin Han

#include "TETDetectorConstruction.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "TETSensitiveDetector.hh"

#include "G4VisAttributes.hh"

TETDetectorConstruction::TETDetectorConstruction(TETModelImport *_tetData)
	: worldPhysical(0), container_logic(0), tetData(_tetData), tetLogic(0)
{
	// initialisation of the variables for phantom information
	phantomSize = tetData->GetPhantomSize();
	phantomBoxMin = tetData->GetPhantomBoxMin();
	phantomBoxMax = tetData->GetPhantomBoxMax();
	nOfTetrahedrons = tetData->GetNumTetrahedron();
}

TETDetectorConstruction::~TETDetectorConstruction()
{
	delete tetData;
}

G4VPhysicalVolume *TETDetectorConstruction::Construct()
{
	SetupWorldGeometry();
	ConstructPhantom();
	PrintPhantomInformation();
	return worldPhysical;
}

void TETDetectorConstruction::SetupWorldGeometry()
{


	// Operating or Supporting
	// Sonde
	//  =======================================================================
	G4double sonde_position = 100. * cm;
	// =======================================================================
	G4bool checkOverlaps = true;
	G4Material *vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	G4Material *AIR = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
	G4Material *WATER = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");

	G4double height_of_World = 7.5 * m;
	G4VSolid *worldSolid = new G4Box("worldSolid", 5. * m, 5. * m, height_of_World); // world solid done

	G4LogicalVolume *worldLogical = new G4LogicalVolume(worldSolid, AIR, "worldLogical"); // done
	worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "worldPhysical", 0, false, 0, checkOverlaps);

	// Soil (x: 10m, y: 10m, z: 10m )
	G4double height_of_soil = 5.0 * m;
	G4VSolid *Soil = new G4Box("Soil", 5.0 * m, 5.0 * m, height_of_soil);

	G4Material *Silicon = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
	G4Material *Aluminium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
	G4Material *Feric = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
	G4Material *Magnesium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Mg");
	G4Material *Calcium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ca");
	G4Material *Sodium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Na");
	G4Material *Potassium = G4NistManager::Instance()->FindOrBuildMaterial("G4_K");
	G4Material *Manganese = G4NistManager::Instance()->FindOrBuildMaterial("G4_Mn");
	G4Material *Phosphorus = G4NistManager::Instance()->FindOrBuildMaterial("G4_P");
	G4Material *Titanium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ti");
	G4Material *Oxygen = G4NistManager::Instance()->FindOrBuildMaterial("G4_O");
	G4Material *Americium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Am");
	G4Material *Carbon = G4NistManager::Instance()->FindOrBuildMaterial("G4_C");
	G4Material *Cronium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cr");
	G4Material *Nikel = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ni");
	G4Material *Sulfur = G4NistManager::Instance()->FindOrBuildMaterial("G4_S");
	G4Material *Beryllium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Be");
	G4Material *Am2O3 = new G4Material("Am2O3", 12.1656 * g / cm3, 2);

	Am2O3->AddMaterial(Americium, 0.9095); // 0.3556 + 0.0027 = 0.
	Am2O3->AddMaterial(Oxygen, 0.0905);

	G4Material *AmBe = new G4Material("AmBe", 2.0986 * g / cm3, 2);

	AmBe->AddMaterial(Am2O3, 0.0241); // 0.3556 + 0.0027 = 0.
	AmBe->AddMaterial(Beryllium, 0.9759);
	//	-2.2550000E+00
	// Soil_Formation.
	G4Material *Soil_formation = new G4Material("Soil_formation", 2.61 * g / cm3, 11);

	Soil_formation->AddMaterial(Silicon, 0.3583); // 0.3556 + 0.0027 = 0.
	Soil_formation->AddMaterial(Aluminium, 0.0671);
	Soil_formation->AddMaterial(Feric, 0.0083);
	Soil_formation->AddMaterial(Magnesium, 0.0006);
	Soil_formation->AddMaterial(Calcium, 0.0051);
	Soil_formation->AddMaterial(Sodium, 0.0269);
	Soil_formation->AddMaterial(Potassium, 0.0398);
	Soil_formation->AddMaterial(Manganese, 0.0004);
	Soil_formation->AddMaterial(Phosphorus, 0.0002);
	Soil_formation->AddMaterial(Titanium, 0.0006);
	Soil_formation->AddMaterial(Oxygen, 0.4927); // 0.9936 -> 0.0064 is ditributed to Silicion and Oxygen each of 0.0037 0.0027

	G4LogicalVolume *lv_Soil = new G4LogicalVolume(Soil, Soil_formation, "Soil");
	G4double placement_of_Soil = (-1) * (height_of_World - height_of_soil);
	G4double centerofSoil = placement_of_Soil;
	//	G4VPhysicalVolume* pv_Soil =
	new G4PVPlacement(0,
					  G4ThreeVector(0.0, 0.0, placement_of_Soil), lv_Soil, "Soil", worldLogical, false, 0, checkOverlaps); // in the world

	// Container

	G4Box *containerSolid = new G4Box("phantomBox", phantomSize.x() / 2 + 10. * cm, // 55.7708 /2 + 10cm = 37.8854
									  phantomSize.y() / 2 + 10. * cm,				// 29.1260 / 2 + 10 cm = 24.563
									  phantomSize.z() / 2 + 10. * cm);				// 176.0047 /2 + 10 cm = 98.00235

	container_logic = new G4LogicalVolume(containerSolid, AIR, "phantomLogical");
	G4double PhantomBoxX = 0.0 * cm;
	G4double PhantomBoxY = 24.563 * cm + (2.54 * 4) * cm + 10.0 * cm; // at the edge of borehole + 10 cm
	G4double PhantomBoxZ = 98.00236 * cm + (height_of_soil + centerofSoil);

	//	G4VPhysicalVolume* container_physical =
	new G4PVPlacement(0,
					  G4ThreeVector(PhantomBoxX, PhantomBoxY + 70. * cm, PhantomBoxZ), container_logic, "PhantomPhysical", worldLogical, false, 0, checkOverlaps);

	container_logic->SetOptimisation(TRUE);
	container_logic->SetSmartless(0.5); // for optimization (default=2)

	// SUS 409
	G4Material *SUS_409 = new G4Material("SUS_409", 7.82 * g / cm3, 9);

	SUS_409->AddMaterial(Feric, 0.8671);
	SUS_409->AddMaterial(Carbon, 0.0008);
	SUS_409->AddMaterial(Cronium, 0.1175);
	SUS_409->AddMaterial(Nikel, 0.0005);
	SUS_409->AddMaterial(Manganese, 0.0010);
	SUS_409->AddMaterial(Silicon, 0.0005);
	SUS_409->AddMaterial(Phosphorus, 0.0003);
	SUS_409->AddMaterial(Sulfur, 0.0048);
	SUS_409->AddMaterial(Titanium, 0.0075);

	// BoreHole
	G4VSolid *BoreHole = new G4Tubs("BoreHole", 0.0 * cm, 10.16 * cm, 5.0 * m, 0 * deg, 360 * deg);
	G4LogicalVolume *lv_BoreHole = new G4LogicalVolume(BoreHole, WATER, "BoreHole");
	//	G4VPhysicalVolume* pv_BoreHole =
	new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, 0.0 * cm), lv_BoreHole, "BoreHole", lv_Soil, false, 0, checkOverlaps); // done

	// imaginary Borehole
	G4VSolid *imaginary_BoreHole = new G4Tubs("imaginary_BoreHole", 0.0 * cm, 10.16 * cm, 250 * cm, 0 * deg, 360 * deg);
	G4LogicalVolume *lv_imaginary_BoreHole = new G4LogicalVolume(imaginary_BoreHole, AIR, "imaginary_BoreHole");
	//	G4VPhysicalVolume* pv_imaginary_BoreHole =
	new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, 250 * cm + height_of_soil - (height_of_World - height_of_soil)),
					  lv_imaginary_BoreHole, "imaginary_BoreHole", worldLogical, false, 0, checkOverlaps); // done

	//	Sonde variables
	G4double stainlessThcik = 1. * mm;
	G4double source_shielding_thickness = 6 * cm;
	G4double neutron_shielding_thickness = 20 * cm;
	G4double housingLength = 220.1 * cm;
	G4double sondeLength = housingLength - stainlessThcik;
	G4double sonde_center = 0.0 * cm;
	G4double placement_of_sonde;

	upper_Housing_length = housingLength + sonde_position + 0.5 * source_shielding_thickness;
	lower_Housing_length = housingLength - upper_Housing_length;
	
	upper_Sonde_length = upper_Housing_length - stainlessThcik;
	lower_Sonde_length = sondeLength - upper_Sonde_length;

	placement_of_sonde = -250. * cm + (housingLength * 0.5) + source_shielding_thickness * 0.5 + sonde_position;
	placement_of_upper_Housing = -250. * cm + (upper_Housing_length * 0.5);
	placement_of_lower_Housing = 500. * cm - (lower_Housing_length * 0.5);

	G4cout << "Sonde position is : " << sonde_position / cm << G4endl;
	G4cout << "upper housing length is : " << upper_Housing_length / cm << " cm" << G4endl;
	G4cout << "lower housing length is : " << lower_Housing_length / cm << " cm" << G4endl;
	G4cout << "upper sonde length is : " << upper_Sonde_length / cm << " cm" << G4endl;
	G4cout << "lower sonde length is : " << lower_Sonde_length / cm << " cm" << G4endl;
	G4cout << "Placement of sonde is : " << (placement_of_sonde / cm) + 250. * cm << " cm" << G4endl;
	G4cout << "Placement of upperhousing is : " << (placement_of_upper_Housing / cm) + 250. * cm << " cm" << G4endl;
	G4cout << "Placement of lowerhousing is : " << (placement_of_lower_Housing / cm) - 500. * cm << " cm" << G4endl;
	G4cout << "Position of bottom surface of upper Housing is : " << (placement_of_upper_Housing / cm) + 250. * cm - upper_Housing_length/cm * 0.5 << " cm" << G4endl;
	G4cout << "Position of top surface of lower Housing is : " << (placement_of_lower_Housing / cm) - 500. * cm + lower_Housing_length/cm * 0.5 << " cm" << G4endl;

	// above the ground
	if (sonde_position > 0.0 * cm)
	{
		G4cout << "source's center is placed at : " << placement_of_sonde / cm << " cm" << G4endl;

		// Housing
		G4VSolid *Housing = new G4Tubs("Sonde", 0.0 * cm, 3.91 * cm, 0.5 * housingLength, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Housing = new G4LogicalVolume(Housing, SUS_409, "Housing");
		//		G4VPhysicalVolume* pv_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, placement_of_sonde), lv_Housing, "Housing", lv_imaginary_BoreHole, false, 0, checkOverlaps);
		// done

		// Sonde
		G4VSolid *Sonde = new G4Tubs("Sonde", 0. * cm, 3.81 * cm, 0.5 * sondeLength, 0 * deg, 360 * deg);
		//		G4VSolid* Housing = new G4Tubs("Housing", 3.81*cm, 3.91*cm, 1.15*m + 11*mm, 0*deg, 360*deg);
		G4LogicalVolume *lv_Sonde = new G4LogicalVolume(Sonde, AIR, "Sonde");
		//		G4VPhysicalVolume* pv_Housing =
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * stainlessThcik), lv_Sonde, "Sonde", lv_Housing, false, 0, checkOverlaps);

		// Source_shielding // 2022.04.06 modified by HSB
		G4VSolid *Source_Shielding = new G4Tubs("Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4Material *Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
		// G4LogicalVolume *lv_Source_Shielding = new G4LogicalVolume(Source_Shielding, SUS_409, "Source_Shielding");
		G4LogicalVolume *lv_Source_Shielding = new G4LogicalVolume(Source_Shielding, SUS_409, "Source_Shielding");
		//		G4VPhysicalVolume* pv_Source_Shielding =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m+source_shielding_thickness*0.5)), lv_Source_Shielding, "Source_Shielding",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-250. * cm + sonde_position)),
						  lv_Source_Shielding, "Source_Shielding", lv_imaginary_BoreHole, false, 0, checkOverlaps);

		// Am-Be source and shield
		G4VSolid *Source = new G4Tubs("Source", 0.0 * cm, 1.25 * cm, 2.5 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source = new G4LogicalVolume(Source, AmBe, "Source");
		//		G4VPhysicalVolume* pv_Source =
		new G4PVPlacement(0, G4ThreeVector(), lv_Source, "Source", lv_Source_Shielding, false, 0, checkOverlaps);

		// Neutron_shielding
		G4VSolid *Neutron_Shielding = new G4Tubs("Neutron_Shielding", 0.0 * cm, 3.81 * cm, neutron_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Neutron_Shielding = new G4LogicalVolume(Neutron_Shielding, Tungsten, "Neutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m+neutron_shielding_thickness*0.5+source_shielding_thickness)), lv_Neutron_Shielding, "Neutron_Shielding",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-0.5 * sondeLength + neutron_shielding_thickness * 0.5)), lv_Neutron_Shielding, "Neutron_Shielding", lv_Sonde, false, 0, checkOverlaps);

		// Near_Detector (Stilbene)
		G4double near_detector_distance = 30 * cm;
		G4Material *Stilbene = G4NistManager::Instance()->FindOrBuildMaterial("G4_STILBENE");
		G4VSolid *Near_Detector = new G4Tubs("Near_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Near_Detector = new G4LogicalVolume(Near_Detector, Stilbene, "Near_Detector");
		//		G4VPhysicalVolume* pv_Near_Detector =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m + source_shielding_thickness*0.5 + near_detector_distance + 3.71*cm)), lv_Near_Detector, "Near_Detector",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-0.5 * sondeLength - source_shielding_thickness * 0.5 + near_detector_distance + 3.81 * cm)), lv_Near_Detector, "Near_Detector", lv_Sonde, false, 0, checkOverlaps);

		// Far_Detector (Stilbene)
		G4double far_detector_distance = 80 * cm;
		G4VSolid *Far_Detector = new G4Tubs("Far_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Far_Detector = new G4LogicalVolume(Far_Detector, Stilbene, "Far_Detector");
		//		G4VPhysicalVolume* pv_Far_Detector =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m + source_shielding_thickness*0.5 + far_detector_distance + 3.71*cm)), lv_Far_Detector, "Far_Detector",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-0.5 * sondeLength - source_shielding_thickness * 0.5 + far_detector_distance + 3.81 * cm)), lv_Far_Detector, "Far_Detector", lv_Sonde, false, 0, checkOverlaps);

		// visualization

		//	//Sonde
		G4VisAttributes *va_Sonde = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.1));
		va_Sonde->SetForceWireframe(true);
		lv_Sonde->SetVisAttributes(va_Sonde);

		//	//Housing
		G4VisAttributes *va_Housing = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
		va_Housing->SetForceWireframe(true);
		lv_Housing->SetVisAttributes(va_Housing);

		// Souce_Shielding
		G4VisAttributes *va_Source_Shielding = new G4VisAttributes(G4Colour(0.0, 0.0, 1.));
		va_Source_Shielding->SetForceWireframe(true);
		lv_Source_Shielding->SetVisAttributes(va_Source_Shielding);

		// Neutron_shielding
		G4VisAttributes *va_Neutron_Shielding = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Neutron_Shielding->SetForceWireframe(true);
		lv_Neutron_Shielding->SetVisAttributes(va_Neutron_Shielding);

		// Near_Detector
		G4VisAttributes *va_Near_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 0.2));
		va_Near_Detector->SetForceWireframe(true);
		lv_Near_Detector->SetVisAttributes(va_Near_Detector);

		// Far_Detector
		G4VisAttributes *va_Far_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 0.2));
		va_Far_Detector->SetForceWireframe(true);
		lv_Far_Detector->SetVisAttributes(va_Far_Detector);

		// Visualization
		// world
		G4VisAttributes *va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
		va_World->SetForceWireframe(true);
		worldLogical->SetVisAttributes(va_World);

		// soil
		G4VisAttributes *va_Soil = new G4VisAttributes(G4Colour(0.6, 0.3, 0.4, 0.1));
		va_Soil->SetForceWireframe(true);
		lv_Soil->SetVisAttributes(va_Soil);

		//		//PhantomBox
		//		G4VisAttributes* va_PhantomBox = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
		//		va_PhantomBox->SetForceWireframe(true);
		//		container_logic->SetVisAttributes(va_PhantomBox);

		// BoreHole
		G4VisAttributes *va_BoreHole = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
		va_BoreHole->SetForceWireframe(true);
		lv_BoreHole->SetVisAttributes(va_BoreHole);

		// imaginary_Borehole
		G4VisAttributes *va_imaginary_BoreHole = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.1));
		va_imaginary_BoreHole->SetForceWireframe(true);
		lv_imaginary_BoreHole->SetVisAttributes(va_imaginary_BoreHole);
	}

	// not used 2022.04.06 by HSB
	else if (sonde_position == 0. * cm)
	{
		// Housing
		G4VSolid *Housing = new G4Tubs("Sonde", 0.0 * cm, 3.91 * cm, 0.5 * housingLength, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Housing = new G4LogicalVolume(Housing, SUS_409, "Housing");
		//		G4VPhysicalVolume* pv_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, placement_of_sonde), lv_Housing, "Housing", lv_imaginary_BoreHole, false, 0, checkOverlaps);
		// done

		// Sonde
		G4VSolid *Sonde = new G4Tubs("Sonde", 0. * cm, 3.81 * cm, 0.5 * sondeLength, 0 * deg, 360 * deg);
		//		G4VSolid* Housing = new G4Tubs("Housing", 3.81*cm, 3.91*cm, 1.15*m + 11*mm, 0*deg, 360*deg);
		G4LogicalVolume *lv_Sonde = new G4LogicalVolume(Sonde, AIR, "Sonde");
		//		G4VPhysicalVolume* pv_Housing =
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * stainlessThcik), lv_Sonde, "Sonde", lv_Housing, false, 0, checkOverlaps);

		// Source_shielding
		G4Material *Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

		// Source_shielding // 2022.04.06 modified by HSB
		G4VSolid *upper_Source_Shielding = new G4Tubs("upper_Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5 * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Source_Shielding = new G4LogicalVolume(upper_Source_Shielding, SUS_409, "upper_Source_Shielding");
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-250. * cm + sonde_position + source_shielding_thickness * 0.5 * 0.5)),
						  lv_upper_Source_Shielding, "upper_Source_Shielding", lv_imaginary_BoreHole, false, 0, checkOverlaps);

		G4VSolid *lower_Source_Shielding = new G4Tubs("lower_Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5 * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Source_Shielding = new G4LogicalVolume(lower_Source_Shielding, SUS_409, "lower_Source_Shielding");
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (500. * cm + sonde_position - source_shielding_thickness * 0.5 * 0.5)),
						  lv_lower_Source_Shielding, "lower_Source_Shielding", lv_BoreHole, false, 0, checkOverlaps);

		// Am-Be source
		G4VSolid *upper_Source = new G4Tubs("upper_Source", 0.0 * cm, 1.25 * cm, 2.5 * cm * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Source = new G4LogicalVolume(upper_Source, AmBe, "upper_Source");
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.25*cm), lv_upper_Source, "upper_Source", lv_upper_Source_Shielding, false, 0, checkOverlaps);

		G4VSolid *lower_Source = new G4Tubs("lower_Source", 0.0 * cm, 1.25 * cm, 2.5 * cm * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Source = new G4LogicalVolume(lower_Source, AmBe, "lower_Source");
		new G4PVPlacement(0, G4ThreeVector(0., 0., +0.25*cm), lv_lower_Source, "lower_Source", lv_lower_Source_Shielding, false, 0, checkOverlaps);

		// Neutron_shielding
		G4VSolid *Neutron_Shielding = new G4Tubs("Neutron_Shielding", 0.0 * cm, 3.81 * cm, neutron_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Neutron_Shielding = new G4LogicalVolume(Neutron_Shielding, Tungsten, "Neutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m+neutron_shielding_thickness*0.5+source_shielding_thickness)), lv_Neutron_Shielding, "Neutron_Shielding",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-0.5 * sondeLength + neutron_shielding_thickness * 0.5)), lv_Neutron_Shielding, "Neutron_Shielding", lv_Sonde, false, 0, checkOverlaps);

		// Near_Detector (Stilbene)
		G4double near_detector_distance = 30 * cm;
		G4Material *Stilbene = G4NistManager::Instance()->FindOrBuildMaterial("G4_STILBENE");
		G4VSolid *Near_Detector = new G4Tubs("Near_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Near_Detector = new G4LogicalVolume(Near_Detector, Stilbene, "Near_Detector");
		//		G4VPhysicalVolume* pv_Near_Detector =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m + source_shielding_thickness*0.5 + near_detector_distance + 3.71*cm)), lv_Near_Detector, "Near_Detector",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-0.5 * sondeLength - source_shielding_thickness * 0.5 + near_detector_distance + 3.81 * cm)), lv_Near_Detector, "Near_Detector", lv_Sonde, false, 0, checkOverlaps);

		// Far_Detector (Stilbene)
		G4double far_detector_distance = 80 * cm;
		G4VSolid *Far_Detector = new G4Tubs("Far_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Far_Detector = new G4LogicalVolume(Far_Detector, Stilbene, "Far_Detector");
		//		G4VPhysicalVolume* pv_Far_Detector =
		//		new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, (-1.15*m + source_shielding_thickness*0.5 + far_detector_distance + 3.71*cm)), lv_Far_Detector, "Far_Detector",lv_Sonde,false,0, checkOverlaps);
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-0.5 * sondeLength - source_shielding_thickness * 0.5 + far_detector_distance + 3.81 * cm)), lv_Far_Detector, "Far_Detector", lv_Sonde, false, 0, checkOverlaps);

		// // visualization
		// // Sonde
		// // upper
		// G4VisAttributes *va_upper_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		// va_upper_Sonde->SetForceWireframe(true);
		// lv_upper_Sonde->SetVisAttributes(va_upper_Sonde);
		// // lower
		// G4VisAttributes *va_lower_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		// va_lower_Sonde->SetForceWireframe(true);
		// lv_lower_Sonde->SetVisAttributes(va_lower_Sonde);

		// // Housing
		// //  upper
		// G4VisAttributes *va_upper_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		// va_upper_Housing->SetForceWireframe(true);
		// lv_upper_Housing->SetVisAttributes(va_upper_Housing);

		// // lower
		// G4VisAttributes *va_lower_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		// va_lower_Housing->SetForceWireframe(true);
		// lv_lower_Housing->SetVisAttributes(va_lower_Housing);

		// // Souce_Shielding
		// // lower
		// G4VisAttributes *va_lower_Source_Shielding = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		// va_lower_Source_Shielding->SetForceWireframe(true);
		// lv_lower_Source_Shielding->SetVisAttributes(va_lower_Source_Shielding);

		// G4VisAttributes *va_upper_Source_Shielding = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
		// va_upper_Source_Shielding->SetForceWireframe(true);
		// lv_upper_Source_Shielding->SetVisAttributes(va_upper_Source_Shielding);

		// // Neutron_shielding
		// G4VisAttributes *va_Neutron_Shielding = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
		// va_Neutron_Shielding->SetForceWireframe(true);
		// lv_Neutron_Shielding->SetVisAttributes(va_Neutron_Shielding);

		// // Near_Detector
		// G4VisAttributes *va_Near_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		// va_Near_Detector->SetForceWireframe(true);
		// lv_Near_Detector->SetVisAttributes(va_Near_Detector);

		// // Far_Detector
		// G4VisAttributes *va_Far_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		// va_Far_Detector->SetForceWireframe(true);
		// lv_Far_Detector->SetVisAttributes(va_Far_Detector);

		// // Visualization
		// // world
		// G4VisAttributes *va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
		// va_World->SetForceWireframe(true);
		// worldLogical->SetVisAttributes(va_World);

		// // soil
		// G4VisAttributes *va_Soil = new G4VisAttributes(G4Colour(0.6, 0.3, 0.4, 0.1));
		// va_Soil->SetForceWireframe(true);
		// lv_Soil->SetVisAttributes(va_Soil);

		// // PhantomBox
		// G4VisAttributes *va_PhantomBox = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
		// va_PhantomBox->SetForceWireframe(true);
		// container_logic->SetVisAttributes(va_PhantomBox);

		// // BoreHole
		// G4VisAttributes *va_BoreHole = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
		// va_BoreHole->SetForceWireframe(true);
		// lv_BoreHole->SetVisAttributes(va_BoreHole);

		// // imaginary_Borehole
		// G4VisAttributes *va_imaginary_BoreHole = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.1));
		// va_imaginary_BoreHole->SetForceWireframe(true);
		// lv_imaginary_BoreHole->SetVisAttributes(va_imaginary_BoreHole);
	}


	else if (-10.0 * cm <= sonde_position && sonde_position <= -20. * cm ){
		// - 10 cm
		G4cout << " underground - 10.0 cm" << G4endl;

		// upper housing
		G4VSolid *upper_Housing = new G4Tubs("upper_Housing", 0.0 * cm, 3.91 * cm, upper_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Housing = new G4LogicalVolume(upper_Housing, SUS_409, "upper_Housing");
		//		G4VPhysicalVolume* pv_upper_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_upper_Housing), lv_upper_Housing, "upper_Housing", lv_imaginary_BoreHole, false, 0, checkOverlaps);

		// upper_Sonde
		G4VSolid *upper_Sonde = new G4Tubs("upper_Sonde", 0. * cm, 3.81 * cm, upper_Sonde_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Sonde = new G4LogicalVolume(upper_Sonde, AIR, "upper_Sonde");
		//		G4VPhysicalVolume* pv_upper_Housing =
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * stainlessThcik), lv_upper_Sonde, "upper_Sonde", lv_upper_Housing, false, 0, checkOverlaps);

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// lower housing
		G4VSolid *lower_Housing = new G4Tubs("lower_Housing", 0.0 * cm, 3.91 * cm, lower_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Housing = new G4LogicalVolume(lower_Housing, SUS_409, "lower_Housing");
		//		G4VPhysicalVolume* pv_lower_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_lower_Housing), lv_lower_Housing, "lower_Housing", lv_BoreHole, false, 0, checkOverlaps);

		// lower_Sonde
		G4VSolid *lower_Sonde = new G4Tubs("lower_Sonde", 0. * cm, 3.81 * cm, lower_Sonde_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Sonde = new G4LogicalVolume(lower_Sonde, AIR, "lower_Sonde");
		//		G4VPhysicalVolume* pv_lower_Housing =
		new G4PVPlacement(0, G4ThreeVector(), lv_lower_Sonde, "lower_Sonde", lv_lower_Housing, false, 0, checkOverlaps);

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// Source_shielding
		G4Material *Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
		G4VSolid *Source_Shielding = new G4Tubs("Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source_Shielding = new G4LogicalVolume(Source_Shielding, SUS_409, "Source_Shielding");
		//		G4VPhysicalVolume* pv_lower_Source_Shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (500. * cm + sonde_position)), lv_Source_Shielding,
						  "Source_Shielding", lv_BoreHole, false, 0, checkOverlaps);

		// Am-Be source
		G4VSolid *Source = new G4Tubs("Source", 0.0 * cm, 1.25 * cm, 2.5 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source = new G4LogicalVolume(Source, AmBe, "Source");
		//		G4VPhysicalVolume* pv_Source =
		new G4PVPlacement(0, G4ThreeVector(), lv_Source, "Source", lv_Source_Shielding, false, 0, checkOverlaps);

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		G4double UpperNeutronShieldLenght = (neutron_shielding_thickness - lower_Sonde_length);
		G4double LowerNeutronShieldLenght = (neutron_shielding_thickness - UpperNeutronShieldLenght);
		G4double halfUpperNeutronShieldLenght = UpperNeutronShieldLenght * 0.5;
		G4double halfLowerNeutronShieldLenght = LowerNeutronShieldLenght * 0.5;
		// upper Neutron_shielding
		G4VSolid *upperNeutron_Shielding = new G4Tubs("upperNeutron_Shielding", 0.0 * cm, 3.81 * cm, halfUpperNeutronShieldLenght, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upperNeutron_Shielding = new G4LogicalVolume(upperNeutron_Shielding, Tungsten, "upperNeutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-upper_Sonde_length * 0.5 + halfUpperNeutronShieldLenght)),
						  lv_upperNeutron_Shielding, "upperNeutron_Shielding", lv_upper_Sonde, false, 0, checkOverlaps);

		// lower Neutron_shielding
		G4VSolid *lowerNeutron_Shielding = new G4Tubs("lowerNeutron_Shielding", 0.0 * cm, 3.81 * cm, halfLowerNeutronShieldLenght, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lowerNeutron_Shielding = new G4LogicalVolume(lowerNeutron_Shielding, Tungsten, "lowerNeutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (+lower_Sonde_length * 0.5 - halfLowerNeutronShieldLenght)),
						  lv_lowerNeutron_Shielding, "lowerNeutron_Shielding", lv_lower_Sonde, false, 0, checkOverlaps);

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// Near_Detector (Stilbene)
		G4double near_detector_distance = 30 * cm;
		G4Material *Stilbene = G4NistManager::Instance()->FindOrBuildMaterial("G4_STILBENE");
		G4VSolid *Near_Detector = new G4Tubs("Near_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Near_Detector = new G4LogicalVolume(Near_Detector, Stilbene, "Near_Detector");
		//		G4VPhysicalVolume* pv_Near_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-upper_Sonde_length * 0.5 + near_detector_distance + sonde_position + 3.81 * cm)),
						  lv_Near_Detector, "Near_Detector", lv_upper_Sonde, false, 0, checkOverlaps);

		// Far_Detector (Stilbene)
		G4double far_detector_distance = 80 * cm;
		G4VSolid *Far_Detector = new G4Tubs("Far_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Far_Detector = new G4LogicalVolume(Far_Detector, Stilbene, "Far_Detector");
		//		G4VPhysicalVolume* pv_Far_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-upper_Sonde_length * 0.5 + far_detector_distance + sonde_position + 3.81 * cm)),
						  lv_Far_Detector, "Far_Detector", lv_upper_Sonde, false, 0, checkOverlaps);
	}

	else if (sonde_position == -30.0 * cm)
	{
			// -30
		G4cout << " underground - 30.0 cm " << G4endl;

		// upper housing
		G4VSolid *upper_Housing = new G4Tubs("upper_Housing", 0.0 * cm, 3.91 * cm, upper_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Housing = new G4LogicalVolume(upper_Housing, SUS_409, "upper_Housing");
		//		G4VPhysicalVolume* pv_upper_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_upper_Housing), lv_upper_Housing, "upper_Housing", lv_imaginary_BoreHole, false, 0, checkOverlaps);

		// upper_Sonde
		G4VSolid *upper_Sonde = new G4Tubs("upper_Sonde", 0. * cm, 3.81 * cm, upper_Sonde_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Sonde = new G4LogicalVolume(upper_Sonde, AIR, "upper_Sonde");
		//		G4VPhysicalVolume* pv_upper_Housing =
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * stainlessThcik), lv_upper_Sonde, "upper_Sonde", lv_upper_Housing, false, 0, checkOverlaps);

		// lower housing
		G4VSolid *lower_Housing = new G4Tubs("lower_Housing", 0.0 * cm, 3.91 * cm, lower_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Housing = new G4LogicalVolume(lower_Housing, SUS_409, "lower_Housing");
		//		G4VPhysicalVolume* pv_lower_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_lower_Housing), lv_lower_Housing, "lower_Housing", lv_BoreHole, false, 0, checkOverlaps);

		// lower_Sonde
		G4VSolid *lower_Sonde = new G4Tubs("lower_Sonde", 0. * cm, 3.81 * cm, lower_Sonde_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Sonde = new G4LogicalVolume(lower_Sonde, AIR, "lower_Sonde");
		//		G4VPhysicalVolume* pv_lower_Housing =
		new G4PVPlacement(0, G4ThreeVector(), lv_lower_Sonde, "lower_Sonde", lv_lower_Housing, false, 0, checkOverlaps);

		// Source_shielding
		G4Material *Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
		G4VSolid *Source_Shielding = new G4Tubs("Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source_Shielding = new G4LogicalVolume(Source_Shielding, SUS_409, "Source_Shielding");
		//		G4VPhysicalVolume* pv_lower_Source_Shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (500. * cm + sonde_position)), lv_Source_Shielding,
						  "Source_Shielding", lv_BoreHole, false, 0, checkOverlaps);

		// Am-Be source
		G4VSolid *Source = new G4Tubs("Source", 0.0 * cm, 1.25 * cm, 2.5 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source = new G4LogicalVolume(Source, AmBe, "Source");
		//		G4VPhysicalVolume* pv_Source =
		new G4PVPlacement(0, G4ThreeVector(), lv_Source, "Source", lv_Source_Shielding, false, 0, checkOverlaps);

		// Neutron_shielding
		G4VSolid *Neutron_Shielding = new G4Tubs("Neutron_Shielding", 0.0 * cm, 3.81 * cm, neutron_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Neutron_Shielding = new G4LogicalVolume(Neutron_Shielding, Tungsten, "Neutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-lower_Sonde_length * 0.5 + neutron_shielding_thickness * 0.5)),
						  lv_Neutron_Shielding, "Neutron_Shielding", lv_lower_Sonde, false, 0, checkOverlaps);

		// Near_Detector (Stilbene)
		G4double near_detector_distance = 30 * cm;
		G4Material *Stilbene = G4NistManager::Instance()->FindOrBuildMaterial("G4_STILBENE");
		G4VSolid *Near_Detector = new G4Tubs("Near_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Near_Detector = new G4LogicalVolume(Near_Detector, Stilbene, "Near_Detector");
		//		G4VPhysicalVolume* pv_Near_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-upper_Sonde_length * 0.5 + near_detector_distance + sonde_position + 3.81 * cm)),
						  lv_Near_Detector, "Near_Detector", lv_upper_Sonde, false, 0, checkOverlaps);

		// Far_Detector (Stilbene)
		G4double far_detector_distance = 80 * cm;
		G4VSolid *Far_Detector = new G4Tubs("Far_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Far_Detector = new G4LogicalVolume(Far_Detector, Stilbene, "Far_Detector");
		//		G4VPhysicalVolume* pv_Far_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-upper_Sonde_length * 0.5 + far_detector_distance + sonde_position + 3.81 * cm)),
						  lv_Far_Detector, "Far_Detector", lv_upper_Sonde, false, 0, checkOverlaps);
		// visualization

		// Sonde
		// upper
		G4VisAttributes *va_upper_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		va_upper_Sonde->SetForceWireframe(true);
		lv_upper_Sonde->SetVisAttributes(va_upper_Sonde);

		// lower
		G4VisAttributes *va_lower_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		va_lower_Sonde->SetForceWireframe(true);
		lv_lower_Sonde->SetVisAttributes(va_lower_Sonde);

		// Housing
		//  upper
		G4VisAttributes *va_upper_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		va_upper_Housing->SetForceWireframe(true);
		lv_upper_Housing->SetVisAttributes(va_upper_Housing);

		// lower
		G4VisAttributes *va_lower_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		va_lower_Housing->SetForceWireframe(true);
		lv_lower_Housing->SetVisAttributes(va_lower_Housing);

		// Souce_Shielding
		// lower
		G4VisAttributes *va_Source_Shielding = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Source_Shielding->SetForceWireframe(true);
		lv_Source_Shielding->SetVisAttributes(va_Source_Shielding);

		// Neutron_shielding
		G4VisAttributes *va_Neutron_Shielding = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
		va_Neutron_Shielding->SetForceWireframe(true);
		lv_Neutron_Shielding->SetVisAttributes(va_Neutron_Shielding);

		// Near_Detector
		G4VisAttributes *va_Near_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Near_Detector->SetForceWireframe(true);
		lv_Near_Detector->SetVisAttributes(va_Near_Detector);

		// Far_Detector
		G4VisAttributes *va_Far_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Far_Detector->SetForceWireframe(true);
		lv_Far_Detector->SetVisAttributes(va_Far_Detector);

		// Visualization
		// world
		G4VisAttributes *va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
		va_World->SetForceWireframe(true);
		worldLogical->SetVisAttributes(va_World);

		// soil
		G4VisAttributes *va_Soil = new G4VisAttributes(G4Colour(0.6, 0.3, 0.4, 0.1));
		va_Soil->SetForceWireframe(true);
		lv_Soil->SetVisAttributes(va_Soil);

		// PhantomBox
		G4VisAttributes *va_PhantomBox = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
		va_PhantomBox->SetForceWireframe(true);
		container_logic->SetVisAttributes(va_PhantomBox);

		// BoreHole
		G4VisAttributes *va_BoreHole = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
		va_BoreHole->SetForceWireframe(true);
		lv_BoreHole->SetVisAttributes(va_BoreHole);

		// imaginary_Borehole
		G4VisAttributes *va_imaginary_BoreHole = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.1));
		va_imaginary_BoreHole->SetForceWireframe(true);
		lv_imaginary_BoreHole->SetVisAttributes(va_imaginary_BoreHole);

		// ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- -----------------------
	}

	// else if (sonde_position == -50.0 * cm)
	else if (-40. * cm <= sonde_position &&  sonde_position <= -80. * cm)
	{

		// - 50
		G4cout << " underground - 50.0 cm " << G4endl;

		// upper housing
		G4VSolid *upper_Housing = new G4Tubs("upper_Housing", 0.0 * cm, 3.91 * cm, upper_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Housing = new G4LogicalVolume(upper_Housing, SUS_409, "upper_Housing");
		//		G4VPhysicalVolume* pv_upper_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_upper_Housing), lv_upper_Housing, "upper_Housing", lv_imaginary_BoreHole, false, 0, checkOverlaps);

		// upper_Sonde
		G4VSolid *upper_Sonde = new G4Tubs("upper_Sonde", 0. * cm, 3.81 * cm, upper_Housing_length * 0.5 - stainlessThcik * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Sonde = new G4LogicalVolume(upper_Sonde, AIR, "upper_Sonde");
		//		G4VPhysicalVolume* pv_upper_Housing =
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * stainlessThcik), lv_upper_Sonde, "upper_Sonde", lv_upper_Housing, false, 0, checkOverlaps);

		// lower housing
		G4VSolid *lower_Housing = new G4Tubs("lower_Housing", 0.0 * cm, 3.91 * cm, lower_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Housing = new G4LogicalVolume(lower_Housing, SUS_409, "lower_Housing");
		//		G4VPhysicalVolume* pv_lower_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_lower_Housing), lv_lower_Housing, "lower_Housing", lv_BoreHole, false, 0, checkOverlaps);

		// lower_Sonde
		G4VSolid *lower_Sonde = new G4Tubs("lower_Sonde", 0. * cm, 3.81 * cm, lower_Housing_length * 0.5 - stainlessThcik * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Sonde = new G4LogicalVolume(lower_Sonde, AIR, "lower_Sonde");
		//		G4VPhysicalVolume* pv_lower_Housing =
		new G4PVPlacement(0, G4ThreeVector(), lv_lower_Sonde, "lower_Sonde", lv_lower_Housing, false, 0, checkOverlaps);

		// Source_shielding
		G4Material *Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
		// lower source shielding
		G4VSolid *Source_Shielding = new G4Tubs("Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source_Shielding = new G4LogicalVolume(Source_Shielding, SUS_409, "Source_Shielding");
		//		G4VPhysicalVolume* pv_lower_Source_Shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (500. * cm + sonde_position)), lv_Source_Shielding,
						  "Source_Shielding", lv_BoreHole, false, 0, checkOverlaps);

		// Am-Be source and shield
		G4VSolid *Source = new G4Tubs("Source", 0.0 * cm, 1.25 * cm, 2.5 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source = new G4LogicalVolume(Source, AmBe, "Source");
		//		G4VPhysicalVolume* pv_Source =
		new G4PVPlacement(0, G4ThreeVector(), lv_Source, "Source", lv_Source_Shielding, false, 0, checkOverlaps);

		// Neutron_shielding
		G4VSolid *Neutron_Shielding = new G4Tubs("Neutron_Shielding", 0.0 * cm, 3.81 * cm, neutron_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Neutron_Shielding = new G4LogicalVolume(Neutron_Shielding, Tungsten, "Neutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-lower_Sonde_length * 0.5 + neutron_shielding_thickness * 0.5)),
						  lv_Neutron_Shielding, "Neutron_Shielding", lv_lower_Sonde, false, 0, checkOverlaps);

		// Near_Detector (Stilbene)
		G4double near_detector_distance = 30 * cm;
		G4Material *Stilbene = G4NistManager::Instance()->FindOrBuildMaterial("G4_STILBENE");
		G4VSolid *Near_Detector = new G4Tubs("Near_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Near_Detector = new G4LogicalVolume(Near_Detector, Stilbene, "Near_Detector");
		//		G4VPhysicalVolume* pv_Near_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-lower_Sonde_length * 0.5 + near_detector_distance + 3.81 * cm - source_shielding_thickness * 0.5)),
						  lv_Near_Detector, "Near_Detector", lv_lower_Sonde, false, 0, checkOverlaps);

		// Far_Detector (Stilbene)
		G4double far_detector_distance = 80 * cm;
		G4VSolid *Far_Detector = new G4Tubs("Far_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Far_Detector = new G4LogicalVolume(Far_Detector, Stilbene, "Far_Detector");
		//		G4VPhysicalVolume* pv_Far_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-upper_Sonde_length * 0.5 + far_detector_distance + sonde_position + 3.81 * cm)),
						  lv_Far_Detector, "Far_Detector", lv_upper_Sonde, false, 0, checkOverlaps);
		// visualization

		// Sonde
		// upper
		G4VisAttributes *va_upper_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		va_upper_Sonde->SetForceWireframe(true);
		lv_upper_Sonde->SetVisAttributes(va_upper_Sonde);

		// lower
		G4VisAttributes *va_lower_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		va_lower_Sonde->SetForceWireframe(true);
		lv_lower_Sonde->SetVisAttributes(va_lower_Sonde);

		// Housing
		//  upper
		G4VisAttributes *va_upper_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		va_upper_Housing->SetForceWireframe(true);
		lv_upper_Housing->SetVisAttributes(va_upper_Housing);

		// lower
		G4VisAttributes *va_lower_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		va_lower_Housing->SetForceWireframe(true);
		lv_lower_Housing->SetVisAttributes(va_lower_Housing);

		// Souce_Shielding
		//  lower
		G4VisAttributes *va_Source_Shielding = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Source_Shielding->SetForceWireframe(true);
		lv_Source_Shielding->SetVisAttributes(va_Source_Shielding);

		// Neutron_shielding
		G4VisAttributes *va_Neutron_Shielding = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
		va_Neutron_Shielding->SetForceWireframe(true);
		lv_Neutron_Shielding->SetVisAttributes(va_Neutron_Shielding);

		// Near_Detector
		G4VisAttributes *va_Near_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Near_Detector->SetForceWireframe(true);
		lv_Near_Detector->SetVisAttributes(va_Near_Detector);

		// Far_Detector
		G4VisAttributes *va_Far_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Far_Detector->SetForceWireframe(true);
		lv_Far_Detector->SetVisAttributes(va_Far_Detector);

		// Visualization
		// world
		G4VisAttributes *va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
		va_World->SetForceWireframe(true);
		worldLogical->SetVisAttributes(va_World);

		// soil
		G4VisAttributes *va_Soil = new G4VisAttributes(G4Colour(0.6, 0.3, 0.4, 0.1));
		va_Soil->SetForceWireframe(true);
		lv_Soil->SetVisAttributes(va_Soil);

		// PhantomBox
		G4VisAttributes *va_PhantomBox = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
		va_PhantomBox->SetForceWireframe(true);
		container_logic->SetVisAttributes(va_PhantomBox);

		// BoreHole
		G4VisAttributes *va_BoreHole = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
		va_BoreHole->SetForceWireframe(true);
		lv_BoreHole->SetVisAttributes(va_BoreHole);

		// imaginary_Borehole
		G4VisAttributes *va_imaginary_BoreHole = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.1));
		va_imaginary_BoreHole->SetForceWireframe(true);
		lv_imaginary_BoreHole->SetVisAttributes(va_imaginary_BoreHole);

		// ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- -----------------------
	}

	// else if (sonde_position == -100.0 * cm)
	else if (-90. * cm <= sonde_position &&  sonde_position < -220. * cm)
	{
		// -100cm
		G4cout << " underground " << sonde_position/cm << G4endl;

		// upper housing
		G4VSolid *upper_Housing = new G4Tubs("upper_Housing", 0.0 * cm, 3.91 * cm, upper_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Housing = new G4LogicalVolume(upper_Housing, SUS_409, "upper_Housing");
		//		G4VPhysicalVolume* pv_upper_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_upper_Housing), lv_upper_Housing, "upper_Housing", lv_imaginary_BoreHole, false, 0, checkOverlaps);

		// upper_Sonde
		G4VSolid *upper_Sonde = new G4Tubs("upper_Sonde", 0. * cm, 3.81 * cm, upper_Housing_length * 0.5 - stainlessThcik * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_upper_Sonde = new G4LogicalVolume(upper_Sonde, AIR, "upper_Sonde");
		//		G4VPhysicalVolume* pv_upper_Housing =
		new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * stainlessThcik), lv_upper_Sonde, "upper_Sonde", lv_upper_Housing, false, 0, checkOverlaps);

		// lower housing
		G4VSolid *lower_Housing = new G4Tubs("lower_Housing", 0.0 * cm, 3.91 * cm, lower_Housing_length * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Housing = new G4LogicalVolume(lower_Housing, SUS_409, "lower_Housing");
		//		G4VPhysicalVolume* pv_lower_Sonde =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, sonde_center, placement_of_lower_Housing), lv_lower_Housing, "lower_Housing", lv_BoreHole, false, 0, checkOverlaps);

		// lower_Sonde
		G4VSolid *lower_Sonde = new G4Tubs("lower_Sonde", 0. * cm, 3.81 * cm, lower_Housing_length * 0.5 - stainlessThcik * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_lower_Sonde = new G4LogicalVolume(lower_Sonde, AIR, "lower_Sonde");
		//		G4VPhysicalVolume* pv_lower_Housing =
		new G4PVPlacement(0, G4ThreeVector(), lv_lower_Sonde, "lower_Sonde", lv_lower_Housing, false, 0, checkOverlaps);

		// Source_shielding
		G4Material *Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
		// lower source shielding
		G4VSolid *Source_Shielding = new G4Tubs("Source_Shielding", 0.0 * cm, 1.5 * cm, source_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source_Shielding = new G4LogicalVolume(Source_Shielding, SUS_409, "Source_Shielding");
		//		G4VPhysicalVolume* pv_lower_Source_Shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (500. * cm + sonde_position)), lv_Source_Shielding,
						  "Source_Shielding", lv_BoreHole, false, 0, checkOverlaps);

		// Am-Be source and shield
		G4VSolid *Source = new G4Tubs("Source", 0.0 * cm, 1.25 * cm, 2.5 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Source = new G4LogicalVolume(Source, AmBe, "Source");
		//		G4VPhysicalVolume* pv_Source =
		new G4PVPlacement(0, G4ThreeVector(), lv_Source, "Source", lv_Source_Shielding, false, 0, checkOverlaps);

		// Neutron_shielding
		G4VSolid *Neutron_Shielding = new G4Tubs("Neutron_Shielding", 0.0 * cm, 3.81 * cm, neutron_shielding_thickness * 0.5, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Neutron_Shielding = new G4LogicalVolume(Neutron_Shielding, Tungsten, "Neutron_Shielding");
		//		G4VPhysicalVolume* pv_Neutron_shielding =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-lower_Sonde_length * 0.5 + neutron_shielding_thickness * 0.5)),
						  lv_Neutron_Shielding, "Neutron_Shielding", lv_lower_Sonde, false, 0, checkOverlaps);

		// Near_Detector (Stilbene)
		G4double near_detector_distance = 30 * cm;
		G4Material *Stilbene = G4NistManager::Instance()->FindOrBuildMaterial("G4_STILBENE");
		G4VSolid *Near_Detector = new G4Tubs("Near_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Near_Detector = new G4LogicalVolume(Near_Detector, Stilbene, "Near_Detector");
		//		G4VPhysicalVolume* pv_Near_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-lower_Sonde_length * 0.5 + near_detector_distance + 3.81 * cm - source_shielding_thickness * 0.5)),
						  lv_Near_Detector, "Near_Detector", lv_lower_Sonde, false, 0, checkOverlaps);

		// Far_Detector (Stilbene)
		G4double far_detector_distance = 80 * cm;
		G4VSolid *Far_Detector = new G4Tubs("Far_Detector", 0.0 * cm, 3.81 * cm, 3.81 * cm, 0 * deg, 360 * deg);
		G4LogicalVolume *lv_Far_Detector = new G4LogicalVolume(Far_Detector, Stilbene, "Far_Detector");
		//		G4VPhysicalVolume* pv_Far_Detector =
		new G4PVPlacement(0, G4ThreeVector(0.0 * cm, 0.0 * cm, (-lower_Sonde_length * 0.5 + far_detector_distance + 3.81 * cm - source_shielding_thickness * 0.5)),
						  lv_Far_Detector, "Far_Detector", lv_lower_Sonde, false, 0, checkOverlaps);
		// visualization

		// Sonde
		// upper
		G4VisAttributes *va_upper_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		va_upper_Sonde->SetForceWireframe(true);
		lv_upper_Sonde->SetVisAttributes(va_upper_Sonde);

		// lower
		G4VisAttributes *va_lower_Sonde = new G4VisAttributes(G4Colour(2.0, 3.0, 0.0, 0.5));
		va_lower_Sonde->SetForceWireframe(true);
		lv_lower_Sonde->SetVisAttributes(va_lower_Sonde);

		// Housing
		//  upper
		G4VisAttributes *va_upper_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		va_upper_Housing->SetForceWireframe(true);
		lv_upper_Housing->SetVisAttributes(va_upper_Housing);

		// lower
		G4VisAttributes *va_lower_Housing = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
		va_lower_Housing->SetForceWireframe(true);
		lv_lower_Housing->SetVisAttributes(va_lower_Housing);

		// Souce_Shielding
		// lower
		G4VisAttributes *va_Source_Shielding = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Source_Shielding->SetForceWireframe(true);
		lv_Source_Shielding->SetVisAttributes(va_Source_Shielding);

		// Neutron_shielding
		G4VisAttributes *va_Neutron_Shielding = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
		va_Neutron_Shielding->SetForceWireframe(true);
		lv_Neutron_Shielding->SetVisAttributes(va_Neutron_Shielding);

		// Near_Detector
		G4VisAttributes *va_Near_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Near_Detector->SetForceWireframe(true);
		lv_Near_Detector->SetVisAttributes(va_Near_Detector);

		// Far_Detector
		G4VisAttributes *va_Far_Detector = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
		va_Far_Detector->SetForceWireframe(true);
		lv_Far_Detector->SetVisAttributes(va_Far_Detector);

		// Visualization
		// world
		G4VisAttributes *va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
		va_World->SetForceWireframe(true);
		worldLogical->SetVisAttributes(va_World);

		// soil
		G4VisAttributes *va_Soil = new G4VisAttributes(G4Colour(0.6, 0.3, 0.4, 0.1));
		va_Soil->SetForceWireframe(true);
		lv_Soil->SetVisAttributes(va_Soil);

		// PhantomBox
		G4VisAttributes *va_PhantomBox = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
		va_PhantomBox->SetForceWireframe(true);
		container_logic->SetVisAttributes(va_PhantomBox);

		// BoreHole
		G4VisAttributes *va_BoreHole = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
		va_BoreHole->SetForceWireframe(true);
		lv_BoreHole->SetVisAttributes(va_BoreHole);

		// imaginary_Borehole
		G4VisAttributes *va_imaginary_BoreHole = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.1));
		va_imaginary_BoreHole->SetForceWireframe(true);
		lv_imaginary_BoreHole->SetVisAttributes(va_imaginary_BoreHole);

		// ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- -----------------------
	}
}

void TETDetectorConstruction::ConstructPhantom()
{

	// Define the tetrahedral mesh phantom as a parameterised geometry
	//
	// solid and logical volume to be used for parameterised geometry
	G4VSolid *tetraSolid = new G4Tet("TetSolid",
									 G4ThreeVector(),
									 G4ThreeVector(1. * cm, 0, 0),
									 G4ThreeVector(0, 1. * cm, 0),
									 G4ThreeVector(0, 0, 1. * cm));

	G4Material *vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	tetLogic = new G4LogicalVolume(tetraSolid, vacuum, "TetLogic");

	// physical volume (phantom) constructed as parameterised geometry
	new G4PVParameterised("wholePhantom",
						  tetLogic,
						  container_logic,
						  kUndefined,
						  tetData->GetNumTetrahedron(),
						  new TETParameterisation(tetData));
}

void TETDetectorConstruction::ConstructSDandField()
{
	// Define detector (Phantom SD) and scorer (eDep)
	G4SDManager *pSDman = G4SDManager::GetSDMpointer();
	G4String phantomSDname = "PhantomSD";
	G4VPrimitiveScorer *primitive;

	// MultiFunctional detector
	G4MultiFunctionalDetector *MFDet = new G4MultiFunctionalDetector(phantomSDname);
	pSDman->AddNewDetector(MFDet);

	G4VSensitiveDetector *eDep_n1 = new TETSensitiveDetector("n1", "eDep_manual_n1", tetData);
	n_1 = new G4SDParticleWithEnergyFilter("n1");
	n_1->add("neutron");
	n_1->SetKineticEnergy(0.0, 1.0 * MeV);
	eDep_n1->SetFilter(n_1);
	pSDman->AddNewDetector(eDep_n1);
	SetSensitiveDetector(tetLogic, eDep_n1);

	G4VSensitiveDetector *eDep_n50 = new TETSensitiveDetector("n50", "eDep_manual_n50", tetData);
	n_50 = new G4SDParticleWithEnergyFilter("n50");
	n_50->add("neutron");
	n_50->SetKineticEnergy(1.0 * MeV, 50.0 * MeV);
	eDep_n50->SetFilter(n_50);
	pSDman->AddNewDetector(eDep_n50);
	SetSensitiveDetector(tetLogic, eDep_n50);

	primitive = new TETPSEnergyDeposit("eDep_gem", tetData);
	gem = new G4SDParticleWithEnergyFilter("gem");
	gem->add("gamma");
	gem->add("e-");
	gem->add("mu+");
	gem->add("mu-");
	primitive->SetFilter(gem);
	MFDet->RegisterPrimitive(primitive);

	primitive = new TETPSEnergyDeposit("eDep_hp", tetData);
	hp = new G4SDParticleWithEnergyFilter("hp");
	hp->add("proton");
	hp->add("pi+");
	hp->add("pi-");
	primitive->SetFilter(hp);
	MFDet->RegisterPrimitive(primitive);

	primitive = new TETPSEnergyDeposit("eDep_alpha", tetData);
	alpha = new G4SDParticleWithEnergyFilter("alpha");
	alpha->add("alpha");
	alpha->add("GenericIon");
	primitive->SetFilter(alpha);
	MFDet->RegisterPrimitive(primitive);

	// attach the detector to logical volume for parameterised geometry (phantom geometry)
	SetSensitiveDetector(tetLogic, MFDet);
}

void TETDetectorConstruction::PrintPhantomInformation()
{
	// print brief information on the imported phantom
	G4cout << G4endl;
	G4cout.precision(3);
	G4cout << "   Phantom name               " << tetData->GetPhantomName() << " TET phantom" << G4endl;
	G4cout << "   Phantom size               " << phantomSize.x() << " * " << phantomSize.y() << " * " << phantomSize.z() << " mm3" << G4endl;
	G4cout << "   Phantom box position (min) " << phantomBoxMin.x() << " mm, " << phantomBoxMin.y() << " mm, " << phantomBoxMin.z() << " mm" << G4endl;
	G4cout << "   Phantom box position (max) " << phantomBoxMax.x() << " mm, " << phantomBoxMax.y() << " mm, " << phantomBoxMax.z() << " mm" << G4endl;
	G4cout << "   Number of tetrahedrons     " << nOfTetrahedrons << G4endl << G4endl;
}
