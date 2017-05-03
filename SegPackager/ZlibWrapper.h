#ifndef ZLIBWRAPPER_H
#define ZLIBWRAPPER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>

class ZlibWrapper {
public:
	void zerr(int ret);
	int zlibDeflate(char *src, char *dst, int bytes_to_compress, int *bytes_after_compressed);
	int zlibInflate(char *src, char *dst, int bytes_to_decompress, int maximum_after_decompress, int* outbytes);
};

#endif //ZLIBWRAPPER_H