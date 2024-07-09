#ifndef QHALCONDISPLAY_H
#define QHALCONDISPLAY_H

#pragma once
#include <QLabel>
#include <QObject>
#include <QtUiPlugin/QDesignerExportWidget>
#include <halconcpp/HalconCpp.h>

#define PAINT_BEFORE_DRAW	 0						//画框之前是1否0显示还原
#define ZOOMRATIO			 1.1					//定义单步放大倍率
#define MAXZOOMINGRATIO		 40						//最大放大倍率
#define MINZOOMINGRATIO		 0.5					//最小放大倍率

using namespace HalconCpp;

class QMenu;
class QDESIGNER_WIDGET_EXPORT QHalconDisplay : public QLabel
{
    Q_OBJECT
public:
    QHalconDisplay(QWidget* parent = 0);
    ~QHalconDisplay();

    struct TextTuple
    {
        HTuple text;
        HTuple font;
        HTuple row;
        HTuple col;
        HTuple show_row;
        HTuple show_col;
        HTuple color;
    };

    //设置更新图像
    void SetImgToDisplay(HObject img, HObject region, HObject selected);
    void SetImgToDisplay(HObject img, HObject region);
    void SetImgToDisplay(HObject img);
    void SetRegionToDisp(HObject region, HObject selected);
    void SetRegionToDisp(HObject region);
    void UpdateRegion(HObject& region, HObject& selected);
    void UpdateRegion(HObject& region);
    void AddToDispText(HTuple text, int size, HTuple font, HTuple row, HTuple col, HTuple color);
    void ClearDispText();

    void MyDrawRect1(HObject& concat_region);		//画矩形
    void MyDrawRect2(HObject& concat_region);		//可旋转矩形
    void MyDrawCircle(HObject& concat_region);		//画圆
    void MyDrawEllipse(HObject& concat_region);		//画椭圆
    void MyDrawFreeRegion(HObject& concat_region);	//画自由区域
    void MyDrawLine(HObject& concat_region);		//画线段

    //显示到窗口
    void PaintOnHWnd(bool refresh);					//重制位置
    void InitHalconWnd(HTuple hWidth, HTuple hHeight,
        QLabel* textlabel = nullptr, bool keep_ratio = 1);		//初始化halcon窗口 keep_ratio1:图像原始比例显示 0：拉伸
    void ImgToChannel(HObject img, HObject& channel);//通道分离

    void wheelEvent(QWheelEvent* ev);				//鼠标滚轮缩放事件
    void mousePressEvent(QMouseEvent* ev);			//鼠标按下事件
    void mouseReleaseEvent(QMouseEvent* ev);		//鼠标释放事件
    void mouseMoveEvent(QMouseEvent* ev);			//鼠标移动事件
    void HoverMove(QHoverEvent* ev);				//悬浮事件
    bool eventFilter(QObject* obj, QEvent* ev);		//事件过滤器
    void contextMenuEvent(QContextMenuEvent*);		//菜单事件

    QString savePath() const;
    void setSavePath(const QString &savePath);

public slots:
    void DeleteRegionSelected();					//删除框slot
    void SaveRegion();								//保存区域slot
    void DeleteAll();								//删除所有框slot
    void SaveHalconWnd();                           //存窗口slot

signals:
    void SendMsg(const QString&);

public:
    HTuple m_HalconID;								//Halcon窗口句柄
    HObject show_img;								//显示图像
    HObject show_region;							//显示区域
    HObject show_regionSelected;					//选中区域
    QList<TextTuple> m_hTextTuple;					//显示字

    QLabel* m_infolabel;							//主界面显示坐标的标签

    QList<QAction*> mActionlist;					//右键菜单list
    QMenu* mpMenu;									//右键菜单

    HTuple hv_MouseDownRow;							//鼠标按下的位置
    HTuple hv_MouseDownCol;							//鼠标按下的位置
    HTuple hv_MouseMoveRow;                         //鼠标移动行
    HTuple hv_MouseMoveCol;                         //鼠标移动列
    HTuple m_hvScaledRate;							//当前放大倍率
    HTuple m_xWndGap;								//保持比例时hwnd与控件间隙
    HTuple m_yWndGap;								//保持比例时hwnd与控件间隙
    HTuple mHwndWidth, nHwndHeight;					//halcon窗口宽高

    bool is_move;									//是否移动图像标识
    bool disable_mouse;								//1：禁用鼠标事件
    bool ban_menu;									//1:禁用右键菜单
    double cur_zoom_rate;							//当前放大倍率

    QString mSavePath;                              //存图路径
};

#endif // QHALCONDISPLAY_H
