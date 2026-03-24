#include "Monitor.h"
#include <iostream>
#include <iomanip>

void Monitor::update() {
    rd.clear(); // clear the vector
	//cpu time
	FILETIME_as_int currentCpu = systemData();

    //get processes
    HANDLE currentSnapshot = getSnapshot();
    std::vector<ProcessInfo> currentProcesses = enumerateProcesses(currentSnapshot);

    if (!initialized) {
        previousCpu = currentCpu;
        initialized = true;
        return;
    }

    cpu_rate = systemDataToInt(previousCpu, currentCpu);
    previousCpu = currentCpu;

    if (cpu_rate.dSystem == 0) return;
    for (const auto& cp : currentProcesses) {
        ProcessKey key = { cp.pId, cp.creationTime };
        auto it = previousKey.find(key);
        if (it != previousKey.end()) {
            RenderDetails render_details;
            uint64_t previousTotal = it->second;
            uint64_t currentTotal = cp.kernelTime + cp.userTime;
            uint64_t processDelta = currentTotal - previousTotal;

            double cpuPercuf = static_cast<double>(processDelta) / cpu_rate.dSystem * 100;

            std::wstringstream wss;
            wss << std::fixed << std::setprecision(2) << cpuPercuf;
            std::wstring cpuPerc = wss.str();
            render_details.pId = cp.pId;
            render_details.fileName = cp.fileName;
            render_details.cpuPerc = cpuPerc;
            render_details.workingSet = cp.workingSet;
            rd.push_back(render_details);
        }

        uint64_t totalCpu = cp.kernelTime + cp.userTime;
        previousKey[key] = totalCpu;

    }
    //used ai for help for this because i didnt udnerstand properly but i do understand what is happening below
    auto it = previousKey.begin();
    while (it != previousKey.end()) {
        bool found = false;
        for (const auto& cp : currentProcesses) {
            if (it->first.pId == cp.pId && it->first.creationTime == cp.creationTime) {
                found = true;
                break;
            }
        }

        if (!found) it = previousKey.erase(it);
        else ++it;
    }
}

void Monitor::render() {

    printSystemData(cpu_rate);
    printMemoryInfo();
    std::wcout << std::left << std::setw(10) << "PID"
        << std::left << std::setw(50) << "fileName"
        << std::left << std::setw(25) << "CPU%"
        << std::left << std::setw(25) << "RAM" << std::endl;

    for (const auto& rdetails : rd) {
        std::wcout << std::left << std::setw(10) << rdetails.pId
            << std::left << std::setw(50) << rdetails.fileName
            << std::left << std::setw(25) << rdetails.cpuPerc
            << std::left << rdetails.workingSet << "MB" << std::endl;
    }
}