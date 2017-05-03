#ifndef DDS_h
#define DDS_h

#include <inttypes.h>
#include <vector>

typedef struct DDS_PIXELFORMAT {
	uint32_t size;
	uint32_t flags;
	uint32_t fourCC;
	uint32_t rgbBitCount;
	uint32_t rBitCount;
	uint32_t gBitCount;
	uint32_t bBitCount;
	uint32_t aBitCount;
} DDS_PIXELFORMAT;

typedef struct DDSHeader {

	uint32_t magic;
	uint32_t size;
	uint32_t flags;
	uint32_t height;
	uint32_t width;
	uint32_t pitch;
	uint32_t depth;
	uint32_t mipMapCount;
	std::vector<uint32_t> reserved = std::vector<uint32_t>(11);
	DDS_PIXELFORMAT pixelFormat;
	uint32_t caps; //ddssurfaceflags
	uint32_t caps2; //ddscubemap
	uint32_t caps3;
	uint32_t caps4; 
	uint32_t reserved2;

} DDSHeader;

typedef struct DDSData {
	char * textureData;
	int textureSize;
} DDSData;

typedef struct DDS {
	DDSHeader header;
	DDSData ddsData;
} DDS;


#endif //DDS_H