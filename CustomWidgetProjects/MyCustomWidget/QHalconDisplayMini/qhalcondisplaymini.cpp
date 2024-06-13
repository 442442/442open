#include "qhalcondisplaymini.h"
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

QHalconDisplayMini::QHalconDisplayMini(QWidget* parent)
    : QLabel(parent)
{
    GenEmptyObj(&show_img);
    GenEmptyObj(&show_region);
}

QHalconDisplayMini::~QHalconDisplayMini()
{}

void QHalconDisplayMini::SetImgToDisplay(HObject img, HObject region)
{
    show_img = img;
    show_region = region;
}

void QHalconDisplayMini::SetImgToDisplay(HObject img)
{
    show_img = img;
}

void QHalconDisplayMini::PaintOnHWnd()
{
    try
    {
        SetPart(m_HalconID, 0, 0, m_imgHeight[0].I() - 1, m_imgWidth[0].I() - 1);
        ClearWindow(m_HalconID);
        DispObj(show_img, m_HalconID);
        SetColor(m_HalconID, "red");
        DispObj(show_region, m_HalconID);
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}

void QHalconDisplayMini::InitHalconWnd(HTuple hWidth, HTuple hHeight, bool keep_ratio)
{
    m_imgWidth = hWidth;
    m_imgHeight = hHeight;
    try
    {
        if (m_HalconID != NULL)
        {
            CloseWindow(m_HalconID);
        }

        HTuple thisID = (Hlong)this->winId();
        //打开窗口
        SetSystem("int_zooming", "false");
        SetWindowAttr("border_width", 0);
        SetCheck("~father");
        OpenWindow(0, 0, this->width(), this->height(), thisID, "visible", "", &m_HalconID);
        SetWindowType("WIN32-Window");
        SetCheck("father");
        SetDraw(m_HalconID, "margin");

        HTuple m_scaledWidth, m_scaledHeight, m_hvScaledRate;
        //获取图像大小
        //GetImageSize(show_img, &m_imgWidth, &m_imgHeight);

        if (keep_ratio)
        {
            //获取缩放系数
            TupleMin2(1.0 * this->width() / m_imgWidth[0].D(), 1.0 * this->height() / m_imgHeight[0].D(), &m_hvScaledRate);
            //缩放
            m_scaledWidth = m_imgWidth[0].D() * m_hvScaledRate[0].D();
            m_scaledHeight = m_imgHeight[0].D() * m_hvScaledRate[0].D();
            //打开窗口
            if (1.0 * this->width() / m_imgWidth[0].D() < 1.0 * this->height() / m_imgHeight[0].D())
            {
                SetWindowExtents(m_HalconID, this->height() / 2.0 - m_scaledHeight[0].D() / 2.0, 0, this->width(), m_scaledHeight[0].D());
            }
            else
            {
                SetWindowExtents(m_HalconID, 0, this->width() / 2.0 - m_scaledWidth[0].D() / 2.0, m_scaledWidth[0].D(), this->height());
            }
        }
        else
        {
            SetWindowExtents(m_HalconID, 0, 0, this->width(), this->height());
        }
        SetPart(m_HalconID, 0, 0, m_imgHeight[0].I() - 1, m_imgWidth[0].I() - 1);
        PaintOnHWnd();
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}

void QHalconDisplayMini::setBackgroundColor(const QColor &color) {
    mColor = color;
    repaint();
}

QColor QHalconDisplayMini::getBackgroundColor() const
{
    return mColor;
}

void QHalconDisplayMini::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QPalette pal = opt.palette;
    //读背景色，会读到red
    QBrush backgroundBrush = pal.brush(QPalette::Window);
    //读文字颜色，会读到white
    QColor textColor = pal.color(QPalette::Text);
    //读鼠标悬停时的背景色，会读到green
    //QBrush hoverBrush = pal.brush(QPalette::Highlight);
    backgroundBrush.setColor(mColor);

    painter.fillRect(event->rect(), backgroundBrush);
    painter.setPen(textColor);
}
