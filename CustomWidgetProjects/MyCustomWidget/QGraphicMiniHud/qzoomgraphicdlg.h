#ifndef QZOOMGRAPHICDLG_H
#define QZOOMGRAPHICDLG_H

#include <QDialog>

namespace Ui {
class QZoomGraphicDlg;
}

class QZoomGraphicDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QZoomGraphicDlg(QWidget *parent = nullptr);
    ~QZoomGraphicDlg();

private:
    Ui::QZoomGraphicDlg *ui;
};

#endif // QZOOMGRAPHICDLG_H
