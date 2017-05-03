#include "DarHandler.h"
#include "FileHandler.h"
#include <string>
#include <iostream>

void DarHandler::extract(Dar dar) {

	CreateDirectory("Extract", NULL);

	for (int i = 0; i < dar.fileCount; i++) {

		std::string filename = "Extract\\" + dar.file[i].filename;

		FileHandler outFile;

		outFile.open(filename.c_str(), std::ios::binary | std::ios::out);
		outFile.write(dar.file[i].data, dar.file[i].fileSize);
		outFile.close();
	}
}


DarFileData DarHandler::makeDarDataFile(std::string fileName) {

	DarFileData data;

	//open data File
	FileHandler darDataFile;
	darDataFile.openFile(fileName);

	//get fileSize
	int fileSize = darDataFile.getFileSize();

	//init fileData buffer
	char * fileData = new char[fileSize];

	//read data to buffer
	darDataFile.read(fileData, fileSize);

	//init darDataFile
	data.filename = fileName;
	data.fileSize = fileSize;
	data.data = fileData;

	//close file
	darDataFile.close();

	//return it
	return data;

}

void DarHandler::writeDarToFile(Dar dar) {

	uint32_t fileCount = dar.file.size();


	FileHandler darOut;
	darOut.open("cache.dar", std::ios::binary | std::ios::out);

	//write file count
	darOut.writeBigEndian(&fileCount, 4);

	for (int i = 0; i < dar.file.size(); i++) {

		//write filename to output file
		darOut.write(dar.file[i].filename.c_str(), dar.file[i].filename.length());

		//align to next info
		darOut.smallAlign();

		//write file size to output file
		darOut.writeBigEndian(&dar.file[i].fileSize, 4);

		//byte align
		darOut.byteAlign(16);

		//write file data to output file
		darOut.write(dar.file[i].data, dar.file[i].fileSize);

		//write zero byte at end
		darOut.writeZeroes(1);

	}

	darOut.close();

}

Dar DarHandler::makeDar(std::string directory) {

	Dar dar;
	FileHandler folder;
	std::vector<WIN32_FIND_DATA> file;
	file = folder.fromDirectory(directory, "*.*");

	dar.fileCount = file.size();

	for (int i = 0; i < file.size(); i++) {

		//open data file
		FileHandler darDataFile;
		std::string filename = file[i].cFileName;
		std::string path = directory + file[i].cFileName;
		darDataFile.openFile(path);

		//resize dar file vector
		dar.file.resize(i + 1);
		dar.file[i].filename = file[i].cFileName;
		dar.file[i].fileSize = file[i].nFileSizeLow;

		//read the data into a buffer and add to dar file vector
		char * fileData = new char[file[i].nFileSizeLow];
		darDataFile.read(fileData, file[i].nFileSizeLow);
		dar.file[i].data = fileData;

		//close data file
		darDataFile.close();
	}

	return dar;

}

Dar DarHandler::parseDar(std::string filename) {

	FileHandler darFile;
	darFile.openFile(filename.c_str());

	Dar dar;

	darFile.readBigEndian(&dar.fileCount, 4);

	for (int i = 0; i < dar.fileCount; i++) {

		DarFileData fileData;

		//read filename then go back before null pointer
		std::getline(darFile, fileData.filename, '\0');
		darFile.seekg(-1, std::ios::cur);

		//align to next information
		darFile.smallAlign();

		//read filesize
		darFile.readBigEndian(&fileData.fileSize, 4);

		// byte alignment
		darFile.byteAlign(16);

		//init char with space for file
		fileData.data = new char[fileData.fileSize];

		//read file as char
		darFile.read(fileData.data, fileData.fileSize);
		
		//add all the info to the dar's file vector
		dar.file.push_back(fileData);

		darFile.seekg(1, std::ios::cur);

	}

	darFile.close();
	return dar;
}