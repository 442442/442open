#include "monitor.h"
#include <QStorageInfo>
#include <Windows.h>
#include <tchar.h>

__int64 Filetime2Int64(const FILETIME *ftime) {
    LARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

__int64 CompareFileTime(FILETIME preTime, FILETIME nowTime) {
    return Filetime2Int64(&nowTime) - Filetime2Int64(&preTime);
}

Monitor::Monitor(QObject *parent) : QThread{parent} {
    //this->start(QThread::LowestPriority);
}

Monitor::~Monitor()
{
    StopMonitor();
    mRunningFlag.store(false);
    this->exit();
    this->wait(mLoopTime * 2);
}

void Monitor::StartMonitor() {
    if (!this->isRunning())
        this->start(QThread::LowestPriority);
    mMonitorFlag.store(true);
}

void Monitor::StopMonitor() { mMonitorFlag.store(false); }

void Monitor::GetResource() {
    auto memUsage = GetMemUsage();
    auto cpuUsage = GetCpuUsage();
    emit NoticeResource(cpuUsage.has_value() ? cpuUsage.value() : 0,
                        memUsage.has_value() ? memUsage.value() : 0);

    auto diskSpace = GetdiskSpace();
    if (diskSpace.has_value())
    {
        emit NoticeDiskFreeSpace(diskSpace.value());
    }
}

std::optional<double> Monitor::GetMemUsage() {
    MEMORYSTATUSEX memsStat;
    memsStat.dwLength = sizeof(memsStat);
    if (!GlobalMemoryStatusEx(
            &memsStat)) // 如果获取系统内存信息不成功，就直接返回
    {
        return std::nullopt;
    }
    double nMemFree = memsStat.ullAvailPhys >> 20;
    double nMemTotal = memsStat.ullTotalPhys >> 20;
    double nMemUsed = nMemTotal - nMemFree;
    return {round(nMemUsed / nMemTotal * 100)};
}

std::optional<double> Monitor::GetCpuUsage() {
    HANDLE hEvent;
    static FILETIME preIdleTime;
    static FILETIME preKernelTime;
    static FILETIME preUserTime;
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
        return std::nullopt;
    preIdleTime = idleTime;
    preKernelTime = kernelTime;
    preUserTime = userTime;
    hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr); // 初始值为nonsignaled
    WaitForSingleObject(hEvent, 500);                     // 等待500毫秒
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
        return std::nullopt;
    long long idle = CompareFileTime(preIdleTime, idleTime);
    long long kernel = CompareFileTime(preKernelTime, kernelTime);
    long long user = CompareFileTime(preUserTime, userTime);
    double cpuRate = round(100.0 * (kernel + user - idle) / (kernel + user));
    return {cpuRate};
}

std::optional<QMap<QString, unsigned long>> Monitor::GetdiskSpace() {
    QMap<QString, unsigned long> freeSpace;
#if 0
    DWORD dwSize = MAX_PATH;
    TCHAR szLogicalDrives[MAX_PATH] = {0};
    DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);

    if (dwResult > 0 && dwResult <= MAX_PATH) {
        TCHAR *szSingleDrive = szLogicalDrives;

        while (*szSingleDrive) {
            uint64_t available, total, free;
            if (GetDiskFreeSpaceEx(szSingleDrive, (ULARGE_INTEGER *)&available,
                                   (ULARGE_INTEGER *)&total,
                                   (ULARGE_INTEGER *)&free)) {
                uint64_t /*Total, Available,*/ Free;
                // Total = total >> 20;
                // Available = available >> 20;
                Free = free >> 30;

                // all_Total += Total;   //总
                // all_Free += Free;   //剩余
                freeSpace[*szSingleDrive] = Free;
            }
            // 获取下一个驱动器号起始地址
            szSingleDrive += _tcslen(szSingleDrive) + 1;
        }
        return {freeSpace};
    }
    else
    {
        return std::nullopt;
    }
#else
    auto infoList = QStorageInfo::mountedVolumes();
    for (const auto &info : infoList) {
        freeSpace[info.rootPath()] = info.bytesFree() >> 30;
    }
    return {freeSpace};
#endif
}

void Monitor::run() {
    mRunningFlag.store(true);
    while (mRunningFlag) {
        if (mMonitorFlag)
            GetResource();
        QThread::msleep(mLoopTime);
    }
}
