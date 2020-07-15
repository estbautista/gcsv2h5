#ifndef PARSE_EDGE_FILE_H
#define PARSE_EDGE_FILE_H

#include "gcsv2h5.hpp"

void parseEdgeFile(fileObj &, dataProp &, dataObj &);
void efErrCheck(fileObj &ef);
void edgeExtractProperties(fileObj &, dataProp &, dataObj &);
void trimWord(std::string& line);
void handleNodesMap(const std::string &, dataProp &);
void handleTimesMap(const std::string &, dataProp &);
void sortData(dataProp &, dataObj &);

#endif
