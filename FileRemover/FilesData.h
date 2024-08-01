#pragma once
#include <windows.h>
#include "fileapi.h"
#include "WinUser.h"
#include <string>
#include <vector>

extern unsigned int PERIOD;	// по умолчания 1000 мс

struct DirectoryData {
	std::string path;
	std::string mask;
	unsigned long long ullAge;
};

//Основная ф-я поиска и удаления файлов по условиям заданым в для директории в config файле
int FindAndDeleteFilesByDirData(const DirectoryData& dirData);

//Получить возраст из fileData в миллисекундах 
unsigned long long GetFileAgeMs(WIN32_FIND_DATA& fileData);

//Получить путь+маску файла в формате std::wstring
std::wstring getPathWithMask(const std::string& path, const std::string& mask);

//Получить полный путь с именем файла в формате std::wstring
std::wstring getFullPathByName(const std::string& path, const WIN32_FIND_DATA& fileData);

//Удаление папки по пути path
int DeleteFolder(const std::wstring& path);

//Удаление файла по пути path
int DeleteFileLocal(const std::wstring& path);
