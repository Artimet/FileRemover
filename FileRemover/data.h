#pragma once
#include <vector>
#include <string>

extern unsigned int FREQ;
struct FileData {
	std::string path;
	std::string mask;
	std::string age;
};

class Parser {
public:
	void ReadFile(const char* fileName);
	std::vector<FileData> filesData;
};