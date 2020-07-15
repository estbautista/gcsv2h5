#ifndef H5_FILE_SETUP_H
#define H5_FILE_SETUP_H

#include "gcsv2h5.hpp"
#include "parseFeaturesFile.hpp"
#include "H5Cpp.h"

struct H5FileObj {
	H5::H5File file;
	std::string fileName;

	H5FileObj(std::string fn)
		: fileName(fn), file(fn, H5F_ACC_TRUNC){}
};

void writeToH5(H5FileObj &, dataProp &, featFileObj &, dataObj &);
void writeFeatures(H5FileObj &, dataProp &);
void writeNumFeatures(H5FileObj &, dataProp &);
void writeTimestamps(H5FileObj &, dataProp &);
void writeNodes(H5FileObj &, dataProp &);
template <typename T> 
void writeDataSet(H5::H5File &, std::string, std::vector<unsigned long> &, T*);

#endif
