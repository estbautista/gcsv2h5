#include "H5FileSetup.hpp"
#include <iostream>
#include <sstream>
#include <regex>


void writeToH5(H5FileObj &h5f, dataProp &dp, featFileObj &ff, dataObj &dat){	

	// Open file 
	h5f.file.openFile(h5f.fileName, H5F_ACC_RDWR);

	// If features file exists and it has features, set their names as attributes
	if(dp.numFeatures > 0){
		writeFeatures(h5f, dp);
	}

	// Write the number of timestamps, features and nodes as an attribute
	{
		writeNumFeatures(h5f,dp);
		writeTimestamps(h5f, dp);
		writeNodes(h5f, dp);
	}

	// Create one dataset for each timestamp edgelists and weights	
	for(auto &elem : dat.data){
		std::vector<unsigned long> dims_el, dims_w;
		dims_el.push_back( elem.second.source.size() );
		dims_el.push_back( 2 );
		dims_w.push_back( elem.second.source.size() );

		unsigned long buffer[dims_el[0]][dims_el[1]];
		for(int i = 0; i < dims_el[0]; i++){
			buffer[i][0] = elem.second.source[i];
			buffer[i][1] = elem.second.target[i];
		}

		// Write data
		writeDataSet(h5f.file, "EdgeList_"+std::to_string(elem.first), dims_el, &buffer[0][0]);
		writeDataSet(h5f.file, "Weights_"+std::to_string(elem.first), dims_w, elem.second.weight.data());
	}

	// Write names of nodes 
	{
		std::vector<const char *> vectCarrays(dp.numNodes);
		for(auto it = dp.nodesMapID.begin(); it != dp.nodesMapID.end(); it++){
			vectCarrays.at(it->second) = (it->first.c_str());
		}
		std::vector<unsigned long> dims;
		dims.push_back(vectCarrays.size());
		writeDataSet(h5f.file, "NameOfNodes", dims, vectCarrays.data());
	}
	
	// Set features
	if(dp.numFeatures > 0){
		float buffer[dp.numNodes][dp.numFeatures];
		int count(0);

		for(int f = 0; f < dp.numFeatures + 1; f++){
			if(f != dp.gtColNum){
				for(int i = 0; i < dat.feat[f].size(); i++){
					buffer[i][count] = dat.feat[f].at(i);
				}
				count++;
			}
		}
		std::vector<unsigned long> dims;
		dims.push_back(dp.numNodes); dims.push_back(dp.numFeatures);
		writeDataSet(h5f.file, "Features", dims, &buffer[0][0]);
	}

	// Set ground truth
	if(ff.fileName != "NO_FILE" ){
		std::vector<unsigned long> dims;
		dims.push_back(dp.numNodes);
		writeDataSet(h5f.file, "GroundTruth", dims, dat.feat[dp.gtColNum].data());
	}

	// Close File
	h5f.file.close();
}

void writeFeatures(H5FileObj &h5f, dataProp &dp){

	// Cast Feature names as standard C arrays and remove the ground truth column
	std::vector<const char*> vectCarrays;
	std::vector<std::string>::iterator it;
	for( it = dp.featuresNames.begin(); it != dp.featuresNames.end(); it++){
		vectCarrays.push_back(it->c_str());
	}

	// Set dataspace
	hsize_t dataDims[1] ={vectCarrays.size()};
        H5::DataSpace dsSpace(1, dataDims);

	// Define the variable length datatype
        H5::StrType varLenStrType(H5::PredType::C_S1, H5T_VARIABLE);

	// Write features as an attribute in the root group
        H5::Attribute attribute = h5f.file.createAttribute("FeaturesNames", varLenStrType, dsSpace);
        attribute.write(varLenStrType, vectCarrays.data());
}

void writeNumFeatures(H5FileObj &h5f, dataProp &dp){
	// Set dataspace
	hsize_t dataDims[1] = {1};
        H5::DataSpace dsSpace(1, dataDims);

	// Write features as an attribute in the root group
        H5::Attribute attribute = h5f.file.createAttribute("NumberFeatures", H5::PredType::NATIVE_ULONG, dsSpace);
        attribute.write(H5::PredType::NATIVE_ULONG, &dp.numFeatures);
}

void writeTimestamps(H5FileObj &h5f, dataProp &dp){
	// Set dataspace
	hsize_t dataDims[1] = {1};
        H5::DataSpace dsSpace(1, dataDims);

	// Write features as an attribute in the root group
        H5::Attribute attribute = h5f.file.createAttribute("NumberTimestamps", H5::PredType::NATIVE_ULONG, dsSpace);
        attribute.write(H5::PredType::NATIVE_ULONG, &dp.numTimestamps);
}

void writeNodes(H5FileObj &h5f, dataProp &dp){
	// Set dataspace
	hsize_t dataDims[1] = {1};
        H5::DataSpace dsSpace(1, dataDims);

	// Write features as an attribute in the root group
        H5::Attribute attribute = h5f.file.createAttribute("NumberNodes", H5::PredType::NATIVE_ULONG, dsSpace);
        attribute.write(H5::PredType::NATIVE_ULONG, &dp.numNodes);
}

template <typename T> 
void writeDataSet(H5::H5File &file, std::string dataSetName, std::vector<unsigned long> &dims, T* dataToWrite){
	hsize_t dataSetDims[dims.size()];
	for(int i = 0; i < dims.size(); i++)
		dataSetDims[i] = dims.at(i);
	H5::DataSpace dataspace(dims.size(), dataSetDims);

	//Act according to T either Float or Variable Length Char
	if (typeid(T) == typeid(float)){
		H5::DataSet dataset = file.createDataSet(dataSetName, H5::PredType::NATIVE_FLOAT, dataspace);
		dataset.write(dataToWrite, H5::PredType::NATIVE_FLOAT);
		dataset.close();
	}else if(typeid(T) == typeid(unsigned long)){
		H5::DataSet dataset = file.createDataSet(dataSetName, H5::PredType::NATIVE_ULONG, dataspace);
		dataset.write(dataToWrite, H5::PredType::NATIVE_ULONG);
		dataset.close();
	}else if(typeid(T) == typeid(const char *)){
		// Define the variable length datatype
        	H5::StrType varLenStrType(H5::PredType::C_S1, H5T_VARIABLE);	
		H5::DataSet dataset = file.createDataSet(dataSetName, varLenStrType, dataspace);
		dataset.write(dataToWrite, varLenStrType);
		dataset.close();
	}else{
		std::string MSG = "Error while initializing H5 " + dataSetName + " dataset";
		throw terminateProgramException(MSG);
	}
	//Close DataSpace
	dataspace.close();
}
