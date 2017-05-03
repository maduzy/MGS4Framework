#include "DdsHandler.h"
#include <iostream>

void DdsHandler::writeDdsToFile(DDS dds, std::string filename) {

	FileHandler ddsOut;
	ddsOut.open(filename.c_str(), std::ios::binary | std::ios::out);

	//write header
	ddsOut.write((char *)&dds.header.magic, 4);
	ddsOut.write((char *)&dds.header.size, 4);
	ddsOut.write((char *)&dds.header.flags, 4);
	ddsOut.write((char *)&dds.header.height, 4);
	ddsOut.write((char *)&dds.header.width, 4);
	ddsOut.write((char *)&dds.header.pitch, 4);
	ddsOut.write((char *)&dds.header.depth, 4);
	ddsOut.write((char *)&dds.header.mipMapCount, 4);

	for (int i = 0; i < dds.header.reserved.size(); i++) {
		ddsOut.write((char *)&dds.header.reserved[i], 4);
	}

	ddsOut.write((char *)&dds.header.pixelFormat.size, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.flags, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.fourCC, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.rgbBitCount, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.rBitCount, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.gBitCount, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.bBitCount, 4);
	ddsOut.write((char *)&dds.header.pixelFormat.aBitCount, 4);

	ddsOut.write((char *)&dds.header.caps, 4);
	ddsOut.write((char *)&dds.header.caps2, 4);
	ddsOut.write((char *)&dds.header.caps3, 4);
	ddsOut.write((char *)&dds.header.caps4, 4);
	ddsOut.write((char *)&dds.header.reserved2, 4);

	ddsOut.write(dds.ddsData.textureData, dds.ddsData.textureSize);

	ddsOut.close();
}

DDS DdsHandler::makeDds(uint32_t height, uint32_t width, uint32_t fourCC, uint32_t mipMapCount, char* texture, uint32_t textureSize) {
	DDS dds;

	dds.header.magic = 0x20534444;
	dds.header.size = 0x7C;
	dds.header.flags = 0x00021007;
	dds.header.height = height;
	dds.header.width = width;
	dds.header.pitch = 0;
	dds.header.depth = 0;
	dds.header.mipMapCount = mipMapCount;
	
	for (int i = 0; i < dds.header.reserved.size(); i++) {
		dds.header.reserved[i] = 0x00000000;
	}

	dds.header.pixelFormat.size = 0x20;
	dds.header.pixelFormat.flags = 0x05;
	dds.header.pixelFormat.fourCC = fourCC;
	dds.header.pixelFormat.rgbBitCount = 0;
	dds.header.pixelFormat.rBitCount = 0;
	dds.header.pixelFormat.gBitCount = 0;
	dds.header.pixelFormat.bBitCount = 0;
	dds.header.pixelFormat.aBitCount = 0;
	dds.header.caps = 0x00401008;
	dds.header.caps2 = 0;
	dds.header.caps3 = 0;
	dds.header.caps4 = 0;
	dds.header.reserved2 = 0;

	dds.ddsData.textureData = texture;
	dds.ddsData.textureSize = textureSize;

	return dds;
}

DDS DdsHandler::parseDds(FileHandler* ddsFile) {
	
	DDSHeader ddsHeader;

	//get filesize
	ddsFile->seekg(0, ddsFile->end);
	int length = ddsFile->tellg();
	ddsFile->seekg(0, ddsFile->beg);

	

	//read header
	ddsFile->read((char*)&ddsHeader.magic, 4);
	ddsFile->read((char*)&ddsHeader.size, 4);
	ddsFile->read((char*)&ddsHeader.flags, 4);
	ddsFile->read((char*)&ddsHeader.height, 4);
	ddsFile->read((char*)&ddsHeader.width, 4);
	ddsFile->read((char*)&ddsHeader.pitch, 4);
	ddsFile->read((char*)&ddsHeader.depth, 4);
	ddsFile->read((char*)&ddsHeader.mipMapCount, 4);
	

	for (int i = 0; i < 11; i++) {
		ddsFile->read((char*)&ddsHeader.reserved[i], 4);
	}
	

	//read pixelformat
	DDS_PIXELFORMAT ddsPixelFormat;

	ddsFile->read((char*)&ddsPixelFormat.size, 4);
	ddsFile->read((char*)&ddsPixelFormat.flags, 4);
	ddsFile->read((char*)&ddsPixelFormat.fourCC, 4);
	ddsFile->read((char*)&ddsPixelFormat.rgbBitCount, 4);
	ddsFile->read((char*)&ddsPixelFormat.rBitCount, 4);
	ddsFile->read((char*)&ddsPixelFormat.gBitCount, 4);
	ddsFile->read((char*)&ddsPixelFormat.bBitCount, 4);
	ddsFile->read((char*)&ddsPixelFormat.aBitCount, 4);

	std::cout << ddsPixelFormat.size << std::endl;

	//add it to header
	ddsHeader.pixelFormat = ddsPixelFormat;

	ddsFile->read((char*)&ddsHeader.caps, 4);
	ddsFile->read((char*)&ddsHeader.caps2, 4);
	ddsFile->read((char*)&ddsHeader.caps3, 4);
	ddsFile->read((char*)&ddsHeader.caps4, 4);
	ddsFile->read((char*)&ddsHeader.reserved2, 4);


	
	//read data
	DDSData ddsData;

	ddsData.textureSize = length - ddsFile->tellg();

	ddsData.textureData = new char[ddsData.textureSize];

	ddsFile->read(ddsData.textureData, ddsData.textureSize);

	/*
	int mainDataSize = ddsHeader.height * ddsHeader.width;

	if (ddsHeader.pixelFormat.fourCC == 0x31545844) {
		mainDataSize /= 2;
	}

	ddsData.mainData = new char[mainDataSize];

	ddsFile->read(ddsData.mainData, mainDataSize);

	int mipMapDataSize = length - ddsFile->tellg();

	ddsData.mipMapData = new char[mipMapDataSize];

	ddsFile->read(ddsData.mipMapData, mainDataSize);
	*/
	DDS dds;
	dds.header = ddsHeader;
	dds.ddsData = ddsData;

	return dds;
}