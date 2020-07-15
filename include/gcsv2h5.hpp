#ifndef GCSV2H5_H
#define GCSV2H5_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

enum fDelim: char { comma = ',', tab = '\t', space = ' '};
enum fMode : unsigned { read, write, closed };

struct dataProp{
	unsigned long numTimestamps;
	unsigned long numFeatures;
	unsigned long numNodes;
	unsigned long gtColNum;
	std::unordered_map<std::string, unsigned long> nodesMapID;	
	std::unordered_map<std::string, unsigned long> timesMapID;
	std::vector<std::string> featuresNames;

	// Constructor
	dataProp() : numTimestamps(0), numFeatures(0), numNodes(0) {}
};

struct dataObj{
	struct stampData{
		std::vector<unsigned long> source;
		std::vector<unsigned long> target;
		std::vector<float> weight;
	};

	std::unordered_map<unsigned long, stampData> data;	
	std::unordered_map<unsigned long, std::vector<float>> feat;
};

struct fileObj{
	std::fstream file;
	std::string fileName;
	fDelim delim;

	// Constructor
	fileObj(){}
	fileObj( std::string fn, fDelim fd ) : fileName(fn), delim(fd) {}

	// Methods
	void open(fMode mode){
		if( mode == read ){ file.open(fileName, std::ios::in); }
		else if( mode == write){ file.open(fileName, std::ios::out|std::ios::app); }
	}
	void close(){ file.close(); }
	void init(){ 
		file.open(fileName, std::ios::out|std::ios::trunc);
		file.close();
	}
};

struct terminateProgramException{
	std::string errInFile;	
	terminateProgramException(std::string msg) : errInFile(msg) {}
};

void trimWord(std::string &);
/*
	gcsv2h5( param1, param2, param3, param4(opt), param5(opt)
	----
	param1: edge list file name 
	param2: h5 output file name
	param3: delimiter. It can be one of {fDelim::comma, fDelim::tab, fDelim::space}
	param4: features file name (optional, leave blank if no features supported on graph vertices)
	param5: name of column of features file name that will be used as ground truth
*/
void gcsv2h5(std::string, std::string, fDelim, std::string ="NO_FILE", std::string="NO_COL");

#endif
