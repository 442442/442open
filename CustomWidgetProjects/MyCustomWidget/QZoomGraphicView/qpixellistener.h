#ifndef QGRAPHICPIXMAPITEMWATCHER_H
#define QGRAPHICPIXMAPITEMWATCHER_H
#pragma once
#include <QColor>
/// <summary>
/// 像素值监听
/// </summary>
class QPixelListener
{
public:
    QPixelListener(){};
    /// <summary>
    /// 获取鼠标处像素值
    /// </summary>
    /// <param name = "rgb">像素值</param>
    virtual void OnPixelInfo([[maybe_unused]] const QColor &rgb){};
};

#endif // QGRAPHICPIXMAPITEMWATCHER_H
