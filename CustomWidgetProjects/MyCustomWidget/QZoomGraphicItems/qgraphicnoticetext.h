#ifndef QGRAPHICNOTICETEXT_H
#define QGRAPHICNOTICETEXT_H
#pragma once
#include <QGraphicsItem>
#include <QtUiPlugin/QDesignerExportWidget>
#include <QFont>

class QGraphicRectItem;
class QGraphicArrowItem;
/// <summary>
/// 文字图元，固定大小，可跟随箭头图元
/// </summary>
class QDESIGNER_WIDGET_EXPORT QGraphicNoticeText : public QGraphicsSimpleTextItem
{
public:
    QGraphicNoticeText(QGraphicsItem *parent = nullptr);
    QGraphicNoticeText(const QString &text, QGraphicsItem *parent = nullptr);
    /**
     * @brief AttachToCustomItem 附着到自定义图元上(箭头方框图片)
     * @param pItem 图元
     */
    void AttachToCustomItem(QGraphicsItem* pItem);
    /**
     * @brief SetNoticeTextFont 设置字体(别用父类的)
     * @param font 字体
     */
    void SetNoticeTextFont(const QFont& font);
    QFont GetNoticeTextFont() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool AttachToArrow(QGraphicArrowItem* pArrow, QPainter* painter, QWidget* widget);
    bool AttachToRect(QGraphicRectItem* pRect, QPainter* painter, QWidget* widget);

    QGraphicsItem* mpItem{nullptr};
    QFont mFont;
    QPointF mShift{};
    bool mInitFlag{false};
    qreal mOriginScale{};
};

#endif // QGRAPHICNOTICETEXT_H
