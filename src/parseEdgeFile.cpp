#include "parseEdgeFile.hpp"
#include <sstream>
#include <regex>
#include <iostream>
#include <string>
#include <numeric>

void parseEdgeFile(fileObj &ef, dataProp &dp, dataObj &dat){	

	// Open file
	ef.open(fMode::read);

	// Check formating errors
	efErrCheck(ef);

	// Extract properties
	edgeExtractProperties(ef, dp, dat);

	// Sort file
	sortData(dp, dat);

	// Close file
	ef.close();
}

void efErrCheck(fileObj &ef){

	// Variables
	std::string line, tmp;
	std::vector<std::string> parsedLine;

	// Parse first line
	std::getline(ef.file, line);
	std::stringstream sline(line);
	while(std::getline(sline, tmp, (char)ef.delim)){ 
		trimWord(tmp);
		parsedLine.push_back(tmp);
	}

	// Check for errors
	if( parsedLine.size() < 2 )
		throw terminateProgramException("Error in edgelist: no source and target");
	else if( parsedLine.size() < 3 ){
		std::cout << "No weights detected: all set to 1.\nNo temporal data detected." << std::endl;
	}
	else if( parsedLine.size() < 4 ){
		std::cout << "No temporal data detected." << std::endl;
	}
	else if( parsedLine.size() > 4 ){
		std::string MSG = "More than 4 columns in edgelist file.\n"
			"Please clean your file according to the following format:\n" 
			"| source | target | weight(opt) | timestamp(opt) |";
		throw terminateProgramException(MSG);
	}

	// Reset file pointer
	ef.file.clear();
	ef.file.seekg(0, std::ios::beg);
}

void edgeExtractProperties(fileObj &ef, dataProp &dp, dataObj &dat){
	// Variables
	std::string default_stamp{"0"}, line, tmp;
	std::vector<std::string> parsedLine; 
	parsedLine.reserve(4);
	char cdelim = (char)ef.delim;

	// Read and extract properties
	while(std::getline(ef.file, line)){		
		// Get line entries and clean them from spaces
		std::stringstream sline(line);	
		while(std::getline(sline,tmp, cdelim)){ 
			trimWord(tmp);
			parsedLine.push_back(tmp);
		}	

		// Map nodes names to indices
		handleNodesMap(parsedLine.at(0), dp);
		handleNodesMap(parsedLine.at(1), dp);

		// Fill missing info if not provided
		if( parsedLine.size() == 2 ){
			parsedLine.push_back("+1");
			parsedLine.push_back(default_stamp);
		}else if( parsedLine.size() == 3 ){
			parsedLine.push_back(default_stamp);
		}

		// Number of timestamps and edges per timestamp 
		handleTimesMap(parsedLine.at(3),dp);

		auto it = dat.data.find( dp.timesMapID[parsedLine.at(3)]);
		if(it == dat.data.end()){
			dataObj::stampData myStamp;
			myStamp.source.push_back(dp.nodesMapID[parsedLine.at(0)]);
			myStamp.target.push_back(dp.nodesMapID[parsedLine.at(1)]);
			myStamp.weight.push_back(std::stof(parsedLine.at(2)));
			dat.data.insert(std::make_pair( dp.timesMapID[parsedLine.at(3)], myStamp));
		}else{
			it->second.source.push_back(dp.nodesMapID[parsedLine.at(0)]);
			it->second.target.push_back(dp.nodesMapID[parsedLine.at(1)]);
			it->second.weight.push_back(std::stof(parsedLine.at(2)));
		}
						
		// clear vector
		parsedLine.clear();
	}

	// Reset file pointer
	ef.file.clear();
	ef.file.seekg(0, std::ios::beg);
}

void trimWord(std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    line = line.substr(start, end - start + 1);
}

void handleNodesMap(const std::string& nodeName, dataProp &dp){	
	// search if nodeName is in the map
	std::unordered_map<std::string, unsigned long>::iterator it = dp.nodesMapID.find( nodeName );

	// If not, then add its key and value
	if(it == dp.nodesMapID.end()){
		dp.nodesMapID.insert(std::make_pair(nodeName, dp.numNodes));
		dp.numNodes++;
	}
}

void handleTimesMap(const std::string& currTimeName, dataProp &dp){
	// search if timestamp is in the map
	std::unordered_map<std::string, unsigned long>::iterator it = dp.timesMapID.find( currTimeName );

	// If its not in the map then add its key and value
	if(it == dp.timesMapID.end()){
		dp.timesMapID.insert(std::make_pair( currTimeName, dp.numTimestamps ));
		dp.numTimestamps++;
	}
}

void sortData(dataProp &dp, dataObj &dat){	

	for(int i = 0; i < dp.numTimestamps; i++){
		// Set index vector to sort
		std::vector<unsigned long> indices(dat.data[i].source.size());
		auto it_beg = indices.begin();
		auto it_end = indices.end();
		int x = 0;
		std::iota(it_beg, it_end, x++);

		// Sort indices with my custom rule
		sort(it_beg, it_end, [&](unsigned long u, unsigned long v){
			     		if( dat.data[i].target[u] == dat.data[i].target[v] )
						return dat.data[i].source[u] < dat.data[i].source[v];
					else
						return dat.data[i].target[u] < dat.data[i].target[v]; });	
		// Update the value in map
		dataObj::stampData myStamp;
		for(auto &x : indices){
			myStamp.source.push_back( dat.data[i].source[x] );
			myStamp.target.push_back( dat.data[i].target[x] );
			myStamp.weight.push_back( dat.data[i].weight[x] );
		}
		dat.data[i] = myStamp;
	}
}
