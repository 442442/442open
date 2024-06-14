#ifndef QGRAPHICPIXMAPITEMWATCHER_H
#define QGRAPHICPIXMAPITEMWATCHER_H
#pragma once
#include <QColor>
#include <QPointF>
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
    /// <param name = "pos">坐标</param>
    /// <param name = "color">像素值</param>
    virtual void OnPixelInfo([[maybe_unused]] const QPointF &pos,
                             [[maybe_unused]] const QColor &color){};
};

#endif // QGRAPHICPIXMAPITEMWATCHER_H
