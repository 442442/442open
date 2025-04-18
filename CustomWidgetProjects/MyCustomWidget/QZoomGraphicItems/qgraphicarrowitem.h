﻿#ifndef QGRAPHICARROWITEM_H
#define QGRAPHICARROWITEM_H
#pragma once
#include <QGraphicsItem>
#include <QtUiPlugin/QDesignerExportWidget>
/// <summary>
/// 箭头图元
/// </summary>
class QDESIGNER_WIDGET_EXPORT QGraphicArrowItem :public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    enum class ArrowType {
        NoArrow = 0,
        SingleArrow,
        DoubleArrow
    };

    enum{ Arrow = UserType + 1};
    QGraphicArrowItem(QGraphicsItem *parent = nullptr);
    QGraphicArrowItem(const QPointF &startPoint, const QPointF &endPoint,
                      int lineWidth = 2, QColor color = Qt::green,
                      ArrowType arrowType = ArrowType::SingleArrow,
                      QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;
    /**
   * @brief SetLineItem 设置箭头起点终点 需要刷新view
   * @param startPoint 起点
   * @param endPoint 终点
   */
    void SetLineItem(const QPointF &startPoint, const QPointF &endPoint);
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
    /**
   * @brief SetDoubleArrow 设置箭头类型
   * @param doubleArrow
   */
    void SetArrowType(ArrowType arrowType);
    /**
     * @brief type 图元类型
     * @return
     */
    int type() const override;

private:
    QPointF mStartPoint;
    QPointF mEndPoint;
    QColor mColor{Qt::green};
    int mLineWidth{2};
    ArrowType mArrowType{ArrowType::SingleArrow};
};

#endif // QGRAPHICARROWITEM_H
