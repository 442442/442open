#ifndef QGRAPHICRECTITEM_H
#define QGRAPHICRECTITEM_H
#pragma once
#include <QGraphicsRectItem>
#include <QtUiPlugin/QDesignerExportWidget>
/// <summary>
/// 方框图元，固定粗细
/// </summary>
class QDESIGNER_WIDGET_EXPORT QGraphicRectItem : public QGraphicsRectItem
{
public:
    explicit QGraphicRectItem(QGraphicsItem *parent = nullptr);
    explicit QGraphicRectItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit QGraphicRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    /**
   * @brief SetColor 设置箭头颜色 需要刷新view
   * @param color 颜色
   */
    void SetColor(const QColor &color);
    QColor Color() const;
    /**
   * @brief SetLineWidth 设置线宽
   * @param width 线宽
   */
    void SetLineWidth(const int &width);
    int lineWidth() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


private:
    QColor mColor{Qt::red};
    int mLineWidth{2};
};

#endif // QGRAPHICRECTITEM_H
