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

//Рекурсивная ф-я поиска дерикторий 
std::wstring findDirRecursive(const std::wstring& path, const std::wstring& mask, const  DirectoryData& dirData);

//Ф-я поска и удаления файлов текущей директории по условиям заданым config файле
int scanCurDir(const std::wstring& path, const std::wstring& mask, const  DirectoryData& dirData);

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
