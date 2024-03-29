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
// TETRun.cc
// \file   MRCP_GEANT4/External/src/TETRun.cc
// \author Haegin Han
//

#include "TETRun.hh"


TETRun::TETRun()
:G4Run()
{}

TETRun::~TETRun()
{
	edepMap.clear();
	edepMap_manual_n1.clear();
	edepMap_manual_n50.clear();
	edepMap_gem.clear();
	edepMap_hp.clear();
	edepMap_alpha.clear();
}

void TETRun::RecordEvent(const G4Event* event)
{
	// G4cout << "Run.cccc ------------------------" <<G4endl;
	auto  fCollID_manual_n1
	= G4SDManager::GetSDMpointer()->GetCollectionID("eDep_manual_n1");

	auto  fCollID_manual_n50
	= G4SDManager::GetSDMpointer()->GetCollectionID("eDep_manual_n50");

	auto  fCollID_gem
	= G4SDManager::GetSDMpointer()->GetCollectionID("PhantomSD/eDep_gem");

	auto  fCollID_hp
	= G4SDManager::GetSDMpointer()->GetCollectionID("PhantomSD/eDep_hp");

	auto  fCollID_alpha
	= G4SDManager::GetSDMpointer()->GetCollectionID("PhantomSD/eDep_alpha");
	
	// Hits collections
	G4HCofThisEvent* HCE = event->GetHCofThisEvent();
	if(!HCE) return;

	G4THitsMap<G4double>* evtMap_manual_n1 =
			static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID_manual_n1));

	G4THitsMap<G4double>* evtMap_manual_n50 =
			static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID_manual_n50));

	G4THitsMap<G4double>* evtMap_gem =
			static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID_gem));

	G4THitsMap<G4double>* evtMap_hp =
			static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID_hp));

	G4THitsMap<G4double>* evtMap_alpha =
			static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID_alpha));
	
	G4bool is_equiv = TRUE;
	// G4bool is_equiv = FALSE;
	// sum up the energy deposition and the square of it
	// manual

	for (auto itr : *evtMap_manual_n1->GetMap()) {
		edepMap_manual_n1[itr.first].first  += *itr.second;                   //sum
		edepMap_manual_n1[itr.first].second += (*itr.second) * (*itr.second); //sum square

		edepMap[itr.first].first  += *itr.second;                   //sum
		edepMap[itr.first].second += (*itr.second) * (*itr.second); //sum square
	}
	for (auto itr : *evtMap_manual_n50->GetMap()) {
		edepMap_manual_n50[itr.first].first  += *itr.second;                   //sum
		edepMap_manual_n50[itr.first].second += (*itr.second) * (*itr.second); //sum square

		edepMap[itr.first].first  += *itr.second;                   //sum
		edepMap[itr.first].second += (*itr.second) * (*itr.second); //sum square
	}

	// manual
	
	for (auto itr : *evtMap_gem->GetMap()) {
		edepMap_gem[itr.first].first  += *itr.second;                   //sum
		edepMap_gem[itr.first].second += (*itr.second) * (*itr.second); //sum square

		edepMap[itr.first].first  += *itr.second;                   //sum
		edepMap[itr.first].second += (*itr.second) * (*itr.second); //sum square
	}
	for (auto itr : *evtMap_hp->GetMap()) {
		if (is_equiv){
			*itr.second *= 2;
		}
		edepMap_hp[itr.first].first  += *itr.second;                   //sum
		edepMap_hp[itr.first].second += (*itr.second) * (*itr.second); //sum square

		edepMap[itr.first].first  += *itr.second;                   //sum
		edepMap[itr.first].second += (*itr.second) * (*itr.second); //sum square
	}
	
	for (auto itr : *evtMap_alpha->GetMap()) {
		if (is_equiv){
			*itr.second *= 20;
		}
		edepMap_alpha[itr.first].first  += *itr.second;                   //sum
		edepMap_alpha[itr.first].second += (*itr.second) * (*itr.second); //sum square

		edepMap[itr.first].first  += *itr.second;                   //sum
		edepMap[itr.first].second += (*itr.second) * (*itr.second); //sum square
	}


}

void TETRun::Merge(const G4Run* run)
{
	// merge the data from each thread
	EDEPMAP localMap = static_cast<const TETRun*>(run)->edepMap;
	EDEPMAP localMap_manual_n1 = static_cast<const TETRun*>(run)->edepMap_manual_n1;
	EDEPMAP localMap_manual_n50 = static_cast<const TETRun*>(run)->edepMap_manual_n50;
	EDEPMAP localMap_gem = static_cast<const TETRun*>(run)->edepMap_gem;
	EDEPMAP localMap_hp = static_cast<const TETRun*>(run)->edepMap_hp;
	EDEPMAP localMap_alpha = static_cast<const TETRun*>(run)->edepMap_alpha;

	for(auto itr : localMap){
		edepMap[itr.first].first  += itr.second.first;
		edepMap[itr.first].second += itr.second.second;
	}

	for(auto itr : localMap_manual_n1){
		edepMap_manual_n1[itr.first].first  += itr.second.first;
		edepMap_manual_n1[itr.first].second += itr.second.second;
	}
	for(auto itr : localMap_manual_n50){
		edepMap_manual_n50[itr.first].first  += itr.second.first;
		edepMap_manual_n50[itr.first].second += itr.second.second;
	}

	for(auto itr : localMap_gem){
		edepMap_gem[itr.first].first  += itr.second.first;
		edepMap_gem[itr.first].second += itr.second.second;
	}
	for(auto itr : localMap_hp){
		edepMap_hp[itr.first].first  += itr.second.first;
		edepMap_hp[itr.first].second += itr.second.second;
	}
	
	for(auto itr : localMap_alpha){
		edepMap_alpha[itr.first].first  += itr.second.first;
		edepMap_alpha[itr.first].second += itr.second.second;
	}

	G4Run::Merge(run);
}




