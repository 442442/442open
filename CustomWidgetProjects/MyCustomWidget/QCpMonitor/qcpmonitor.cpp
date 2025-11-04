#include "qcpmonitor.h"
#include "monitor.h"
#include <QLayout>
#include <QStorageInfo>

#define WARNING_STYLE "QLabel{background-color:red;color:white;}"
#define NORMAL_STYLE "QLabel{background-color:transparent;color:black;}"

QCpMonitor::QCpMonitor(QWidget *parent)
    : QWidget{parent}, mpMonitor{new Monitor(parent)} {
    connect(mpMonitor, &Monitor::NoticeResource, this,
            &QCpMonitor::SlotOnResource, Qt::QueuedConnection);
    connect(mpMonitor, &Monitor::NoticeDiskFreeSpace, this,
            &QCpMonitor::SlotOnDiskFreeSpace, Qt::QueuedConnection);

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(3);

    QLabel *pCpuLabel = new QLabel("CPU");
    pCpuLabel->setFrameShape(QFrame::Box);
    pLayout->addWidget(pCpuLabel);

    QLabel *pMemLabel = new QLabel("Mem");
    pMemLabel->setFrameShape(QFrame::Box);
    pLayout->addWidget(pMemLabel);

    mMonitorLabels.insert("Cpu", pCpuLabel);
    mMonitorLabels.insert("Mem", pMemLabel);

    for (const auto &info : QStorageInfo::mountedVolumes()) {
        if (info.isValid() && info.isReady())
        {
            if (!info.isReadOnly())
            {
                QLabel *pDiskLabel = new QLabel(info.rootPath());
                pDiskLabel->setFrameShape(QFrame::Box);
                mMonitorLabels.insert(info.rootPath(), pDiskLabel);
                pLayout->addWidget(pDiskLabel);
            }
        }
    }

    for (const auto &plabel : std::as_const(mMonitorLabels)) {
        plabel->setAlignment(Qt::AlignCenter);
    }
}

QCpMonitor::~QCpMonitor()
{
    StopMonitor();
    delete mpMonitor;
    mpMonitor = nullptr;
    qDeleteAll(mMonitorLabels);
}

QCpMonitor::MonitorShowFlags QCpMonitor::showFlag() const { return mShowFlag; }

void QCpMonitor::setShowFlag(const MonitorShowFlags &newShowFlag) {
    if (mShowFlag == newShowFlag)
        return;
    mShowFlag = newShowFlag;

    for (auto it = mMonitorLabels.begin(); it != mMonitorLabels.end(); it++)
        if (it.key() == "Cpu")
            it.value()->setVisible(
                mShowFlag.testFlag(QCpMonitor::MonitorShowFlag::CPU));
        else if (it.key() == "Mem")
            it.value()->setVisible(
                mShowFlag.testFlag(QCpMonitor::MonitorShowFlag::Memory));
        else
            it.value()->setVisible(
                mShowFlag.testFlag(QCpMonitor::MonitorShowFlag::DiskSpace));

    emit showFlagChanged();
}

void QCpMonitor::SlotOnResource(double cpu, double memory) {
    if (auto it = mMonitorLabels.find("Cpu"); it != mMonitorLabels.end()){
        it.value()->setText("CPU\n" + QString::number(cpu) + "%");
        if (cpu > mCpuWarning) {
            emit NoticeCpuWarning();
            it.value()->setStyleSheet(WARNING_STYLE);
        }
        else
            it.value()->setStyleSheet(NORMAL_STYLE);
    }
    if (auto it = mMonitorLabels.find("Mem"); it != mMonitorLabels.end()){
        it.value()->setText(u8"内存\n" + QString::number(memory) + "%");
        if (memory > mMemoryWarning) {
            emit NoticeMemoryWarning();
            it.value()->setStyleSheet(WARNING_STYLE);
        }
        else
            it.value()->setStyleSheet(NORMAL_STYLE);
    }
    emit NoticeResource(cpu, memory);
}

void QCpMonitor::SlotOnDiskFreeSpace(const QMap<QString, unsigned long> &size) {
    for (auto it = size.begin(); it != size.end(); ++it)
        if (auto it_find = mMonitorLabels.constFind(it.key());
            it_find != mMonitorLabels.cend()) {
            it_find.value()->setText(it.key() + "\n" + QString::number(it.value()) +
                                     "Gb");
            if (it.value() < mDiskSpaceWarning) {
                emit NoticeSpaceWarning(it.key());
                it_find.value()->setStyleSheet(WARNING_STYLE);
            }
            else
                it_find.value()->setStyleSheet(NORMAL_STYLE);
        }
    emit NoticeDiskFreeSpace(size);
}

unsigned long QCpMonitor::diskSpaceWarning() const { return mDiskSpaceWarning; }

void QCpMonitor::setDiskSpaceWarning(unsigned long newDiskSpaceWarning) {
    if (mDiskSpaceWarning == newDiskSpaceWarning)
        return;
    mDiskSpaceWarning = newDiskSpaceWarning;
    emit diskSpaceWarningChanged();
}

void QCpMonitor::StartMonitor() { mpMonitor->StartMonitor(); }

void QCpMonitor::StopMonitor() { mpMonitor->StopMonitor(); }

double QCpMonitor::memoryWarning() const { return mMemoryWarning; }

void QCpMonitor::setMemoryWarning(double newMemoryWarning) {
    if (qFuzzyCompare(mMemoryWarning, newMemoryWarning))
        return;
    mMemoryWarning = newMemoryWarning;
    emit memoryWarningChanged();
}

double QCpMonitor::cpuWarning() const { return mCpuWarning; }

void QCpMonitor::setCpuWarning(double newCpuWarning) {
    if (qFuzzyCompare(mCpuWarning, newCpuWarning))
        return;
    mCpuWarning = newCpuWarning;
    emit cpuWarningChanged();
}
