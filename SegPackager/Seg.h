#ifndef SEG_H
#define SEG_H

#include <inttypes.h>
#include <vector>

typedef struct SegData {
	std::vector< std::vector<char> > compressedData;
} SegData;

typedef struct SegIndex {
	uint16_t chunkCompressedSize;
	uint16_t chunkUncompressedSize;
	uint32_t chunkOffset;
} SegIndex;


typedef struct SegHeader {
	uint32_t magic;
	uint16_t flag = 0x0004;
	uint16_t chunkCount;
	uint32_t uncompressedSize;
	uint32_t compressedSize;
	std::vector<SegIndex> segIndex;
} SegHeader;

typedef struct Seg {
	SegHeader header;
	SegData data;
} Seg;

#endif //SEG_H