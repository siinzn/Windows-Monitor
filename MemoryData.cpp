#include "MemoryData.h"
#include <Windows.h>
#include <iostream>

MemoryInfo getMemoryInfo() {
	MemoryInfo mi;
	MEMORYSTATUSEX myMemoryInfo64;
	myMemoryInfo64.dwLength = sizeof(MEMORYSTATUSEX);
	if (!GlobalMemoryStatusEx(&myMemoryInfo64)) return mi;
	mi.totalPhys = myMemoryInfo64.ullTotalPhys;
	mi.availPhys = myMemoryInfo64.ullAvailPhys;
	return mi;
}

void printMemoryInfo() {
	MemoryInfo mi = getMemoryInfo();
	uint64_t totalPhysGB = mi.totalPhys / (1024 * 1024 * 1024);
	uint64_t availPhysGB = mi.availPhys / (1024 * 1024 * 1024);
	std::cout << "Total Physical Memory : " << totalPhysGB << "GB" << std::endl;
	std::cout << "Available Physical Memory : " << availPhysGB << "GB" << std::endl;
}
