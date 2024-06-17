#ifndef QHALCONDISPLAYMINI_H
#define QHALCONDISPLAYMINI_H

#pragma once
#include <halconcpp/HalconCpp.h>
#include <QLabel>
#include <QtUiPlugin/QDesignerExportWidget>

using namespace HalconCpp;

class QDESIGNER_WIDGET_EXPORT QHalconDisplayMini  : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY nameChanged)

public:
    QHalconDisplayMini(QWidget* parent = nullptr);
    ~QHalconDisplayMini();

    void SetImgToDisplay(HObject img, HObject region);
    void SetImgToDisplay(HObject img);

    //显示到窗口
    void PaintOnHWnd();
    void InitHalconWnd(HTuple hWidth, HTuple hHeight, bool keep_ratio = 1);

    /// <summary>
    /// qss颜色
    /// </summary>
    /// <param name="color"></param>
    void setBackgroundColor(const QColor& color);
    /// <summary>
    /// qss颜色
    /// </summary>
    /// <returns></returns>
    QColor getBackgroundColor() const;

    void paintEvent(QPaintEvent* event);

signals:
    void SendMsg(const QString&);
    void nameChanged();

public:
    HTuple m_HalconID;
    //Halcon窗口句柄
    HObject show_img;								//显示图像
    HObject show_region;							//显示区域

    HTuple m_imgWidth, m_imgHeight;

    QColor mColor;
};

#endif // QHALCONDISPLAYMINI_H
