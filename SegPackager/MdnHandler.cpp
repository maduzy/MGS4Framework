#include "MdnHandler.h"

template<typename T>
void readType(FileHandler& file, int number, int size, T type, std::vector<T>& buffer) {

	for (int i = 0; i < number; i++) {
		type;
		file.read((char *)&type, size);
		buffer.push_back(x);

	}
}

Mdn MdnHandler::parseMdn(std::string filename) {

	//open file
	FileHandler mdnFile;
	mdnFile.openFile(filename.c_str());

	Mdn mdn;

	//read header
	mdnFile.readBigEndian(&mdn.header.magic, 4);
	mdnFile.readBigEndian(&mdn.header.filename, 4);
	//counts
	mdnFile.readBigEndian(&mdn.header.boneCount, 4);
	mdnFile.readBigEndian(&mdn.header.unknownCount, 4);
	mdnFile.readBigEndian(&mdn.header.vertexIndexCount, 4);
	mdnFile.readBigEndian(&mdn.header.faceIndexCount, 4);
	mdnFile.readBigEndian(&mdn.header.vertexDefinitionCount, 4);
	mdnFile.readBigEndian(&mdn.header.unknown2Count, 4);
	mdnFile.readBigEndian(&mdn.header.materialCount, 4);
	mdnFile.readBigEndian(&mdn.header.bonePalletCount, 4);
	//offsets
	mdnFile.readBigEndian(&mdn.header.boneOffset, 4);
	mdnFile.readBigEndian(&mdn.header.unknownOffset, 4);
	mdnFile.readBigEndian(&mdn.header.vertexIndexOffset, 4);
	mdnFile.readBigEndian(&mdn.header.faceIndexOffset, 4);
	mdnFile.readBigEndian(&mdn.header.vertexDefinitionOffset, 4);
	mdnFile.readBigEndian(&mdn.header.unknown2Offset, 4);
	mdnFile.readBigEndian(&mdn.header.materialOffset, 4);
	mdnFile.readBigEndian(&mdn.header.bonePalletOffset, 4);
	//extra offset
	mdnFile.readBigEndian(&mdn.header.vertexBufferOffset, 4);
	mdnFile.readBigEndian(&mdn.header.vertexBufferSize, 4);
	mdnFile.readBigEndian(&mdn.header.faceBufferOffset, 4);
	mdnFile.readBigEndian(&mdn.header.faceBufferSize, 4);
	mdnFile.readBigEndian(&mdn.header.nullBytes, 4);
	mdnFile.readBigEndian(&mdn.header.fileSize, 4);

	//read boundary
	mdnFile.readBigEndianFloat(&mdn.boundary.maxX, 4);
	mdnFile.readBigEndianFloat(&mdn.boundary.maxY, 4);
	mdnFile.readBigEndianFloat(&mdn.boundary.maxZ, 4);
	mdnFile.readBigEndianFloat(&mdn.boundary.maxW, 4);

	mdnFile.readBigEndianFloat(&mdn.boundary.minX, 4);
	mdnFile.readBigEndianFloat(&mdn.boundary.minY, 4);
	mdnFile.readBigEndianFloat(&mdn.boundary.minZ, 4);
	mdnFile.readBigEndianFloat(&mdn.boundary.minW, 4);

	//read bones
	for (int i = 0; i < mdn.header.boneCount; i++) {
		mdn.bones.resize(i + 1);
		mdnFile.readBigEndian(&mdn.bones[i].name, 4);
		mdnFile.readBigEndian(&mdn.bones[i].y0, 4);
		mdnFile.readBigEndian(&mdn.bones[i].parent, 4);
		mdnFile.readBigEndian(&mdn.bones[i].w0, 4);

		mdnFile.readBigEndianFloat(&mdn.bones[i].rotX, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].rotY, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].rotZ, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].rotW, 4);

		mdnFile.readBigEndianFloat(&mdn.bones[i].posX, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].posY, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].posZ, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].posW, 4);

		mdnFile.readBigEndianFloat(&mdn.bones[i].maxX, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].maxY, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].maxZ, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].maxW, 4);

		mdnFile.readBigEndianFloat(&mdn.bones[i].minX, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].minY, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].minZ, 4);
		mdnFile.readBigEndianFloat(&mdn.bones[i].minW, 4);
	}

	//read unknown
	for (int i = 0; i < mdn.header.unknownCount; i++) {
		mdn.unknown.resize(i + 1);
		mdnFile.readBigEndian(&mdn.unknown[i].name, 4);
		mdnFile.readBigEndian(&mdn.unknown[i].nullBytes, 4);
		mdnFile.readBigEndian(&mdn.unknown[i].parent, 4);
		mdnFile.readBigEndian(&mdn.unknown[i].w0, 4);
	}

	//read vertex index
	for (int i = 0; i < mdn.header.vertexIndexCount; i++) {
		mdn.vertexIndex.resize(i + 1);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].unknown, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].unknown2, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].faceSectionCount, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].faceSectionStart, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].vertexId, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].bonePalletId, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].vertexCount, 4);
		mdnFile.readBigEndian(&mdn.vertexIndex[i].nullBytes, 4);

		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MaxX, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MaxY, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MaxZ, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MaxW, 4);

		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MinX, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MinY, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MinZ, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].MinW, 4);

		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].PosX, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].PosY, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].PosZ, 4);
		mdnFile.readBigEndianFloat(&mdn.vertexIndex[i].PosW, 4);
	}
	
	//read face index
	for (int i = 0; i < mdn.header.faceIndexCount; i++) {

		mdn.faceIndex.resize(i + 1);

		mdnFile.readBigEndian(&mdn.faceIndex[i].type, 2);
		mdnFile.readBigEndian(&mdn.faceIndex[i].count, 2);
		mdnFile.readBigEndian(&mdn.faceIndex[i].offset, 4);
		mdnFile.readBigEndian(&mdn.faceIndex[i].group, 4);
		mdnFile.readBigEndian(&mdn.faceIndex[i].vertexStart, 2);
		mdnFile.readBigEndian(&mdn.faceIndex[i].vertexLength, 2);
	}

	//read vertex definition
	for (int i = 0; i < mdn.header.vertexDefinitionCount; i++) {
		mdn.vertexDefinition.resize(i + 1);

		mdnFile.readBigEndian(&mdn.vertexDefinition[i].nullBytes, 4);
		mdnFile.readBigEndian(&mdn.vertexDefinition[i].defintionCount, 4);
		mdnFile.readBigEndian(&mdn.vertexDefinition[i].size, 4);
		mdnFile.readBigEndian(&mdn.vertexDefinition[i].start, 4);

		for (int n = 0; n < mdn.vertexDefinition[i].defintionCount; n++) {
			mdn.vertexDefinition[i].definition.resize(n + 1);
			mdnFile.read((char *)&mdn.vertexDefinition[i].definition[n], 1);
		}

		mdnFile.seekg((0x10 - mdn.vertexDefinition[i].defintionCount), mdnFile.cur);

		for (int n = 0; n < mdn.vertexDefinition[i].defintionCount; n++) {
			mdn.vertexDefinition[i].position.resize(n + 1);
			mdnFile.read((char *)&mdn.vertexDefinition[i].position[n], 1);
		}

		mdnFile.seekg((0x10 - mdn.vertexDefinition[i].defintionCount), mdnFile.cur);

	}

	//read unknown2
	for (int i = 0; i < mdn.header.unknown2Count; i++) {
		mdn.unknown2.resize(i + 1);

		mdnFile.readBigEndian(&mdn.unknown2[i].size, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].name, 4);

		mdnFile.readBigEndian(&mdn.unknown2[i].count, 4);

		mdnFile.readBigEndian(&mdn.unknown2[i].count2, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].count3, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].count4, 4);

		mdnFile.readBigEndian(&mdn.unknown2[i].count5, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].count6, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].count7, 4);

		mdnFile.readBigEndian(&mdn.unknown2[i].count8, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].count9, 4);
		mdnFile.readBigEndian(&mdn.unknown2[i].count10, 4);

		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someX, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someY, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someZ, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someW, 2);

		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someX2, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someY2, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someZ2, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someW2, 2);

		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someX3, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someY3, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someZ3, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someW3, 2);

		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someX4, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someY4, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someZ4, 2);
		mdnFile.readBigEndianHalf(&mdn.unknown2[i].someW4, 2);

		mdnFile.read((char *)&mdn.unknown2[i].nullBytes, 8);
		mdnFile.read((char *)&mdn.unknown2[i].nullBytes2, 8);
		mdnFile.read((char *)&mdn.unknown2[i].nullBytes3, 8);
		mdnFile.read((char *)&mdn.unknown2[i].nullBytes4, 8);
	}

	//read material index
	for (int i = 0; i < mdn.header.materialCount; i++) {
		mdn.materialIndex.resize(i + 1);

		mdnFile.readBigEndian(&mdn.materialIndex[i].name, 4);
		mdnFile.readBigEndian(&mdn.materialIndex[i].nullBytes, 4);

		mdnFile.readBigEndianFloat(&mdn.materialIndex[i].t, 4);
		mdnFile.readBigEndianFloat(&mdn.materialIndex[i].u, 4);

		mdnFile.read((char *)&mdn.materialIndex[i].nullBytes2, 8);
		mdnFile.read((char *)&mdn.materialIndex[i].nullBytes3, 8);

	}

	//read bone pallet
	for (int i = 0; i < mdn.header.bonePalletCount; i++) {

		mdn.bonePallet.resize(i + 1);

		mdnFile.readBigEndian(&mdn.bonePallet[i].unknown, 4);
		mdnFile.readBigEndian(&mdn.bonePallet[i].count, 2);
		mdnFile.readBigEndian(&mdn.bonePallet[i].nullBytes, 2);

		for (int n = 0; n < mdn.bonePallet[i].count; n++) {
			mdn.bonePallet[i].boneId.resize(n + 1);
			mdnFile.read((char *)&mdn.bonePallet[i].boneId[n], 1);
		}
		
		mdnFile.seekg(((0x28) - (0x08 + mdn.bonePallet[i].count)), mdnFile.cur);
	}

	mdnFile.byteAlign(0x10);

	std::cout << std::hex << mdnFile.tellg() << std::endl;

	//read vertex buffer
	for (int i = 0; i < mdn.header.vertexIndexCount; i++) {

		mdn.vertexBuffer.resize(i + 1);

		for (int n = 0; n < mdn.vertexIndex[i].vertexCount; n++) {

			int vStart = mdnFile.tellg();

			for (int j = 0; j < mdn.vertexDefinition[i].defintionCount ; j++) {

				mdnFile.seekg(vStart + mdn.vertexDefinition[i].position[j], mdnFile.beg);

				switch (mdn.vertexDefinition[i].definition[j]) {
				case 0x10: {
							   float x, y, z;
							   mdnFile.readBigEndianFloat(&x, 4);
							   mdnFile.readBigEndianFloat(&y, 4);
							   mdnFile.readBigEndianFloat(&z, 4);
							   mdn.vertexBuffer[i].meshBuffer.push_back(x);
							   mdn.vertexBuffer[i].meshBuffer.push_back(y);
							   mdn.vertexBuffer[i].meshBuffer.push_back(z);
							   break;
				}
				case 0x78: {
							   half t, u;
							   mdnFile.read((char *)&t, 2);
							   mdnFile.read((char *)&u, 2);
							   mdn.vertexBuffer[i].textureBuffer.push_back(t);
							   mdn.vertexBuffer[i].textureBuffer.push_back(u);
							   //Half x;
							   //readType<Half>(mdnFile, 2, 2, x, mdn.vertexBuffer[i].textureBuffer);
							   mdn.vertexBuffer[i].textureBuffer.push_back(half(0));
							   break;
				}
				case 0x81: {
							   uint8_t w1, w2, w3, w4;
							   mdnFile.read((char *)&w1, 1);
							   mdnFile.read((char *)&w2, 1);
							   mdnFile.read((char *)&w3, 1);
							   mdnFile.read((char *)&w4, 1);
							   mdn.vertexBuffer[i].weightBuffer.push_back(w1);
							   mdn.vertexBuffer[i].weightBuffer.push_back(w2);
							   mdn.vertexBuffer[i].weightBuffer.push_back(w3);
							   mdn.vertexBuffer[i].weightBuffer.push_back(w4);
							   break;
				}
				case 0x83: {
							   uint8_t r, g, b, a;
							   mdnFile.read((char *)&r, 1);
							   mdnFile.read((char *)&g, 1);
							   mdnFile.read((char *)&b, 1);
							   mdnFile.read((char *)&a, 1);
							   mdn.vertexBuffer[i].colourBuffer.push_back(r);
							   mdn.vertexBuffer[i].colourBuffer.push_back(g);
							   mdn.vertexBuffer[i].colourBuffer.push_back(b);
							   mdn.vertexBuffer[i].colourBuffer.push_back(a);
							   break;
				}
				case 0x97: {
							   uint8_t b1, b2, b3, b4;
							   mdnFile.read((char *)&b1, 1);
							   mdnFile.read((char *)&b2, 1);
							   mdnFile.read((char *)&b3, 1);
							   mdnFile.read((char *)&b4, 1);
							   mdn.vertexBuffer[i].boneBuffer.push_back(b1);
							   mdn.vertexBuffer[i].boneBuffer.push_back(b2);
							   mdn.vertexBuffer[i].boneBuffer.push_back(b3);
							   mdn.vertexBuffer[i].boneBuffer.push_back(b4);
							   break;
				}
				case 0xA2: case 0xAE: {
							   uint8_t nx, ny, nz, nw;
							   mdnFile.read((char *)&nx, 1);
							   mdnFile.read((char *)&ny, 1);
							   mdnFile.read((char *)&nz, 1);
							   mdnFile.read((char *)&nw, 1);
							   mdn.vertexBuffer[i].boneBuffer.push_back(nx);
							   mdn.vertexBuffer[i].boneBuffer.push_back(ny);
							   mdn.vertexBuffer[i].boneBuffer.push_back(nz);
							   mdn.vertexBuffer[i].boneBuffer.push_back(nw);
							   break;
				}
				}

			}
			mdnFile.seekg(vStart + mdn.vertexDefinition[i].size, mdnFile.beg);
		}
		mdnFile.byteAlign(0x10);
	}

	std::cout << std::hex << mdnFile.tellg() << std::endl;

	//read face buffer
	for (int i = 0; i < mdn.header.vertexIndexCount; i++) {

		mdn.faceBuffer.resize(i + 1);

		for (int j = mdn.vertexIndex[i].faceSectionStart; j < (mdn.vertexIndex[i].faceSectionStart) + (mdn.vertexIndex[i].faceSectionCount); j++) {

			std::cout << j << std::endl;

			for (int n = 0; n < (mdn.faceIndex[j].count / 3); n++) {
				uint16_t fa, fb, fc;
				mdnFile.readBigEndian(&fa, 2);
				mdnFile.readBigEndian(&fb, 2);
				mdnFile.readBigEndian(&fc, 2);
				mdn.faceBuffer[i].faceBuffer.push_back(fa);
				mdn.faceBuffer[i].faceBuffer.push_back(fb);
				mdn.faceBuffer[i].faceBuffer.push_back(fc);
			}
		}
	}

	std::cout << std::hex << mdnFile.tellg() << std::endl;

	return mdn;
}