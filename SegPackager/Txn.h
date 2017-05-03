#ifndef TXN_H
#define TXN_H

#include <inttypes.h>
#include <vector>

typedef struct TextureData {
	char * mainData;
	uint32_t mainDataSize; //doesnt actually exist but i cheated by putting it in
	char * mipMapData;
	uint32_t mipMapDataSize; //doesnt actually exist but i cheated by putting it in
} TextureData ;

typedef struct TxnIndex{
	uint16_t width;
	uint16_t height;
	uint16_t fourCC;
	uint16_t flag;
	uint32_t offset;
	uint32_t mipMapOffset;

} TxnIndex;

typedef struct TxnIndex2{
	uint32_t unknown;
	uint32_t materialId;
	uint32_t objectId;
	uint16_t width;
	uint16_t height;
	uint16_t xPosition;
	uint16_t yPosition;
	uint32_t offset;
	uint32_t nullBytes2;
	float xWeight;
	float yWeight;
	float xWeight2;
	float yWeight2;
	uint32_t nullBytes3;
} TxnIndex2;

typedef struct TxnHeader {

	uint32_t nullBytes;
	uint32_t flags;
	uint32_t texCount;
	uint32_t indexOffset;
	uint32_t texCount2;
	uint32_t index2Offset;
	uint32_t nullBytes2;
	uint32_t nullBytes3;
} TxnHeader;

typedef struct TxnBody {
	TxnHeader txnHeader;
	std::vector<TxnIndex> txnIndex;
	std::vector<TxnIndex2> txnIndex2;
} TxnBody;

typedef struct Txn {
	TxnBody txnBody;
	std::vector<TextureData> textureData;
} Txn;

#endif //TXN_H