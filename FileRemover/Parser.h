#pragma once
#include "data.h"
#include "_tinyxml2.h"

//Функция переходит в файл конфигураци fileName для считывании данных и записи их в вектор структур FileData
void ReadXmlData(const char* fileName, std::vector<DirectoryInfo>& configData);

//Функция переходит к первому некорневому элементу указанного xml файла 
const tinyxml2::XMLElement* GoToFirstElem(const tinyxml2::XMLDocument& xml_doc);

//Функция считывает параметры для удаления и записывает их в fileData
int fileDataByXmlElement(const tinyxml2::XMLElement* xmlElement, DirectoryInfo& fileData);

