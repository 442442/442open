#include "qgrahpicviewminihud.h"
#include <QLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFont>

QGrahpicViewMiniHud::QGrahpicViewMiniHud(QWidget *parent)
    :QWidget{parent},mpLayout{new QVBoxLayout(this)}
    ,mpInfoLabel{new QLabel(this)},mpImageLabel{new QLabel(this)}
{
    mpLayout->addWidget(mpImageLabel);
    mpLayout->addWidget(mpInfoLabel);
    mpImageLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    mpInfoLabel->setSizePolicy(QSizePolicy::Policy::Preferred,QSizePolicy::Policy::Preferred);
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
