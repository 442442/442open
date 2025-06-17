#ifndef QGRAPHICCIRCLEITEM_H
#define QGRAPHICCIRCLEITEM_H

#include <QGraphicsItem>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT QGraphicCircleItem : public QGraphicsEllipseItem
{
public:
    enum{ CustomCircle = UserType + 4};

    explicit QGraphicCircleItem(QGraphicsItem *parent = nullptr);
    explicit QGraphicCircleItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit QGraphicCircleItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
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
    /**
     * @brief type 图元类型
     * @return
     */
    int type() const override;

private:
    QColor mColor{Qt::red};
    int mLineWidth{2};
};

#endif // QGRAPHICCIRCLEITEM_H
