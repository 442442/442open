#include "xmltreedelegate.h"
#include "CustomEdits/qpathedit.h"
#include "qxmltreewidget2.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#define CREATE_EDITOR(ClassName) ClassName *editor = new ClassName(parent);\
connect(editor, &ClassName::editingFinished, this, &XmlTreeDelegate::commitAndCloseEditor);

void XmlTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *XmlTreeDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const {
    Q_UNUSED(option);
    if (index.data(Qt::UserRole + 1).canConvert<QXmlTreeWidget2::NodeData>()) {
        auto nodeData =
            index.data(Qt::UserRole + 1).value<QXmlTreeWidget2::NodeData>();
        if (nodeData._valueType == "int") {
            CREATE_EDITOR(QSpinBox);
            auto range = nodeData._valueRange.split(",");
            if (range.size() != 2)
                return editor;
            editor->setRange(range.at(0).toInt(), range.at(1).toInt());
            return editor;
        } else if (nodeData._valueType == "float" ||
                   nodeData._valueType == "double") {
            CREATE_EDITOR(QDoubleSpinBox);
            auto range = nodeData._valueRange.split(",");
            if (range.size() != 2)
                return editor;
            editor->setRange(range.at(0).toDouble(), range.at(1).toDouble());
            return editor;
        } else if (nodeData._valueType == "string") {
            CREATE_EDITOR(QLineEdit);
            return editor;
        } else if (nodeData._valueType == "enum") {
            QComboBox *editor = new QComboBox(parent);
            auto range = nodeData._valueRange.split(",");
            editor->addItems(range);
            connect(editor, &QComboBox::currentIndexChanged, this,
                    &XmlTreeDelegate::commitAndCloseEditor);
            return editor;
        } else if (nodeData._valueType == "path") {
            CREATE_EDITOR(QPathEdit);
            return editor;
        } else if (nodeData._valueType == "file") {
            CREATE_EDITOR(QFileEdit);
            return editor;
        }
    }
    //return QStyledItemDelegate::createEditor(parent, option, index);
    return nullptr;
}

void XmlTreeDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (index.data(Qt::UserRole + 1).canConvert<QXmlTreeWidget2::NodeData>()) {
        auto nodeData =
            index.data(Qt::UserRole + 1).value<QXmlTreeWidget2::NodeData>();
        if (nodeData._valueType == "int") {
            QSpinBox *peditor = qobject_cast<QSpinBox *>(editor);
            peditor->setValue(index.data().value<QString>().toInt());
        } else if (nodeData._valueType == "float" ||
                   nodeData._valueType == "double") {
            QDoubleSpinBox *peditor = qobject_cast<QDoubleSpinBox *>(editor);
            peditor->setValue(index.data().value<QString>().toDouble());
        } else if (nodeData._valueType == "string") {
            QLineEdit *peditor = qobject_cast<QLineEdit *>(editor);
            peditor->setText(index.data().value<QString>());
        } else if (nodeData._valueType == "enum") {
            QComboBox *peditor = qobject_cast<QComboBox *>(editor);
            peditor->setCurrentIndex(
                peditor->findText(index.data().value<QString>()));
        } else if (nodeData._valueType == "path") {
            QPathEdit *peditor = qobject_cast<QPathEdit *>(editor);
            peditor->setText(index.data().value<QString>());
        } else if (nodeData._valueType == "file") {
            QFileEdit *peditor = qobject_cast<QFileEdit *>(editor);
            peditor->setText(index.data().value<QString>());
        }
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void XmlTreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (index.data(Qt::UserRole + 1).canConvert<QXmlTreeWidget2::NodeData>()) {
        model->blockSignals(true);
        auto nodeData =
            index.data(Qt::UserRole + 1).value<QXmlTreeWidget2::NodeData>();
        if (nodeData._valueType == "int") {
            QSpinBox *peditor = qobject_cast<QSpinBox *>(editor);
            model->setData(index, QString::number(peditor->value()),Qt::EditRole);
        } else if (nodeData._valueType == "float" ||
                   nodeData._valueType == "double") {
            QDoubleSpinBox *peditor = qobject_cast<QDoubleSpinBox *>(editor);
            model->setData(index, QString::number(peditor->value()),Qt::EditRole);
        } else if (nodeData._valueType == "string") {
            QLineEdit *peditor = qobject_cast<QLineEdit *>(editor);
            model->setData(index, peditor->text(),Qt::EditRole);
        } else if (nodeData._valueType == "enum") {
            QComboBox *peditor = qobject_cast<QComboBox *>(editor);
            model->setData(index, peditor->currentText(),Qt::EditRole);
        } else if (nodeData._valueType == "path") {
            QPathEdit *peditor = qobject_cast<QPathEdit *>(editor);
            model->setData(index, peditor->text(),Qt::EditRole);
        } else if (nodeData._valueType == "file") {
            QFileEdit *peditor = qobject_cast<QFileEdit *>(editor);
            model->setData(index, peditor->text(),Qt::EditRole);
        }
        model->blockSignals(false);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void XmlTreeDelegate::commitAndCloseEditor() {
    if (QWidget *editor = qobject_cast<QWidget *>(sender())) {
        if(!editor->isVisible()) return;
        emit commitData(editor);
        emit closeEditor(editor, QAbstractItemDelegate::EndEditHint::SubmitModelCache);
    }
}
