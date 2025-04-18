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
    enum{ CustomPixmap = UserType + 3};

    QGraphicImgItem(QGraphicsItem *parent = nullptr);
    QGraphicImgItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    /// <summary>
    /// 添加像素值监听
    /// </summary>
    /// <param name = "listener">监听对象</param>
    void AddPixelListener(QPixelListener* listener);

    /**
     * @brief type 图元类型
     * @return
     */
    int type() const override;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    QPixelListener* mWatcher;
};

#endif // QGRAPHICIMGITEM_H
