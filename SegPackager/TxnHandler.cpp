#include "TxnHandler.h"
#include "DdsHandler.h"
#include "ZlibWrapper.h"
#include "Txn.h"
#include "HexOps.h"
#include <map>
#include <iostream>
#include <algorithm>

std::vector<DataContainer> TxnHandler::compress() {
	
	FileHandler texFile;
	ZlibWrapper zlib;
	texFile.openFile("testinun9.seg");

	//get filesize
	texFile.seekg(0, texFile.end);
	int length = texFile.tellg();
	texFile.seekg(0, texFile.beg);
	int i = 1;

	char * uncompressedData = new char[16384];
	
	int after;

	std::vector<DataContainer> dataContainers;

	while (length - texFile.tellg() > 16384) {
		
		char * compressedData = new char[16384];

		texFile.read(uncompressedData, 16384);

		zlib.zlibDeflate(uncompressedData, compressedData, 16384, &after);

		DataContainer dataContainer;

		dataContainer.compressedSize = after;
		dataContainer.uncompressedSize = 16384;
		dataContainer.compressedData = compressedData;

		dataContainers.push_back(dataContainer);

	}

	int left = length - texFile.tellg();

	char * compressedData = new char[16384];

	texFile.read(uncompressedData, left);

	zlib.zlibDeflate(uncompressedData, compressedData, left, &after);

	DataContainer dataContainer;

	dataContainer.compressedSize = after;
	dataContainer.uncompressedSize = left;
	dataContainer.compressedData = compressedData;

	dataContainers.push_back(dataContainer);

	texFile.close();
	
	return dataContainers;

}

void TxnHandler::ddsFromTxn(Txn txn) {
	DdsHandler ddsHandler;


	CreateDirectory("TxnExtract\\", NULL);
	std::string directory = "TxnExtract\\" + intToHexString(txn.txnBody.txnIndex2[0].objectId) + "\\";
	CreateDirectory(directory.c_str(), NULL);


	for (int i = 0; i < txn.txnBody.txnHeader.texCount; i++) {

		int textureSize = txn.textureData[i].mainDataSize + txn.textureData[i].mipMapDataSize;

		char * texture = new char[textureSize];

		memcpy(&texture[0], txn.textureData[i].mainData, txn.textureData[i].mainDataSize);
		memcpy(&texture[txn.textureData[i].mainDataSize], txn.textureData[i].mipMapData, txn.textureData[i].mipMapDataSize);

		std::string fourCC;
		switch (txn.txnBody.txnIndex[i].fourCC) {
		case DXT1:
			fourCC = "DXT1";
			break;
		case DXT5:
			fourCC = "DXT5";
			break;
		default:
			fourCC = "DXT1";
			break;
		}

		DDS dds;
		int mipMapCount = log2(max(txn.txnBody.txnIndex[i].height, txn.txnBody.txnIndex[i].width));
		dds = ddsHandler.makeDds(txn.txnBody.txnIndex[i].height, txn.txnBody.txnIndex[i].width, charToUInt32(fourCC.c_str()), mipMapCount, texture, textureSize);

		std::string filename = directory + intToHexString(txn.txnBody.txnIndex2[i].materialId) + ".dds";

		ddsHandler.writeDdsToFile(dds, filename);

		delete texture;
	}

}

void TxnHandler::ddsFromTxn2(Txn txn) {
	DdsHandler ddsHandler;

	//make directories
	CreateDirectory("TxnExtract\\", NULL);
	std::string directory = "TxnExtract\\" + intToHexString(txn.txnBody.txnIndex2[0].objectId) + "\\";
	CreateDirectory(directory.c_str(), NULL);

	for (int i = 0; i < txn.txnBody.txnHeader.texCount2; i++) {
		int entry = ((txn.txnBody.txnIndex2[i].offset - 0x20) / 0x10);

		std::cout << "entry:" << entry << std::endl;

		int textureSize = txn.textureData[entry].mainDataSize + txn.textureData[entry].mipMapDataSize;

		char * texture = new char[textureSize];

		memcpy(&texture[0], txn.textureData[entry].mainData, txn.textureData[entry].mainDataSize);
		memcpy(&texture[txn.textureData[entry].mainDataSize], txn.textureData[entry].mipMapData, txn.textureData[entry].mipMapDataSize);

		std::string fourCC;
		switch (txn.txnBody.txnIndex[entry].fourCC) {
		case DXT1:
			fourCC = "DXT1";
			break;
		case DXT5:
			fourCC = "DXT5";
			break;
		default:
			fourCC = "DXT1";
			break;
		}

		DDS dds;
		int mipMapCount = log2(max(txn.txnBody.txnIndex[entry].height, txn.txnBody.txnIndex[entry].width));
		dds = ddsHandler.makeDds(txn.txnBody.txnIndex[entry].height, txn.txnBody.txnIndex[entry].width, charToUInt32(fourCC.c_str()), mipMapCount, texture, textureSize);

		std::string filename = directory + intToHexString(txn.txnBody.txnIndex2[entry].materialId) + ".dds";

		ddsHandler.writeDdsToFile(dds, filename);

		delete texture;

	}
}
void TxnHandler::writeTxnToFile(Txn txn) {
	FileHandler txnOut;
	txnOut.open("txnout.txn", std::ios::binary | std::ios::out);

	//write header
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.nullBytes, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.flags, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.texCount, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.indexOffset, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.texCount2, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.index2Offset, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.nullBytes2, 4);
	txnOut.writeBigEndian(&txn.txnBody.txnHeader.nullBytes3, 4);

	//write first index
	for (int i = 0; i < txn.txnBody.txnIndex.size(); i++) {
		txnOut.writeBigEndian(&txn.txnBody.txnIndex[i].width, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex[i].height, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex[i].fourCC, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex[i].flag, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex[i].offset, 4);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex[i].mipMapOffset, 4);
	}

	//write second index
	for (int i = 0; i < txn.txnBody.txnIndex2.size(); i++) {
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].unknown, 4);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].materialId, 4);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].objectId, 4);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].width, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].height, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].xPosition, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].yPosition, 2);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].offset, 4);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].nullBytes2, 4);
		txnOut.writeBigEndianFloat(&txn.txnBody.txnIndex2[i].xWeight, 4);
		txnOut.writeBigEndianFloat(&txn.txnBody.txnIndex2[i].yWeight, 4);
		txnOut.writeBigEndianFloat(&txn.txnBody.txnIndex2[i].xWeight2, 4);
		txnOut.writeBigEndianFloat(&txn.txnBody.txnIndex2[i].yWeight2, 4);
		txnOut.writeBigEndian(&txn.txnBody.txnIndex2[i].nullBytes3, 4);
	}

	txnOut.byteAlign(0x80);
	
	//write files
	for (int i = 0; i < txn.txnBody.txnIndex.size(); i++) {

		if (txn.textureData[i].mainData != NULL) {
			txnOut.write(txn.textureData[i].mainData, txn.textureData[i].mainDataSize);
			txnOut.write(txn.textureData[i].mipMapData, txn.textureData[i].mipMapDataSize);
		} else {
			txnOut.write(txn.textureData[i].mipMapData, txn.textureData[i].mipMapDataSize);
		}

		txnOut.byteAlign(0x80);

	}

	txnOut.close();

}

Txn TxnHandler::parseTxn(std::string filename) {

	FileHandler txnFile;
	txnFile.openFile(filename.c_str());

	Txn txn;

	//read header
	txnFile.readBigEndian(&txn.txnBody.txnHeader.nullBytes, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.flags, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.texCount, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.indexOffset, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.texCount2, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.index2Offset, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.nullBytes2, 4);
	txnFile.readBigEndian(&txn.txnBody.txnHeader.nullBytes3, 4);

	//read first index
	for (int i = 0; i < txn.txnBody.txnHeader.texCount; i++) {
		txn.txnBody.txnIndex.resize(i + 1);
		txnFile.readBigEndian(&txn.txnBody.txnIndex[i].width, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex[i].height, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex[i].fourCC, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex[i].flag, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex[i].offset, 4);
		txnFile.readBigEndian(&txn.txnBody.txnIndex[i].mipMapOffset, 4);
	}

	//read second index
	for (int i = 0; i < txn.txnBody.txnHeader.texCount2; i++) {
		txn.txnBody.txnIndex2.resize(i + 1);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].unknown, 4);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].materialId, 4);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].objectId, 4);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].width, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].height, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].xPosition, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].yPosition, 2);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].offset, 4);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].nullBytes2, 4);
		txnFile.readBigEndianFloat(&txn.txnBody.txnIndex2[i].xWeight, 4);
		txnFile.readBigEndianFloat(&txn.txnBody.txnIndex2[i].yWeight, 4);
		txnFile.readBigEndianFloat(&txn.txnBody.txnIndex2[i].xWeight2, 4);
		txnFile.readBigEndianFloat(&txn.txnBody.txnIndex2[i].yWeight2, 4);
		txnFile.readBigEndian(&txn.txnBody.txnIndex2[i].nullBytes3, 4);
	}

	txnFile.byteAlign(0x80);

	return txn;
}

void TxnHandler::getTexForIndex(uint32_t type, uint32_t txnId, int n, std::vector<Tex> texs, std::vector<TextureData>& textureData) {
	for (int i = 0; i < texs.size(); i++) {
		if ((texs[i].texHeader.type == type) && (texs[i].texHeader.hashId == txnId) && (texs[i].texHeader.entryNumber == n)) {
			textureData.resize(n + 1);

			if (type == 0x02031000) {
				textureData[n].mipMapData = texs[i].texData.data;
				textureData[n].mipMapDataSize = texs[i].texHeader.dataSize;
			} else {
				textureData[n].mainData = texs[i].texData.data;
				textureData[n].mainDataSize = texs[i].texHeader.dataSize;
			}
			//type == 0x02031000 ? (textureData[n].mipMapData = texs[i].texData.data) : (textureData[n].mainData = texs[i].texData.data);
			break;
		}
	}
}

void TxnHandler::attachTexToTxn(Txn& txn, std::vector<Tex> texs) {
	uint32_t txnId = txn.txnBody.txnIndex2[0].objectId;

	std::vector<TextureData> textureData;
	for (int i = 0; i < txn.txnBody.txnHeader.texCount; i++) {
		getTexForIndex(0x02001000, txnId, i, texs, textureData);
		getTexForIndex(0x02031000, txnId, i, texs, textureData);
	}

	txn.textureData = textureData;
}

void TxnHandler::parseTex(char* uncompressedData, int size, std::vector<Tex>& texs) {

	std::cout << "size is:" << size << std::endl;

	membuf sbuf(uncompressedData, size);
	std::istream in_stream(&sbuf);

	while ((int(in_stream.tellg()) + 64) < size) {
		Tex tex;
		in_stream.read((char*)&tex.texHeader.type, 4);
		tex.texHeader.type = _byteswap_ulong(tex.texHeader.type);
		//std::cout << std::hex << tex.texHeader.type << std::endl;

		in_stream.read((char*)&tex.texHeader.null, 4);
		//std::cout << std::hex << tex.texHeader.null << std::endl;

		in_stream.read((char*)&tex.texHeader.hashId, 4);
		tex.texHeader.hashId = _byteswap_ulong(tex.texHeader.hashId);
		//std::cout << std::hex << tex.texHeader.hashId << std::endl;

		in_stream.read((char*)&tex.texHeader.parentDataSize, 4);
		tex.texHeader.parentDataSize = _byteswap_ulong(tex.texHeader.parentDataSize);
		//std::cout << std::hex << tex.texHeader.parentDataSize << std::endl;

		in_stream.read((char*)&tex.texHeader.dataSize, 4);
		tex.texHeader.dataSize = _byteswap_ulong(tex.texHeader.dataSize);
		//std::cout << std::hex << tex.texHeader.dataSize << std::endl;

		in_stream.read((char*)&tex.texHeader.mipMapCount, 4);
		tex.texHeader.mipMapCount = _byteswap_ulong(tex.texHeader.mipMapCount);
		//std::cout << std::hex << tex.texHeader.mipMapCount << std::endl;

		in_stream.read((char*)&tex.texHeader.entryNumber, 4);
		tex.texHeader.entryNumber = _byteswap_ulong(tex.texHeader.entryNumber);
		//std::cout << std::hex << tex.texHeader.entryNumber << std::endl;

		in_stream.read((char*)&tex.texHeader.padding, 4);
		tex.texHeader.padding = _byteswap_ulong(tex.texHeader.padding);
		//std::cout << std::hex << tex.texHeader.padding << std::endl;

		//init fileData buffer
		char * fileData = new char[tex.texHeader.dataSize];

		//read data to buffer
		in_stream.read(fileData, tex.texHeader.dataSize);

		//assign it to the tex
		tex.texData.data = fileData;

		//add to vector
		texs.push_back(tex);

		//byte align
		int skip = (16 - (in_stream.tellg() % 16));
		if (skip != 16) {
			in_stream.seekg(skip, std::ios::cur);
		}

		std::cout << std::hex << "tellg:" << in_stream.tellg() << " vec size:" << texs.size() << std::endl;
	}
	

	//return texs;

}

TxnHandler::~TxnHandler() {
	// Deallocate the memory that was previously reserved
}


/*
void TxnHandler::createTxn(std::vector<DDS> ddsVector) {
	
	

	TxnHeader txnHeader;

	txnHeader.flags = 0x0120; //temporary
	txnHeader.texCount = ddsVector.size();
	txnHeader.indexOffset = 0x20;
	txnHeader.texCount2 = ddsVector.size();
	
	TxnIndex txnIndex;

	txnIndex.height = ddsVector[0].header.height;
	txnIndex.width = ddsVector[0].header.width;
	txnIndex.fourCC = 0x09; //temporary


	/*
	uint32_t nullBytes;
	uint32_t flags;
	uint32_t texCount;
	uint32_t indexOffset;
	uint32_t texCount2;
	uint32_t index2Offset;
	uint32_t nullBytes2;
	uint32_t nullBytes3;*/

//}