#ifndef QAR_H
#define QAR_H

#include <inttypes.h>
#include <vector>

typedef struct QarData {
	std::vector<char *> data;
} QarData;

typedef struct QarFileInfo {
	char fileType[1];
	char hashId[3];
} QarFileInfo;

typedef struct QarDataInfo {
	uint32_t fileInfo;
	uint32_t fileSize;

} QarDataInfo;

typedef struct QarHeader {
	uint16_t fileCount;
	std::vector<QarDataInfo> dataInfo;
	std::vector<std::string> filename;
	uint32_t offset;

} QarHeader;

typedef struct Qar {
	QarData data;
	QarHeader header;

} Qar;



#endif //QAR_H