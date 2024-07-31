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
	int dcounter = 0;
	int fcounter = 0;
	WIN32_FIND_DATA ffd;
	LPCWSTR lpcws_path = ws_path.c_str();
	HANDLE hFind;
	hFind = FindFirstFile(lpcws_path, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			fcounter++;
			std::wstring fileNameCopy = ffd.cFileName;
			std::wstring path_ws = std::wstring(removeInfo.path.begin(), removeInfo.path.end());

			std::wstring ws_path = path_ws + ffd.cFileName;
			unsigned long long fileAge = GetFileAgeMs(ffd);
			std::wcout << "Файл: " << ws_path << "\tвозраст: " << fileAge;
			if (fileAge > removeInfo.ullAge) {
				if (DeleteFile(ws_path.c_str())) {
					std::wcout << " deleted" << std::endl;
					dcounter++;
				}
				else if (RemoveDirectory(ws_path.c_str()))
				{
					std::wcout << " deleted" << std::endl;
					dcounter++;
				}
				else
				{
					std::cout << std::endl << GetLastError() << std::endl;
					int a = 0;
				}
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
		unsigned long PERIOD = 10000;
		Parser removerConfig;
		removerConfig.filesData.clear();
		tinyxml2::XMLDocument xml_doc;
		tinyxml2::XMLElement* rootElement, * periodElement, * directoryElement;
		rootElement = periodElement = directoryElement = nullptr;
		tinyxml2::XMLError eResult = xml_doc.LoadFile("removeInfo.xml");
		if (eResult == tinyxml2::XML_SUCCESS) {
			rootElement = xml_doc.RootElement();
			if (rootElement) periodElement = xml_doc.RootElement()->FirstChildElement();
			if (periodElement != nullptr) {
				PERIOD = std::stoul(periodElement->Attribute("period"));
				directoryElement = periodElement->FirstChildElement();
			}

			for (auto curDirElement = directoryElement; curDirElement; curDirElement = curDirElement->NextSiblingElement())
			{
				FileData fd;
				fd.path = curDirElement->Attribute("path");
				fd.mask = curDirElement->Attribute("mask");
				fd.ullAge = std::stoull(curDirElement->Attribute("age"));
				removerConfig.filesData.push_back(fd);
			}
		}

		auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(PERIOD);
		for (auto& curFileRemoveInfo : removerConfig.filesData)
		{
			std::string path_copy = curFileRemoveInfo.path + curFileRemoveInfo.mask;
			std::wstring ws_path = std::wstring(path_copy.begin(), path_copy.end());
			RemoveByPath(ws_path, curFileRemoveInfo);
		}
		std::this_thread::sleep_until(x);
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::thread PeriodicTask(FileRemover);
	PeriodicTask.detach();

	getchar();
	return 0;
}

