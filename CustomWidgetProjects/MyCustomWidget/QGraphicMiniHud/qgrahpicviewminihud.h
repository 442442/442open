#ifndef QGRAHPICVIEWMINIHUD_H
#define QGRAHPICVIEWMINIHUD_H
#pragma once
#include <QWidget>
#include <QGraphicsItem>
#include <QtUiPlugin/QDesignerExportWidget>

class QLabel;
class QVBoxLayout;
class QDESIGNER_WIDGET_EXPORT QGrahpicViewMiniHud : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString Text READ GetText WRITE SetText NOTIFY TextChanged)
public:    

    explicit QGrahpicViewMiniHud(QWidget* parent = nullptr);
    ~QGrahpicViewMiniHud();

    void SetImage(const QPixmap &pix, const QString &text, const QList<QGraphicsItem*>& item);
    void SetImage(const QPixmap &pix, const QString &text);
    void SetImage(const QPixmap &pix);

    void SetText(const QString &text);
    QString GetText() const;

    void SetGraphicsItemList(const QList<QGraphicsItem*>& item);

    void SetData(const QString& key,const QVariant& data);
    QVariant GetData(const QString& key);

    void SetHighLight();
    void CancelHighLight();

signals:
    void TextChanged();

private:
    /// <summary>
    /// 双击事件
    /// </summary>
    /// <param name="event"></param>
    void mouseDoubleClickEvent(QMouseEvent* event);


    QVBoxLayout* mpLayout;//布局
    QLabel* mpInfoLabel;//显示信息label
    QLabel* mpImageLabel;//显示图片label

    QList<QGraphicsItem*> mItem;
    QPixmap mPixmap;
    QString mText;

    QMap<QString,QVariant> mData;
};

#endif // QGRAHPICVIEWMINIHUD_H
