#include "FilesData.h"
#include <iostream>
#include <atlbase.h>

unsigned int PERIOD = 5000;

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
	USES_CONVERSION;
	const wchar_t* wchPathWithMask = A2CW(pathWithMask.c_str());
	return wchPathWithMask;
}

std::wstring getFullPathByName(const std::string& path, const WIN32_FIND_DATA& fileData) {
	std::string pathCopy = path + "\\";
	USES_CONVERSION;
	const wchar_t* wchPathCopy = A2CW(pathCopy.c_str());
	std::wstring fileNameCopy = fileData.cFileName;
	std::wstring wsFullPath = std::wstring(wchPathCopy) + fileData.cFileName;
	return wsFullPath;
}

int DeleteFolder(const std::wstring& path) {
	if (RemoveDirectory(path.c_str())) {
		std::wcout << "\tdeleted" << std::endl;
		return 1;
	}
	std::wcout << L"\t����� �� ����� ���� �������, ������: " << GetLastError() << std::endl;
	return 0;
}

int DeleteFileLocal(const std::wstring& path) {
	if (DeleteFile(path.c_str())) {
		std::wcout << "\tdeleted" << std::endl;
		return 1;
	}
	std::wcout << L"\t���� �� ����� ���� ������, ������: " << GetLastError() << std::endl;
	return 0;
}

std::wstring findDirRecursive(const std::wstring& path, const std::wstring& mask, const  DirectoryData& dirData)
{
	std::wstring pathCopy = path;
	std::wstring::size_type pos = path.find_first_of('*');
	if (pos == std::string::npos) {
		scanCurDir(path, mask, dirData);
		return path;
	}
	std::wstring::size_type lastSlashPos = path.find_last_of('\\', pos);
	std::wstring::size_type nextSlashPos = path.find_first_of('\\', pos);

	std::wstring trimedPath = path;
	std::wstring trimedPathMask = path;
	std::wstring remainingPath = L"";
	if (nextSlashPos == std::string::npos)
		nextSlashPos = path.size();
	else
		remainingPath = std::wstring(path.begin() + nextSlashPos, path.end());

	if (lastSlashPos != std::string::npos)
		trimedPath = std::wstring(path.begin(), path.begin() + lastSlashPos);
	trimedPathMask = std::wstring(path.begin(), path.begin() + nextSlashPos);


	std::wcout << L"\n���� �������� �����:";
	std::wcout << std::endl << trimedPathMask << std::endl;

	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	hFind = FindFirstFile(trimedPathMask.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!wcscmp(ffd.cFileName, L".") || !wcscmp(ffd.cFileName, L".."))
				continue;

			std::wstring wsFullPath = trimedPath + L"\\" + ffd.cFileName;
			unsigned long long fileAge = GetFileAgeMs(ffd);
			bool isFolder = GetFileAttributes(wsFullPath.c_str()) == FILE_ATTRIBUTE_DIRECTORY;

			if (isFolder) {
				std::wcout << L"������� � ����������: " << ffd.cFileName << std::endl;
				pathCopy = findDirRecursive(wsFullPath + remainingPath, mask, dirData);
				scanCurDir(pathCopy, mask, dirData);
			}


		} while (FindNextFile(hFind, &ffd) != 0);
	}
	return path;
}

int scanCurDir(const std::wstring& path, const std::wstring& mask, const  DirectoryData& dirData)
{
	unsigned int deleteCounter = 0, findCounter = 0;

	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	std::wcout << L"\n�����: " << path + L"\\" + mask << L":\n";

	hFind = FindFirstFile((path + L"\\" + mask).c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!wcscmp(ffd.cFileName, L".") || !wcscmp(ffd.cFileName, L".."))
				continue;
			findCounter++;
			std::wstring wsFullPath = path + L"\\" + ffd.cFileName;
			unsigned long long fileAge = GetFileAgeMs(ffd);
			bool isFolder = GetFileAttributes(wsFullPath.c_str()) == FILE_ATTRIBUTE_DIRECTORY;

			if (isFolder) std::wcout << L"������� �����: " << ffd.cFileName << L"\t�������: " << fileAge;
			else std::wcout << L"������ ����: " << ffd.cFileName << L"\t�������: " << fileAge;

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

	std::wcout << L"\n������ ������: " << findCounter << std::endl;
	std::wcout << L"������ �������: " << deleteCounter << std::endl << std::endl;
	return deleteCounter;
}
