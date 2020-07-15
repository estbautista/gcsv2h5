#include "parseFeaturesFile.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>


void parseFeaturesFile(featFileObj &ff, dataProp &dp, dataObj &dat){

	// Verify existence of file
	if(ff.fileName == "NO_FILE"){
		return;
	}
		
	// Open the file
	ff.open(fMode::read);

	// Get the first line (header)
	std::string line, tmp;	
	std::getline(ff.file, line);
	std::stringstream sline(line);
	while(std::getline(sline, tmp, (char)ff.delim)){
		tmp.erase(std::remove(tmp.begin(),tmp.end(),'"'),tmp.end());
		dp.featuresNames.push_back(tmp);	
	}

	// Check errors
	if( (dp.featuresNames.size() < 2) )
		throw terminateProgramException("Error in features file.");

	// Set GT column
	gtFindColumn(ff, dp);

	// Set ID Col if nodes names are used
	ff.IDColName = dp.featuresNames[0]; 
	ff.IDColNum = 0;

	// Name and number of features
	dp.featuresNames.erase(dp.featuresNames.begin() + ff.gtColNum);
	dp.featuresNames.erase(dp.featuresNames.begin()); 
	dp.numFeatures = dp.featuresNames.size();

	// reserve memory numfeatures + gt
	for(int i = 0; i < dp.numFeatures + 1; i++)
		dat.feat.insert(std::make_pair(i, std::vector<float> (dp.numNodes, 0)));

	// Read features
	char cdelim = (char)ff.delim;
	unsigned long index;

	// Load the feature columns into vectors in a map (each key is a column)
	while(std::getline(ff.file, line)){	
		unsigned count(0);
		std::stringstream sline(line);
		while(std::getline(sline, tmp, cdelim)){
			trimWord(tmp);
			if(count == 0){
				index = dp.nodesMapID[tmp];
			}else{
				dat.feat[count-1].at(index) = std::stof(tmp);
			}
			count++;
		}
	}	
	// gt item in the map container
	dp.gtColNum = ff.gtColNum - 1;

	// Close file
	ff.file.close();
}

void gtFindColumn(featFileObj &ff, dataProp &dp){
	bool success = false;
	while(!success){
		// Find which column matches the ground truth
		std::vector<std::string>::iterator it;
		it = find(dp.featuresNames.begin(), dp.featuresNames.end(), ff.gtColName);
		unsigned dist = std::distance(dp.featuresNames.begin(),it);

		// If not found ask for a correction
		if( dist < dp.featuresNames.size() ){
			ff.gtColNum = dist;
			success = true;
		}else{
			std::cout << "Ground Truth column not found. " << std::endl;
			std::cout << "Please re-introduce column name: ";
			std::cin >> ff.gtColName;
		}
	}
}
