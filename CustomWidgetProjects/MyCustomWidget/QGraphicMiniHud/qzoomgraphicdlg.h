#ifndef QZOOMGRAPHICDLG_H
#define QZOOMGRAPHICDLG_H

#include <QDialog>

namespace Ui {
class QZoomGraphicDlg;
}
class MyListener;
class QGraphicsItem;
class QZoomGraphicDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QZoomGraphicDlg(QWidget *parent = nullptr);
    ~QZoomGraphicDlg();

    void InitSceneSize(int width, int height);

    void AddImage(const QPixmap &pixmap);

    void AddGraphicsItem(QGraphicsItem* item);

    void Update();

private:
    void showEvent(QShowEvent *) override;

    Ui::QZoomGraphicDlg *ui;
    MyListener* mpListener{ nullptr };
};

#endif // QZOOMGRAPHICDLG_H
