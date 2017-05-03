#ifndef DARHANDLER_H
#define DARHANDLER_H

#include "Dar.h"

class DarHandler {
public:
	void extract(Dar dar);
	void writeDarToFile(Dar dar);
	DarFileData makeDarDataFile(std::string fileName);
	Dar makeDar(std::string directory);
	Dar parseDar(std::string filename);
};

#endif //DARHANDLER_H