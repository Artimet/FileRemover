#include "stdafx.h"
#include "data.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

unsigned int FREQ = 0;
void Parser::ReadFile(const char* fileName)
{
	filesData.clear();
	std::string line;
	std::ifstream in(fileName);
	if (in.is_open()) {
		while (std::getline(in, line))
		{	
			if (line == "FREQ:") {
				std::getline(in, line);
				FREQ = std::stoi(line);
			}
			if (line == "File:") {
				FileData tmp;
				std::getline(in, tmp.path);
				std::getline(in, tmp.mask);
				std::getline(in, tmp.age);
				filesData.push_back(tmp);
			}
		}
	}
	in.close();
}

