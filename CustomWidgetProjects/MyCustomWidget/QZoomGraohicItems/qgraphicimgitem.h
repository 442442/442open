#ifndef QGRAPHICIMGITEM_H
#define QGRAPHICIMGITEM_H

#pragma once
#include <QGraphicsPixmapItem>
#include <QtUiPlugin/QDesignerExportWidget>

class QPixelListener;
/// <summary>
/// 图片qgraphicsitem类
/// </summary>
class QDESIGNER_WIDGET_EXPORT QGraphicImgItem : public QGraphicsPixmapItem
{
    Q_INTERFACES(QGraphicsPixmapItem)
public:
    QGraphicImgItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    /// <summary>
    /// 添加像素值监听
    /// </summary>
    /// <param name = "listener">监听对象</param>
    void AddPixelListener(QPixelListener* listener);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    QPixelListener* mWatcher;
};

#endif // QGRAPHICIMGITEM_H
