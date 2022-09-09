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
// 	Author: yskim
//

#ifndef SensitiveDetector_hh_
#define SensitiveDetector_hh_

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
// #include "G4THitsMap.hh"
#include "TETPSEnergyDeposit.hh"
#include "TETModelImport.hh"
#include "G4THitsCollection.hh"

class G4HCofThisEvent;
class G4TouchableHistory;

class TETSensitiveDetector: public G4VSensitiveDetector
{
public:
	TETSensitiveDetector(const G4String& name, const G4String& hitsCollectionName, TETModelImport* _tetData);
	// void SensitiveDetector_manual(G4String name, G4String hitsCollectionName);
	// virtual void ~SensitiveDetector_manual();
	virtual ~TETSensitiveDetector();

	void Initialize(G4HCofThisEvent* HCE);
	G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent* HCE);
private:
	G4int organID;
	G4double particleEnergy;
	G4double eDep;
	// G4int HCID;
	G4THitsMap<G4double>* EvtMap_manual;
	// G4THitsCollection<G4VHit> EvtMap_manual;
	// TETMyHitsCollection* fHitsCollection;
	TETModelImport* tetData;

};

#endif
