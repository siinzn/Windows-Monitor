#include "Monitor.h"
#include <ranges>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <format>

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

            double cpuPerc = static_cast<double>(processDelta) / cpu_rate.dSystem * 100;

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
    
    system("cls");
    std::cout << "Refresh Rate: " << refresh_rate << std::endl;
    std::cout << "-----------------------------------------------------------------------------" << std::endl;

    printSystemData(cpu_rate);
    std::cout << "-----------------------------------------------------------------------------" << std::endl;

    printMemoryInfo();
    std::cout << "-----------------------------------------------------------------------------\n" << std::endl;

    std::cout << "Total Processess: " << rd.size() << std::endl;
    std::wcout << std::left << std::setw(10) << "PID"
        << std::left << std::setw(35) << "fileName"
        << std::left << std::setw(15) << "CPU%"
        << "RAM" << std::endl;
    std::cout << "-----------------------------------------------------------------------------\n" << std::endl;

    std::stable_sort(rd.begin(), rd.end(), [](const RenderDetails& a, const RenderDetails& b) {
        
        if (a.cpuPerc != b.cpuPerc) return a.cpuPerc > b.cpuPerc;
        if (a.workingSet != b.workingSet) return a.workingSet > b.workingSet;
        if (a.fileName != b.fileName) return a.fileName < b.fileName;
        return a.pId > b.pId;
    });

    size_t maxProcesses = 15;
    for (const auto& rdetails : rd | std::views::take(maxProcesses)) {
        std::wcout << std::left << std::setw(10) << rdetails.pId
            << std::left << std::setw(35) << rdetails.fileName
            << std::left << std::setw(15) << std::format(L"{:.2f}%", rdetails.cpuPerc)
            << std::left << rdetails.workingSet << L"MB" << std::endl;
    }
    std::cout << "-----------------------------------------------------------------------------\n" << std::endl;
}