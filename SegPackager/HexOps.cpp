#include "HexOps.h"

uint32_t charToUInt32(const char* src) {
	uint32_t ret = 0;
	char* dst = (char*)&ret;
	for (int i = 0; (i < 4) && (*src); ++i, ++src)
		dst[i] = *src;

	return ret;
}

std::string intToHexString(int value) {
	std::stringstream ss;
	ss << std::hex << value;
	std::string result(ss.str());
	return result;
}