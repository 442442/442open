#ifndef QCPMONITOR_H
#define QCPMONITOR_H
#pragma once
#include <QLabel>
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>
#include <QMap>

class Monitor;
class QDESIGNER_WIDGET_EXPORT QCpMonitor : public QWidget {
    Q_OBJECT
    Q_PROPERTY(MonitorShowFlags showFlag READ showFlag WRITE setShowFlag NOTIFY
                   showFlagChanged FINAL)
    Q_PROPERTY(double cpuWarning READ cpuWarning WRITE setCpuWarning NOTIFY
                   cpuWarningChanged FINAL)
    Q_PROPERTY(double memoryWarning READ memoryWarning WRITE setMemoryWarning
                   NOTIFY memoryWarningChanged FINAL)
    Q_PROPERTY(unsigned long diskSpaceWarning READ diskSpaceWarning WRITE
                   setDiskSpaceWarning NOTIFY diskSpaceWarningChanged FINAL)
public:
    explicit QCpMonitor(QWidget *parent = nullptr);
    ~QCpMonitor();

    enum MonitorShowFlag {
        None = 0x00,

        CPU = 0x01,
        Memory = 0x02,
        DiskSpace = 0x04,

        All = CPU | Memory | DiskSpace
    };
    Q_DECLARE_FLAGS(MonitorShowFlags, MonitorShowFlag)
    Q_FLAG(MonitorShowFlags)

    MonitorShowFlags showFlag() const;
    void setShowFlag(const MonitorShowFlags &newShowFlag);

    double cpuWarning() const;
    void setCpuWarning(double newCpuWarning);

    double memoryWarning() const;
    void setMemoryWarning(double newMemoryWarning);

    unsigned long diskSpaceWarning() const;
    void setDiskSpaceWarning(unsigned long newDiskSpaceWarning);

public slots:
    /// <summary>
    /// 开始性能监视
    /// </summary>
    void StartMonitor();
    /// <summary>
    /// 停止性能监控
    /// </summary>
    void StopMonitor();

signals:
    void showFlagChanged();
    void cpuWarningChanged();
    void memoryWarningChanged();
    void diskSpaceWarningChanged();

    void NoticeResource(double cpu, double memory);
    void NoticeDiskFreeSpace(const QMap<QString, unsigned long> &size);
    void NoticeCpuWarning();
    void NoticeMemoryWarning();
    void NoticeSpaceWarning(const QString &);

private slots:
    void SlotOnResource(double cpu, double memory);
    void SlotOnDiskFreeSpace(const QMap<QString, unsigned long> &size);

private:
    void ShowWarning(QLabel* plabel);


    MonitorShowFlags mShowFlag{MonitorShowFlag::All}; // 显示标志位
    QMap<QString, QLabel *> mMonitorLabels;
    Monitor *mpMonitor;
    double mCpuWarning{90};
    double mMemoryWarning{90};
    unsigned long mDiskSpaceWarning{1};
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QCpMonitor::MonitorShowFlags)
#endif // QCPMONITOR_H
