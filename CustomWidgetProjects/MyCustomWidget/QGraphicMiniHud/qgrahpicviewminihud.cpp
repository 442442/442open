#include "qgrahpicviewminihud.h"
#include <QLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>
#include "qzoomgraphicdlg.h"

template<typename T>
constexpr auto HIGHLIGHT_FONT(T msg) { return QString(u8"<font color=\'red\'><b>%1</b></font>").arg(msg); }

QGrahpicViewMiniHud::QGrahpicViewMiniHud(QWidget* parent)
	:QWidget{ parent }, mpLayout{ new QVBoxLayout(this) }
	, mpInfoLabel{ new QLabel(this) }, mpImageLabel{ new QLabel(this) }
{
	mpLayout->addWidget(mpImageLabel);
	mpLayout->addWidget(mpInfoLabel);
    mpImageLabel->setAlignment(Qt::AlignCenter);
	mpImageLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    mpImageLabel->setMinimumSize(100,100);
	mpInfoLabel->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
	mpInfoLabel->setAlignment(Qt::AlignCenter);
	mpLayout->setContentsMargins(0, 0, 0, 0);
	mpLayout->setSpacing(5);
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
    QFontMetricsF fontWidth(mpInfoLabel->font());
    QString elideNote = fontWidth.elidedText(mText,Qt::ElideRight,mpInfoLabel->width());
    mpInfoLabel->setText(HIGHLIGHT_FONT(elideNote));
}

void QGrahpicViewMiniHud::CancelHighLight()
{
    SetText(mText);
}

void QGrahpicViewMiniHud::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
		return;

    if (mpZoomDlg == nullptr)
	{
		mpZoomDlg = new QZoomGraphicDlg();
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





