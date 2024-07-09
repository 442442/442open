#include "qgraphicarroritem.h"
#include <QPainter>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define ARROW_HEAD_RATE 20

QGraphicArrorItem::QGraphicArrorItem(QGraphicsItem* parent)
	: QGraphicsItem(parent)
{
	//this->setFlag(ItemIsSelectable);
	//this->setFlag(ItemIsMovable);
	//this->setAcceptHoverEvents(true);
}

QGraphicArrorItem::QGraphicArrorItem(const QPointF& startPoint,
	const QPointF& endPoint, int lineWidth,
	QColor color, ArrowType arrowType,
	QGraphicsItem* parent)
	: QGraphicsItem(parent), mStartPoint(startPoint), mEndPoint(endPoint),
	mColor(color), mLineWidth(lineWidth), mArrowType(arrowType) {}

QRectF QGraphicArrorItem::boundingRect() const
{
	return QRectF(0, 0, mEndPoint.x(), mEndPoint.y());
}

void QGraphicArrorItem::paint(QPainter* painter, [[maybe_unused]] const QStyleOptionGraphicsItem* option,
	[[maybe_unused]] QWidget* widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	QPen pen(mColor, mLineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	QBrush brush(mColor, Qt::SolidPattern);

	double scaleFactor = painter->transform().m11();
	if (scaleFactor == 0)scaleFactor = 1;

	//qDebug()<<"scale:"<<scaleFactor<<"widthf:"<<pen.widthF();
	pen.setWidthF(pen.widthF() / scaleFactor); // 线段保持原来的线宽
	//qDebug()<<"widthf:"<<pen.widthF();
	painter->setPen(pen);
	painter->setBrush(brush);

	QLineF line(mEndPoint, mStartPoint);
	double angle = std::atan2(-line.dy(), line.dx());
	QPointF arrowP1, arrowP2, arrow2P1, arrow2P2;
	switch (mArrowType)
	{
	case ArrowType::DoubleArrow:
		arrow2P1 = line.p2() - QPointF(sin(angle + M_PI / 3) * mLineWidth * ARROW_HEAD_RATE,
			cos(angle + M_PI / 3) * mLineWidth * ARROW_HEAD_RATE);
		arrow2P2 = line.p2() - QPointF(sin(angle + M_PI - M_PI / 3) * mLineWidth * ARROW_HEAD_RATE,
			cos(angle + M_PI - M_PI / 3) * mLineWidth * ARROW_HEAD_RATE);
		painter->drawPolygon(QPolygonF() << line.p2() << arrow2P1 << arrow2P2);
		[[fallthrough]];
	case ArrowType::SingleArrow:
		arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * mLineWidth * ARROW_HEAD_RATE,
			cos(angle + M_PI / 3) * mLineWidth * ARROW_HEAD_RATE);
		arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * mLineWidth * ARROW_HEAD_RATE,
			cos(angle + M_PI - M_PI / 3) * mLineWidth * ARROW_HEAD_RATE);
		painter->drawPolygon(QPolygonF() << line.p1() << arrowP1 << arrowP2);
		[[fallthrough]];
	case ArrowType::NoArrow:
		painter->drawLine(line);
		break;
	default:
		break;
	}
}

void QGraphicArrorItem::SetLineItem(const QPointF& startPoint, const QPointF& endPoint)
{
	mStartPoint = startPoint;
	mEndPoint = endPoint;
}

QPointF QGraphicArrorItem::GetLineItem() const
{
	return (mStartPoint + mEndPoint) / 2;
}

void QGraphicArrorItem::SetColor(const QColor& color) {
	mColor = color;
}

QColor QGraphicArrorItem::Color() const
{
	return mColor;
}

void QGraphicArrorItem::SetLineWidth(const int& width) { mLineWidth = width; }

int QGraphicArrorItem::lineWidth() const
{
	return mLineWidth;
}

void QGraphicArrorItem::SetArrowType(ArrowType arrowType)
{
	mArrowType = arrowType;
}


