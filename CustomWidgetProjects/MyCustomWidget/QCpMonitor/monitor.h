#ifndef MONITOR_H
#define MONITOR_H
#pragma once
#include <QMap>
#include <QThread>
#include <optional>

class Monitor : public QThread {
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = nullptr);
    ~Monitor();
    /// <summary>
    /// 开始性能监视
    /// </summary>
    void StartMonitor();
    /// <summary>
    /// 停止性能监控
    /// </summary>
    void StopMonitor();

signals:
    void NoticeResource(double cpu, double memory);
    void NoticeDiskFreeSpace(const QMap<QString, unsigned long> &size);

private:
    /// <summary>
    /// 获取资源占用情况
    /// </summary>
    void GetResource();
    /// <summary>
    /// 获取内存使用率
    /// </summary>
    /// <returns>使用率</returns>
    std::optional<double> GetMemUsage();
    /// <summary>
    /// 获取cpu使用率
    /// </summary>
    /// <returns>使用率</returns>
    std::optional<double> GetCpuUsage();
    /// <summary>
    /// 获取硬盘空间(GB)
    /// </summary>
    /// <returns>使用率</returns>
    std::optional<QMap<QString, unsigned long>> GetdiskSpace();
    /// <summary>
    /// 轮询状态线程查询
    /// </summary>
    void run() override;

    const int mLoopTime{1000};
    std::atomic_bool mRunningFlag{false};
    std::atomic_bool mMonitorFlag{false};
};

#endif // MONITOR_H
