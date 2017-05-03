#ifndef QARHANDLER_H
#define QARHANDLER_H

#include "Qar.h"

class QarHandler {
public:
	void extract(Qar qar);
	void writeQarToFile(Qar qar);
	Qar makeQar(std::string directory);
	Qar parseQar(std::string filename);
};

#endif //QAR_H