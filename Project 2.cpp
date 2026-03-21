#include "Process.h"
#include "CpuTime.h"
#include "MemoryData.h"
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <string>
#include <format>


// basically remove the loop below and make it in a way that rate is handled corrently
//there should be a previous value, and then a time limit, and then the current value which keeps changing
//main idea is the current value right now will the previous value in the next iteration

int main()
{
    BOOL running = TRUE;
    FILETIME_as_int previousCpu = {}; // set the first run to 0
    std::map<ProcessKey, uint64_t> previousKey;

    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));


        system("cls");
        //cpu time
        FILETIME_as_int currentCpu = systemData();
        cpuDeltas cpu_rate;

        //get processes
        HANDLE currentSnapshot = getSnapshot();
        std::vector<ProcessInfo> currentProcesses = enumerateProcesses(currentSnapshot);

        if (previousCpu.idle || previousCpu.kernel || previousCpu.user) { // check if previous is non zero

            cpu_rate = systemDataToInt(previousCpu, currentCpu);
            printSystemData(cpu_rate);
            printMemoryInfo();

        }
        else {
            std::cout << "Waiting for next run\n";
        }
        previousCpu = currentCpu;


        std::cout << std::left << std::setw(10) << "PID"
            << std::left << std::setw(50) << "fileName"
            << std::left << std::setw(25) << "CPU%"
            << std::left << std::setw(25) << "RAM" << std::endl;
        for (const auto& cp : currentProcesses) {
            ProcessKey key = { cp.pId, cp.creationTime };
            auto it = previousKey.find(key);
            if (it != previousKey.end()) {
                uint64_t previousTotal = it->second;
                uint64_t currentTotal = cp.kernelTime + cp.userTime;
                uint64_t processDelta = currentTotal - previousTotal;
                if (cpu_rate.dSystem != 0) {
                    double cpuPercuf = static_cast<double>(processDelta) / cpu_rate.dSystem * 100;
                    std::wstring cpuPerc = std::format(L"{:.2f}", cpuPercuf);
                    std::wcout << std::left << std::setw(10) << cp.pId
                        << std::left << std::setw(50) << cp.fileName
                        << std::left << std::setw(25) << cpuPerc 
                        << std::left << cp.workingSet << "MB" << std::endl;
                }
            }
        }

        for (const auto& cp : currentProcesses) {
            ProcessKey key = { cp.pId, cp.creationTime };
            uint64_t totalCpu = cp.kernelTime + cp.userTime;
            previousKey[key] = totalCpu;
        }
    }
}

