#ifndef TEX_H
#define TEX_H

#include <inttypes.h>
#include <vector>

typedef struct TexHeader {
	uint32_t type;
	uint32_t null = 0x0000;
	uint32_t hashId;
	uint32_t parentDataSize;
	uint32_t dataSize;
	uint32_t mipMapCount;
	uint32_t entryNumber;
	uint32_t padding = 0x0000;

} TexHeader;

typedef struct TexData {
	char* data;
} TexData;

typedef struct Tex {
	TexHeader texHeader;
	TexData texData;
} Tex;


#endif //TEX_H