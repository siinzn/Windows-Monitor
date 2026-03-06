#include "Process.h"
#include "CpuTime.h"
#include "MemoryData.h"
#include "Global.h"
#include <locale>
#include <iostream>
#include <thread>
#include <vector>
#include <map>

// basically remove the loop below and make it in a way that rate is handled corrently
//there should be a previous value, and then a time limit, and then the current value which keeps changing
//main idea is the current value right now will the previous value in the next iteration

int main()
{
    //std::wcout.imbue(std::locale(""));
    //printProcess();
    

    BOOL running = TRUE;
    FILETIME_as_int previousCpu = {}; // set the first run to 0
    std::vector<ProcessInfo> previousProcesses= {};
    std::map<ProcessKey, uint64_t> previousKey;
    const int cores = getCores();

    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        HANDLE currentSnapshot = getSnapshot();
        std::vector<ProcessInfo> currentProcesses = getProcess(currentSnapshot);
        system("cls");
        FILETIME_as_int currentCpu = systemData();
        cpuDeltas cpu_rate;
        if (previousCpu.idle || previousCpu.kernel || previousCpu.user) { // check if previous is non zero

            cpu_rate = systemDataToInt(previousCpu, currentCpu);
            printSystemData(cpu_rate);
            printMemoryInfo();

        }
        else {
            std::cout << "Waiting for next run\n";
        }
        previousCpu = currentCpu;
        
        for (const auto cp : currentProcesses) {
            ProcessKey key = { cp.pId, cp.creationTime };
            auto it = previousKey.find(key);
            if (it != previousKey.end()) {
                uint64_t previousTotal = it->second;
                uint64_t currentTotal = cp.kernelTime + cp.userTime;
                uint64_t processDelta = currentTotal - previousTotal;
                uint64_t cpuPerc = processDelta / (10000000LL * cores) * 100;
                std::wcout << cp.fileName << " : " << cpuPerc << std::endl;
            }
        }

        for (const auto cp : currentProcesses) {
            ProcessKey key = { cp.pId, cp.creationTime };
            uint64_t totalCpu = cp.kernelTime + cp.userTime;
            previousKey[key] = totalCpu;
        }
        previousProcesses = currentProcesses;
    }
}

