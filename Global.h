#pragma once
#include <sysinfoapi.h>

DWORD getCores() {
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	return sys.dwNumberOfProcessors;
}
