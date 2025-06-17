#include "qgrahpicviewminihud.h"
#include <QLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>
#include "qzoomgraphicdlg.h"

QGrahpicViewMiniHud::QGrahpicViewMiniHud(QWidget* parent)
	:QWidget{ parent }, mpLayout{ new QVBoxLayout(this) }
	, mpInfoLabel{ new QLabel(this) }, mpImageLabel{ new QLabel(this) }
{
	mpLayout->addWidget(mpImageLabel);
	mpLayout->addWidget(mpInfoLabel);
    mpImageLabel->setAlignment(Qt::AlignCenter);
	mpImageLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    mpImageLabel->setMinimumSize(100,100);
    mpImageLabel->setProperty("Selected","0");
    mpImageLabel->setObjectName("ImageLabel");
	mpInfoLabel->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
	mpInfoLabel->setAlignment(Qt::AlignCenter);
    mpInfoLabel->setProperty("Selected","0");
    mpInfoLabel->setObjectName("InfoLabel");
	mpLayout->setContentsMargins(0, 0, 0, 0);
    mpLayout->setSpacing(0);
    this->setProperty("Selected","0");
}

QGrahpicViewMiniHud::~QGrahpicViewMiniHud()
{

}

void QGrahpicViewMiniHud::SetImage(const QPixmap& pix, const QString& text, QList<QGraphicsItem*> item)
{
	SetImage(pix, text);
	SetGraphicsItemList(item);
}

void QGrahpicViewMiniHud::SetImage(const QPixmap& pix, const QString& text)
{
	SetImage(pix);
	this->SetText(text);
}

void QGrahpicViewMiniHud::SetImage(const QPixmap& pix)
{
	mPixmap = pix;
    mpImageLabel->setPixmap(mPixmap.scaled(mpImageLabel->size(),Qt::KeepAspectRatio,Qt::FastTransformation));
}

void QGrahpicViewMiniHud::SetText(const QString& text)
{
	mText = text;
    QFontMetricsF fontWidth(mpInfoLabel->font());
    QString elideNote = fontWidth.elidedText(text,Qt::ElideRight,mpInfoLabel->width());
    mpInfoLabel->setText(elideNote);
}

QString QGrahpicViewMiniHud::GetText() const
{
	return mpInfoLabel->text();
}

void QGrahpicViewMiniHud::SetGraphicsItemList(const QList<QGraphicsItem*>& item)
{
	mItem = item;
}

void QGrahpicViewMiniHud::ClearGraphicsItems()
{
	qDeleteAll(mItem);
	mItem.clear();
	delete mpZoomDlg;
	mpZoomDlg = nullptr;
}

void QGrahpicViewMiniHud::SetData(const QString& key, const QVariant& data)
{
	mData[key] = data;
}

QVariant QGrahpicViewMiniHud::GetData(const QString& key)
{
	return mData.value(key);
}

void QGrahpicViewMiniHud::SetHighLight()
{
    mpInfoLabel->setProperty("Selected","1");
    mpImageLabel->setProperty("Selected","1");
    this->setProperty("Selected","1");
    this->setStyleSheet("");
}

void QGrahpicViewMiniHud::CancelHighLight()
{
    mpInfoLabel->setProperty("Selected","0");
    mpImageLabel->setProperty("Selected","0");
    this->setProperty("Selected","0");
    this->setStyleSheet("");
}

void QGrahpicViewMiniHud::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
		return;

    if (mpZoomDlg == nullptr)
	{
        mpZoomDlg = new QZoomGraphicDlg(this);
        if(auto parent = qobject_cast<QWidget*>(this->parent()))
            mpZoomDlg->setWindowIcon(parent->windowIcon());
		mpZoomDlg->InitSceneSize(mPixmap.width(), mPixmap.height());
	}
    for (const auto& item : std::as_const(mItem)) {
        mpZoomDlg->AddGraphicsItem(item);
    }
    mpZoomDlg->Update();
    mpZoomDlg->AddImage(mPixmap);
    mpZoomDlg->setWindowTitle(mText);

	mpZoomDlg->show();
}
