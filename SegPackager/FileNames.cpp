#include "FileNames.h"
#include "MGSHash.h"

std::map<const int, std::string> FileNames;

void initFileNames() {

	MGSHash hashTool;

	std::ifstream file("dictionary.txt");
	std::string str;
	while (std::getline(file, str))
	{
		int hashed = hashTool.GetIDFromString(str.c_str());
		FileNames[hashed] = str;
	}
}
void test() {
	for (auto const &entry : FileNames) {
		// ent1.first is the first key
		std::cout << std::hex << entry.first << std::endl;
	}
	if (FileNames[0xA7B3F9] != "") {
		std::cout << "found it" << std::endl;
	}
}