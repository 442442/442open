#ifndef REPLAYZOOMDIALOG_H
#define REPLAYZOOMDIALOG_H

#pragma once
#include <QDialog>
#include <halconcpp/HalconCpp.h>

using namespace HalconCpp;

namespace Ui {
class ReplayZoomDialog;
}

class ReplayZoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplayZoomDialog(QWidget *parent = nullptr);
    ~ReplayZoomDialog();

    void SetImgToDisplay(HObject img, HObject region);
    void InitHalconWnd(HTuple hWidth, HTuple hHeight, bool keep_ratio = 1);
    void AddToDispText(QString text, int size, QString font, int row, int col, QString color);
    void PaintOnHWnd();

private:
    void resizeEvent(QResizeEvent* e); 

    Ui::ReplayZoomDialog *ui;
};

#endif // REPLAYZOOMDIALOG_H
