#include <windows.h>
#include <iostream>

double CalculateCPULoad(FILETIME idleTime, FILETIME kernelTime, FILETIME userTime) {
    static FILETIME prevIdleTime = {0, 0};
    static FILETIME prevKernelTime = {0, 0};
    static FILETIME prevUserTime = {0, 0};

    ULARGE_INTEGER idle, kernel, user;
    ULARGE_INTEGER prevIdle, prevKernel, prevUser;

    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;

    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;

    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    prevIdle.LowPart = prevIdleTime.dwLowDateTime;
    prevIdle.HighPart = prevIdleTime.dwHighDateTime;

    prevKernel.LowPart = prevKernelTime.dwLowDateTime;
    prevKernel.HighPart = prevKernelTime.dwHighDateTime;

    prevUser.LowPart = prevUserTime.dwLowDateTime;
    prevUser.HighPart = prevUserTime.dwHighDateTime;

    ULONGLONG idleDiff = idle.QuadPart - prevIdle.QuadPart;
    ULONGLONG kernelDiff = kernel.QuadPart - prevKernel.QuadPart;
    ULONGLONG userDiff = user.QuadPart - prevUser.QuadPart;

    ULONGLONG totalDiff = kernelDiff + userDiff;

    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;

    if (totalDiff == 0) {
        return 0.0;
    }

    return (1.0 - (idleDiff * 1.0 / totalDiff)) * 100.0;
}

int main() {
    FILETIME idleTime, kernelTime, userTime;

    while (true) {
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            double cpuLoad = CalculateCPULoad(idleTime, kernelTime, userTime);
            std::cout << "CPU Usage: " << cpuLoad << "%" << std::endl;
        } else {
            std::cerr << "Failed to retrieve system times." << std::endl;
        }
        Sleep(1000); // Update every second
    }

    return 0;
}
