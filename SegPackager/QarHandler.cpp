#include "QarHandler.h"
#include "FileHandler.h"
#include "MGSHash.h"
#include <algorithm>
#include <iostream>
#include <string>

bool sortByFileSize(WIN32_FIND_DATA a, WIN32_FIND_DATA b) {
	return a.nFileSizeLow > b.nFileSizeLow;
}

int getHeaderOffset(Qar qar) {

	int headerOffset = 0;

	for (int i = 0; i < qar.header.fileCount; i++) {
		headerOffset += qar.header.dataInfo[i].fileSize;
	}

	return headerOffset;
}

void QarHandler::extract(Qar qar) {

	CreateDirectory("QarExtract", NULL);

	for (int i = 0; i < qar.header.fileCount; i++) {

		std::string filename = "QarExtract\\" + qar.header.filename[i];

		FileHandler outFile;

		outFile.open(filename.c_str(), std::ios::binary | std::ios::out);
		outFile.write(qar.data.data[i], qar.header.dataInfo[i].fileSize);
		outFile.close();
	}

}

void QarHandler::writeQarToFile(Qar qar) {

	FileHandler qarOut;
	qarOut.open("cache.qar", std::ios::binary | std::ios::out);


	for (int i = 0; i < qar.header.fileCount; i++) {
		//write file data to output file
		qarOut.write(qar.data.data[i], qar.header.dataInfo[i].fileSize);
	}

	qarOut.writeBigEndian(&qar.header.fileCount, 2);
	qarOut.writeZeroes(2);

	for (int i = 0; i < qar.header.fileCount; i++) {
		//write index data to output file
		qarOut.writeBigEndian(&qar.header.dataInfo[i].fileInfo, 4);
		qarOut.writeBigEndian(&qar.header.dataInfo[i].fileSize, 4);
	}

	for (int i = 0; i < qar.header.fileCount; i++) {
		//write filename to output file
		qarOut.write(qar.header.filename[i].c_str(), qar.header.filename[i].length());
		qarOut.writeZeroes(1);
	}

	qarOut.seekg(-1, qarOut.cur);
	qarOut.smallAlign();
	qarOut.writeBigEndian(&qar.header.offset, 4);
	
	qarOut.close();
}

Qar QarHandler::makeQar(std::string directory) {

	Qar qar;
	FileHandler folder;
	MGSHash hashTool;
	std::vector<WIN32_FIND_DATA> file;

	file = folder.fromDirectory(directory, "*.txn");

	qar.header.fileCount = file.size();

	//sort vector by filesize
	std::sort(file.begin(), file.end(), sortByFileSize);

	//iterate
	for (int i = 0; i < file.size(); i++) {

		//open data file
		FileHandler qarDataFile;
		std::string filename = file[i].cFileName;
		std::string path = directory + file[i].cFileName;
		qarDataFile.openFile(path);

		//resize qar file vector
		qar.data.data.resize(i + 1);
		qar.header.dataInfo.resize(i + 1);
		qar.header.filename.resize(i + 1);
	
		//assign attributes
		qar.header.filename[i] = file[i].cFileName;
		qar.header.dataInfo[i].fileSize = file[i].nFileSizeLow;

		//get raw name of file type
		std::string rawName = qar.header.filename[i].substr(0, qar.header.filename[i].find_last_of("."));
		int32_t fileType = _byteswap_ulong(3);
		uint32_t hash = hashTool.GetIDFromString(rawName.c_str());

		//assign last attribute
		qar.header.dataInfo[i].fileInfo = fileType | hash;

		//read the data into a buffer and add to qar file vector
		char * fileData = new char[file[i].nFileSizeLow];
		qarDataFile.read(fileData, file[i].nFileSizeLow);
		qar.data.data[i] = fileData;

		//close data file
		qarDataFile.close();
	}

	qar.header.offset = getHeaderOffset(qar);

	return qar;

}

Qar QarHandler::parseQar(std::string filename) {

	FileHandler qarFile;
	qarFile.openFile(filename.c_str());

	Qar qar;

	//read header offset
	qarFile.seekg(-4, qarFile.end);
	qarFile.readBigEndian(&qar.header.offset, 4);

	//go to header offset
	qarFile.seekg(qar.header.offset, qarFile.beg);

	//get filecount
	qarFile.readBigEndian(&qar.header.fileCount, 2);

	//go to index start
	qarFile.seekg(2, qarFile.cur);

	std::cout << qarFile.tellg() << std::endl;

	//get index info
	for (int i = 0; i < qar.header.fileCount; i++) {
		qar.header.dataInfo.resize(i + 1);
		qarFile.readBigEndian(&qar.header.dataInfo[i].fileInfo, 4);
		qarFile.readBigEndian(&qar.header.dataInfo[i].fileSize, 4);
	}

	//get file names
	for (int i = 0; i < qar.header.fileCount; i++) {
		qar.header.filename.resize(i + 1);
		std::getline(qarFile, qar.header.filename[i], '\0');
	}

	//go back to start
	qarFile.seekg(0, qarFile.beg);

	//read the data
	for (int i = 0; i < qar.header.fileCount; i++) {
		
		//resize vector for each file
		qar.data.data.resize(i + 1);

		//make a buffer for the data and read it
		char * qarData = new char[qar.header.dataInfo[i].fileSize];
		qarFile.read(qarData, qar.header.dataInfo[i].fileSize);

		//add it to the qar object
		qar.data.data[i] = qarData;
	}

	qarFile.close();

	return qar;
}