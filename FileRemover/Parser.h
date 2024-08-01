#pragma once
#include "data.h"
#include "_tinyxml2.h"

//������� ��������� � ���� ����������� fileName ��� ���������� ������ � ������ �� � ������ �������� FileData
void ReadXmlData(const char* fileName, std::vector<DirectoryInfo>& configData);

//������� ��������� � ������� ����������� �������� ���������� xml ����� 
const tinyxml2::XMLElement* GoToFirstElem(const tinyxml2::XMLDocument& xml_doc);

//������� ��������� ��������� ��� �������� � ���������� �� � fileData
int fileDataByXmlElement(const tinyxml2::XMLElement* xmlElement, DirectoryInfo& fileData);

