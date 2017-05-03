#ifndef MTAR_H
#define MTAR_H

#include <inttypes.h>
#include <vector>

typedef struct Mtar{

	uint32_t magic = 0x4D746172;
	uint16_t boneIdCount;
	uint16_t null;
	uint16_t boneIdCount2;
	uint16_t secondIndexcount;
	uint16_t unknownCount4;
	uint16_t null;

	uint32_t mainDataOffset;
	uint32_t finalIndexOffset;
	uint32_t boneIndexOffset;
	uint32_t secondIndexOffset;

	std::vector<uint32_t> boneId = std::vector<uint32_t>(boneIdCount);

} Mtar;

typedef struct MtarSecondIndex{

	uint32_t offset;
	uint32_t null;
	uint32_t size;
	uint32_t unknown;
	uint32_t unknown2;
	

} MtarSecondIndex;

typedef struct MtarMainData{

} MtarMainData;

typedef struct MtarFinalIndex{

	uint32_t dataId;
	uint32_t null;
	uint32_t unknown;
	uint32_t null2;
	uint32_t null3;
	uint32_t unknown2;
	uint32_t unknown3;
	uint32_t null4;
};

#endif //MTAR_H