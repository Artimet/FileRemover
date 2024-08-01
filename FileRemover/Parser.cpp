#include "Parser.h"
#include <iostream>

const tinyxml2::XMLElement* GoToFirstElem(const tinyxml2::XMLDocument& xml_doc)
{
	const tinyxml2::XMLElement* rootElement, *ParentElement, *directoryElement;
	rootElement = ParentElement = directoryElement = nullptr;
	rootElement = xml_doc.RootElement();
	if (!rootElement)
	{
		std::cout << "\n������ � ��������� ����� �� ������ �������� �������\n";
		return nullptr;
	}
	ParentElement = xml_doc.RootElement()->FirstChildElement();

	if (!ParentElement) {
		std::cout << "\n������ � ��������� ����� �� ������ �������� �������\n";
		return nullptr;
	}

	if (!ParentElement->UnsignedAttribute("period")) {
		std::cout << "\n����������� ������ ��� ������ ����� 0\n";
		return nullptr;
	}

	PERIOD = ParentElement->UnsignedAttribute("period");
	return ParentElement->FirstChildElement();
}

int fileDataByXmlElement(const tinyxml2::XMLElement* xmlElement, DirectoryInfo& fileData)
{
	if (!xmlElement->Attribute("path"))
	{
		std::cout << "\n������� path �� ��� ������\n";
		return 0;
	}
	fileData.path = xmlElement->Attribute("path");
	if (!xmlElement->Attribute("mask"))
	{
		std::cout << "\n������� mask �� ��� ������\n";
		return 0;
	}
	fileData.mask = xmlElement->Attribute("mask");
	fileData.ullAge = xmlElement->UnsignedAttribute("age");
	return 1;
}

void ReadXmlData(const char* fileName, std::vector<DirectoryInfo>& configData)
{
	tinyxml2::XMLDocument config_xml_doc;
	tinyxml2::XMLError eResult = config_xml_doc.LoadFile(fileName);

	if (eResult != tinyxml2::XML_SUCCESS) {
		std::cout << "\nXmlFile " << fileName << " �� ��� ��������\n";
		return;
	}

	const tinyxml2::XMLElement* firstDirElement = GoToFirstElem(config_xml_doc);
	if (!firstDirElement) {
		std::cout << "\n������ � ��������� ����� �� ������� ���������� � �����������\n";
		return;
	}

	for (const tinyxml2::XMLElement* curElement = firstDirElement; curElement; curElement = curElement->NextSiblingElement())
	{
		DirectoryInfo fileData;
		if (fileDataByXmlElement(curElement, fileData))
			configData.push_back(fileData);
	}
}
