#include "qgrahpicviewminihud.h"
#include <QLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFont>
#include <QMouseEvent>

#include "qzoomgraphicdlg.h"

QGrahpicViewMiniHud::QGrahpicViewMiniHud(QWidget *parent)
    :QWidget{parent},mpLayout{new QVBoxLayout(this)}
    ,mpInfoLabel{new QLabel(this)},mpImageLabel{new QLabel(this)}
{
    mpLayout->addWidget(mpImageLabel);
    mpLayout->addWidget(mpInfoLabel);
    mpImageLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    mpInfoLabel->setSizePolicy(QSizePolicy::Policy::Preferred,QSizePolicy::Policy::Preferred);
    mpInfoLabel->setAlignment(Qt::AlignCenter);
    mpLayout->setContentsMargins(0, 0, 0, 0);
    mpLayout->setSpacing(5);
}

QGrahpicViewMiniHud::~QGrahpicViewMiniHud()
{

}

void QGrahpicViewMiniHud::SetImage(const QPixmap &pix, const QString &text, const QList<QGraphicsItem *> &item)
{
    SetImage(pix, text);
    SetGraphicsItemList(item);
}

void QGrahpicViewMiniHud::SetImage(const QPixmap &pix, const QString &text)
{
    SetImage(pix);
    this->SetText(text);
}

void QGrahpicViewMiniHud::SetImage(const QPixmap &pix)
{
    mPixmap = pix;
    if (mpImageLabel->width() < mpImageLabel->height()) {
        mpImageLabel->setPixmap(mPixmap.scaledToHeight(mpImageLabel->height()));
    } else {
        mpImageLabel->setPixmap(mPixmap.scaledToWidth(mpImageLabel->width()));
    }
}

void QGrahpicViewMiniHud::SetText(const QString &text)
{
    mpInfoLabel->setText(text);
}

QString QGrahpicViewMiniHud::GetText() const
{
    return mpInfoLabel->text();
}

void QGrahpicViewMiniHud::SetGraphicsItemList(const QList<QGraphicsItem *> &item)
{
    mItem = item;
}

void QGrahpicViewMiniHud::SetData(const QString &key, const QVariant &data)
{
    mData[key] = data;
}

QVariant QGrahpicViewMiniHud::GetData(const QString &key)
{
    return mData.value(key);
}

void QGrahpicViewMiniHud::SetHighLight()
{
    mpInfoLabel->setPalette(Qt::yellow);
}

void QGrahpicViewMiniHud::CancelHighLight()
{
    mpInfoLabel->setPalette(Qt::transparent);
}

void QGrahpicViewMiniHud::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QZoomGraphicDlg* pZoom = new QZoomGraphicDlg(this);
    pZoom->AddImage(mPixmap);
    pZoom->setWindowTitle(mpInfoLabel->text());
    for (const auto& item : mItem) {
        pZoom->AddGraphicsItem(item);
    }
    pZoom->InitSceneSize(mPixmap.width(),mPixmap.height());
    pZoom->show();
}





