#pragma once
#include <Windows.h>
#include <cstdint>

//declarations

struct FILETIME_as_int{ uint64_t idle,kernel,user;};
struct cpuDeltas{ uint64_t dIdle, dKernel, dUser, dSystem;};

//convert FILETIME type to int 64
uint64_t FileTimeToInt64(const FILETIME ft);

//Get Idle, Kernel, User values as int 64
FILETIME_as_int systemData();

//calculate Idle, Kernel, User, System values(cpu maths)
cpuDeltas systemDataToInt(FILETIME_as_int previous, FILETIME_as_int current);
void printSystemData(cpuDeltas dv);
