#include "qzoomgraphicdlg.h"
#include "ui_qzoomgraphicdlg.h"

QZoomGraphicDlg::QZoomGraphicDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QZoomGraphicDlg)
{
    ui->setupUi(this);
}

QZoomGraphicDlg::~QZoomGraphicDlg()
{
    delete ui;
}
