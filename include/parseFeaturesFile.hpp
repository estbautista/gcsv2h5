#ifndef PARSE_FEATURES_FILE_H
#define PARSE_FEATURES_FILE_H

#include "gcsv2h5.hpp"

struct featFileObj : fileObj {
	std::string IDColName;
	std::string gtColName;
	unsigned int gtColNum;
	unsigned int IDColNum;

	// constructor
	featFileObj(std::string fn, fDelim del, std::string gt) 
		: fileObj(fn, del), gtColName(gt) {}
};

void parseFeaturesFile(featFileObj &, dataProp &, dataObj &);
void gtFindColumn(featFileObj &, dataProp &);
#endif
