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
// * technical work of the GEANT4 collaboration.             W         *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// TETSteppingAction.cc
// \file   MRCP_GEANT4/External/src/TETSteppingAction.cc
// \author Haegin Han
//

#include "TETSteppingAction.hh"
#include "G4Event.hh"
#include "G4MTRunManager.hh"
#include "G4SystemOfUnits.hh"


TETSteppingAction::TETSteppingAction()
: G4UserSteppingAction(), kCarTolerance(1.0000000000000002e-07), stepCounter(0),
  checkFlag(0)
{
	//	for (int i =0; i <148; i++){
	//		count[i]=0.;
	//	}
	//	ofs.open("./20210203_Ambe.txt");
}

TETSteppingAction::~TETSteppingAction()
{
	//	for (int i =0; i <148; i++){
	//		G4double prob = count[i] / sumprob;
	//		ofs << prob << G4endl;
	//	}
	//	ofs.close();

}

void TETSteppingAction::UserSteppingAction(const G4Step* step)
{
	//	G4cout<<"stepping action cc"<<G4endl;
	//	G4double radiationfactor;
	//	G4double energys;
	//	G4double equivalent_dose;
	// Slightly move the particle when the step length of five continuous steps is
	// shorter than the tolerance (0.1 nm)
	//
	G4Track* theTrack = step->GetTrack();
	G4bool CheckingLength = (step->GetStepLength() < kCarTolerance);
	//	G4double erg = step->GetPreStepPoint()->GetKineticEnergy();
	//	auto a = theTrack->GetParentID();
	//	auto b = theTrack->GetTrackID();
	//	if (a == 0 && b == 1){
	//		for (int i =0; i <148; i++){
	//			if (energybin[i]<=erg && erg < energybin[i+1]){
	//				count[i] += 1;
	//				sumprob +=1;
	//				theTrack->SetTrackStatus(fStopAndKill);
	//				break;
	//			}
	//		}
	//	}
	//	G4double cn = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
	//		G4String namee = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	//		G4cout << namee << G4endl;
	//		theTrack->SetTrackStatus(fStopAndKill);
	//	name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
	//	G4double delta_ke = step->GetDeltaEnergy();
	//		energys = step->GetTotalEnergyDeposit();
	//	G4String post_pv_name = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
	//	post_pv_name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

	if(CheckingLength)
	{
		++stepCounter;
		if( checkFlag && stepCounter>=5 )
		{
			// kill the track if the particle is stuck even after the slight move
			// (this hardly occurs)
			theTrack->SetTrackStatus(fStopAndKill);
			stepCounter=0;
			checkFlag=0;
		}
		else if( stepCounter>=5 )
		{
			// if a particle is at the same position (step length < 0.1 nm) for five consecutive steps,
			// slightly move (0.1 nm) the stuck particle in the direction of momentum
			theTrack->SetPosition(theTrack->GetPosition() + theTrack->GetMomentumDirection()*kCarTolerance);
			checkFlag=1;
		}
	}
	else stepCounter=0;
}
