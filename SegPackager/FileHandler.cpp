#include "FileHandler.h"
#include <iostream>
#include <string>
#include <inttypes.h>

std::vector<WIN32_FIND_DATA> FileHandler::fromDirectory(std::string foldername, std::string filetype) {

	std::string filepath = foldername + filetype;

	WIN32_FIND_DATA search_data;

	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

	HANDLE handle = FindFirstFile(filepath.c_str(), &search_data);

	std::vector<WIN32_FIND_DATA> files;

	while (handle != INVALID_HANDLE_VALUE)
	{
		//printf("Found file: %s\r\n", search_data.cFileName);
		
		if (search_data.nFileSizeLow != 0) {
			files.push_back(search_data);
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	return files;
}

std::vector<WIN32_FIND_DATA> FileHandler::getDirectories(std::string foldername) {

	WIN32_FIND_DATA search_data;

	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

	HANDLE handle = FindFirstFile((foldername + "*").c_str(), &search_data);

	std::vector<WIN32_FIND_DATA> directories;

	while (handle != INVALID_HANDLE_VALUE)
	{
		if (strcmp(search_data.cFileName, ".") == 2
			|| strcmp(search_data.cFileName, "..") == 1
			|| search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY != 0)  {
			directories.push_back(search_data);
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	return directories;
}

half reverseHalf(const half inHalf) {
	half retVal;
	char *halfToConvert = (char*)& inHalf;
	char *returnHalf = (char*)& retVal;

	returnHalf[0] = halfToConvert[1];
	returnHalf[1] = halfToConvert[0];

	return retVal;
}

float reverseFloat(const float inFloat)
{
	float retVal;
	char *floatToConvert = (char*)& inFloat;
	char *returnFloat = (char*)& retVal;

	// swap the bytes into a temporary buffer
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];

	return retVal;
}


void FileHandler::openFile(std::string filename) {

	this->open(filename, std::ios::binary | std::ios::in);
}

void FileHandler::readBigEndianFloat(float* value, int length) {
	this->read((char*)value, length);
	*value = reverseFloat(*value);
}

void FileHandler::readBigEndianHalf(half* value, int length) {
	this->read((char*)value, length);
	*value = reverseHalf(*value);
}

void FileHandler::writeBigEndianFloat(float* value, int length) {
	*value = reverseFloat(*value);
	this->write((char *)value, length);
	*value = reverseFloat(*value);
}

void FileHandler::readBigEndian(unsigned int* value, int length) {
	this->read((char *)value, length);

	*value = _byteswap_ulong(*value);

}

void FileHandler::readBigEndian(unsigned short* value, int length) {
	this->read((char *)value, length);
	*value = _byteswap_ushort(*value);
}

void FileHandler::writeBigEndian(unsigned int* value, int length) {
	*value = _byteswap_ulong(*value);
	this->write((char *)value, length);
	*value = _byteswap_ulong(*value);

}

void FileHandler::writeBigEndian(unsigned short* value, int length) {
	*value = _byteswap_ushort(*value);
	this->write((char *)value, length);
	*value = _byteswap_ushort(*value);
}

void  FileHandler::writeZeroes(int length) {
	
	uint8_t zero = 0;

	for (int i = 0; i < length; i++) {
		this->write((char*)&zero, 1);
	}

}

//get filesize
int FileHandler::getFileSize() {
		this->seekg(0, this->end);
		int length = this->tellg();
		this->seekg(0, this->beg);

		return length;
}

//small byte alignment used in DAR and QAR
void FileHandler::smallAlign() {
	int skip = 4 - ((this->tellg() % 16) % 4);
	this->seekg(skip, std::ios::cur);

}

//byte alignment
void FileHandler::byteAlign(int size) {
	int skip = (size - (this->tellg() % size));
	if (skip != size) {
		this->seekg(skip, std::ios::cur);
	}
}