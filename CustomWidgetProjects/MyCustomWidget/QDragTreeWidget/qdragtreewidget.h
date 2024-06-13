#ifndef QDRAGTREEWIDGET_H
#define QDRAGTREEWIDGET_H
#pragma once
#include <QTreeWidget>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT QDragTreeWidget : public QTreeWidget
{
    Q_OBJECT
    Q_PROPERTY(InteractionDragBehavior m_Behavior READ Behavior WRITE setBehavior NOTIFY behaviorChanged FINAL)

public:
    QDragTreeWidget(QWidget* parent = Q_NULLPTR);
    /// <summary>
    /// 2个树互相拖拽
    /// </summary>
    enum class InteractionDragBehavior
    {
        DragToCopy = 0,//复制节点
        DragToMove//移动节点
    };
    Q_ENUM(InteractionDragBehavior)
    InteractionDragBehavior Behavior() const;
    void setBehavior(const InteractionDragBehavior &Behavior);

signals:
    void behaviorChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    bool isRelated(const QTreeWidgetItem* parent, const QTreeWidgetItem* child);

    QPoint m_startDragPoint;
    InteractionDragBehavior m_Behavior;
};

#endif // QDRAGTREEWIDGET_H
