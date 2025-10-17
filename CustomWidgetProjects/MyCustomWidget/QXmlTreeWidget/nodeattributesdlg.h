#ifndef NODEATTRIBUTESDLG_H
#define NODEATTRIBUTESDLG_H

#include <QDialog>

namespace Ui {
class NodeAttributesDlg;
}

class NodeAttributesDlg : public QDialog
{
    Q_OBJECT

public:
    struct NodeData{
        QString _name;
        QString _id;
        QString _value;
        QString _valueType;
        QString _valueRange;
        QString _checkable;
        bool _checked;
        bool _enable;
    };

    explicit NodeAttributesDlg(QWidget *parent = nullptr);
    ~NodeAttributesDlg();

    NodeAttributesDlg::NodeData getData();

private:
    Ui::NodeAttributesDlg *ui;
};

#endif // NODEATTRIBUTESDLG_H
