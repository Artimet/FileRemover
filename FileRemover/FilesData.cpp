#include "FilesData.h"
#include <iostream>

unsigned int PERIOD = 1000;

unsigned long long GetFileAgeMs(WIN32_FIND_DATA& fileData) {
	FILETIME ft = fileData.ftLastWriteTime;
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

std::wstring getFullPathByName(const std::string& path, const WIN32_FIND_DATA& fileData) {
	std::string pathCopy = path + "\\";
	std::wstring fileNameCopy = fileData.cFileName;
	std::wstring wsPath = std::wstring(pathCopy.begin(), pathCopy.end());
	std::wstring wsFullPath = wsPath + fileData.cFileName;
	return wsFullPath;
}

int DeleteFolder(const std::wstring& path) {
	if (RemoveDirectory(path.c_str())) {
		std::wcout << "\tdeleted" << std::endl;
		return 1;
	}
	std::wcout << L"\tПапка не может быть удалена, ошибка: " << GetLastError() << std::endl;
	return 0;
}

int DeleteFileLocal(const std::wstring& path) {
	if (DeleteFile(path.c_str())) {
		std::wcout << "\tdeleted" << std::endl;
		return 1;
	}
	std::wcout << L"\tФайл не может быть удален, ошибка: " << GetLastError() << std::endl;
	return 0;
}

int FindAndDeleteFilesByDirData(const DirectoryData& dirData)
{
	unsigned int deleteCounter = 0, findCounter = 0;
	std::wstring wsPathWithMask = getPathWithMask(dirData.path, dirData.mask);
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	std::wcout << L"\nПоиск: " << wsPathWithMask << ":\n";

	hFind = FindFirstFile(wsPathWithMask.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!wcscmp(ffd.cFileName, L".") || !wcscmp(ffd.cFileName, L".."))
				continue;
			findCounter++;
			std::wstring wsFullPath = getFullPathByName(dirData.path, ffd);
			unsigned long long fileAge = GetFileAgeMs(ffd);
			bool isFolder = GetFileAttributes(wsFullPath.c_str()) == FILE_ATTRIBUTE_DIRECTORY;

			if (isFolder) std::wcout << L"Найдена папка: " << ffd.cFileName << L"\tвозраст: " << fileAge;
			else std::wcout << L"Найден файл: " << ffd.cFileName << L"\tвозраст: " << fileAge;

			if (fileAge > dirData.ullAge) {
				if (isFolder) {
					if (DeleteFolder(wsFullPath)) deleteCounter++;
				}
				else {
					if (DeleteFileLocal(wsFullPath)) deleteCounter++;
				}
			}
			else
				std::wcout << " <= " << dirData.ullAge << std::endl;
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	std::wcout << L"\nФайлов нйдено: " << findCounter << std::endl;
	std::wcout << L"Файлов удалено: " << deleteCounter << std::endl << std::endl;
	return deleteCounter;
}
