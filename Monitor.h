#pragma once
#include "CpuTime.h"
#include "MemoryData.h"
#include "Process.h"
#include <map>
#include <vector>
#include <sstream>

class Monitor {

public:
	void update();
	void render();
	int refresh_rate{ 500 };
private:
	struct RenderDetails {
		DWORD pId;
		std::wstring fileName;
		std::wstring cpuPerc;
		size_t workingSet;
	};

	std::vector<RenderDetails> rd; 
	cpuDeltas cpu_rate;
	FILETIME_as_int previousCpu = {};
	std::map<ProcessKey, uint64_t> previousKey;
	bool initialized = false;
	
};
