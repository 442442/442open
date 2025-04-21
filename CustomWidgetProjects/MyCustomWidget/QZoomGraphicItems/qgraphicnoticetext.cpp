#include "qgraphicnoticetext.h"
#include "qgraphicarrowitem.h"
#include "qgraphicrectitem.h"
#include <QPainter>
#include <QWidget>
#include <QDebug>

constexpr int LINE_WIDTH_FONT_RATIO = 5;

QGraphicNoticeText::QGraphicNoticeText(QGraphicsItem* parent)
	:QGraphicsSimpleTextItem(parent)
{
}

QGraphicNoticeText::QGraphicNoticeText(const QString& text, QGraphicsItem* parent)
	:QGraphicsSimpleTextItem(text, parent)
{

}

void QGraphicNoticeText::AttachToCustomItem(QGraphicsItem* pItem)
{
    mpItem = pItem;
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
    if (mpItem)
    {
        bool repaint{false};
        switch (mpItem->type()) {
        case (int)QGraphicsItem::UserType + 1:
            repaint = AttachToArrow(static_cast<QGraphicArrowItem*>(mpItem), painter, widget);
            break;
        case (int)QGraphicsItem::UserType + 2:
            repaint = AttachToRect(static_cast<QGraphicRectItem*>(mpItem), painter, widget);
            break;
        default:
            break;
        }
        if(repaint)
            QGraphicsSimpleTextItem::paint(painter, option, widget);
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
        QGraphicsSimpleTextItem::paint(painter, option, widget);
    }	
}

bool QGraphicNoticeText::AttachToArrow(QGraphicArrowItem *pArrow, QPainter* painter, QWidget* widget)
{
    if(!pArrow || !widget) return false;

    painter->setPen(pArrow->Color());
    painter->setBrush(pArrow->Color());

    QFont font(this->GetNoticeTextFont());

    qreal scaleFactor = painter->transform().m11();

    int fontSize = pArrow->lineWidth() * LINE_WIDTH_FONT_RATIO / scaleFactor;
    font.setPointSize(fontSize);
    this->setFont(font);
    qreal h = this->boundingRect().height();
    qreal w = this->boundingRect().width();
    QPointF newPos = pArrow->boundingRect().center() - QPointF(w/2, h/2) + QPointF(fontSize, fontSize);
    if(!mInitFlag)
    {
        const QRectF newRect = { newPos.x(), newPos.y(), w, h };
        const QRectF widgetRect = {0, 0, widget->width() / scaleFactor, widget->height() / scaleFactor };
        mOriginScale = scaleFactor;
        if(newRect.left() < 0)
            mShift.setX(0 - newPos.x());
        if(newRect.top() < 0)
            mShift.setY(0 - newPos.y());
        if(newRect.right() > widgetRect.right())
            mShift.setX(widgetRect.right() - newRect.right());
        if(newRect.bottom() > widgetRect.bottom())
            mShift.setY(widgetRect.bottom() - newPos.y());
        mInitFlag = true;
        this->setPos(newPos += mShift);
        return false;
    }
    else
    {
        newPos += mShift * mOriginScale / scaleFactor;
        this->setPos(newPos);
        return true;
    }
}

bool QGraphicNoticeText::AttachToRect(QGraphicRectItem *pRect, QPainter *painter, QWidget *widget)
{
    if(!pRect || !widget) return false;

    painter->setPen(pRect->Color());
    painter->setBrush(pRect->Color());

    double scaleFactor = painter->transform().m11();
    if (scaleFactor == 0)
        scaleFactor = 1;
    QFont font(this->GetNoticeTextFont());
    int fontSize = pRect->lineWidth() * LINE_WIDTH_FONT_RATIO / scaleFactor;
    font.setPointSize(fontSize);
    font.setPointSize(fontSize);
    this->setFont(font);

    const QRectF newRect = {pRect->boundingRect().topRight(), this->boundingRect().size()};
    const QRectF widgetRect = {0, 0, widget->width() / scaleFactor, widget->height() / scaleFactor };
    if(newRect.right() > widgetRect.width())
    {
        mShift.setX(pRect->boundingRect().left() - newRect.right());
    }
    else
    {
        mShift = QPointF{};
    }
    this->setPos(pRect->boundingRect().topRight() + mShift);
    if(!mInitFlag)
    {
        mInitFlag = true;
        return false;
    }
    else
    {
        return true;
    }
}
