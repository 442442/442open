#include "qgraphicnoticetext.h"
#include "qgraphicarroritem.h"
#include <QPainter>
#include <QWidget>
#include <QDebug>

QGraphicNoticeText::QGraphicNoticeText(QGraphicsItem* parent)
	:QGraphicsSimpleTextItem(parent)
{
}

QGraphicNoticeText::QGraphicNoticeText(const QString& text, QGraphicsItem* parent)
	:QGraphicsSimpleTextItem(text, parent)
{

}

void QGraphicNoticeText::AttachToArrow(QGraphicArrorItem* pArrow)
{
	mpArrow = pArrow;
}

void QGraphicNoticeText::SetNoticeTextFont(const QFont& font)
{
	mFont = font;
	this->setFont(font);
}

QFont QGraphicNoticeText::GetNoticeTextFont() const
{
	return mFont;
}

void QGraphicNoticeText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (mpArrow)
	{
		painter->setPen(mpArrow->Color());
		painter->setBrush(mpArrow->Color());

		QFont font(this->GetNoticeTextFont());

		double scaleFactor = painter->transform().m11();

		int fontSize = mpArrow->lineWidth() * 5 / scaleFactor;
		font.setPointSize(fontSize);
		this->setFont(font);
		qreal h = this->boundingRect().height() / 2;
		qreal w = this->boundingRect().width() / 2;
		QPointF newPos = mpArrow->GetLineItem() - QPointF(w, h);

		if (newPos.y() * scaleFactor < fontSize / 2)
		{
			newPos += QPointF(0, fontSize);
		}
		else if (newPos.y() * scaleFactor >= fontSize / 2
			&& newPos.y() * scaleFactor < widget->height() / 2)
		{
			newPos -= QPointF(0, fontSize);
		}
		else if (newPos.y() * scaleFactor >= widget->height() / 2
			&& newPos.y() * scaleFactor < widget->height() - fontSize / 2)
		{
			newPos += QPointF(0, fontSize);
		}
		else
		{
			newPos -= QPointF(0, fontSize);
		}
		this->setPos(newPos);
	}
	else
	{
		double scaleFactor = painter->transform().m11();

		if (scaleFactor == 0)
			scaleFactor = 1;
		QFont font(this->GetNoticeTextFont());
		int fontSize = mFont.pointSize() / scaleFactor;
		font.setPointSize(fontSize);
		this->setFont(font);
	}
	QGraphicsSimpleTextItem::paint(painter, option, widget);
}
