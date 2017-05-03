#ifndef MDNHANDLER_H
#define MDNHANDLER_H

#include "Mdn.h"
#include "FileHandler.h"

class MdnHandler {
public:
	Mdn parseMdn(std::string filename);
};

#endif //MDNHANDLER_H