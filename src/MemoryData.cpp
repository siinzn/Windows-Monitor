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
	mi.totalVirtual = myMemoryInfo64.ullTotalVirtual;
	mi.availVirtual = myMemoryInfo64.ullAvailVirtual;
	return mi;
}

void printMemoryInfo() {
	MemoryInfo mi = getMemoryInfo();
	uint64_t totalPhys = mi.totalPhys / (1024 * 1024 * 1024);
	uint64_t availPhys = mi.availPhys / (1024 * 1024 * 1024);
	std::cout << "MEMORY: " << totalPhys << "GB / " << availPhys << "GB free" << std::endl;
}
