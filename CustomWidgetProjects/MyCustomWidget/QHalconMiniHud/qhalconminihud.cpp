#include "qhalconminihud.h"
#include "QHalconDisplayMini/qhalcondisplaymini.h"
#include "ReplayZoomDialog.h"
#include <QLayout>
#include <QMouseEvent>

QHalconMiniHud::QHalconMiniHud(QWidget* parent)
    :QWidget(parent)
    , mReplayDialogEnable(true)
{
    mpLayout = new QVBoxLayout(this);
    mpDisplayMini = new QHalconDisplayMini();
    mpLayout->addWidget(mpDisplayMini);
    mpInfoLabel = new QLabel("info");
    mpLayout->addWidget(mpInfoLabel);

    mpLayout->setContentsMargins(0, 0, 0, 0);
    mpLayout->setSpacing(0);
    mpDisplayMini->setMinimumHeight(100);
    auto dispPolicy = mpDisplayMini->sizePolicy();
    dispPolicy.setHorizontalPolicy(QSizePolicy::Policy::Preferred);
    dispPolicy.setVerticalPolicy(QSizePolicy::Policy::Expanding);
    //dispPolicy.setVerticalStretch(10);
    mpDisplayMini->setSizePolicy(dispPolicy);

    mpInfoLabel->setAlignment(Qt::AlignHCenter);
    auto infoPolicy = mpInfoLabel->sizePolicy();
    infoPolicy.setHorizontalPolicy(QSizePolicy::Policy::Preferred);
    infoPolicy.setVerticalPolicy(QSizePolicy::Policy::Preferred);
    //infoPolicy.setVerticalStretch(1);
    mpInfoLabel->setSizePolicy(infoPolicy);
    mpInfoLabel->setMaximumHeight(50);
}

QHalconMiniHud::~QHalconMiniHud()
{
    if (mpDisplayMini != nullptr)
    {
        delete mpDisplayMini;
        mpDisplayMini = nullptr;
    }
    if (mpInfoLabel != nullptr)
    {
        delete mpInfoLabel;
        mpInfoLabel = nullptr;
    }
}

void QHalconMiniHud::SetImgToDisplay(HObject img, HObject region)
{
    if (mpDisplayMini != nullptr)
    {
        mpDisplayMini->SetImgToDisplay(img, region);
    }
}

void QHalconMiniHud::SetImgToDisplay(HObject img)
{
    if (mpDisplayMini != nullptr)
    {
        mpDisplayMini->SetImgToDisplay(img);
    }
}

void QHalconMiniHud::SetRegionNotToDisplay(HObject region)
{
    mHiddenRegion = region;
}

void QHalconMiniHud::PaintOnHWnd()
{
    if (mpDisplayMini != nullptr)
    {
        mpDisplayMini->PaintOnHWnd();
    }
}

void QHalconMiniHud::InitHalconWnd(int hWidth, int hHeight, bool keep_ratio)
{
    if (mpDisplayMini != nullptr)
    {
        mpDisplayMini->InitHalconWnd(hWidth, hHeight, keep_ratio);
    }
}

void QHalconMiniHud::SetInfoText(const QString& text)
{
    if (mpInfoLabel != nullptr)
    {
        mpInfoLabel->setText(text);
    }
}

QString QHalconMiniHud::GetInfoText() const
{
    if (mpInfoLabel != nullptr)
    {
        return mpInfoLabel->text();
    }
    return QString();
}

void QHalconMiniHud::SetReplayDialogEnable(bool enable)
{
    mReplayDialogEnable = enable;
}


bool QHalconMiniHud::GetReplayDialogEnable() const
{
    return mReplayDialogEnable;
}

void QHalconMiniHud::AddToDispText(QString text, int size, QString font, int row, int col, QString color)
{
    TextStruct textTuple;
    textTuple.text = text;
    textTuple.row = row;
    textTuple.col = col;
    textTuple.color = color;
    textTuple.size = size;
    textTuple.font = font;
    mTextList.append(textTuple);
}

void QHalconMiniHud::ClearDispText()
{
    mTextList.clear();
}

void QHalconMiniHud::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;
    if (!mReplayDialogEnable)
        return;
    if (mpDisplayMini->m_imgWidth.TupleLength().D() <= 0)
        return;
    HObject hv_Empty;
    GenEmptyObj(&hv_Empty);
    if(mpDisplayMini->show_img.TestEqualObj(hv_Empty))
        return;
    ReplayZoomDialog* pZoom = new ReplayZoomDialog();
    pZoom->setWindowTitle(GetInfoText());
    pZoom->show();
    HObject showRegion;
    ConcatObj(mHiddenRegion,mpDisplayMini->show_region,&showRegion);
    pZoom->SetImgToDisplay(mpDisplayMini->show_img, showRegion);
    HTuple handle;
    if(mpDisplayMini->m_imgWidth * 9 / 16 > mpDisplayMini->m_imgHeight)
        pZoom->InitHalconWnd(mpDisplayMini->m_imgWidth, mpDisplayMini->m_imgWidth * 9 / 16);
    else
        pZoom->InitHalconWnd(mpDisplayMini->m_imgHeight * 16 / 9, mpDisplayMini->m_imgHeight);
    for (const auto& t : mTextList)
    {
        pZoom->AddToDispText(t.text,t.size,t.font,t.row,t.col,t.color);
    }
    pZoom->PaintOnHWnd();
}

QColor QHalconMiniHud::getBackgroundColor() const
{
    return mpDisplayMini->getBackgroundColor();
}

void QHalconMiniHud::setBackgroundColor(QColor backgroundColor)
{
    mpDisplayMini->setBackgroundColor(backgroundColor);
}
