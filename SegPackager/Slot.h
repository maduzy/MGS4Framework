#ifndef SLOT_H
#define SLOT_H

#include <inttypes.h>
#include <vector>

typedef struct SlotHeader {
	uint32_t slotId;
	uint16_t unknown;
	uint16_t unknown2;
	uint16_t indexCount;
	uint32_t nullBytes;
	uint16_t unknown3;

} SlotHeader;

typedef struct SlotFileInfo {
	char fileType[1];
	char hashId[3];
} SlotFileInfo;

typedef struct SlotIndexRow {
	uint32_t fileInfo;
	uint32_t compressedSize;
	uint32_t nullBytes;
	uint32_t offset;
} SlotIndexRow;

typedef struct SlotIndex {
	uint32_t lineCount;
	uint32_t nullBytes;
	std::vector<SlotIndexRow> row;
	std::vector<char*> fileData;
} SlotIndex;

typedef struct SlotBody {
	std::vector<SlotIndex> index;
} SlotBody;

typedef struct Slot {
	SlotHeader header;
	SlotBody body;
} Slot;


#endif //SLOT_H