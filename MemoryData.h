#pragma once
#include <Windows.h>
struct MemoryInfo {
	DWORDLONG totalPhys = 0;
	DWORDLONG availPhys = 0;
	DWORDLONG totalVirtual = 0;
	DWORDLONG availVirtual = 0;
};

MemoryInfo getMemoryInfo();
void printMemoryInfo();