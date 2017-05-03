#include "SlotHandler.h"
#include "FileTypes.h"
#include "HexOps.h"
#include <iostream>

void splitFilename(std::string file, std::string& rawName, std::string& extension) {

	rawName = file.substr(0, file.find_last_of("."));
	extension = file.substr(file.find_last_of(".") + 1);
}

void SlotHandler::writeSlotToFile(Slot slot) {

	FileHandler slotOut;
	slotOut.open("output.slot", std::ios::binary | std::ios::out);

	//write header
	slotOut.writeBigEndian(&slot.header.slotId, 4);
	slotOut.writeBigEndian(&slot.header.unknown, 2);
	slotOut.writeBigEndian(&slot.header.unknown2, 2);
	slotOut.writeBigEndian(&slot.header.indexCount, 2);
	slotOut.writeBigEndian(&slot.header.nullBytes, 4);
	slotOut.writeBigEndian(&slot.header.unknown3, 2);

	//byte align
	slotOut.byteAlign(0x800);

	for (int i = 0; i < slot.header.indexCount; i++) {
	
		//write index header
		slotOut.writeBigEndian(&slot.body.index[i].lineCount, 4);
		slotOut.writeBigEndian(&slot.body.index[i].nullBytes, 4);
		
		//write the slot rows
		for (int j = 0; j < slot.body.index[i].lineCount - 1; j++) {
			slotOut.writeBigEndian(&slot.body.index[i].row[j].fileInfo, 4);
			slotOut.writeBigEndian(&slot.body.index[i].row[j].compressedSize, 4);
			slotOut.writeBigEndian(&slot.body.index[i].row[j].nullBytes, 4);
			slotOut.writeBigEndian(&slot.body.index[i].row[j].offset, 4);
		}

		//not all rows are files
		int n = 0;

		//write out the files from a directory
		for (int j = 0; j < slot.body.index[i].lineCount - 1; j++) {

			//get fileType
			int fileType = _byteswap_ulong(slot.body.index[i].row[j].fileInfo & 0xFF000000);

			//handle fileType
			switch (fileType) {
			case 127:
				slotOut.byteAlign(0x800);
				break;
			default:
				int fileSize = (slot.body.index[i].row[j + 1].offset) - (slot.body.index[i].row[j].offset);
				slotOut.write(slot.body.index[i].fileData[n], fileSize);
				n++;
			}

		}

	}

	slotOut.close();

}

//slots are expected to have a directory 
Slot SlotHandler::makeSlot(std::string directory) {
	Slot slot;

	FileHandler fileHandler;

	std::vector<WIN32_FIND_DATA> directories;
	directories = fileHandler.getDirectories(directory);

	slot.header.slotId  =    0x4A41594B;
	slot.header.unknown =    0x01;
	slot.header.unknown2 =   0x01;
	slot.header.indexCount = directories.size();
	slot.header.nullBytes =  0x0000;
	slot.header.unknown3 = 0x01;

	for (int i = 0; i < slot.header.indexCount; i++) {

		std::string path = directory + directories[i].cFileName + "\\";

		std::vector<WIN32_FIND_DATA> innerDirectories;
		innerDirectories = fileHandler.getDirectories(path);

		slot.body.index.resize(i + 1);

		int currentRow = 0;
		int z = 0;
		for (int j = 0; j < innerDirectories.size(); j++) {

			path = directory + directories[i].cFileName + "\\" + innerDirectories[j].cFileName + "\\";

			std::vector<WIN32_FIND_DATA> files;
			files = fileHandler.fromDirectory(path, "*");

			slot.body.index[i].row.resize(currentRow + 1);

			slot.body.index[i].row[currentRow].fileInfo = std::stoi(innerDirectories[j].cFileName) ^ _byteswap_ulong(0x7F);
			slot.body.index[i].row[currentRow].compressedSize = 0;
			slot.body.index[i].row[currentRow].nullBytes = 0;

			currentRow++;

			int fileOffset = 0;

			for (int n = 0; n < files.size(); n++) {
				slot.body.index[i].row.resize(currentRow + 1);
				std::string rawName;
				std::string extension;

				splitFilename(files[n].cFileName, rawName, extension);

				slot.body.index[i].row[currentRow].fileInfo = std::stoul(rawName, nullptr, 16) ^ _byteswap_ulong(getFileTypeNumber(extension));
				slot.body.index[i].row[currentRow].compressedSize = 0;
				slot.body.index[i].row[currentRow].nullBytes = 0;
				slot.body.index[i].row[currentRow].offset = fileOffset;

				char * data = new char[files[n].nFileSizeLow];
				FileHandler file;
				file.openFile(path + "\\" + files[n].cFileName);
				slot.body.index[i].fileData.resize(z + 1);

				file.read(data, files[n].nFileSizeLow);

				slot.body.index[i].fileData[z] = data;

				z++;

				file.close();
				

				fileOffset += files[n].nFileSizeLow;
				currentRow++;
			}

			slot.body.index[i].row.resize(currentRow + 1);
			slot.body.index[i].row[currentRow].fileInfo = _byteswap_ulong(0x7F);
			slot.body.index[i].row[currentRow].compressedSize = 0;
			slot.body.index[i].row[currentRow].nullBytes = 0;
			slot.body.index[i].row[currentRow].offset = fileOffset;
			currentRow++;

		}

		slot.body.index[i].lineCount = slot.body.index[i].row.size() + 1;
		slot.body.index[i].nullBytes = 0;
	}
	return slot;
}

void SlotHandler::extract(Slot slot) {

	for (int i = 0; i < slot.header.indexCount; i++) {

		//make a directory for the index
		CreateDirectory("SlotExtract\\", NULL);
		CreateDirectory(("SlotExtract\\" + std::to_string(i)).c_str(), NULL);

		//set n as 0, define segment
		int n = 0;
		std::string segment;

		//read index rows
		for (int j = 0; j < slot.body.index[i].lineCount - 1; j++) {

			int fileType = _byteswap_ulong(slot.body.index[i].row[j].fileInfo & 0xFF000000);
			uint32_t fileName = slot.body.index[i].row[j].fileInfo & 0x00FFFFFF;
			

			if (fileType == 127) {
				segment = std::to_string(fileName);
				if (fileName != 0) CreateDirectory(("SlotExtract\\" + std::to_string(i) + "\\" + segment + "\\").c_str(), NULL);
			}

			//0x70 and above files are archival related
			if (fileType < 112) {
				//removed std::to_string(j) + "_" + 
				std::string directoryName = "SlotExtract\\" + std::to_string(i) + "\\" + segment + "\\" + intToHexString(fileName) + "." + FileTypes[fileType];
				std::cout << directoryName << std::endl;
				int fileSize = (slot.body.index[i].row[j + 1].offset) - (slot.body.index[i].row[j].offset);
				FileHandler outFile;

				outFile.open(directoryName.c_str(), std::ios::binary | std::ios::out);
				outFile.write(slot.body.index[i].fileData[n], fileSize);
				outFile.close();
				n++;
			}

		}
	}

}

Slot SlotHandler::parseSlot(std::string filename) {

	FileHandler slotFile;
	slotFile.openFile(filename.c_str());

	Slot slot;

	//read header
	slotFile.readBigEndian(&slot.header.slotId, 4);
	slotFile.readBigEndian(&slot.header.unknown, 2);
	slotFile.readBigEndian(&slot.header.unknown2, 2);
	slotFile.readBigEndian(&slot.header.indexCount, 2);
	slotFile.readBigEndian(&slot.header.nullBytes, 4);
	slotFile.readBigEndian(&slot.header.unknown3, 2);

	//byte align
	slotFile.byteAlign(0x800);

	//for each index
	for (int i = 0; i < slot.header.indexCount; i++) {

		//read index head
		slot.body.index.resize(i + 1);
		slotFile.readBigEndian(&slot.body.index[i].lineCount, 4);
		slotFile.readBigEndian(&slot.body.index[i].nullBytes, 4);

		//read index rows
		for (int j = 0; j < slot.body.index[i].lineCount - 1; j++) {
			slot.body.index[i].row.resize(j + 1);
			slotFile.readBigEndian(&slot.body.index[i].row[j].fileInfo, 4);
			slotFile.readBigEndian(&slot.body.index[i].row[j].compressedSize, 4);
			slotFile.readBigEndian(&slot.body.index[i].row[j].nullBytes, 4);
			slotFile.readBigEndian(&slot.body.index[i].row[j].offset, 4);
		}

		//read slot file data for given index row
		int n = 0;
		for (int j = 0; j < slot.body.index[i].lineCount - 1; j++) {

			int fileType = _byteswap_ulong(slot.body.index[i].row[j].fileInfo & 0xFF000000);
			uint32_t fileName = slot.body.index[i].row[j].fileInfo & 0x00FFFFFF;

			switch (fileType) {
			case 127:
				slotFile.byteAlign(0x800);
				break;
			default:
				int fileSize = (slot.body.index[i].row[j + 1].offset) - (slot.body.index[i].row[j].offset);
				char * data = new char[fileSize];
				slot.body.index[i].fileData.resize(n + 1);
				slotFile.read(data, fileSize);
				slot.body.index[i].fileData[n] = data;
				n++;
			}
		}
	}
	return slot;

}