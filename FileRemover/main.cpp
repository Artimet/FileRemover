#include "data.h"
#include "fstream"
#include <windows.h>
#include "fileapi.h"
#include "WinUser.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include "_tinyxml2.h"
extern unsigned int FREQ;

unsigned long long GetFileAgeMs(WIN32_FIND_DATA& ffd) {
	FILETIME ft = ffd.ftLastWriteTime;
	ULARGE_INTEGER ulFileTime, ulCurrentTime, diff;
	ulFileTime.LowPart = ft.dwLowDateTime;
	ulFileTime.HighPart = ft.dwHighDateTime;
	SYSTEMTIME st;
	GetSystemTime(&st);
	FILETIME st_to_ft;
	SystemTimeToFileTime(&st, &st_to_ft);
	ulCurrentTime.LowPart = st_to_ft.dwLowDateTime;
	ulCurrentTime.HighPart = st_to_ft.dwHighDateTime;

	diff.QuadPart = ulCurrentTime.QuadPart - ulFileTime.QuadPart;
	diff.QuadPart /= 10000000;
	return static_cast<unsigned long long>(diff.QuadPart);
}

int RemoveByPath(std::wstring& ws_path, FileData& removeInfo)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int dcounter = 0;
	int fcounter = 0;
	WIN32_FIND_DATA ffd;
	LPCWSTR lpcws_path = ws_path.c_str();
	//тест
	std::stringstream ss;
	ss << "C:\\Parser\\configParser\\configParser\\*.bmp";
	std::string str;
	std::getline(ss, str);
	std::wstring ws = std::wstring(str.begin(), str.end());
	lpcws_path = ws.c_str();
	removeInfo.ullAge = 15;
	//
	HANDLE hFind;
	hFind = FindFirstFile(lpcws_path, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			fcounter++;
			std::wstring fileNameCopy = ffd.cFileName;
			std::string fileNameStr = std::string(fileNameCopy.begin(), fileNameCopy.end());
			unsigned long long fileAge = GetFileAgeMs(ffd);
			std::cout << "Файл: " << fileNameStr << "\tвозраст: " << fileAge;
			if (fileAge > removeInfo.ullAge)
				if (DeleteFile(ffd.cFileName)) {
					std::wcout << " deleted" << std::endl;
					dcounter++;
				}
			std::cout << std::endl;
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	std::cout << "Директория: " << removeInfo.path << std::endl;
	std::cout << "Файлов нйдено: " << fcounter << std::endl;
	std::cout << "Файлов удалено: " << dcounter << std::endl << std::endl;
	return dcounter;
}

void FileRemover() {

	while (true)
	{
		Parser removerConfig;
		removerConfig.ReadFile("configData.ini");
		
		auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(FREQ);
		for (auto& curFileRemoveInfo : removerConfig.filesData)
		{
			curFileRemoveInfo.path += curFileRemoveInfo.mask;
			std::wstring ws_path = std::wstring(curFileRemoveInfo.path.begin(), curFileRemoveInfo.path.end());
			RemoveByPath(ws_path, curFileRemoveInfo);
		}
		std::this_thread::sleep_until(x);
	}
}

int main()
{
	std::thread PeriodicTask(FileRemover);
	PeriodicTask.detach();

	getchar();
	return 0;
}

