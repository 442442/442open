#include "qgraphicrectitem.h"
#include <QPen>
#include <QPainter>

QGraphicRectItem::QGraphicRectItem(QGraphicsItem *parent)
    : QGraphicsRectItem{parent} {}

QGraphicRectItem::QGraphicRectItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem{rect, parent} {}

QGraphicRectItem::QGraphicRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem{x, y, w, h, parent} {}

void QGraphicRectItem::SetColor(const QColor &color) { mColor = color; }

QColor QGraphicRectItem::Color() const { return mColor; }

void QGraphicRectItem::SetLineWidth(const int &width) { mLineWidth = width; }

int QGraphicRectItem::lineWidth() const { return mLineWidth; }

void QGraphicRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(mColor, mLineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    double scaleFactor = painter->transform().m11();
    if (scaleFactor == 0)
        scaleFactor = 1;

    pen.setWidthF(pen.widthF() / scaleFactor);
    this->setPen(pen);

    QGraphicsRectItem::paint(painter, option, widget);
}
