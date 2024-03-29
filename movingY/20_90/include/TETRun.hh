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
// TETRun.hh
// \file   MRCP_GEANT4/External/include/TETRun.hh
// \author Haegin Han
//

#ifndef TETRun_h
#define TETRun_h 1

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

typedef std::map<G4int, std::pair<G4double, G4double>> EDEPMAP;

// *********************************************************************
// This is G4Run class that sums up energy deposition from each event.
// The sum of the square of energy deposition was also calculated to
// produce the relative error of the dose.
// -- RecordEvent: Sum up the energy deposition and the square of it.
//                 The sums for each organ were saved as the form of
//                 std::map.
// -- Merge: Merge the data calculated in each thread.
// *********************************************************************

class TETRun : public G4Run 
{
public:
	TETRun();
	virtual ~TETRun();

	virtual void RecordEvent(const G4Event*);
	void ConstructMFD(const G4String& mfdName);
    virtual void Merge(const G4Run*);

 	EDEPMAP* GetEdepMap() {return &edepMap;};
	EDEPMAP* GetEdepMap_manual_n1() {return &edepMap_manual_n1;};
	EDEPMAP* GetEdepMap_manual_n50() {return &edepMap_manual_n50;}
	// EDEPMAP* GetEdepMap_n1() {return &edepMap_n1;};
	// EDEPMAP* GetEdepMap_n50() {return &edepMap_n50;};
	EDEPMAP* GetEdepMap_gem() {return &edepMap_gem;};
	EDEPMAP* GetEdepMap_hp() {return &edepMap_hp;};
	EDEPMAP* GetEdepMap_alpha() {return &edepMap_alpha;};

private:
	EDEPMAP edepMap;
	EDEPMAP edepMap_manual_n1;
	EDEPMAP edepMap_manual_n50;
	EDEPMAP edepMap_n1;
	EDEPMAP edepMap_n50;
	EDEPMAP edepMap_gem;
	EDEPMAP edepMap_hp;
	EDEPMAP edepMap_alpha;
};

#endif
