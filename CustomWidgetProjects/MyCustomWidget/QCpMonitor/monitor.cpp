#include "monitor.h"
#include <QStorageInfo>
#if defined(WIN32) || defined(_MSC_VER) || defined(_WIN64)
#include <Windows.h>
#include <tchar.h>
#elif defined(__linux__) || defined(__linux) || defined (__gnu_linux__)
#include <fstream>
#include <iostream>
#include <unistd.h>
#endif

#if defined(WIN32) || defined(_MSC_VER) || defined(_WIN64)
__int64 Filetime2Int64(const FILETIME *ftime) {
    LARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

__int64 CompareFileTime(FILETIME preTime, FILETIME nowTime) {
    return Filetime2Int64(&nowTime) - Filetime2Int64(&preTime);
}
#elif defined(__linux__) || defined(__linux) || defined (__gnu_linux__)

struct CpuOccupy
{
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
};

CpuOccupy GetFileTime()
{
    std::ifstream file("/proc/stat");
    std::string line;
    unsigned long user = 0, nice = 0, system = 0, idle = 0;
    if (file.is_open()){
        std::getline(file,line);
        sscanf(line.c_str(),"cpu %lu %lu %lu %lu", &user,&nice,&system,&idle);
    }
    return {user,nice,system,idle};
}

double CalculateCpuUsage(const CpuOccupy& prev, const CpuOccupy& cur)
{
    unsigned long total_prev = prev.user + prev.nice + prev.system + prev.idle;
    unsigned long total_cur = cur.user + cur.nice + cur.system + cur.idle;
    unsigned long idle_diff = cur.idle - prev.idle;
    unsigned long total_diff = total_cur - total_prev;

    return 100.0 * (1.0 - static_cast<double>(idle_diff) / static_cast<double>(total_diff));
}

#endif

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
#if defined(WIN32) || defined(_MSC_VER) || defined(_WIN64)
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
#elif defined(__linux__) || defined(__linux) || defined (__gnu_linux__)
    std::ifstream file("/proc/meminfo");
    std::string key, value, unit;
    unsigned long long mem_total = 0, mem_free = 0;

    while (!file.eof()) {
        file >> key >> value >> unit;
        if(key == "MemTotal:")
            mem_total = std::stoull(value);
        if(key == "MemFree:")
            mem_free = std::stoull(value);
        if(mem_free > 0 && mem_total > 0)
            break;

    }

    return { 100.0 - round(static_cast<double>(mem_free) / static_cast<double>(mem_total) * 100.0) };
#endif
}

std::optional<double> Monitor::GetCpuUsage() {
#if defined(WIN32) || defined(_MSC_VER) || defined(_WIN64)
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
#elif defined(__linux__) || defined(__linux) || defined (__gnu_linux__)
    auto prev = GetFileTime();
    QThread::msleep(100);
    auto cur = GetFileTime();
    // qDebug() << "cpu" << CalculateCpuUsage(prev, cur);
    return { round(CalculateCpuUsage(prev, cur)) };
#endif
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
    for (const auto &info : QStorageInfo::mountedVolumes()) {
        if (info.isValid() && info.isReady())
            if (!info.isReadOnly())
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
