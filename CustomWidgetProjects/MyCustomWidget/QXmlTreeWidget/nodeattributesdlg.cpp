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

void NodeAttributesDlg::setDate(const NodeData &data)
{
    ui->nameEdit->setText(data._name);
    ui->idEdit->setText(data._id);
    ui->valueEdit->setText(data._value);
    ui->valueTypeEdit->setText(data._valueType);
    ui->valueRangeEdit->setText(data._valueRange);
    ui->checkableEdit->setText(data._checkable);
    ui->checkedCheck->setCheckState(data._checked ? Qt::Checked : Qt::Unchecked);
    ui->enableCheck->setCheckState(data._enable ? Qt::Checked : Qt::Unchecked);
}
