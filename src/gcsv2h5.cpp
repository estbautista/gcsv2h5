#include "gcsv2h5.hpp"
#include "parseEdgeFile.hpp"
#include "parseFeaturesFile.hpp"
#include "H5FileSetup.hpp"

#include <iostream>

void gcsv2h5(std::string edgeFN, std::string h5FN, fDelim fdel, std::string featFN, std::string gtCN){

	// Object for data properties
	dataProp dataPropObj;
	dataObj data;

	// Parse edgeFile 
	std::cout << "Parsing edgefile... " << std::endl;
	fileObj edgeObj(edgeFN, fdel);
	parseEdgeFile(edgeObj, dataPropObj, data);

	// Parse Features File
	std::cout << "Checking and Parsing features file... " << std::endl;
	featFileObj featObj(featFN, fdel, gtCN);
	parseFeaturesFile(featObj, dataPropObj, data);

	// Initialize H5 file
	std::cout << "Writing data to H5 File.." << std::endl;
	H5FileObj H5Obj(h5FN);
	writeToH5(H5Obj, dataPropObj, featObj, data);

	std::cout << "Succesfull operation." << std::endl;
}
