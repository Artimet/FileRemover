// configParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "data.h"
#include "fstream"
#include <windows.h>
#include "fileapi.h"
#include "WinUser.h"
#include <iostream>
#include <thread>
#include <chrono>
extern unsigned int FREQ;
void FileRemover() {
	Parser parserConfig;
	while (true)
	{
		Parser parserConfig;
		parserConfig.ReadFile("configData.txt");

		auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(FREQ);
		

		WIN32_FIND_DATA ffd;
		std::wstring ws = std::wstring(parserConfig.filesData[0].path.begin(), parserConfig.filesData[0].path.end());
		LPCWSTR path = ws.c_str();
		HANDLE hFind;
		hFind = FindFirstFile(path, &ffd);
		if (hFind == INVALID_HANDLE_VALUE)
			std::cout << "\nNoFile";
		else {
			auto fileNameCopy = ffd.cFileName;
			if (DeleteFile(ffd.cFileName)) {
				std::cout << std::endl << fileNameCopy<<" deleted" << std::endl;
			}
		}
		std::this_thread::sleep_until(x);
	}
};

int main()
{
	std::thread PeriodicTask(FileRemover);
	PeriodicTask.detach();

	getchar();
    return 0;
}

