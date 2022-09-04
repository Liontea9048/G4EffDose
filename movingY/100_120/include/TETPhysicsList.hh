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
// TETPhysicsList.hh
// \file   MRCP_GEANT4/External/include/TETPhysicsList.hh
//

#ifndef TETPhysicsList_h
#define TETPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonPhysics.hh"
#include "GammaPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

class G4VPhysicsConstructor;

// *********************************************************************
// Please note that only basic physics were registered in this
// ModularPhysicsList, and rather precise models were used for the
// production of dose coefficients provided in the ICRP Publication.
// -- SetCuts: cut values were set as default values. This can be
//             modified according to specific purposes or applications.
// *********************************************************************

class TETPhysicsList: public G4VModularPhysicsList
{
public:
	TETPhysicsList();
	virtual ~TETPhysicsList();

	virtual void SetCuts();
};

#endif


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
// // TETPhysicsList.hh
// // \file   MRCP_GEANT4/External/include/TETPhysicsList.hh
// // \author Haegin Han
// //

// #ifndef TETPhysicsList_h
// #define TETPhysicsList_h 1

// #include "G4VModularPhysicsList.hh"
// #include "globals.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4UnitsTable.hh"
// #include "G4EmLivermorePhysics.hh"
// #include "G4DecayPhysics.hh"
// #include "G4RadioactiveDecayPhysics.hh"
// #include "G4HadronPhysicsQGSP_BIC_HP.hh"
// #include "G4HadronElasticPhysicsHP.hh"
// #include "G4HadronElasticDataSet.hh"

// #include "G4NeutronHPThermalScattering.hh"
// #include "G4NeutronHPThermalScatteringData.hh"
// #include "G4NeutronHPElasticData.hh"
// #include "G4NeutronHPElastic.hh"
// #include "G4NeutronHPFission.hh"
// #include "G4NeutronHPFissionData.hh"
// #include "G4NeutronHPCapture.hh"
// #include "G4NeutronHPCaptureData.hh"
// #include "G4NeutronHPInelastic.hh"
// #include "G4NeutronHPInelasticData.hh"
// #include "G4HadronElasticProcess.hh"
// #include "G4HadronInelasticProcess.hh"
// #include "G4HadronFissionProcess.hh"
// #include "G4HadronCaptureProcess.hh"
// #include "G4ThermalNeutrons.hh"

// #include "G4NeutronBuilder.hh"
// #include "G4QGSPNeutronBuilder.hh"
// #include "G4NeutronHPBuilder.hh"
// #include "G4VPhysicsConstructor.hh"
// #include "G4ProcessManager.hh"
// #include "G4PhotoNuclearProcess.hh"
// #include "G4PhotoNuclearCrossSection.hh"
// #include "G4LENDorBERTModel.hh"
// #include "G4CascadeInterface.hh"
// #include "G4IonPhysics.hh"

// class G4VPhysicsConstructor;

// // *********************************************************************
// // Please note that only basic physics were registered in this
// // ModularPhysicsList, and rather precise models were used for the
// // production of dose coefficients provided in the ICRP Publication.
// // -- SetCuts: cut values were set as default values. This can be
// //             modified according to specific purposes or applications.
// // *********************************************************************

// class TETPhysicsList: public G4VModularPhysicsList
// {
// public:
// 	TETPhysicsList();
// 	virtual ~TETPhysicsList();

// 	void ConstructParticle();
// 	void ConstructProcess();
// 	void AddPhysicsList();
// 	virtual void SetCuts();

// private:
// 	G4Neutron * neutron;
// 	G4Gamma * gamma;
// 	G4Electron * electron;
// 	G4EmLivermorePhysics * emPhysicsList;
// 	G4DecayPhysics * decay_List;
// 	G4RadioactiveDecayPhysics * radioactive_List;
// 	G4HadronPhysicsQGSP_BIC_HP * hadronphysics_List;
// 	G4IonPhysics * ionphysics_List;
// //	G4ThermalNeutrons * thermalneutrons_List;

// 	std::vector<G4VPhysicsConstructor*>  hadronPhys;

// //  void CreateModels();
// //  G4NeutronBuilder * theNeutrons;
// //  TETHPphyscis * theHPNeutron;

// };


// #endif
