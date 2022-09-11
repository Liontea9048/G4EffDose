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

#include "TETSensitiveDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDManager.hh"
#include <cmath>

TETSensitiveDetector::TETSensitiveDetector(const G4String &name, const G4String &hitsCollectionName, TETModelImport *_tetData)
    : G4VSensitiveDetector(name), organID(0), particleEnergy(0), tetData(_tetData)
{
    collectionName.insert(hitsCollectionName);
}

TETSensitiveDetector::~TETSensitiveDetector()
{
}

void TETSensitiveDetector::Initialize(G4HCofThisEvent *HCE) // called at the beginning of the event
{
    EvtMap_manual = new G4THitsMap<G4double>(SensitiveDetectorName, collectionName[0]);
    auto HCID =
        G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(HCID, (G4VHitsCollection *)EvtMap_manual);
}

G4bool TETSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *) // Called for each G4Step in sensitive volume
{
    eDep = aStep->GetTotalEnergyDeposit();
    if (eDep == 0.)
        return FALSE;
    // true
    // auto is_neutron = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
    // if (is_neutron != "neutron") return FALSE;
    // else{
    particleEnergy = aStep->GetPreStepPoint()->GetKineticEnergy();
    G4int copyNo = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
    G4int index = tetData->GetMaterialIndex(copyNo);

    if (particleEnergy < 1.0 * MeV)
    {
        // absorbed dose to equivalent dose 
        // eDep = 2.5 + 18.2 * exp( -(log(eDep) * log(eDep)) / 6 );

        EvtMap_manual->add(index, eDep);
    }
    else if (particleEnergy >= 1.0 * MeV)
    {
        // absorbed dose to equivalent dose 
        // eDep = 5.0 + 17.0 * exp( -(log(2*eDep) * log(2*eDep)) / 6 );
        
        EvtMap_manual->add(index, eDep);
    }
    return TRUE;
}

void TETSensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE) // called at the end of the event
{
}
