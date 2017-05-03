#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "half/half.h"

struct membuf : std::streambuf {

	
	membuf(char* base, std::ptrdiff_t n) {
		this->setg(base, base, base + n);
	}

	virtual pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) {
		if (dir == std::ios_base::cur) gbump(off);
		return gptr() - eback();
	}

};

class FileHandler: public std::fstream {
public:
	std::vector<WIN32_FIND_DATA> fromDirectory(std::string foldername, std::string filetype);
	std::vector<WIN32_FIND_DATA> getDirectories(std::string foldername);
	void openFile(std::string filename);
	void readBigEndian(unsigned int* value, int length);
	void readBigEndian(unsigned short* value, int length);
	void readBigEndianFloat(float* value, int length);
	void readBigEndianHalf(half* value, int length);
	void writeBigEndianFloat(float* value, int length);
	void writeBigEndian(unsigned int* value, int length);
	void writeBigEndian(unsigned short* value, int length);
	void writeZeroes(int length);
	void smallAlign();
	void byteAlign(int size);
	int getFileSize();
};


#endif //FILEHANDLER_H