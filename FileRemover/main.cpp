#include "data.h"
#include "fstream"
#include <windows.h>
#include "fileapi.h"
#include "WinUser.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Parser.h"
#include "_tinyxml2.h"

extern unsigned int PERIOD;

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

std::wstring getPathWithMask(const std::string& path, const std::string& mask) {
	std::string pathWithMask = path + "\\" + mask;
	std::wstring wsPathWithMask = std::wstring(pathWithMask.begin(), pathWithMask.end());
	return wsPathWithMask;
}

std::wstring getFullPathByName(const std::string& path, const WIN32_FIND_DATA& ffd) {
	std::string pathCopy = path + "\\";
	std::wstring fileNameCopy = ffd.cFileName;
	std::wstring wsPath = std::wstring(pathCopy.begin(), pathCopy.end());
	std::wstring wsFullPath = wsPath + ffd.cFileName;
	return wsFullPath;
}

int RemoveByParam(DirectoryInfo& dirData)
{
	unsigned int deleteCounter = 0, findCounter = 0;
	std::wstring wsPathWithMask = getPathWithMask(dirData.path, dirData.mask);
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	std::cout << "\nДиректория: " << dirData.path << ":\n";
	hFind = FindFirstFile(wsPathWithMask.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			findCounter++;
			std::wstring wwFullPath = getFullPathByName(dirData.path, ffd);
			unsigned long long fileAge = GetFileAgeMs(ffd);

			if (GetFileAttributes(wwFullPath.c_str()) == FILE_ATTRIBUTE_DIRECTORY) {
				std::wcout << "Найдена папка: " << wwFullPath << "\tвозраст: " << fileAge;
				const WCHAR* dot = L".", * ddot = L"..";
				if (fileAge > dirData.ullAge && wcscmp(ffd.cFileName, dot) && wcscmp(ffd.cFileName, ddot)) {
					if (RemoveDirectory(wwFullPath.c_str())) {
						std::wcout << "\tdeleted";
						deleteCounter++;
					}
					else
						std::cout << GetLastError();
				}
				std::cout << std::endl;
			}
			else {
				std::wcout << "Найден файл: " << wwFullPath << "\tвозраст: " << fileAge;
				if (fileAge > dirData.ullAge) {
					if (DeleteFile(wwFullPath.c_str())) {
						std::wcout << "\tdeleted";
						deleteCounter++;
					}
					else
						std::cout << GetLastError();
				}
				std::cout << std::endl;
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	std::cout << "\nФайлов нйдено: " << findCounter << std::endl;
	std::cout << "Файлов удалено: " << deleteCounter << std::endl << std::endl;
	return deleteCounter;
}

void Deleter() {
	while (true)
	{
		std::vector<DirectoryInfo> configData;
		ReadXmlData("removeInfo.xml", configData);
		auto nextResponseTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(PERIOD);
		for (auto& curDirData : configData)
			RemoveByParam(curDirData);
		std::this_thread::sleep_until(nextResponseTime);
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::thread PeriodicTask(Deleter);
	PeriodicTask.detach();

	getchar();
	return 0;
}

