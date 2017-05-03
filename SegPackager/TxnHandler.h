#ifndef TXNHANDLER_H
#define TXNHANDLER_H

#include "FileHandler.h"
#include "Tex.h"
#include "Txn.h"

enum DDS_FOURCC {
	DXT1 = 0x09,
	DXT5 = 0x0B
};

typedef struct DataContainer{

	char * compressedData;
	int compressedSize;
	int uncompressedSize;

} DataContainer;

class TxnHandler {
private:
	void getTexForIndex(uint32_t type, uint32_t txnId, int n, std::vector<Tex> texs, std::vector<TextureData>& textureData);
public:
	std::vector<DataContainer> compress();
	void ddsFromTxn(Txn txn);
	void ddsFromTxn2(Txn txn);
	Txn parseTxn(std::string filename);
	void parseTex(char* uncompressedData, int size, std::vector<Tex>& texs);
	void attachTexToTxn(Txn& txn, std::vector<Tex> texs);
	void writeTxnToFile(Txn txn);
	//void createTxn(std::vector<DDS> ddsVector);
	~TxnHandler();
};

#endif //TXNHANDLER_H
