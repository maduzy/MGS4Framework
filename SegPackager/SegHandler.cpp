#include "SegHandler.h"
#include "ZlibWrapper.h"
#include <iostream>

//131072
/*
std::copy(
std::istream_iterator<char>(*tempBuffer),
std::istream_iterator<char>(),
std::back_inserter(out));
*/

int getTotalUncompressedSize(Dlz dlz) {
	int segCount = dlz.seg.size();
	int uncompressedDataSize = 0;

	for (int i = 0; i < segCount; i++) {
		uncompressedDataSize += dlz.seg[i].header.uncompressedSize;
	}

	return uncompressedDataSize;
}

//calculates seg size but also adds offsets to index
int calculateSegSize(Seg& seg) {
	int size = 0x10;
	int chunks = seg.header.chunkCount;
	size += (chunks * 0x08);

	
	//byte alignment
	if (size % 16 != 0) {
		size += 0x08;
	}
	

	//chunk sizes
	for (int i = 0; i < chunks; i++) {

		seg.header.segIndex[i].chunkOffset = (size + 1);

		size += seg.header.segIndex[i].chunkCompressedSize;
		int skip = (16 - (size % 16));
		if (skip != 16) {
			size += skip;
		}
		
	}

	return size;

}

//write a dlz to file
void SegHandler::writeDlzToFile(Dlz dlz, FileHandler& file) {

	int segCount = dlz.seg.size();

	for (int i = 0; i < segCount; i++) {
		writeSegToFile(dlz.seg[i], file);

		//byte alignment
		if (i != (segCount - 1)) {
			int padding = (131072 - (file.tellg() % 131072));
			if (padding != 131072) {
				file.writeZeroes(padding);
			}
		}
	}

	// byte alignment
	int padding = (2048 - (file.tellg() % 2048));
	if (padding != 2048) {
		file.writeZeroes(padding);
	}
	

}

//write a seg to file
void SegHandler::writeSegToFile(Seg seg, FileHandler& outFile) {



	outFile.writeBigEndian(&seg.header.magic, 4);
	outFile.writeBigEndian(&seg.header.flag, 2);
	outFile.writeBigEndian(&seg.header.chunkCount, 2);
	outFile.writeBigEndian(&seg.header.uncompressedSize, 4);
	outFile.write((char*)&seg.header.compressedSize, 4);
	
	//write segIndexes
	for (int i = 0; i < seg.header.chunkCount; i++) {
		outFile.writeBigEndian(&seg.header.segIndex[i].chunkCompressedSize, 2);
		outFile.writeBigEndian(&seg.header.segIndex[i].chunkUncompressedSize, 2);
		outFile.writeBigEndian(&seg.header.segIndex[i].chunkOffset, 4);
	}

	//byte alignment
	if (outFile.tellg() % 16 != 0) {
		outFile.writeZeroes(8);
	}
	
	for (int i = 0; i < seg.header.chunkCount; i++) {
		outFile.write(&seg.data.compressedData[i][0], seg.header.segIndex[i].chunkCompressedSize);

		//byte alignment
		int padding = (16 - (outFile.tellg() % 16));
		if (padding != 16) {
			outFile.writeZeroes(padding);
		}
	}


}

//make a dlz from compressed data
Dlz SegHandler::makeDlz(std::vector<DataContainer> dataContainers) {

	int totalChunks = dataContainers.size();
	int y = 0;

	Dlz dlz;

	Seg seg;
	seg.header.magic = 0x73656773;
	seg.header.flag = 4;
	seg.header.chunkCount = 0;
	seg.header.uncompressedSize = 0;
	seg.header.compressedSize = 0;

	for (int i = 0; i < totalChunks; i++) {

		//if its going to go over 131072
		if (seg.header.compressedSize + dataContainers[i].compressedSize > 131072) {

			//add current seg to vector
			dlz.seg.push_back(seg);

			//and start a new one
			y = 0;
			seg.header.uncompressedSize = 0;
		}

		seg.header.segIndex.resize(y + 1);
		seg.header.segIndex[y].chunkCompressedSize = dataContainers[i].compressedSize;
		seg.header.segIndex[y].chunkUncompressedSize = dataContainers[i].uncompressedSize;
		seg.header.segIndex[y].chunkOffset = 0;


		seg.data.compressedData.resize(y + 1);
		seg.data.compressedData[y].resize(dataContainers[i].compressedSize);

		for (int n = 0; n < dataContainers[i].compressedSize; n++) {
			seg.data.compressedData[y][n] = dataContainers[i].compressedData[n];
		}

		seg.header.chunkCount = seg.data.compressedData.size();

		seg.header.uncompressedSize += dataContainers[i].uncompressedSize;
		seg.header.compressedSize = calculateSegSize(seg);

		y++;
	}

	dlz.seg.push_back(seg);

	std::cout << dlz.seg.size() << std::endl;
	return dlz;
}

//takes a seg file and creates a struct from it
Seg SegHandler::parseSeg(FileHandler* segFile) {

	int fileBeginning = segFile->tellg();

	//build the header
	SegHeader segHeader;

	segFile->readBigEndian(&segHeader.magic, 4);
	segFile->readBigEndian(&segHeader.flag, 2);
	segFile->readBigEndian(&segHeader.chunkCount, 2);
	segFile->readBigEndian(&segHeader.uncompressedSize, 4);
	segFile->read((char *)&segHeader.compressedSize, 4);
	
	//build the index
	std::vector<SegIndex> segIndex(segHeader.chunkCount);
	
	for (int i = 0; i < segHeader.chunkCount; i++) {
		segFile->readBigEndian(&segIndex[i].chunkCompressedSize, 2);
		segFile->readBigEndian(&segIndex[i].chunkUncompressedSize, 2);
		segFile->readBigEndian(&segIndex[i].chunkOffset, 4);
	}

	segHeader.segIndex = segIndex;

	//byte alignment
	if (segFile->tellg() % 16 != 0) {
		segFile->seekg(0x08, segFile->cur);
	}

	//build the data
	SegData segData;
	std::vector< std::vector<char> > segDataVector;

	for (int i = 0; i < segHeader.chunkCount; i++) {
		int offset = (segIndex[i].chunkOffset - 1 + fileBeginning);
		segFile->seekg(offset, segFile->beg);
		std::vector<char> data(segIndex[i].chunkCompressedSize);
		segFile->read(&data[0], segIndex[i].chunkCompressedSize);
		segDataVector.push_back(data);

	}
	
	segData.compressedData = segDataVector;

	//make the seg
	Seg seg;

	seg.header = segHeader;
	seg.data = segData;

	return seg;
}

//takes a dlz file and creates a readable structure from its contents
Dlz SegHandler::parseDlz(std::string filename) {
	Dlz dlz;

	//open file
	FileHandler dlzFile;
	dlzFile.openFile(filename.c_str());

	//get filesize
	dlzFile.seekg(0, dlzFile.end);
	int length = dlzFile.tellg();
	dlzFile.seekg(0, dlzFile.beg);
	
	//iterate all segs in dlz
	Seg aSeg;
	while (length - dlzFile.tellg() > 131072) {

		int segStart = dlzFile.tellg();
		aSeg = parseSeg(&dlzFile);
		int currentPos = dlzFile.tellg();
		currentPos = currentPos - segStart;
		int padding = 131072 - currentPos;

		dlzFile.seekg(padding, dlzFile.cur);
		dlz.seg.push_back(aSeg);
	}

	//add the last seg
	aSeg = parseSeg(&dlzFile);
	dlz.seg.push_back(aSeg);

	//close the file
	dlzFile.close();

	return dlz;
}

char* SegHandler::unpackDlz(Dlz dlz, std::string filename, int* size) {

	int segCount = dlz.seg.size();

	*size = getTotalUncompressedSize(dlz);

	char * uncompressedData = new char[*size];

	int position = 0;

	for (int i = 0; i < segCount; i++) {

		char * uncompressedSegData;
		uncompressedSegData = unpackSeg(dlz.seg[i]);

		memcpy(&uncompressedData[position], uncompressedSegData, dlz.seg[i].header.uncompressedSize);

		position += dlz.seg[i].header.uncompressedSize;
	}

	
	std::ofstream outFile(filename.c_str(), std::ios::binary | std::ios::out);
	outFile.write(uncompressedData, *size);
	outFile.close();

	return uncompressedData;
}

char* SegHandler::unpackSeg(Seg seg) {

	ZlibWrapper zlib;
	int after;

	char * uncompressedData = new char[seg.header.uncompressedSize];

	int position = 0;

	for (int i = 0; i < seg.header.chunkCount; i++) {

		int uncompLength = seg.header.segIndex[i].chunkUncompressedSize;
		int compLength = seg.header.segIndex[i].chunkCompressedSize;
		char * data = &seg.data.compressedData[i][0];
		char * output = new char[uncompLength];

		zlib.zlibInflate(data, output, compLength, uncompLength, &after);

		memcpy(&uncompressedData[position], output, after);
		position += seg.header.segIndex[i].chunkUncompressedSize;
	}

	return uncompressedData;

}