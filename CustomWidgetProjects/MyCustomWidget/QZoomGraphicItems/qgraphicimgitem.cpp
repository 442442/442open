#include "qgraphicimgitem.h"
#include "QZoomGraphicView/qpixellistener.h"
#include <QPixmap>
#include <QColor>
#include <QGraphicsSceneEvent>
#include <qevent.h>
#include <qfiledialog.h>

QGraphicImgItem::QGraphicImgItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), mWatcher(nullptr) {
    setFlags(QGraphicsItem::ItemIsFocusable);
}

QGraphicImgItem::QGraphicImgItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent), mWatcher(nullptr) {
    setFlags(QGraphicsItem::ItemIsFocusable);
}

QGraphicImgItem::~QGraphicImgItem()
{
    if (mWatcher)
    {
        delete mWatcher;
        mWatcher = nullptr;
    }
}

void QGraphicImgItem::AddPixelListener(QPixelListener *watcher)
{
     mWatcher = watcher;
}

int QGraphicImgItem::type() const
{
    return (int)CustomPixmap;
}

void QGraphicImgItem::keyPressEvent(QKeyEvent* event)
{
    if (event->keyCombination() == QKeyCombination(Qt::CTRL, Qt::Key_S))
    {
        if (this->pixmap().isNull())
            return;
        QString path = QFileDialog::getSaveFileName(NULL, "Save", "D:/", "Bitmap (*.bmp);; JPEG (*.jpg);; PNG (*.png)");
        if (path.isEmpty())
            return;
        this->pixmap().save(path);
    }
}

void QGraphicImgItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    if(mWatcher)
    {
        QPoint point = ev->pos().toPoint();
        if(this->pixmap().rect().contains(point))
            mWatcher->OnPixelInfo(point, QColor(this->pixmap().toImage().pixel(point)));
    }
}

void QGraphicImgItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    if(mWatcher && ev->modifiers() == Qt::ControlModifier)
    {
        QPoint point = ev->pos().toPoint();
        if(this->pixmap().rect().contains(point))
            mWatcher->OnPixelInfo(point, QColor(this->pixmap().toImage().pixel(point)));
    }
}
