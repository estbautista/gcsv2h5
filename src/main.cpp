#include "gcsv2h5.hpp"
#include <iostream>

void printINFO(){
	std::cerr << "usage : ./gcsv2h5 <param1> <param2> <param3> <param4(opt)> <param5(opt)> " << std::endl;
	std::cerr << "\tParam1: edgelist file name with format | Source | Target | Weight(opt) | Timestamp(opt) |" << std::endl;
	std::cerr << "\tParam2: h5 output file name " << std::endl;
	std::cerr << "\tParam3: delimiter, it can be one of {comma, tab, space}" << std::endl;
	std::cerr << "\tParam4: features file name (optional, first column must contain IDs that match names of vertices in edgelist)" << std::endl;
	std::cerr << "\tParam5: name of column of features file that will be used as ground truth (optional, only if param4 is provided)" << std::endl;

}

fDelim testDelim(std::string &tst){
	if(tst == "comma")
		return comma;
	else if(tst == "tab")
		return tab;
	else if(tst == "space")
		return space;
	else{
		printINFO();
		throw terminateProgramException("Error in delimiter");	
	}
}

int main(int argc, char* argv[]){	

	// Put args in a vector
	std::vector<std::string> myArgs;
	for(int i = 1; i < argc; i++){
		myArgs.push_back(argv[i]);
	}

	try{
		if (argc < 4){ printINFO(); }
		else if (argc == 4){
			fDelim myDelim = testDelim(myArgs[2]);
			gcsv2h5(myArgs[0], myArgs[1], myDelim);	
		}else if (argc == 5){
			fDelim myDelim = testDelim(myArgs[2]);
			gcsv2h5(myArgs[0], myArgs[1], myDelim, myArgs[3]);	
		}else if (argc == 6){
			fDelim myDelim = testDelim(myArgs[2]);
			gcsv2h5(myArgs[0], myArgs[1], myDelim, myArgs[3], myArgs[4]);	
		}
	}				
	catch(terminateProgramException &e){
		std::cerr << e.errInFile << std::endl;
	}
	return 0;
}
