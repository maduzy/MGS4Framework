#include <iostream>
#include <string>
#include "FileHandler.h"
#include "Seg.h"
#include "SegHandler.h"
#include "Tex.h"
#include "DDSHandler.h"
#include "TxnHandler.h"
#include "DarHandler.h"



//using the seg handler
void segHandling() {

	SegHandler segHandler;
	Dlz dlz;

	//use parseDlz to make a dlz type from a file
	dlz = segHandler.parseDlz("cache.dlz.dec");

	//now you can access its data

	//you can see how many segs it contains
	int segCount;
	segCount = dlz.seg.size();

	//you can get one of its segs from its index
	Seg seg;
	seg = dlz.seg[0];

	//as the seg is also parsed, you can get its data as well
	int segCompressedSize;
	segCompressedSize = seg.header.compressedSize;

	//all of its info, the index, the chunk information, etc.
	int thirdChunkUncompressedSize;
	thirdChunkUncompressedSize = seg.header.segIndex[2].chunkUncompressedSize;

	//Access it like this
	//&segDataVector[chunkNumber][0]
	//&segDataVector[10][0]
	//&segData.data[10][0]
	//&seg.data.compressedData[10][0]
	//&dlz.seg[0].data.compressedData[10][0]

	//you can also unpack the dlz to get all the segs uncompressed data and output it to a file
	int size;
	segHandler.unpackDlz(dlz, "output_file.filetype", &size);

	//or if you'd like to just uncompress a single Seg you can do that too
	segHandler.unpackSeg(dlz.seg[2]);

	//we'll use the TxnHandler here to show how to create a dlz
	TxnHandler txnHandler;

	//Firstly a file must be compressed and we need a datacontainer
	//this can be done with the txnHandler
	std::vector<DataContainer> dataContainers;
	dataContainers = txnHandler.compress();

	//now we just make a dlz from the data container
	Dlz newDlz;
	newDlz = segHandler.makeDlz(dataContainers);

	//we can do what we want with it, it makes sense to write it out to use it in game
	//so lets write it to the disk

	//FileHandler inherits from fstream, but adds extra functionality
	FileHandler outputFile;
	outputFile.read("output_cache.dlz", std::ios::out | std::ios::binary);

	//simply add the dlz you want to write, and your output filehandler
	segHandler.writeDlzToFile(newDlz, outputFile);

	//and of course you can do the same with just a single seg
	segHandler.writeSegToFile(newDlz.seg[0], outputFile);

	//remember to close the output file when you're done
	outputFile.close();

}

//using DarHandler

void DarHandling() {

	Dar dar;
	DarHandler darHandler;

	//use parseDar to make a Dar type from a file
	dar = darHandler.parseDar("cache.dar.dec");


	//you can extract a dar using by passing a dar to extract
	darHandler.extract(dar);

	//you can make a dar by sending a directory to makeDar, it will return a dar
	Dar anotherDar;
	anotherDar = darHandler.makeDar("Extract\\");

	//you can also make a single file adopt the standard to be put in a dar
	DarFileData darFileData;
	darFileData = darHandler.makeDarDataFile("chaff_01.mdn");


	//you could use this to replace files in a dar seperately, for example
	anotherDar.file[2] = darFileData;

	//once you have a dar type you can write it to disk
	darHandler.writeDarToFile(anotherDar);

}

void txnHandling() {
	TxnHandler txnHandler;
	Txn txn;
	SegHandler segHandler;
	Dlz dlz;
	Dlz dlz2;
	char * uncompressedData;
	char * uncompressedData2;
	dlz = segHandler.parseDlz("n018a\\cache.dlz");
	dlz2 = segHandler.parseDlz("n018a\\cache_d.dlz");

	int size;

	std::vector<Tex> texs;


	//parse a txn
	txn = txnHandler.parseTxn("n018a\\stage_01.txn");

	//unzlib a dlz, parse data inside
	uncompressedData = segHandler.unpackDlz(dlz, "output", &size);

	txnHandler.parseTex(uncompressedData, size, texs);

	std::cout << "texs size:" << texs.size() << std::endl;

	//unzlib another dlz, parse data inside
	uncompressedData2 = segHandler.unpackDlz(dlz2, "output2", &size);

	txnHandler.parseTex(uncompressedData2, size, texs);

	std::cout << "texs size:" << texs.size() << std::endl;

	//print some stuff
	std::cout << texs[0].texHeader.type << std::endl;

	//next part
	std::cout << "next phase" << std::endl;

	//complete txn using unzlibbed data
	txnHandler.attachTexToTxn(txn, texs);

	//write the txn out
	//txnHandler.writeTxnToFile(txn);

	//make dds from txn
	txnHandler.ddsFromTxn2(txn);

	std::cout << "done" << std::endl;
}