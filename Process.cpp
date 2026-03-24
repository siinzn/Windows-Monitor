#include "Process.h"
#include "CpuTime.h"
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <iostream>
#include <vector>


HANDLE getSnapshot() {
    HANDLE hProcess;
    hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcess == INVALID_HANDLE_VALUE) return INVALID_HANDLE_VALUE;
    return hProcess;
}

std::vector<ProcessInfo> enumerateProcesses(HANDLE process) {
    std::vector<ProcessInfo> processes;
    PROCESS_MEMORY_COUNTERS pmc;

    if (process == INVALID_HANDLE_VALUE) return processes;
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    BOOL Process32F = Process32First(process, &pe32);


    if (!Process32F) {
        CloseHandle(process);
        return processes;
    }

    do {
        ProcessInfo pi;
        pi.pId = pe32.th32ProcessID;
        pi.fileName = std::wstring(pe32.szExeFile);
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pi.pId);
        if (hProc == NULL) { continue; } //this skips directly to the while i didnt know that crazy
        FILETIME kernel, user, creation, exit_time;
        BOOL okay = GetProcessTimes(hProc, &creation, &exit_time, &kernel, &user);

        if (!okay) {
            CloseHandle(hProc);
            continue;
        }   //jumps to the while

        pi.creationTime = FileTimeToInt64(creation);
        pi.exitTime = FileTimeToInt64(exit_time);
        pi.kernelTime = FileTimeToInt64(kernel);
        pi.userTime = FileTimeToInt64(user);

        if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))) {
            size_t wssMB = pmc.WorkingSetSize / (1024 * 1024);
            pi.workingSet = wssMB;
        }
        else {
            pi.workingSet = 0;
        }

        processes.push_back(pi);
        CloseHandle(hProc);

    } while (Process32Next(process, &pe32));
    CloseHandle(process);
    return processes;
}

/*void printProcess() {
    HANDLE snapshot = getSnapshot();
    std::vector<ProcessInfo> processes = enumerateProcesses(snapshot);
    for (const auto& p : processes) {
        std::wcout << p.fileName << L" - " << p.pId << std::endl;
    }
}
*/