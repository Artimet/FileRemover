#include <iostream>
#include <thread>
#include <chrono>
#include "Parser.h"
#include "FilesData.h"

extern unsigned int PERIOD;

void Deleter() {
	while (true)
	{
		std::vector<DirectoryData> configData;
		ReadXmlData("removeInfo.xml", configData);
		auto nextResponseTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(PERIOD);
		for (auto& curDirData : configData)
			FindAndDeleteFilesByDirData(curDirData);
		std::this_thread::sleep_until(nextResponseTime);
	}
}

int main()
{
	std::wcout.imbue(std::locale("rus_rus.866"));

	std::thread PeriodicTask(Deleter);
	PeriodicTask.detach();

	getchar();
	return 0;
}

