#pragma once
#include <windows.h>
#include "fileapi.h"
#include "WinUser.h"
#include <string>
#include <vector>

extern unsigned int PERIOD;	// �� ��������� 1000 ��

struct DirectoryData {
	std::string path;
	std::string mask;
	unsigned long long ullAge;
};

//�������� �-� ������ � �������� ������ �� �������� ������� � ��� ���������� � config �����
int FindAndDeleteFilesByDirData(const DirectoryData& dirData);

//�������� ������� �� fileData � ������������� 
unsigned long long GetFileAgeMs(WIN32_FIND_DATA& fileData);

//�������� ����+����� ����� � ������� std::wstring
std::wstring getPathWithMask(const std::string& path, const std::string& mask);

//�������� ������ ���� � ������ ����� � ������� std::wstring
std::wstring getFullPathByName(const std::string& path, const WIN32_FIND_DATA& fileData);

//�������� ����� �� ���� path
int DeleteFolder(const std::wstring& path);

//�������� ����� �� ���� path
int DeleteFileLocal(const std::wstring& path);
