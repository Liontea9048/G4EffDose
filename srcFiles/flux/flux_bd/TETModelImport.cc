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
// * technical work of the GEANT4 collaboration.                      *phantomName
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// TETModelImport.cc
// \file   MRCP_GEANT4/External/src/TETModelImport.cc
// \author Haegin Han
//

#include "TETModelImport.hh"

TETModelImport::TETModelImport(G4bool isAF, G4UIExecutive* ui)
{
	// set path for phantom data
	char* pPATH = getenv("PHANTOM_PATH"); // pPath -> get Phantom_Path
	if( pPATH == 0 ){
		// exception for the case when PHANTOM_PATH environment variable was not set
		G4Exception("TETModelImport::TETModelImport","",JustWarning,
				G4String("PHANTOM_PATH environment variable was not set.").c_str());
		// default path for phantom data
//		phantomDataPath = "../phantoms";
		phantomDataPath = "/data/liontea/MRCP/geant4/phantoms";
	}
	else {
		// set path for phantom data as PHANTOM_PATH
		phantomDataPath = pPATH;
	}

	// set phantom name
	if(!isAF) phantomName = "AM-bending"; //If the Phantom name is not a AF -> Set the PhantomName as MRCP_AM
	else      phantomName = "AM-bending"; //If the Phantom name is a AF -> Set the PhantomName as MRCP_AF
	

	G4cout << "================================================================================"<<G4endl;
	G4cout << "\t" << phantomName << " was implemented in this CODE!!   "<< G4endl;
	G4cout << "================================================================================"<<G4endl;

	G4String eleFile      =  phantomName + ".ele"; //element file
	G4String nodeFile     =  phantomName + "-shifted.node"; // node file AM-squatting-shifted
	G4String materialFile =  phantomName + ".material"; //material file

	// read phantom data files (*. ele, *.node)
	DataRead(eleFile, nodeFile);
	// read material file (*.material)
	MaterialRead(materialFile);
	// read colour data file (colour.dat) if this is interactive mode
	if(ui) ColourRead();
	// print the summary of phantom information
	PrintMaterialInfomation();
}

void TETModelImport::DataRead(G4String eleFile, G4String nodeFile)
{
	G4String tempStr;
	G4int tempInt;

	// Read *.node file
	//
	std::ifstream ifpNode;

	ifpNode.open((phantomDataPath + "/" + nodeFile).c_str()); // access to the MRCP_AM or AF.node
	if(!ifpNode.is_open()) {
		// exception for the case when there is no *.node file
		G4Exception("TETModelImport::DataRead","",FatalErrorInArgument,
				G4String("      There is no " + nodeFile ).c_str());
	}
	G4cout << "  Opening TETGEN node (vertex points: x y z) file '" << nodeFile << "'" <<G4endl; //node file name printing

	G4int numVertex;
	G4double xPos, yPos, zPos;
	G4double xMin(DBL_MAX), yMin(DBL_MAX), zMin(DBL_MAX);
	G4double xMax(DBL_MIN), yMax(DBL_MIN), zMax(DBL_MIN);

	ifpNode >> numVertex >> tempInt >> tempInt >> tempInt;

	for(G4int i=0; i<numVertex; i++)
	{
		ifpNode >> tempInt >> xPos >> yPos >> zPos; // 0 1 23 ,,, / x/y/z position.

		// set the unit
		xPos*=cm;
		yPos*=cm;
		zPos*=cm;

		// save the node data as the form of std::vector<G4ThreeVector>
		vertexVector.push_back(G4ThreeVector(xPos, yPos, zPos));

		// to get the information of the bounding box of phantom
		if (xPos < xMin) xMin = xPos;
		if (xPos > xMax) xMax = xPos;
		if (yPos < yMin) yMin = yPos;
		if (yPos > yMax) yMax = yPos;
		if (zPos < zMin) zMin = zPos;
		if (zPos > zMax) zMax = zPos; //find the boundary of Box that the Phantom can be confined.
	}

	// set the variables for the bounding box and phantom size
	boundingBox_Min = G4ThreeVector(xMin,yMin,zMin);
	boundingBox_Max = G4ThreeVector(xMax,yMax,zMax);
	phantomSize = G4ThreeVector(xMax-xMin,yMax-yMin,zMax-zMin);

	ifpNode.close();

	// Read *.ele file
	//
	std::ifstream ifpEle;

	ifpEle.open((phantomDataPath + "/" + eleFile).c_str());
	if(!ifpEle.is_open()) {
		// exception for the case when there is no *.ele file
		G4Exception("TETModelImport::DataRead","",FatalErrorInArgument,
				G4String("      There is no " + eleFile ).c_str());
	}
	G4cout << "  Opening TETGEN elements (tetrahedron with node No.) file '" << eleFile << "'" <<G4endl;

	G4int numEle;
	ifpEle >> numEle  >> tempInt >> tempInt;

	for(G4int i=0; i<numEle; i++)
	{
		ifpEle >> tempInt;
		G4int* ele = new G4int[4];
		for(G4int j=0;j<4;j++){
			ifpEle >> tempInt;
			ele[j]=tempInt;
		}
		eleVector.push_back(ele);
		ifpEle >> tempInt;
		materialVector.push_back(tempInt);

		// save the element (tetrahedron) data as the form of std::vector<G4Tet*>
		tetVector.push_back(new G4Tet("Tet_Solid",
							   		  vertexVector[ele[0]],
									  vertexVector[ele[1]],
									  vertexVector[ele[2]],
									  vertexVector[ele[3]])); //tetvector[ele[0],1,2,3]

		// calculate the total volume and the number of tetrahedrons for each organ
		std::map<G4int, G4double>::iterator FindIter = volumeMap.find(materialVector[i]);
//		std::map<G4int, G4double>::iterator FindIter_surface = surfaceMap.find(materialVector[i]);
		if(FindIter!=volumeMap.end()){ //if the FindIter is not the end;
			FindIter->second += tetVector[i]->GetCubicVolume(); // get the ele[0]'s volume (cubic)
//			FindIter->second += tetVector[i]->GetSurfaceArea(); // get the ele[0]'s surface area.
//			FindIter_surface->second += tetVector[i]->GetSurfaceArea();
			numTetMap[materialVector[i]]++;
		}
		else {
			volumeMap[materialVector[i]] = tetVector[i]->GetCubicVolume();
//			cout << "volume :" << volumeMap[materialVector[i]]
//			surfaceMap[materialVector[i]] = tetVector[i]->GetSurfaceArea();
			numTetMap[materialVector[i]] = 1;
		}
	}
	ifpEle.close();
}

void TETModelImport::MaterialRead(G4String materialFile)
{
	// Read material file (*.material)
	//
	std::ifstream ifpMat;

	ifpMat.open((phantomDataPath + "/" + materialFile).c_str());
	if(!ifpMat.is_open()) {
		// exception for the case when there is no *.material file
		G4Exception("TETModelImport::DataRead","",FatalErrorInArgument,
				G4String("      There is no " + materialFile ).c_str());
	}

	G4cout << "  Opening material file '" << materialFile << "'" <<G4endl;

	char read_data[50];
	char* token;
	G4double zaid;
	G4double fraction;
	G4String MaterialName;
	G4double density;

	while(!ifpMat.eof()) // if the Material file does not end.
	{
		ifpMat >> read_data;                   //ex) 'C' RBM
		ifpMat >> MaterialName;                //ex)  C 'RBM'
		ifpMat >> read_data;
		density = std::atof(read_data);        //ex) 1.30
		ifpMat >> read_data;                   //ex) g/cm3
		ifpMat >> read_data;
		token = std::strtok(read_data,"m");
		G4int matID = std::atoi(token);        //ex) m'10'
		materialIndex.push_back(matID);
		organNameMap[matID]= MaterialName;
		densityMap[matID] = density*g/cm3;

		for(G4int i=0 ;  ; i++)
		{
			ifpMat >> read_data;
			if(std::strcmp(read_data, "C")==0 || ifpMat.eof()) break;

			zaid = (G4int)(std::atoi(read_data)/1000);
			ifpMat >> read_data;
			fraction = -1.0 * std::atof(read_data);
			materialIndexMap[matID].push_back(std::make_pair(G4int(zaid), fraction));
		}
	}
	ifpMat.close();

	// // Construct materials for each organ
	// //
	// G4NistManager* nistManager = G4NistManager::Instance();

	// for(G4int i=0;i<(G4int)materialIndex.size();i++){
	// 	G4int idx = materialIndex[i];
	// 	G4Material* mat = new G4Material(organNameMap[idx], densityMap[idx], G4int(materialIndexMap[idx].size()), kStateSolid, NTP_Temperature, STP_Pressure);
	// 	for(G4int j=0;j<G4int(materialIndexMap[idx].size());j++){
	// 		mat->AddElement(nistManager->FindOrBuildElement(materialIndexMap[idx][j].first), materialIndexMap[idx][j].second);
	// 	}
	// 	materialMap[idx]=mat;
	// 	massMap[idx]=densityMap[idx]*volumeMap[idx];
	// }
	// Construct materials for each organ
	//
	G4NistManager* nistManager = G4NistManager::Instance();
	G4Element *TH = new G4Element("TS_H_of_Water", "H", 1., 1.01*g/mole);

	for(G4int i=0;i<(G4int)materialIndex.size();i++){
		G4int idx = materialIndex[i];
		G4Material* mat = new G4Material(organNameMap[idx], densityMap[idx], G4int(materialIndexMap[idx].size()), kStateSolid, NTP_Temperature, STP_Pressure);
		for(G4int j=0;j<G4int(materialIndexMap[idx].size());j++){
			if (materialIndexMap[idx][j].first == 1)
				mat->AddElement(TH, materialIndexMap[idx][j].second);
			else
				mat->AddElement(nistManager->FindOrBuildElement(materialIndexMap[idx][j].first), materialIndexMap[idx][j].second);
		}
		materialMap[idx]=mat;
		massMap[idx]=densityMap[idx]*volumeMap[idx];
	}
}

void TETModelImport::ColourRead()
{
	// Read colour data file (colour.dat)
	//
	std::ifstream ifpColour;

	ifpColour.open((phantomDataPath + "/" + "colour.dat").c_str());
	if(!ifpColour.is_open()) {
		// exception for the case when there is no colour.dat file
		G4Exception("TETModelImport::DataRead","",FatalErrorInArgument,
				G4String("Colour data file was not found ").c_str());
	}

	G4cout << "  Opening colour data file 'colour.dat'" <<G4endl;

	G4int organID;
	G4double red, green, blue, alpha;
	while( ifpColour >> organID >> red >> green >> blue >> alpha )
		colourMap[organID] = G4Colour(red, green, blue, alpha);

	ifpColour.close();
}

void TETModelImport::PrintMaterialInfomation()
{
	// Print the overall information for each organ
	//
	G4cout << G4endl
		   << std::setw(9)  << "Organ ID"
		   << std::setw(11) << "# of Tet"
		   << std::setw(11) << "vol [cm3]"
		   << std::setw(11) << "d [g/cm3]"
		   << std::setw(11) << "mass [g]"
//		   << std::setw(11) << "surface [cm2]"
		   << "\t" << "organ/tissue"<< G4endl ;
	G4cout << "--------------------------------------------------------------------------------"<<G4endl;

	std::map<G4int, G4Material*>::iterator matIter;
	G4cout<<std::setiosflags(std::ios::fixed);
	G4cout.precision(3);
	for(matIter=materialMap.begin(); matIter!=materialMap.end();matIter++)
	{
		G4int idx = matIter->first;

		G4cout << std::setw(9)  << idx                         // organ ID
			   << std::setw(11) << numTetMap[idx]              // # of tetrahedrons
			   << std::setw(11) << volumeMap[idx]/cm3          // organ volume
			   << std::setw(11) << materialMap[idx]
			                       ->GetDensity()/(g/cm3)      // organ density
			   << std::setw(11) << massMap[idx]/g              // organ mass
//			   << std::setw(11) << surfaceMap[idx]/cm2		   // organ surface in cm^2
			   << "\t"<<materialMap[idx]->GetName() << G4endl; // organ name
	}
}
