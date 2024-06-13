#pragma once
#include "qhalcondisplay.h"
#include <QDateTime>
#include <QAction>
#include <QMenu>
#include <QEvent>
#include <QWheelEvent>
#include <QDebug>

QHalconDisplay::QHalconDisplay(QWidget* parent)
    : QLabel(parent)
    , m_HalconID(NULL)
    , m_infolabel(nullptr)
    , is_move(false)
    , disable_mouse(true)
    , ban_menu(true)
    , cur_zoom_rate(1.0)
    , mSavePath("./")
{
    QAction* refresh = new QAction(u8"&还原", this);
    QAction* del = new QAction(u8"&删除选中框", this);
    QAction* delete_all = new QAction(u8"&删除所有框", this);
    QAction* save = new QAction(u8"&保存选中区域", this);
    QAction* catch_screen = new QAction(u8"&保存窗口图像", this);
    mActionlist << refresh << del << delete_all << save << catch_screen;
    mpMenu = new QMenu(this);
    mpMenu->addActions(mActionlist);

    hv_MouseMoveRow = 0;
    hv_MouseMoveCol = 0;

    GenEmptyObj(&show_img);
    GenEmptyObj(&show_region);
    GenEmptyObj(&show_regionSelected);

    this->setAttribute(Qt::WA_Hover, true);
    this->installEventFilter(this);

    connect(refresh, &QAction::triggered, this, [this]() {PaintOnHWnd(1); });
    connect(del, &QAction::triggered, this, &QHalconDisplay::DeleteRegionSelected);
    connect(save, &QAction::triggered, this, &QHalconDisplay::SaveRegion);
    connect(delete_all, &QAction::triggered, this, &QHalconDisplay::DeleteAll);
    connect(catch_screen, &QAction::triggered, this, &QHalconDisplay::SaveHalconWnd);
}

QHalconDisplay::~QHalconDisplay()
{
    delete mpMenu;
    qDeleteAll(mActionlist);
    mActionlist.clear();
}

/////////////////////////////////////////////显示/////////////////////////////////////////

//设置更新图像
void QHalconDisplay::SetImgToDisplay(HObject img, HObject region, HObject selected)
{
    show_img = img;
    show_region = region;
    show_regionSelected = selected;
    //ImgToChannel(show_img, show_channeled);
}
void QHalconDisplay::SetImgToDisplay(HObject img, HObject region)
{
    show_img = img;
    show_region = region;
    //ImgToChannel(show_img, show_channeled);
}
void QHalconDisplay::SetImgToDisplay(HObject img)
{
    show_img = img;
    //ImgToChannel(show_img, show_channeled);
}

//设置显示region
void QHalconDisplay::SetRegionToDisp(HObject region, HObject selected) {
    show_region = region;
    show_regionSelected = selected;
}
void QHalconDisplay::SetRegionToDisp(HObject region) {
    show_region = region;
}

//更新region
void QHalconDisplay::UpdateRegion(HObject& region, HObject& selected)
{
    region = show_region;
    selected = show_regionSelected;
}
void QHalconDisplay::UpdateRegion(HObject& region) {
    region = show_region;
}

//显示文字
void QHalconDisplay::AddToDispText(HTuple text, int size, HTuple font, HTuple row, HTuple col, HTuple color) {
    try
    {
        TextTuple textTuple;
        textTuple.text = text;
        textTuple.row = row;
        textTuple.col = col;
        textTuple.color = color;
#ifdef _DEBUG
        HTuple hFont;
        QueryFont(m_HalconID, &hFont);
        HTuple index = hFont.TupleFind(font);

        if (index[0].I() == -1) {
            ClearDispText();
            emit SendMsg(u8"字体不存在");
        }
        else
        {
           hFont = font + "-" + HTuple(size);//FONTNAME-STYLE-FONT_SIZE;STYLE:Normal/Bold/BoldItalic/Italic
            textTuple.font = hFont;
#endif
            m_hTextTuple.append(textTuple);
#ifdef _DEBUG
        }
#endif
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}

//清除文字
void QHalconDisplay::ClearDispText() {
    m_hTextTuple.clear();
    PaintOnHWnd(0);
}

//初始化窗口
void QHalconDisplay::InitHalconWnd(HTuple hWidth, HTuple hHeight, QLabel* textlabel, bool keep_ratio)
{
    m_infolabel = textlabel;
    mHwndWidth = hWidth;
    nHwndHeight = hHeight;
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

        HTuple m_scaledWidth, m_scaledHeight;
        //获取图像大小
        //GetImageSize(show_img, &m_imgWidth, &m_imgHeight);

        if (keep_ratio)
        {
            //获取缩放系数
            TupleMin2(1.0 * this->width() / mHwndWidth[0].D(), 1.0 * this->height() / nHwndHeight[0].D(), &m_hvScaledRate);
            //缩放
            m_scaledWidth = mHwndWidth[0].D() * m_hvScaledRate[0].D();
            m_scaledHeight = nHwndHeight[0].D() * m_hvScaledRate[0].D();
            //打开窗口
            if (1.0 * this->width() / mHwndWidth[0].D() < 1.0 * this->height() / nHwndHeight[0].D())
            {
                SetWindowExtents(m_HalconID, this->height() / 2.0 - m_scaledHeight[0].D() / 2.0, 0, this->width(), m_scaledHeight[0].D());
                m_xWndGap = 0;
                m_yWndGap = this->height() / 2.0 - m_scaledHeight[0].D() / 2.0;
            }
            else
            {
                SetWindowExtents(m_HalconID, 0, this->width() / 2.0 - m_scaledWidth[0].D() / 2.0, m_scaledWidth[0].D(), this->height());
                m_xWndGap = this->width() / 2.0 - m_scaledWidth[0].D() / 2.0;
                m_yWndGap = 0;
            }
        }
        else
        {
            SetWindowExtents(m_HalconID, 0, 0, this->width(), this->height());
            m_xWndGap = 0;
            m_yWndGap = 0;
        }
        SetPart(m_HalconID, 0, 0, nHwndHeight[0].I() - 1, mHwndWidth[0].I() - 1);
        PaintOnHWnd(0);
        disable_mouse = false;
        ban_menu = false;
        cur_zoom_rate = 1;
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}

//通道分离
void QHalconDisplay::ImgToChannel(HObject show_img, HObject& show_channeled) {
    HTuple hv_htNumber;
    CountChannels(show_img, &hv_htNumber);

    GenEmptyObj(&show_channeled);
    if (hv_htNumber[0].I() == 1)
    {
        CopyObj(show_img, &show_channeled, 1, -1);
    }

    if (hv_htNumber[0].I() == 3)
    {
        HObject ho_hImage1, ho_hImage2, ho_hImage3;
        Decompose3(show_img, &ho_hImage1, &ho_hImage2, &ho_hImage3);
        ConcatObj(ho_hImage1, ho_hImage2, &show_channeled);
        ConcatObj(show_channeled, ho_hImage3, &show_channeled);
    }

    if (hv_htNumber[0].I() == 4)
    {
        HObject ho_hImage1, ho_hImage2, ho_hImage3, ho_hImage4;
        Decompose4(show_img, &ho_hImage1, &ho_hImage2, &ho_hImage3, &ho_hImage4);
        ConcatObj(ho_hImage1, ho_hImage2, &show_channeled);
        ConcatObj(show_channeled, ho_hImage3, &show_channeled);
        ConcatObj(show_channeled, ho_hImage4, &show_channeled);
    }
}

//显示到窗口
void QHalconDisplay::PaintOnHWnd(bool refresh) {
    try
    {
        if (refresh) {
            SetPart(m_HalconID, 0, 0, nHwndHeight[0].I() - 1, mHwndWidth[0].I() - 1);
            cur_zoom_rate = 1;
            hv_MouseMoveCol=0;
            hv_MouseMoveRow=0;
            for(auto& t: m_hTextTuple)
            {
                t.show_row = t.row;
                t.show_col = t.col;
            }
        }
        ClearWindow(m_HalconID);
        DispObj(show_img, m_HalconID);
        SetColor(m_HalconID, "red");
        DispObj(show_region, m_HalconID);
        SetColor(m_HalconID, "blue");
        DispObj(show_regionSelected, m_HalconID);
        for(auto& t: m_hTextTuple)
        {
            SetColor(m_HalconID, t.color);
#ifdef _DEBUG
            SetFont(m_HalconID, t.font);
#endif
            t.show_row += hv_MouseMoveRow / cur_zoom_rate;
            t.show_col += hv_MouseMoveCol / cur_zoom_rate;
            SetTposition(m_HalconID, t.row, t.col);
            WriteString(m_HalconID, t.text);
        }
        SetColor(m_HalconID, "red");
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
        //qDebug()<<error_msg.Text();
    }
}

QString QHalconDisplay::savePath() const
{
    return mSavePath;
}

void QHalconDisplay::setSavePath(const QString &savePath)
{
    mSavePath = savePath;
}

//////////////////////////////////////////事件/////////////////////////////////////////

//鼠标滚轮缩放事件，用于缩放图像
void QHalconDisplay::wheelEvent(QWheelEvent* ev)
{
    if (!disable_mouse)
    {
        double Zoom;   //放大或缩小倍率
        HTuple  mouseRow, mouseCol;
        HTuple startRowBf, startColBf, endRowBf, endColBf, Ht, Wt, startRowAft, startColAft, endRowAft, endColAft;
        //滚轮前滑，放大
#if QT_VERSION <= QT_VERSION_CHECK(6,0,0)
        if (ev->delta() > 0)//鼠标滚轮向前滚动
#else
        if (ev->angleDelta().y() > 0)//鼠标滚轮向前滚动
#endif
        {
            Zoom = ZOOMRATIO;
            cur_zoom_rate *= ZOOMRATIO;
        }
        else//否则缩小
        {
            Zoom = 1 / ZOOMRATIO;
            cur_zoom_rate /= ZOOMRATIO;
        }
        //获取原图显示的部分，注意也是原图坐标
        GetPart(m_HalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
#if QT_VERSION <= QT_VERSION_CHECK(6,0,0)
        //获取光标在原图上的位置，注意是原图坐标，不是Label下的坐标
        mouseRow = (ev->pos().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
        mouseCol = (ev->pos().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();
#else
        //获取光标在原图上的位置，注意是原图坐标，不是Label下的坐标
        mouseRow = (ev->position().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
        mouseCol = (ev->position().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();
#endif
        //缩放前显示的图像宽高
        Ht = endRowBf - startRowBf;
        Wt = endColBf - startColBf;
        //普通版halcon能处理的图像最大尺寸是32K*32K。如果无限缩小原图像，导致显示的图像超出限制，则会造成程序崩溃
        if (1)
        {
            //计算缩放后的图像区域
            startRowAft = mouseRow - ((mouseRow - startRowBf) / Zoom);
            startColAft = mouseCol - ((mouseCol - startColBf) / Zoom);
            endRowAft = startRowAft + (Ht / Zoom);
            endColAft = startColAft + (Wt / Zoom);
            //如果放大过大，则返回
            if (endRowAft - startRowAft < 2 || cur_zoom_rate < MINZOOMINGRATIO || cur_zoom_rate > MAXZOOMINGRATIO)
            {
                cur_zoom_rate /= Zoom;
                return;
            }

            if (m_HalconID != NULL)
            {
                //如果有图像，则先清空图像
                DetachBackgroundFromWindow(m_HalconID);
            }
            SetPart(m_HalconID, startRowAft, startColAft, endRowAft, endColAft);
            //AttachBackgroundToWindow(show_img, m_HalconID);
            PaintOnHWnd(0);
            //qDebug()<<mouseRow.D()<<mouseCol.D();
            // qDebug()<<startRowAft.D()<<startColAft.D()<<endRowAft.D()<<endColAft.D();
        }
    }
}

//鼠标按下事件
void QHalconDisplay::mousePressEvent(QMouseEvent* ev)
{
    if (!disable_mouse && ev->button() == Qt::LeftButton)
    {
        HTuple startRowBf, startColBf;
        GetPart(m_HalconID, &startRowBf, &startColBf, nullptr, nullptr);
        //鼠标按下时的行列坐标
        hv_MouseDownRow = (ev->pos().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
        hv_MouseDownCol = (ev->pos().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();
        is_move = true;
    }
}

//鼠标释放事件
void QHalconDisplay::mouseReleaseEvent(QMouseEvent* ev)
{
    if (is_move && !disable_mouse)
    {
        if (ev->button() == Qt::LeftButton)
        {
            try
            {
                HTuple mouseRow, mouseCol, hv_htIndex = NULL;
                HTuple startRowBf, startColBf;
                is_move = false;
                GetPart(m_HalconID, &startRowBf, &startColBf, nullptr, nullptr);
                mouseRow = (ev->pos().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
                mouseCol = (ev->pos().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();

                CountObj(show_region, &hv_htIndex);
                if (hv_htIndex[0].I() == 0)return;
                GetRegionIndex(show_region, mouseRow.TupleInt(), mouseCol.TupleInt(), &hv_htIndex);
                if (hv_htIndex != NULL && hv_htIndex.TupleLength().I()) {
                    HTuple hv_Value1, hv_Value2, hv_Value3, hv_Value4, hv_Value5, hv_Value6, hv_Value7, hv_Value8, hv_Value9;
                    HTuple hv_area, hv_htRow, hv_htCol, oldFont;

                    SelectObj(show_region, &show_regionSelected, hv_htIndex);

                    if (hv_htIndex.Length() > 1)
                    {
                        HTuple hv_area_temp, hv_indice, hv_area_len;
                        AreaCenter(show_regionSelected, &hv_area_temp, nullptr, nullptr);
                        TupleSortIndex(hv_area_temp, &hv_indice);
                        TupleLength(hv_htIndex, &hv_area_len);
                        SelectObj(show_regionSelected, &show_regionSelected, hv_indice[0] + 1);
                    }

                    PaintOnHWnd(0);

                    //显示各种信息
                    HalconCpp::GetFont(m_HalconID, &oldFont);
                    HalconCpp::SetFont(m_HalconID, oldFont);
                    AreaCenter(show_regionSelected, &hv_area, &hv_htRow, &hv_htCol);

                    RegionFeatures(show_regionSelected, "area", &hv_Value1);
                    RegionFeatures(show_regionSelected, "width", &hv_Value2);
                    RegionFeatures(show_regionSelected, "height", &hv_Value3);
                    RegionFeatures(show_regionSelected, "row", &hv_Value4);
                    RegionFeatures(show_regionSelected, "column", &hv_Value5);
                    RegionFeatures(show_regionSelected, "row1", &hv_Value6);
                    RegionFeatures(show_regionSelected, "column1", &hv_Value7);
                    RegionFeatures(show_regionSelected, "row2", &hv_Value8);
                    RegionFeatures(show_regionSelected, "column2", &hv_Value9);

                    QString m_htString;
                    m_htString = QString::fromLocal8Bit("面积:") + QString::number(hv_Value1[0].D()) + "\n";
                    m_htString += QString::fromLocal8Bit("宽:") + QString::number(hv_Value2[0].D()) + "  ";
                    m_htString += QString::fromLocal8Bit("高:") + QString::number(hv_Value3[0].D()) + "\n";
                    m_htString += QString::fromLocal8Bit("行:") + QString::number(hv_Value4[0].D()) + "  ";
                    m_htString += QString::fromLocal8Bit("列:") + QString::number(hv_Value5[0].D()) + "\n";
                    m_htString += "row1:" + QString::number(hv_Value6[0].D()) + "  ";
                    m_htString += "col1:" + QString::number(hv_Value7[0].D()) + "\n";
                    m_htString += "row2:" + QString::number(hv_Value8[0].D()) + "  ";
                    m_htString += "col2:" + QString::number(hv_Value9[0].D()) + "\n";

                    DispText(m_HalconID, m_htString.toStdString().c_str(), "image", hv_htRow, hv_htCol,
                        "red", HTuple(), HTuple());
                }
                else {
                    GenEmptyObj(&show_regionSelected);
                    PaintOnHWnd(0);
                }
            }
            catch (HException& e)
            {
                HString error_msg = e.ProcName();
                error_msg += "()\r\n";
                error_msg += e.ErrorMessage();
                emit SendMsg(error_msg.Text());
            }
        }
    }
}

//鼠标移动事件
void QHalconDisplay::mouseMoveEvent(QMouseEvent* ev)
{
    if (!disable_mouse)
    {
        HTuple startRowBf, startColBf, endRowBf, endColBf, mouseRow, mouseCol;
        //得到当前的窗口坐标
        GetPart(m_HalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
        mouseRow = (ev->pos().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
        mouseCol = (ev->pos().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();

        try
        {
            //鼠标按下并移动时，移动图像，否则只显示坐标
            if (is_move)
            {
                //计算移动值
                hv_MouseMoveRow = mouseRow - hv_MouseDownRow[0].D();
                hv_MouseMoveCol = mouseCol - hv_MouseDownCol[0].D();
                //移动图像
                if (m_HalconID != NULL)
                {
                    //如果有图像，则先清空图像
                    DetachBackgroundFromWindow(m_HalconID);
                }
                SetPart(m_HalconID, startRowBf - hv_MouseMoveRow, startColBf - hv_MouseMoveCol
                        , endRowBf - hv_MouseMoveRow, endColBf - hv_MouseMoveCol);
                PaintOnHWnd(0);
            }
        }
        catch (HException& e)
        {
            HString error_msg = e.ProcName();
            error_msg += "()\r\n";
            error_msg += e.ErrorMessage();
            emit SendMsg(error_msg.Text());
        }
    }
}

//悬浮事件
void QHalconDisplay::HoverMove(QHoverEvent* ev) {
    HTuple hv_htNumber = NULL;
    if (!disable_mouse && (ev->type() == QEvent::HoverMove || ev->type() == QEvent::HoverEnter))
    {
        HTuple startRowBf, startColBf, endRowBf, endColBf, mouseRow, mouseCol, mouseButton;
        //得到当前的窗口坐标
        GetPart(m_HalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
#if QT_VERSION <= QT_VERSION_CHECK(6,0,0)
        mouseRow = (ev->pos().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
        mouseCol = (ev->pos().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();
#else
        mouseRow = (ev->position().y() - m_yWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startRowBf[0].D();
        mouseCol = (ev->position().x() - m_xWndGap[0].D()) / m_hvScaledRate[0].D() / cur_zoom_rate + startColBf[0].D();
#endif
        //设置坐标
        if (m_infolabel != nullptr) {
            //获取灰度值
            HTuple point_grayval, hv_gray;
            try
            {
                CountChannels(show_img, &hv_htNumber);
                if (hv_htNumber[0].I() == 1)
                    GetGrayval(show_img, mouseRow, mouseCol, &point_grayval);
                else if (hv_htNumber[0].I() == 3 || hv_htNumber[0].I() == 4)
                {
                    GetGrayval(show_img, mouseRow, mouseCol, &point_grayval);
                    hv_gray = point_grayval[0].D() * 0.299 + point_grayval[1].D() * 0.587 + point_grayval[2].D() * 0.112;
                }
                else
                    return;
            }
            catch (HException)
            {
                if (hv_htNumber != NULL)
                    switch (hv_htNumber[0].I())
                    {
                    case 1:m_infolabel->setText(QString::fromLocal8Bit("X：- Y：- <font color=\'#666666\'>灰度：- ")); break;
                    case 3:m_infolabel->setText(QString::fromLocal8Bit("X：- Y：-  <font color=\'#666666\'>灰度： - </font>"
                        "<font color=\'red\'>R：- </font><font color=\'green\'>G: - </font><font color=\'blue\'>B: - "));
                        break;
                    case 4:m_infolabel->setText(QString::fromLocal8Bit("X：- Y：- <font color=\'#666666\'>灰度：- </font>"
                        "<font color=\'red\'>R：- </font><font color=\'green\'>G: - </font><font color=\'blue\'>B: - </font>A: - "));
                        break;
                    default:
                        break;
                    }
                return;
            }
            switch (hv_htNumber[0].I())
            {
            case 1:
                m_infolabel->setText(QString::fromLocal8Bit("X：<b>%1</b> Y：<b>%2</b> <font color=\'#666666\'>灰度：<b>%3 ")
                    .arg(mouseCol[0].D()).arg(mouseRow[0].D()).arg(point_grayval[0].D())); break;
            case 3:
                m_infolabel->setText(QString::fromLocal8Bit("X：<b>%1</b> Y：<b>%2</b> <font color=\'#666666\'>灰度：<b>%3 </b>"
                    "<font color=\'red\'>R：<b>%4 </b><font color=\'green\'>G: <b>%5 </b><font color=\'blue\'>B: <b>%6")
                    .arg(mouseCol[0].D()).arg(mouseRow[0].D()).arg(hv_gray[0].D()).arg(point_grayval[0].I())
                    .arg(point_grayval[1].I()).arg(point_grayval[2].I())); break;
            case 4:
                m_infolabel->setText(QString::fromLocal8Bit("X：<b>%1</b> Y：<b>%2</b> <font color=\'#666666\'>灰度：<b>%3 </b>"
                    "<font color=\'red\'>R：<b>%4 </b><font color=\'green\'>G: <b>%5 </b><font color=\'blue\'>B: <b>%6 </b>A: <b>%7 ")
                    .arg(mouseCol[0].D()).arg(mouseRow[0].D()).arg(hv_gray[0].D()).arg(point_grayval[0].I())
                    .arg(point_grayval[1].I()).arg(point_grayval[2].I()).arg(point_grayval[3].I())); break;
            default:
                break;
            }
        }
    }
    else
    {
        if (m_infolabel != nullptr)
            try
        {
            CountChannels(show_img, &hv_htNumber);
            if (hv_htNumber != NULL)
                switch (hv_htNumber[0].I())
                {
                case 1:m_infolabel->setText(QString::fromLocal8Bit("X坐标：- Y坐标：- 灰度：- ")); break;
                case 3:m_infolabel->setText(QString::fromLocal8Bit("X坐标：- Y坐标：- 灰度：- R：- G: - B: - ")); break;
                case 4:m_infolabel->setText(QString::fromLocal8Bit("X坐标：- Y坐标：- 灰度：- R：- G: - B: - A: - ")); break;
                default:
                    break;
                }
        }
        catch (HException)
        {
            return;
        }
    }
}

//事件过滤器
bool QHalconDisplay::eventFilter(QObject* obj, QEvent* ev) {
    if (ev->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* m_mousePress = static_cast<QMouseEvent*>(ev);
        this->mousePressEvent(m_mousePress);
        return true;
    }
    else if (ev->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* m_mouseRelease = static_cast<QMouseEvent*>(ev);
        this->mouseReleaseEvent(m_mouseRelease);
        return true;
    }
    else if (ev->type() == QEvent::Wheel)
    {
        QWheelEvent* m_wheelEvent = static_cast<QWheelEvent*>(ev);
        this->wheelEvent(m_wheelEvent);
        return true;
    }
    else if (ev->type() == QEvent::HoverMove || ev->type() == QEvent::HoverEnter)
    {
        QHoverEvent* m_hoverEvent = static_cast<QHoverEvent*>(ev);
        this->HoverMove(m_hoverEvent);
        return true;
    }
    else
        return QObject::eventFilter(obj, ev);
}

//////////////////////////////////////////画roi//////////////////////////////////////////

//画矩形
void QHalconDisplay::MyDrawRect1(HObject& concat_region)
{
    HTuple hv_Row1, hv_Row2, hv_Col1, hv_Col2;
    HObject region_tmp;

    disable_mouse = true;
#if PAINT_BEFORE_DRAW
    PaintOnHWnd(1);
#endif // PAINT_BEFORE_DRAW
    DrawRectangle1(m_HalconID, &hv_Row1, &hv_Col1, &hv_Row2, &hv_Col2);
    GenRectangle1(&region_tmp, hv_Row1, hv_Col1, hv_Row2, hv_Col2);
    ConcatObj(concat_region, region_tmp, &concat_region);
    SetImgToDisplay(show_img, concat_region);
    PaintOnHWnd(0);
    disable_mouse = false;
    ban_menu = true;
}

//可旋转矩形
void QHalconDisplay::MyDrawRect2(HObject& concat_region)
{
    HTuple hv_Row, hv_Col, hv_Phi, hv_Length1, hv_Length2;
    HObject region_tmp;

    disable_mouse = true;
#if PAINT_BEFORE_DRAW
    PaintOnHWnd(1);
#endif // PAINT_BEFORE_DRAW
    DrawRectangle2(m_HalconID, &hv_Row, &hv_Col, &hv_Phi, &hv_Length1, &hv_Length2);
    GenRectangle2(&region_tmp, hv_Row, hv_Col, hv_Phi, hv_Length1, hv_Length2);
    ConcatObj(concat_region, region_tmp, &concat_region);
    SetImgToDisplay(show_img, concat_region);
    PaintOnHWnd(0);
    disable_mouse = false;
    ban_menu = true;
}

//画圆
void QHalconDisplay::MyDrawCircle(HObject& concat_region)
{
    HTuple hv_Row, hv_Col, hv_Radius;
    HObject region_tmp;

    disable_mouse = true;
#if PAINT_BEFORE_DRAW
    PaintOnHWnd(1);
#endif // PAINT_BEFORE_DRAW
    DrawCircle(m_HalconID, &hv_Row, &hv_Col, &hv_Radius);
    GenCircle(&region_tmp, hv_Row, hv_Col, hv_Radius);
    ConcatObj(concat_region, region_tmp, &concat_region);
    SetImgToDisplay(show_img, concat_region);
    PaintOnHWnd(0);
    disable_mouse = false;
    ban_menu = true;
}

//画椭圆
void QHalconDisplay::MyDrawEllipse(HObject& concat_region)
{
    HTuple hv_Row, hv_Col, hv_Phi, hv_Radius1, hv_Radius2;
    HObject region_tmp;

    disable_mouse = true;
#if PAINT_BEFORE_DRAW
    PaintOnHWnd(1);
#endif // PAINT_BEFORE_DRAW
    DrawEllipse(m_HalconID, &hv_Row, &hv_Col, &hv_Phi, &hv_Radius1, &hv_Radius2);
    GenEllipse(&region_tmp, hv_Row, hv_Col, hv_Phi, hv_Radius1, hv_Radius2);
    ConcatObj(concat_region, region_tmp, &concat_region);
    SetImgToDisplay(show_img, concat_region);
    PaintOnHWnd(0);
    disable_mouse = false;
    ban_menu = true;
}

//画自由区域
void QHalconDisplay::MyDrawFreeRegion(HObject& concat_region)
{
    HObject region_tmp;
    disable_mouse = true;
#if PAINT_BEFORE_DRAW
    PaintOnHWnd(1);
#endif // PAINT_BEFORE_DRAW
    DrawRegion(&region_tmp, m_HalconID);
    ConcatObj(concat_region, region_tmp, &concat_region);
    SetImgToDisplay(show_img, concat_region);
    PaintOnHWnd(0);
    disable_mouse = false;
    ban_menu = true;
}

//画线段
void QHalconDisplay::MyDrawLine(HObject& concat_region) {
    HObject region_tmp;
    disable_mouse = true;
#if PAINT_BEFORE_DRAW
    PaintOnHWnd(1);
#endif // PAINT_BEFORE_DRAW
    HTuple hv_Row1, hv_Col1, hv_Row2, hv_Col2;
    DrawLine(m_HalconID, &hv_Row1, &hv_Col1, &hv_Row2, &hv_Col2);
    GenRegionLine(&region_tmp, hv_Row1, hv_Col1, hv_Row2, hv_Col2);
    ConcatObj(concat_region, region_tmp, &concat_region);
    SetImgToDisplay(show_img, concat_region);
    PaintOnHWnd(0);
    disable_mouse = false;
    ban_menu = true;
}

////////////////////////////////////////右键菜单///////////////////////////////////////////

//菜单事件
void QHalconDisplay::contextMenuEvent(QContextMenuEvent*)
{
    QCursor cur = this->cursor();
    if (!ban_menu)
    {
        mpMenu->exec(cur.pos());
    }
    else
    {
        ban_menu = false;
    }
}

//删除框slot
void QHalconDisplay::DeleteRegionSelected()
{
    try
    {
        HTuple show_num, select_num, eq;
        HObject select_tmp;
        CountObj(show_region, &show_num);
        CountObj(show_regionSelected, &select_num);
        if (select_num[0].I() == 1)
            for (int i = 0; i < show_num.I(); i++)
            {
                SelectObj(show_region, &select_tmp, i + 1);
                TestEqualRegion(select_tmp, show_regionSelected, &eq);
                if (eq[0].I())
                {
                    RemoveObj(show_region, &show_region, i + 1);
                    RemoveObj(show_regionSelected, &show_regionSelected, 1);
                    PaintOnHWnd(0);
                    return;
                }
            }
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }

}

//保存区域slot
void QHalconDisplay::SaveRegion() {
    try
    {
        HObject empty_region;
        GenEmptyObj(&empty_region);
        HTuple IsEquel;
        TestEqualObj(empty_region, show_regionSelected, &IsEquel);
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd-hh-mm-ss");
        if(mSavePath.isEmpty())
            mSavePath = "./";
        str = mSavePath + str + ".bmp";
        if (IsEquel[0].I())
        {
            emit SendMsg(u8"无选中区域");
        }
        else
        {
            HObject save_img;
            ReduceDomain(show_img, show_regionSelected, &save_img);
            CropDomain(save_img, &save_img);
            WriteImage(save_img, "bmp", 0, str.toStdString().c_str());
        }
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}

//存窗口slot
void QHalconDisplay::SaveHalconWnd() {
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd-hh-mm-ss");
    str = mSavePath + str + ".bmp";
    try
    {
        HObject img;
        DumpWindowImage(&img, m_HalconID);
        WriteImage(img, "jpeg", 0, str.toStdString().c_str());
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}

//删除所有框slot
void QHalconDisplay::DeleteAll()
{
    try
    {
        disable_mouse = true;
        GenEmptyObj(&show_region);
        GenEmptyObj(&show_regionSelected);
        PaintOnHWnd(0);
        disable_mouse = false;
    }
    catch (HException& e)
    {
        HString error_msg = e.ProcName();
        error_msg += "()\r\n";
        error_msg += e.ErrorMessage();
        emit SendMsg(error_msg.Text());
    }
}
