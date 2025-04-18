#include "qgraphicimgitem.h"
#include "QZoomGraphicView/qpixellistener.h"
#include <QPixmap>
#include <QColor>
#include <QGraphicsSceneEvent>

QGraphicImgItem::QGraphicImgItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), mWatcher(nullptr) {
    setFlags(QGraphicsItem::ItemIsFocusable);
}

QGraphicImgItem::QGraphicImgItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent), mWatcher(nullptr) {
    setFlags(QGraphicsItem::ItemIsFocusable);
}

void QGraphicImgItem::AddPixelListener(QPixelListener *watcher)
{
     mWatcher = watcher;
}

int QGraphicImgItem::type() const
{
    return (int)CustomPixmap;
}

void QGraphicImgItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    if(mWatcher)
    {
        auto point = ev->pos();
        mWatcher->OnPixelInfo(
            point, QColor(this->pixmap().toImage().pixel(point.toPoint())));
    }
}
