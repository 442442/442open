#ifndef QZOOMGRAPHICVIEW_H
#define QZOOMGRAPHICVIEW_H
#pragma once
#include <QGraphicsView>
#include <QPointF>
#include <QtUiPlugin/QDesignerExportWidget>
/// <summary>
/// 能放大的QGraphicsView
/// </summary>
class QDESIGNER_WIDGET_EXPORT QZoomGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    QZoomGraphicView(QWidget *parent = nullptr);
    ~QZoomGraphicView();
    /// <summary>
    /// 初始化场景大小
    /// </summary>
    /// <param name = "width">宽</param>
    /// <param name = "height">高</param>
    void InitSceneSize(int width, int height);
    /// <summary>
    /// 刷新显示
    /// </summary>
    void Update();
    /// <summary>
    /// 显示还原
    /// </summary>
    void Restore();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    QGraphicsScene* mpScene;
    bool isMousePressed{false};
    double mCurrentRatio{1};
    double mDefaultRatio{1};

    int mWidth{0};
    int mHeight{0};

    QPointF mMousePress;
};

#endif // QZOOMGRAPHICVIEW_H
