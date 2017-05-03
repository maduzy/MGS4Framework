#ifndef DAR_H
#define DAR_H

#include <inttypes.h>
#include <vector>

typedef struct DarFileData {
	std::string filename;
	uint32_t fileSize;
	char * data;
} DatFileData;

typedef struct Dar {
	uint32_t fileCount;
	std::vector<DarFileData> file;

} Dar;



#endif //DAR_H