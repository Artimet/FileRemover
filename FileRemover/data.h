#pragma once
#include <vector>
#include <string>

extern unsigned int PERIOD;

struct DirectoryInfo {
	std::string path;
	std::string mask;
	unsigned long long ullAge;
};

