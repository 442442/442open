#include "nodeattributesdlg.h"
#include "ui_nodeattributesdlg.h"

NodeAttributesDlg::NodeAttributesDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeAttributesDlg)
{
    ui->setupUi(this);
}

NodeAttributesDlg::~NodeAttributesDlg()
{
    delete ui;
}

NodeAttributesDlg::NodeData NodeAttributesDlg::getData() {
    return NodeData{ui->nameEdit->text(),       ui->idEdit->text(),
                    ui->valueEdit->text(),      ui->valueTypeEdit->text(),
                    ui->valueRangeEdit->text(), ui->checkableEdit->text(),
                    ui->checkedCheck->isChecked(),    ui->enableCheck->isChecked()};
}
