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
// TETRunAction.cc
// \file   MRCP_GEANT4/External/src/TETRunAction.cc
// \author Haegin Han
//

#include "TETRunAction.hh"

TETRunAction::TETRunAction(TETModelImport* _tetData, G4String _output)
:tetData(_tetData), fRun(0), numOfEvent(0), runID(0), outputFile(_output)
{
	start = std::chrono::system_clock::now();
}

TETRunAction::~TETRunAction()
{}

G4Run* TETRunAction::GenerateRun()
{
	// generate run
	fRun = new TETRun();
	return fRun;
}


void TETRunAction::BeginOfRunAction(const G4Run* aRun)
{
	// print the progress at the interval of 10%
	numOfEvent=aRun->GetNumberOfEventToBeProcessed();
	G4RunManager::GetRunManager()->SetPrintProgress(int(numOfEvent*0.01));
}

void TETRunAction::EndOfRunAction(const G4Run* aRun)
{
	// print the result only in the Master
	if(!isMaster) return;

	// get the run ID
	runID = aRun->GetRunID();

	// Print the run result by G4cout and std::ofstream
	//
	// print by G4cout
	// PrintResult(G4cout);

	// print by std::ofstream
	// auto outputfile_name = outputFile + "_nDose.out";
	std::ofstream ofs(outputFile.c_str());
	PrintResult(ofs);
	ofs.close();

	// outputfile_name = outputFile + "pe_Dose.out";
	// std::ofstream ofs_pe(outputFile.c_str());
	// PrintResult_pe(ofs_pe);
	// ofs_pe.close();
}

void TETRunAction::PrintResult(std::ostream &out)
{
	// Print run result
	//
	using namespace std;
	EDEPMAP edepMap = *fRun->GetEdepMap();
	EDEPMAP edepMap_manual_n1 = *fRun->GetEdepMap_manual_n1();
	EDEPMAP edepMap_manual_n50 = *fRun->GetEdepMap_manual_n50();
	EDEPMAP edepMap_gem = *fRun->GetEdepMap_gem();
	EDEPMAP edepMap_hp = *fRun->GetEdepMap_hp();
	EDEPMAP edepMap_alpha = *fRun->GetEdepMap_alpha();

	out << G4endl
	    << "=====================================================================" << G4endl
	    << " Run #" << runID << " / Number of event processed : "<< numOfEvent     << G4endl
	    << "=====================================================================" << G4endl
	    << "organ ID| "
		<< setw(19) << "Organ Mass (g)"
		<< setw(19) << "Dose (Gy/source)"
		<< setw(19) << "Relative Error" << G4endl;

	out.precision(3);
	auto massMap = tetData->GetMassMap();
	for(auto itr : massMap){
		G4double meanDose    = edepMap[itr.first].first  / itr.second / numOfEvent;
		G4double squareDoese = edepMap[itr.first].second / (itr.second*itr.second);
		G4double variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
		G4double relativeE   = sqrt(variance)/meanDose;

		out << setw(8)  << itr.first  << setw(8) <<"| "
			<< setw(19) << fixed      << itr.second/g;
		out	<< setw(19) << scientific << meanDose/(joule/kg);
		out	<< setw(19) << fixed      << relativeE << G4endl;

		// // //manual	
		// meanDose    = edepMap_manual_n1[itr.first].first  / itr.second / numOfEvent;
		// squareDoese = edepMap_manual_n1[itr.first].second / (itr.second*itr.second);
		// variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
		// relativeE   = sqrt(variance)/meanDose;

		// out << setw(8)  << itr.first << setw(8) <<"| n1"
		// 	<< setw(19) << fixed      << itr.second/g;
		// out	<< setw(19) << scientific << meanDose/(joule/kg);
		// out	<< setw(19) << fixed      << relativeE << G4endl;

		// meanDose    = edepMap_manual_n50[itr.first].first  / itr.second / numOfEvent;
		// squareDoese = edepMap_manual_n50[itr.first].second / (itr.second*itr.second);
		// variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
		// relativeE   = sqrt(variance)/meanDose;

		// out << setw(8)  << itr.first << setw(8) <<"| n50"
		// 	<< setw(19) << fixed      << itr.second/g;
		// out	<< setw(19) << scientific << meanDose/(joule/kg);
		// out	<< setw(19) << fixed      << relativeE << G4endl;
		// // //manual

		// meanDose    = edepMap_gem[itr.first].first  / itr.second / numOfEvent;
		// squareDoese = edepMap_gem[itr.first].second / (itr.second*itr.second);
		// variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
		// relativeE   = sqrt(variance)/meanDose;

		// out << setw(8)  << itr.first << setw(8) <<"| gem"
		// 	<< setw(19) << fixed      << itr.second/g;
		// out	<< setw(19) << scientific << meanDose/(joule/kg);
		// out	<< setw(19) << fixed      << relativeE << G4endl;

		// meanDose    = edepMap_hp[itr.first].first  / itr.second / numOfEvent;
		// squareDoese = edepMap_hp[itr.first].second / (itr.second*itr.second);
		// variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
		// relativeE   = sqrt(variance)/meanDose;

		// out << setw(8)  << itr.first << setw(8) <<"| hp"
		// 	<< setw(19) << fixed      << itr.second/g;
		// out	<< setw(19) << scientific << meanDose/(joule/kg);
		// out	<< setw(19) << fixed      << relativeE << G4endl;

		// meanDose    = edepMap_alpha[itr.first].first  / itr.second / numOfEvent;
		// squareDoese = edepMap_alpha[itr.first].second / (itr.second*itr.second);
		// variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
		// relativeE   = sqrt(variance)/meanDose;

		// out << setw(8)  << itr.first << setw(8) <<"| alpha"
		// 	<< setw(19) << fixed      << itr.second/g;
		// out	<< setw(19) << scientific << meanDose/(joule/kg);
		// out	<< setw(19) << fixed      << relativeE << G4endl;
		// out << "=====================================================================" << G4endl << G4endl;
	}
	out << "=====================================================================" << G4endl << G4endl;
}

// void TETRunAction::PrintResult_pe(std::ostream &out)
// {
// 	// Print run result
// 	//
// 	using namespace std;
// 	EDEPMAP edepMap = *fRun->GetEdepMap_pe();

// 	out << G4endl
// 	    << "=====================================================================" << G4endl
// 	    << " Run #" << runID << " / Number of event processed : "<< numOfEvent     << G4endl
// 	    << "=====================================================================" << G4endl
// 	    << "organ ID| "
// 		<< setw(19) << "Organ Mass (g)"
// 		<< setw(19) << "Dose (Gy/source)"
// 		<< setw(19) << "Relative Error" << G4endl;

// 	out.precision(3);
// 	auto massMap = tetData->GetMassMap();
// 	for(auto itr : massMap){
// 		G4double meanDose    = edepMap[itr.first].first  / itr.second / numOfEvent;
// 		G4double squareDoese = edepMap[itr.first].second / (itr.second*itr.second);
// 		G4double variance    = ((squareDoese/numOfEvent) - (meanDose*meanDose))/numOfEvent;
// 		G4double relativeE   = sqrt(variance)/meanDose;

// 		out << setw(8)  << itr.first << "| "
// 			<< setw(19) << fixed      << itr.second/g;
// 		out	<< setw(19) << scientific << meanDose/(joule/kg);
// 		out	<< setw(19) << fixed      << relativeE << G4endl;
// 	}
// 	out << "=====================================================================" << G4endl << G4endl;

// std::chrono::_V2::system_clock::time_point end =  std::chrono::system_clock::now();
// std::chrono::duration<double> elapsed_seconds = end-start;
// std::time_t end_time = std::chrono::system_clock::to_time_t(end);
// G4cout << "finished computation at " << std::ctime(&end_time)
// << "elapsed time: " << elapsed_seconds.count() << "s\n";

// }
