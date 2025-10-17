#ifndef XMLTREEDELEGATE_H
#define XMLTREEDELEGATE_H

#include <QStyledItemDelegate>

class XmlTreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private slots:
    void commitAndCloseEditor();
};

#endif // XMLTREEDELEGATE_H
