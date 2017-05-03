#ifndef SEGHANDLER_H
#define SEGHANDLER_H

#include "FileHandler.h"
#include "TxnHandler.h"
#include "Seg.h"
#include "Dlz.h"

class SegHandler {
public:
	Seg SegHandler::parseSeg(FileHandler* segFile);
	char* SegHandler::unpackSeg(Seg seg);
	Dlz SegHandler::parseDlz(std::string filename);
	char* SegHandler::unpackDlz(Dlz dlz, std::string filename, int* size);
	void SegHandler::writeSegToFile(Seg seg, FileHandler& file);
	void SegHandler::writeDlzToFile(Dlz dlz, FileHandler& file);
	Dlz SegHandler::makeDlz(std::vector<DataContainer> dataContainers);
};


#endif //SEGHANDLER_H