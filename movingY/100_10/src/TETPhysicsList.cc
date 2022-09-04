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
// TETPhysicsList.cc
// \file   MRCP_GEANT4/External/src/TETPhysicsList.cc
//

#include "TETPhysicsList.hh"

TETPhysicsList::TETPhysicsList()
:G4VModularPhysicsList()
{
	G4int verb=0;
	SetVerboseLevel(verb);

	new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);
	new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
	new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);

	// Hadron Elastic scattering
	RegisterPhysics( new HadronElasticPhysicsHP(verb) );

	// Hadron Inelastic Physics
	RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));

	// Ion Physics
	RegisterPhysics( new G4IonPhysics(verb));

	// Gamma-Nuclear Physics
	RegisterPhysics( new GammaPhysics("gamma"));

	// EM physics
	RegisterPhysics(new G4EmLivermorePhysics(verb));

	// Decay
	RegisterPhysics(new G4DecayPhysics(verb));

	// Radioactive decay
	RegisterPhysics(new G4RadioactiveDecayPhysics(verb));
}

TETPhysicsList::~TETPhysicsList()
{}

void TETPhysicsList::SetCuts()
{
	SetCutValue(1*mm, "proton");
	SetCutValue(1*mm, "e-");
	SetCutValue(1*mm, "e+");
	SetCutValue(1*mm, "gamma");
}

// // ********************************************************************
// // ********************************************************************
// // ********************************************************************

// //
// // ********************************************************************
// // * License and Disclaimer                                           *
// // *                                                                  *
// // * The  Geant4 software  is  copyright of the Copyright Holders  of *
// // * the Geant4 Collaboration.  It is provided  under  the terms  and *
// // * conditions of the Geant4 Software License,  included in the file *
// // * LICENSE and available at  http://cern.ch/geant4/license .  These *
// // * include a list of copyright holders.                             *
// // *                                                                  *
// // * Neither the authors of this software system, nor their employing *
// // * institutes,nor the agencies providing financial support for this *
// // * work  make  any representation or  warranty, express or implied, *
// // * regarding  this  software system or assume any liability for its *
// // * use.  Please see the license in the file  LICENSE  and URL above *
// // * for the full disclaimer and the limitation of liability.         *
// // *                                                                  *
// // * This  code  implementation is the result of  the  scientific and *
// // * technical work of the GEANT4 collaboration.                      *
// // * By using,  copying,  modifying or  distributing the software (or *
// // * any work based  on the software)  you  agree  to acknowledge its *
// // * use  in  resulting  scientific  publications,  and indicate your *
// // * acceptance of all terms of the Geant4 Software license.          *
// // ********************************************************************
// //
// // TETPhysicsList.cc
// // \file   MRCP_GEANT4/External/src/TETPhysicsList.cc
// // \author Haegin Han
// //

// #include "TETPhysicsList.hh"

// TETPhysicsList::TETPhysicsList()
// 	: G4VModularPhysicsList(), neutron(0), gamma(0), electron(0)
// //, G4VPhysicsConstructor()
// //,theHPNeutron(0), theNeutrons(0)
// {
// 	G4int verb = 0;
// 	G4VModularPhysicsList::SetVerboseLevel(verb);
// 	//	SetVerboseLevel(verb);

// 	//	auto decay_List = new G4DecayPhysics();

// 	// Hadron Physics
// 	RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP());
// 	hadronphysics_List = new G4HadronPhysicsQGSP_BIC_HP();

// 	// EM physics
// 	RegisterPhysics(new G4EmLivermorePhysics());
// 	emPhysicsList = new G4EmLivermorePhysics();

// 	// Decay
// 	RegisterPhysics(new G4DecayPhysics());
// 	decay_List = new G4DecayPhysics();

// 	// Radioactive decay
// 	RegisterPhysics(new G4RadioactiveDecayPhysics());
// 	radioactive_List = new G4RadioactiveDecayPhysics();

// 	//	thermalneutrons_List = new G4ThermalNeutrons();
// 	RegisterPhysics(new G4IonPhysics());
// 	ionphysics_List = new G4IonPhysics();
// }

// void TETPhysicsList::ConstructParticle()
// {
// 	// hadronphysics_List->ConstructParticle();
// 	// emPhysicsList->ConstructParticle();
// 	// decay_List->ConstructParticle();
// 	// radioactive_List->ConstructParticle();
// 	// ionphysics_List->ConstructParticle();

// 	neutron = G4Neutron::Neutron();
// 	gamma = G4Gamma::Gamma();
// 	electron = G4Electron::Electron();
// }
// //
// //#include "G4ProcessManager.hh"
// void TETPhysicsList::ConstructProcess()
// {
// 	//	ConstructParticle();
// 	// Transportation
// 	AddTransportation();
// 	decay_List->ConstructProcess();			//decay
// 	emPhysicsList->ConstructProcess();		// livermore
// 	hadronphysics_List->ConstructProcess(); // qgsp_bic_hp
// 	radioactive_List->ConstructProcess();	// radioactive
// 	ionphysics_List->ConstructProcess();	// ion physics

// 	// Gamma phys
// 	G4ProcessManager *pManager = G4Gamma::Gamma()->GetProcessManager();
// 	//
// 	G4PhotoNuclearProcess *process = new G4PhotoNuclearProcess();
// 	//
// 	G4CascadeInterface *bertini = new G4CascadeInterface();
// 	bertini->SetMaxEnergy(100 * GeV);
// 	process->RegisterMe(bertini);
// 	//
// 	pManager->AddDiscreteProcess(process);
// 	//	hadronPhys->

// 	//Thermal
// 	//	Process
// 	G4HadronElasticProcess *theNeutronElasticProcess = new G4HadronElasticProcess();
// 	G4NeutronInelasticProcess *theNeutronInelasticProcess = new G4NeutronInelasticProcess();
// 	G4HadronFissionProcess *theNeutronFissionProcess = new G4HadronFissionProcess();
// 	G4HadronCaptureProcess *theNeutronCaptureProcess = new G4HadronCaptureProcess();

// 	// data set
// 	G4NeutronHPElasticData *theHPElasticData = new G4NeutronHPElasticData();
// 	theNeutronElasticProcess->AddDataSet(theHPElasticData);

// 	G4NeutronHPThermalScatteringData *theHPThermalScatteringData = new G4NeutronHPThermalScatteringData();
// 	theHPThermalScatteringData->SetMaxKinEnergy(4.0 * eV);
// 	theNeutronElasticProcess->AddDataSet(theHPThermalScatteringData);

// 	G4NeutronHPInelasticData *theHPInelasticData = new G4NeutronHPInelasticData();
// 	theNeutronInelasticProcess->AddDataSet(theHPInelasticData);

// 	G4NeutronHPCaptureData *theHPCaptureData = new G4NeutronHPCaptureData();
// 	theNeutronCaptureProcess->AddDataSet(theHPCaptureData);

// 	G4NeutronHPFissionData *theHPFissionData = new G4NeutronHPFissionData();
// 	theNeutronFissionProcess->AddDataSet(theHPFissionData);

// 	// Models
// 	G4NeutronHPElastic *theNeutronElasticModel = new G4NeutronHPElastic();
// 	theNeutronElasticModel->SetMinEnergy(4.0 * eV);
// 	theNeutronElasticProcess->RegisterMe(theNeutronElasticModel);

// 	G4NeutronHPThermalScattering *theNeutronThermalElasticModel = new G4NeutronHPThermalScattering();
// 	theNeutronThermalElasticModel->SetMaxEnergy(4.0 * eV);
// 	theNeutronElasticProcess->RegisterMe(theNeutronThermalElasticModel);

// 	G4NeutronHPInelastic *theNeutronInelasticModel = new G4NeutronHPInelastic();
// 	theNeutronInelasticProcess->RegisterMe(theNeutronInelasticModel);

// 	G4NeutronHPCapture *theNeutronCaptureModel = new G4NeutronHPCapture();
// 	theNeutronCaptureProcess->RegisterMe(theNeutronCaptureModel);

// 	G4NeutronHPFission *theNeutronFissionModel = new G4NeutronHPFission();
// 	theNeutronFissionProcess->RegisterMe(theNeutronFissionModel);

// 	//	 Apply Processes to Process Manager of Neutron
// 	G4ProcessManager *pmanager = neutron->GetProcessManager();
// 	pmanager->AddDiscreteProcess(theNeutronElasticProcess);	  // HPelastic and thermal scattering
// 	pmanager->AddDiscreteProcess(theNeutronInelasticProcess); // HPelastic and thermal scattering
// 	pmanager->AddDiscreteProcess(theNeutronCaptureProcess);	  // HPelastic and thermal scattering
// 	pmanager->AddDiscreteProcess(theNeutronFissionProcess);	  // HPelastic and thermal scattering
// }

// void TETPhysicsList::AddPhysicsList()
// {
// 	//	hadronPhys.push_back( new G4DecayPhysics());
// 	//	hadronPhys.push_back( new G4RadioactiveDecayPhysics());
// 	//	hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
// 	//	hadronPhys.push_back( new G4EmLivermorePhysics());
// 	//	hadronPhys.push_back( new G4HadronElasticPhysicsHP());
// }

// TETPhysicsList::~TETPhysicsList()
// {
// 	//	delete theHPNeutron;
// 	//	delete theNeutrons;
// }

// void TETPhysicsList::SetCuts()
// {
// 	SetCutsWithDefault();
// 	// SetCutValue(1 * mm, "proton");
// 	// SetCutValue(1 * mm, "e-");
// 	// SetCutValue(1 * mm, "e+");
// 	// SetCutValue(1 * mm, "gamma");
// }
