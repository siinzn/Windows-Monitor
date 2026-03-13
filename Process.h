#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
struct ProcessInfo
{
	DWORD pId;
	std::wstring fileName;
	uint64_t kernelTime, userTime, creationTime, exitTime;
};

struct ProcessKey {
	DWORD pId;
	//std::wstring fileName;
	uint64_t creationTime;

	bool operator<(const ProcessKey& other) const {
		if (pId != other.pId) return pId < other.pId;
		return creationTime < other.creationTime;
	}
};

HANDLE getSnapshot();
std::vector<ProcessInfo> getProcess(HANDLE process);
uint64_t display(std::vector<ProcessInfo>currentProcesses);
