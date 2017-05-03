#ifndef MDN_H
#define MDN_H

#include <inttypes.h>
#include <vector>
#include "half/half.h"

typedef struct Vertex {
	float x;
	float y;
	float z;
};

typedef struct MdnHeader {
	uint32_t magic;
	uint32_t filename;
	//sizes
	uint32_t boneCount;
	uint32_t unknownCount;  //unknown
	uint32_t vertexIndexCount;
	uint32_t faceIndexCount;
	uint32_t vertexDefinitionCount;
	uint32_t unknown2Count;  //unknown
	uint32_t materialCount;
	uint32_t bonePalletCount;
	//offsets
	uint32_t boneOffset;
	uint32_t unknownOffset;  //unknown
	uint32_t vertexIndexOffset;
	uint32_t faceIndexOffset;
	uint32_t vertexDefinitionOffset;
	uint32_t unknown2Offset; //unknown
	uint32_t materialOffset;
	uint32_t bonePalletOffset;
	//extra offsets
	uint32_t vertexBufferOffset;
	uint32_t vertexBufferSize;
	uint32_t faceBufferOffset;
	uint32_t faceBufferSize;
	uint32_t nullBytes;
	uint32_t fileSize;

} MdnHeader;

typedef struct MdnBoundary {
	float maxX;
	float maxY;
	float maxZ;
	float maxW;
	float minX;
	float minY;
	float minZ;
	float minW;
} MdnBoundary;

typedef struct MdnBones {

	uint32_t name;
	uint32_t y0;
	uint32_t parent;
	uint32_t w0;
	float rotX;
	float rotY;
	float rotZ;
	float rotW;
	float posX;
	float posY;
	float posZ;
	float posW;
	float maxX;
	float maxY;
	float maxZ;
	float maxW;
	float minX;
	float minY;
	float minZ;
	float minW;
} MdnBones;

typedef struct MdnCount2 {
	uint32_t name;
	uint32_t nullBytes;
	uint32_t parent;
	uint32_t w0;

} MdnCount2;

typedef struct MdnVertexIndex {
	uint32_t unknown;
	uint32_t unknown2;
	uint32_t faceSectionCount;
	uint32_t faceSectionStart;
	uint32_t vertexId;
	uint32_t bonePalletId;
	uint32_t vertexCount;
	uint32_t nullBytes;

	float MaxX;
	float MaxY;
	float MaxZ;
	float MaxW;
	float MinX;
	float MinY;
	float MinZ;
	float MinW;
	float PosX;
	float PosY;
	float PosZ;
	float PosW;

} MdnVertCountIndex;

typedef struct MdnFaceIndex {
	uint16_t type;
	uint16_t count;
	uint32_t offset;
	uint32_t group;
	uint16_t vertexStart;
	uint16_t vertexLength;

} MdnFaceCountIndex ;

typedef struct MdnVertexDefinition {

	uint32_t nullBytes;
	uint32_t defintionCount;
	uint32_t size;
	uint32_t start;
	std::vector <uint8_t> definition;
	std::vector <uint8_t> position;

} MdnVertSizeIndex ;

typedef struct MdnCount6 {

	uint32_t size;
	uint32_t name;
	uint32_t count;
	uint32_t count2;
	uint32_t count3;
	uint32_t count4;
	uint32_t count5;
	uint32_t count6;
	uint32_t count7;
	uint32_t count8;
	uint32_t count9;
	uint32_t count10;
	half someX;
	half someY;
	half someZ;
	half someW;
	half someX2;
	half someY2;
	half someZ2;
	half someW2;
	half someX3;
	half someY3;
	half someZ3;
	half someW3;
	half someX4; 
	half someY4; 
	half someZ4; 
	half someW4;
	uint64_t nullBytes;
	uint64_t nullBytes2;
	uint64_t nullBytes3;
	uint64_t nullBytes4;
} MdnCount6;

typedef struct MdnMaterialIndex {

	uint32_t name;
	uint32_t nullBytes;
	float t;
	float u;
	uint64_t nullBytes2;
	uint64_t nullBytes3;

} MdnMaterialIndex ;

typedef struct MdnBonePallet {

	uint32_t unknown;
	uint16_t count;
	uint16_t nullBytes;
	std::vector <uint8_t> boneId;

} MdnbonePallet;

typedef struct MdnVertexBuffer {
	std::vector<float> meshBuffer;
	std::vector<half> textureBuffer;
	std::vector<uint8_t> weightBuffer;
	std::vector<uint8_t> colourBuffer;
	std::vector<uint8_t> boneBuffer;
	std::vector<uint8_t> normalBuffer;
} MdnVertexBuffer;

typedef struct MdnFaceBuffer {

	std::vector<uint16_t> faceBuffer;

} MdnFaceBuffer;

typedef struct Mdn{

	MdnHeader header;
	MdnBoundary boundary;
	std::vector<MdnBones> bones;
	std::vector<MdnCount2> unknown;
	std::vector<MdnVertexIndex> vertexIndex;
	std::vector<MdnFaceIndex> faceIndex;
	std::vector<MdnVertexDefinition>  vertexDefinition;
	std::vector<MdnCount6> unknown2;
	std::vector<MdnMaterialIndex> materialIndex;
	std::vector<MdnBonePallet> bonePallet;
	std::vector<MdnVertexBuffer> vertexBuffer;
	std::vector<MdnFaceBuffer> faceBuffer;

} Mdn;

#endif //MDN_H