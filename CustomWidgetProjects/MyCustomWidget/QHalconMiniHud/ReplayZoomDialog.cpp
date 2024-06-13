#include "ReplayZoomDialog.h"
#include "ui_ReplayZoomDialog.h"
#include <QResizeEvent>

ReplayZoomDialog::ReplayZoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplayZoomDialog)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
}

ReplayZoomDialog::~ReplayZoomDialog()
{
    delete ui;
}

void ReplayZoomDialog::SetImgToDisplay(HObject img, HObject region)
{
    ui->HDisplay->SetImgToDisplay(img, region);
}

void ReplayZoomDialog::InitHalconWnd(HTuple hWidth, HTuple hHeight, bool keep_ratio)
{
    ui->HDisplay->InitHalconWnd(hWidth, hHeight, ui->InfoLabel, keep_ratio);
}

void ReplayZoomDialog::AddToDispText(QString text, int size, QString font, int row, int col, QString color)
{
    ui->HDisplay->AddToDispText(text.toStdString().c_str(),size,font.toStdString().c_str(),row,col,color.toStdString().c_str());
}

void ReplayZoomDialog::PaintOnHWnd()
{
    ui->HDisplay->PaintOnHWnd(1);
}

//窗口缩放
void ReplayZoomDialog::resizeEvent(QResizeEvent* e) {
    if (e->type() == QEvent::Resize) {
        HObject emp;
        HTuple eq = NULL;
        GenEmptyObj(&emp);
        TestEqualObj(emp, ui->HDisplay->show_img, &eq);
        if (!eq[0].I())
        {
            HTuple handle;
            ui->HDisplay->SetImgToDisplay(ui->HDisplay->show_img);
            ui->HDisplay->InitHalconWnd(ui->HDisplay->mHwndWidth, ui->HDisplay->mHwndWidth, ui->InfoLabel);
        }
    }
    else
        return;
}
