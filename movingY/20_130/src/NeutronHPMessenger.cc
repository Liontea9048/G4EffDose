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
// NeutronHPMessenger.cc
// \file   MRCP_GEANT4/External/include/NeutronHPMessenger.cc
//

#include "NeutronHPMessenger.hh"

NeutronHPMessenger::NeutronHPMessenger(HadronElasticPhysicsHP* phys)
:G4UImessenger(),fNeutronPhysics(phys),
 fPhysDir(0), fThermalCmd(0)
{ 
	fPhysDir = new G4UIdirectory("/hadron/phys/");
	fPhysDir->SetGuidance("physics list commands");

	fThermalCmd = new G4UIcmdWithABool("/hadron/phys/thermalScattering",this);
	fThermalCmd->SetGuidance("set thermal scattering model");
	fThermalCmd->SetParameterName("thermal",true);
	fThermalCmd->AvailableForStates(G4State_PreInit);
}



NeutronHPMessenger::~NeutronHPMessenger()
{
	delete fThermalCmd;
	delete fPhysDir;
}


void NeutronHPMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{   
	if (command == fThermalCmd)
	{
		fNeutronPhysics->SetThermalPhysics(fThermalCmd->GetNewBoolValue(newValue));
	}
}


