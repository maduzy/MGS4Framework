#ifndef DDSHANDLER_H
#define DDSHANDLER_H

#include "DDS.h"
#include "FileHandler.h"

class DdsHandler {
public:
	DDS makeDds(uint32_t height, uint32_t width, uint32_t fourCC, uint32_t mipMapCount, char* texture, uint32_t textureSize);
	void writeDdsToFile(DDS dds, std::string filename);
	DDS parseDds(FileHandler* ddsFile);
};

#endif //DDSHANDLER_H