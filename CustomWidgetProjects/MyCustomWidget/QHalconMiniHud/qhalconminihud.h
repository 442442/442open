#ifndef QHALCONMINIHUD_H
#define QHALCONMINIHUD_H

#pragma once
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>
#include <halconcpp/HalconCpp.h>

using namespace HalconCpp;

class QLabel;
class QVBoxLayout;
class QHalconDisplayMini;
class QDESIGNER_WIDGET_EXPORT QHalconMiniHud : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor BackgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QString Text READ GetInfoText WRITE SetInfoText NOTIFY infoTextChanged)
    Q_PROPERTY(bool ZoomWidgetEnable READ GetReplayDialogEnable WRITE SetReplayDialogEnable NOTIFY zoomEnableChanged)

public:
    struct TextStruct
    {
        QString text;
        QString font;
        int row;
        int col;
        int size;
        QString color;
    };
    /// <summary>
    /// 控件构造
    /// </summary>
    /// <param name="parent"></param>
    QHalconMiniHud(QWidget* parent = nullptr);
    ~QHalconMiniHud();
    /// <summary>
    /// 显示图片+区域
    /// </summary>
    /// <param name="img">图片</param>
    /// <param name="region">区域</param>
    void SetImgToDisplay(HObject img, HObject region);
    void SetImgToDisplay(HObject img);
    /// <summary>
    /// 不显示的区域
    /// </summary>
    /// <param name="region">区域</param>
    void SetRegionNotToDisplay(HObject region);
    //显示到窗口
    void PaintOnHWnd();
    /// <summary>
    /// 初始化窗口
    /// </summary>
    /// <param name="hWidth">图片宽</param>
    /// <param name="hHeight">图片高</param>
    /// <param name="keep_ratio">保存比例显示</param>
    void InitHalconWnd(int hWidth, int hHeight, bool keep_ratio = 1);
    /// <summary>
    /// 设置label图像名称
    /// </summary>
    /// <param name="text">图像id</param>
    void SetInfoText(const QString& text);
    /// <summary>
    /// 获取图像id
    /// </summary>
    /// <returns>图像id</returns>
    QString GetInfoText() const;
    /// <summary>
    /// 设置双击是否弹出回放窗口
    /// </summary>
    /// <param name="enable"></param>
    void SetReplayDialogEnable(bool enable);
    /// <summary>
    /// 获取双击是否弹出回放窗口
    /// </summary>
    /// <returns></returns>
    bool GetReplayDialogEnable() const;
    /// <summary>
    /// 文字显示
    /// </summary>
    /// <returns></returns>
    void AddToDispText(QString text, int size, QString font, int row, int col, QString color);
    /// <summary>
    /// 清空文字
    /// </summary>
    /// <returns></returns>
    void ClearDispText();

    QColor getBackgroundColor() const;
    void  setBackgroundColor(QColor backgroundColor);

signals:
    void zoomEnableChanged();
    void infoTextChanged();
    void backgroundColorChanged();

private:
    /// <summary>
    /// 双击事件
    /// </summary>
    /// <param name="event"></param>
    void mouseDoubleClickEvent(QMouseEvent* event);

    QHalconDisplayMini* mpDisplayMini;//显示图片控件
    QLabel* mpInfoLabel;//显示信息label
    QVBoxLayout* mpLayout;//布局
    QList<TextStruct> mTextList;
    HObject mHiddenRegion;

    bool mReplayDialogEnable;//回放dialog使能
};

#endif // QHALCONMINIHUD_H
