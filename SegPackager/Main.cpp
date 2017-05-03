#include <iostream>
#include <string>
#include "FileHandler.h"
#include "Seg.h"
#include "SegHandler.h"
#include "Tex.h"
#include "DDSHandler.h"
#include "TxnHandler.h"
#include "ZlibWrapper.h"
#include "DarHandler.h"
#include "QarHandler.h"
#include "MGSHash.h"
#include "FileTypes.h"
#include "FileNames.h"
#include "SlotHandler.h"
#include "MdnHandler.h"

int main() {

	MdnHandler mdnHandler;
	Mdn mdn;

	MGSHash mgsHash;
	int id = mgsHash.GetIDFromString("n018a");

	std::cout << std::hex << id << std::endl;

	std::cout << "done" <<std::endl;

	getchar();

}
