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
// External.cc
// \file          MRCP_GEANT4/External/External.cc
// \author        Haegin Han
// \contributors  Min Cheol Han, Bangho Shin, Chansoo Choi, Yeon Soo Yeom, 
//                Jonghwi Jeong, Chan Hyeong Kim

#include "TETDetectorConstruction.hh"
#include "TETModelImport.hh"
#include "TETPhysicsList.hh"
#include "TETActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#ifdef G4UI_USE_QT
#include "G4VisExecutive.hh"
#endif

#ifdef G4VIS_USE_OI
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

void PrintUsage(){
	G4cerr<< "Usage: ./External -m [MACRO] -o [OUTPUT] -f (option for MRCP-AF phantom)"  <<G4endl;
	G4cerr<< "Example: ./External -m run.mac -o run.out (-f)" <<G4endl;
}
// ./External -m example.in -o example.out -f (-f means that we are going to use Female Mesh Phantom)
// Argv [External, -m, example.in, -o, example.out, -f (or null)] -> for male, argc = 5 and for female, argc = 6
int main(int argc,char** argv) 
{
	// Read the arguments for batch mode
	//
	G4String macro;
	G4String output;
	G4bool   isAF(false); // default is male.
	G4UIExecutive* ui = 0;

	for ( G4int i=1; i<argc; i++ ) {
		// macro file name
		if ( G4String(argv[i]) == "-m" ) { // when argv[1] = -m
			macro = argv[i+1]; // macro file is argv[2], example.in
			i++; // set the i(1) to the i++ (2) and after this code, i becomes 3
		}
		// output file name
		else if ( G4String(argv[i]) == "-o" ) { // when argv[3] = -o
			output = argv[i+1]; // output file is argv[4]=example.out
			i++; //set the i from 3 to 4, after this codes, i becoems 5, at this point, if the length of arguments was 5 (in the case of male) this main ftn ends.
		}
		// switch for MRCP-AF phantom
		else if ( G4String(argv[i]) == "-f" ) { // when argv[5] = -f
			isAF = true; // the female phantom will be used.
		}
		else {
			PrintUsage(); //when the wrong arguments usage.
			return 1;
		}
	}

	// print usage when there are more than six arguments
	if ( argc>6 ){
		PrintUsage();
		return 1;
	}

	// Detect interactive mode (if no macro file name) and define UI session // when the arguments has not -m part.
	//
	//	std::cout<<macro.size()<<std::endl;
	//	std::cout<<G4UI_USE_QT<<std::endl;
	//	std::cout<<G4VIS_USE_OI<<std::endl;
	if ( !macro.size() ) { //when the macro size is not 0
#ifdef G4UI_USE_QT
		ui = new G4UIExecutive(argc, argv, "csh");

#else
		G4cerr<<"ERROR: Interactive mode is not available. Please provide macro file."<<G4endl;
		return 1;
#endif
	}
	// default output file name
	else if ( !output.size() ) \
	output ="./data/liontea/sonde/geant4/latest/"+ macro + ".out"; 
	// when the output file is not defined, the default output file's name is macro.out

	// Choose the Random engine
	//
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4Random::setTheSeed(time(NULL));

	// Construct the default run manager
	//
#ifdef G4MULTITHREADED
	//	G4RunManager * runManager = new G4RunManager;
	G4MTRunManager * runManager = new G4MTRunManager;
	runManager->SetNumberOfThreads(10);

#else
	G4RunManager * runManager = new G4RunManager;
#endif

	// Set a class to import phantom data
	//
	TETModelImport* tetData = new TETModelImport(isAF, ui); // set the variance tetData as command for TETModelImport.

	// Set mandatory initialisation classes
	//
	// detector construction
	runManager->SetUserInitialization(new TETDetectorConstruction(tetData));
	// physics list
	runManager->SetUserInitialization(new TETPhysicsList());
	// user action initialisation
	runManager->SetUserInitialization(new TETActionInitialization(tetData, output));

#ifdef G4VIS_USE_OI
	// Visualization manager
	//
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialise();
#endif

	// Process macro or start UI session
	//
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if ( ! ui ){
		// batch mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+macro);
	}
	else {
		// interactive mode
#ifdef G4VIS_USE_OI
		UImanager->ApplyCommand("/control/execute init_vis.mac");
#endif
		ui->SessionStart();
#ifdef G4VIS_USE_OI
		delete visManager;
#endif
		delete ui;
	}

	// Job termination
	//
	delete runManager;
}


