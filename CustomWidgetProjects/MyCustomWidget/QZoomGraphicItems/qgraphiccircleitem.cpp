#include "qgraphiccircleitem.h"
#include <QPen>
#include <QPainter>

QGraphicCircleItem::QGraphicCircleItem(QGraphicsItem *parent)
    : QGraphicsEllipseItem{ parent } {}

QGraphicCircleItem::QGraphicCircleItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsEllipseItem{ rect, parent } {}

QGraphicCircleItem::QGraphicCircleItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsEllipseItem{ x, y, w, h, parent } {}

void QGraphicCircleItem::SetColor(const QColor &color)
{
    mColor = color;
}

QColor QGraphicCircleItem::Color() const
{
    return mColor;
}

void QGraphicCircleItem::SetLineWidth(const int &width)
{
    mLineWidth = width;
}

int QGraphicCircleItem::lineWidth() const
{
    return mLineWidth;
}

void QGraphicCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(mColor, mLineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    double scaleFactor = painter->transform().m11();
    if (scaleFactor == 0)
        scaleFactor = 1;

    pen.setWidthF(pen.widthF() / scaleFactor);
    this->setPen(pen);

    QGraphicsEllipseItem::paint(painter, option, widget);
}

int QGraphicCircleItem::type() const
{
    return (int)QGraphicCircleItem::CustomCircle;
}
