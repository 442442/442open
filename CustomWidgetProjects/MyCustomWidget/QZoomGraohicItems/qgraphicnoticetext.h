#ifndef QGRAPHICNOTICETEXT_H
#define QGRAPHICNOTICETEXT_H
#pragma once
#include <QGraphicsItem>
#include <QtUiPlugin/QDesignerExportWidget>
#include <QFont>

class QGraphicArrorItem;
/// <summary>
/// 文字图元，固定大小，可跟随箭头图元
/// </summary>
class QDESIGNER_WIDGET_EXPORT QGraphicNoticeText : public QGraphicsTextItem
{
public:
    QGraphicNoticeText(QGraphicsItem *parent = nullptr);
    QGraphicNoticeText(const QString &text, QGraphicsItem *parent = nullptr);
    /**
     * @brief AttachToArrow 附着到箭头图元上
     * @param pArrow 箭头图元
     */
    void AttachToArrow(QGraphicArrorItem* pArrow);
    /**
     * @brief SetNoticeTextFont 设置字体(别用父类的)
     * @param font 字体
     */
    void SetNoticeTextFont(const QFont& font);
    QFont GetNoticeTextFont() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QGraphicArrorItem* mpArrow{nullptr};
    QFont mFont;
};

#endif // QGRAPHICNOTICETEXT_H
