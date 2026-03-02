#include "Process.h"
#include "CpuTime.h"
#include "MemoryData.h"
#include <locale>
#include <iostream>
#include <thread>


int main()
{
    FILETIME_as_int first_run = systemData();
    int cpu_runs = 10;
    for (int i{ 0 }; i < cpu_runs; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        FILETIME_as_int second_run = systemData();

        cpuDeltas deltas = systemDataToInt(first_run, second_run);
        system("cls");
        printSystemData(deltas);
        first_run = second_run;
    }

    std::wcout.imbue(std::locale(""));
    printProcess();

    printMemoryInfo();
}

