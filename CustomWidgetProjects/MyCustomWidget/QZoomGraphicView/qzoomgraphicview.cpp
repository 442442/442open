#include "qzoomgraphicview.h"
#include <QWheelEvent>
#include <QPointer>
//#include <QDebug>

#define ZOOM_RATE 1.1
#define MAX_ZOOM_RATIO		 50						//最大放大倍率
#define MIN_ZOOM_RATIO		 0.5					//最小放大倍率

QZoomGraphicView::QZoomGraphicView(QWidget *parent)
    : QGraphicsView{parent}
    , mpScene{new QGraphicsScene(this->rect(), this)}
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mpScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(mpScene);
}

QZoomGraphicView::~QZoomGraphicView()
{
}

void QZoomGraphicView::InitSceneSize(int width, int height)
{
    mWidth = width;
    mHeight = height;
    Restore();
}

void QZoomGraphicView::Update()
{
    this->viewport()->update();
}

void QZoomGraphicView::Restore()
{
    mCurrentRatio = 1;
    mDefaultRatio = 1;
    // qDebug() << width << height;
    qreal wscale = rect().width() * 1.0 / mWidth;
    qreal hscale = rect().height() * 1.0 / mHeight;
    // qDebug() << rect();
    //qDebug() << wscale << hscale;
    mDefaultRatio = wscale < hscale ? wscale : hscale;

    this->setSceneRect(0, 0, mWidth, mHeight);
    this->fitInView(0, 0, mWidth, mHeight, Qt::KeepAspectRatio);

    Update();
}

void QZoomGraphicView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(this->scene() == nullptr) return; 
#if QT_VERSION <= QT_VERSION_CHECK(6,0,0)
        mMousePress = mapToScene(event->pos());
#else
        mMousePress = mapToScene(event->position().toPoint());
#endif
        //qDebug() << "mMousePress:" << mMousePress;
        isMousePressed = true;
        this->viewport()->setCursor(Qt::ClosedHandCursor);
    }
    else if(event->button() == Qt::RightButton)
    {
        Restore();
    }
    QGraphicsView::mousePressEvent(event);
}

void QZoomGraphicView::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePressed)
    {
#if QT_VERSION <= QT_VERSION_CHECK(6,0,0)
        QPointF curPos = mapToScene(event->pos());
#else
        QPointF curPos = mapToScene(event->position().toPoint());
#endif
        QPointF move = curPos - mMousePress;
        //qDebug()<<"move"<<move;
        QRectF curSceneRect = this->sceneRect();
        QRectF newSceneRect(curSceneRect.x() - move.x(),
                            curSceneRect.y() - move.y(), curSceneRect.width(),
                            curSceneRect.height());
        this->setSceneRect(newSceneRect);
        this->fitInView(newSceneRect,Qt::KeepAspectRatio);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void QZoomGraphicView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isMousePressed)
    {
        isMousePressed = false;
        this->viewport()->setCursor(Qt::OpenHandCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void QZoomGraphicView::wheelEvent(QWheelEvent *event)
{
    double zoom;
    double newRatio = mCurrentRatio;
#if QT_VERSION <= QT_VERSION_CHECK(6,0,0)
    if (event->delta() > 0)//鼠标滚轮向前滚动
#else
    if (event->angleDelta().y() > 0)//放大
#endif
    {
        zoom = ZOOM_RATE;
        newRatio *= ZOOM_RATE;
    }
    else//缩小
    {
        zoom = 1 / ZOOM_RATE;
        newRatio /= ZOOM_RATE;
    }
    //qDebug()<<newRatio;
    if (newRatio > MAX_ZOOM_RATIO || newRatio < MIN_ZOOM_RATIO)
        return;
    mCurrentRatio = newRatio;
    QRectF curSceneRect = this->sceneRect();
    auto scenePos = mapToScene(event->position().toPoint());
    //qDebug() << "scenePos:" << scenePos;
    double newRectX = scenePos.x() - (scenePos.x() - curSceneRect.x()) / zoom;
    double newRectY = scenePos.y() - (scenePos.y() - curSceneRect.y()) / zoom;
    double newWidth = curSceneRect.width() / zoom;
    double newHeight = curSceneRect.height() / zoom;

    this->setSceneRect(newRectX, newRectY, newWidth, newHeight);
    this->fitInView(newRectX, newRectY, newWidth, newHeight, Qt::KeepAspectRatio);

    mCurrentRatio = mCurrentRatio * zoom;
    Update();
}
