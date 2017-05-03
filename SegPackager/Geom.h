#ifndef GEOM_H
#define GEOM_H

#include <vector>
#include <inttypes.h>

typedef struct GeomEntries {
	uint16_t number;
	uint16_t nullBytes;
	uint32_t size;
	uint32_t offset;
} GeomEntries;

typedef struct GeomHeader{
	uint32_t magic;
	uint32_t fileSize;
	uint32_t entrySize;
	uint32_t nullBytes;
	float x;
	float y;
	float z;
	float w;
	std::vector<GeomEntries> entry;
	uint32_t unknown;
	uint32_t nullBytes2;
	uint32_t unknown2;
	uint32_t nullBytes3;
	uint32_t nullBytes4;
	uint64_t nullBytes5;
	uint64_t nullBytes6;
} GeomHeader;


typedef struct GeomChunk {
	float maxX;
	float maxY;
	float maxZ;
	uint32_t endOffset;
	float minX;
	float minY;
	float minZ;
	float minW;
	uint32_t unknown;
	uint32_t unknown2;
	uint32_t unknown3;
	uint32_t totalSize;
	uint32_t lastFlag;
	uint16_t unknown4 = 0x08;
	uint16_t unknown5 = 0x0A;
	uint32_t offset;
	uint32_t offset2;
}GeomChunk;

typedef struct Geom0Entry {
	std::vector<GeomChunk> chunk;
};

#endif //GEOM_H