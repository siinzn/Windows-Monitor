#include "CpuTime.h"
#include <thread>
#include <iostream>

uint64_t FileTimeToInt64(const FILETIME ft) {
    ULARGE_INTEGER uli = { 0 };
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}
FILETIME_as_int systemData() {

    FILETIME_as_int fi;
    FILETIME idle, kernel, user;
    if (!GetSystemTimes(&idle, &kernel, &user)) return FILETIME_as_int{ 0,0,0 };
    fi.idle = FileTimeToInt64(idle);
    fi.kernel = FileTimeToInt64(kernel);
    fi.user = FileTimeToInt64(user);
    return fi;
}
cpuDeltas systemDataToInt(FILETIME_as_int previous, FILETIME_as_int current) {

    cpuDeltas dv;
    dv.dIdle = current.idle - previous.idle;
    dv.dKernel = current.kernel - previous.kernel;
    dv.dUser = current.user - previous.user;
    dv.dSystem = dv.dKernel + dv.dUser;

    return dv;
}
void printSystemData(cpuDeltas dv) {
    if (dv.dSystem != 0) {
        uint64_t cpu_percent = (dv.dSystem - dv.dIdle) * 100 / dv.dSystem;
        std::cout << "Idle : " << dv.dIdle << std::endl;
        std::cout << "Kernel : " << dv.dKernel << std::endl;
        std::cout << "User : " << dv.dUser << std::endl;
        std::cout << "System : " << cpu_percent << "%" << std::endl;
    }
}


