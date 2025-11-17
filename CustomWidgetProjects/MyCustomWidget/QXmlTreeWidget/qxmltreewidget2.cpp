#include "qxmltreewidget2.h"

#include "xmltreedelegate.h"
#include "nodeattributesdlg.h"
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QHeaderView>
#include <QMenu>
#include <QUrl>

#ifdef _DEBUG
#include <QDebug>
#endif

#define STRING_IS_TRUE(str)                                                    \
((str) == "true" || (str) == "True" || (str) == "1" || (str) == "TRUE")
#define STRING_IS_ENABLE(str)                                                  \
((str) == "true" || (str) == "True" || (str) == "1" || (str) == "TRUE" ||    \
                                                                                 (str) == "ENABLE" || (str) == "enable" || (str) == "Enable")
const QString XML_COMMENT2 =
    u8"name显示文字;id唯一标识;valueType参数类型(int/float/double/string/enum/"
                             u8"path/"
                             u8"file);valueRange值域(逗号分割);checkable可勾选;checked勾选;enable使能;";

QXmlTreeWidget2::QXmlTreeWidget2(QWidget *parent)
    : QTreeWidget(parent), mpMenu(new QMenu(this)),
    mpOpenAction(new QAction(u8"打开路径", this)),
    mpCopyAction(new QAction(u8"复制文本", this)),
    mpAddAction(new QAction(u8"添加子节点", this)),
    mpDelAction(new QAction(u8"删除节点", this)) {
    this->setAlternatingRowColors(true);
    this->header()->setSectionResizeMode(QHeaderView::Stretch);
    this->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    this->setColumnCount(2);
    this->setHeaderLabels(QStringList() << u8"名称" << u8"数据");
    this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    this->setItemDelegate(new XmlTreeDelegate(this));
    this->setEditTriggers(QAbstractItemView::DoubleClicked |
                          QAbstractItemView::SelectedClicked);

    // connect(this, &QXmlTreeWidget2::itemDoubleClicked, this,
    // 		&QXmlTreeWidget2::SlotOnDoubleClickedItem);
    connect(this, &QXmlTreeWidget2::itemChanged, this,
            &QXmlTreeWidget2::SlotUpdateChildItemStatus);
    connect(this, &QXmlTreeWidget2::customContextMenuRequested, this,
            &QXmlTreeWidget2::SlotOnRightClickedMenu);
    connect(mpCopyAction, &QAction::triggered, this,
            &QXmlTreeWidget2::SlotOnCopyAction);
    connect(mpOpenAction, &QAction::triggered, this,
            &QXmlTreeWidget2::SlotOnOpenAction);
    connect(mpAddAction, &QAction::triggered, this,
            &QXmlTreeWidget2::SlotOnAddAction);
    connect(mpDelAction, &QAction::triggered, this,
            &QXmlTreeWidget2::SlotOnDelAction);
}

QXmlTreeWidget2::~QXmlTreeWidget2() {}

bool QXmlTreeWidget2::InitFromXmlConfig(const QString& path) {
    QFile file(path);
    if (!file.exists())
    {
        emit XmlErrorOcurred(u8"xml文件不存在");
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit XmlErrorOcurred(u8"xml文件打开失败");
        return false;
    }

    mDoc.clear();
    if (auto res = mDoc.setContent(&file); !res)
    {
        emit XmlErrorOcurred(res.errorMessage);
        file.close();
        return false;
    }
    file.close();

    auto rootEle = mDoc.documentElement();
    if (rootEle.isNull())
    {
        emit XmlErrorOcurred(u8"找不到根节点");
        return false;
    }

    InitXml(rootEle, nullptr);
    return true;
}

bool QXmlTreeWidget2::InitFromXmlStr(QAnyStringView xml) {
    mDoc.clear();
    if (auto res = mDoc.setContent(xml); !res)
    {
        emit XmlErrorOcurred(res.errorMessage);
        return false;
    }
    auto rootEle = mDoc.documentElement();
    if (rootEle.isNull())
    {
        emit XmlErrorOcurred(u8"找不到根节点");
        return false;
    }

    InitXml(rootEle, nullptr);
    return true;
}

bool QXmlTreeWidget2::SaveXmlConfig(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        emit XmlErrorOcurred(u8"保存失败，无法创建配置文件");
        return false;
    }
    mDoc.clear();

    auto instruction = mDoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    mDoc.appendChild(instruction);

    auto xmlComment = mDoc.createComment(XML_COMMENT2);
    mDoc.appendChild(xmlComment);

    auto root = mDoc.createElement("Root");
    mDoc.appendChild(root);

    for (int i = 0; i < this->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = this->topLevelItem(i);
        SaveXmlConfig(root, item);
    }

    QTextStream out(&file);
    mDoc.save(out, 4);

    file.close();

    return true;
}

void QXmlTreeWidget2::ClearTree() {
    QTreeWidgetItemIterator it(this);
    while (*it)
        delete *it;
}

QString QXmlTreeWidget2::GetNodeValue(const QString &id) {
    QTreeWidgetItem *item = mNodeMap.value(id, nullptr);
    if (item) {
        if (item->flags() & Qt::ItemFlag::ItemIsUserCheckable)
            return (item->checkState(0) == Qt::Checked) ? "1" : "0";
        else
            return item->text(1);
    }
    return QString();
}

void QXmlTreeWidget2::SetNodeValue(const QString &id, const QString &value) {
    QTreeWidgetItem *item = mNodeMap.value(id, nullptr);
    if (item) {
        if (item->flags() & Qt::ItemFlag::ItemIsUserCheckable)
            item->setCheckState(0,
                                STRING_IS_TRUE(value) ? Qt::Checked : Qt::Unchecked);
        else
            item->setText(1, value);
    }
}

void QXmlTreeWidget2::InitXml(const QDomElement& element,
                              QTreeWidgetItem *parent) {
    auto node = element.firstChildElement();
    while (!node.isNull()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
        item->setText(0, node.attribute("name"));
        item->setText(1, node.attribute("value"));

        QString checkable{node.attribute("checkable")};
        QString checked{node.attribute("checked")};
        QString enable{node.attribute("enable")};

        NodeData data(node.attribute("id"), node.attribute("valueType"),
                      node.attribute("valueRange"), checkable,
                      STRING_IS_ENABLE(enable));
        item->setData(1, Qt::UserRole + 1, QVariant::fromValue(data));
        mNodeMap[node.attribute("id")] = item;

        if (parent) {
            parent->addChild(item);
            if (STRING_IS_TRUE(checkable)) {
                if (STRING_IS_TRUE(checked) &&
                    ((parent->flags() & Qt::ItemFlag::ItemIsUserCheckable)
                                                    ? (parent->checkState(0) == Qt::CheckState::Checked)
                                                                                                      : true)) {
                    item->setCheckState(0, Qt::CheckState::Checked);
                } else {
                    item->setCheckState(0, Qt::CheckState::Unchecked);
                }
            } else {
                item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsUserCheckable);
            }
            if (!STRING_IS_ENABLE(enable) || parent->isDisabled() ||
                (parent->checkState(0) == Qt::CheckState::Unchecked &&
                                                                      (parent->flags() & Qt::ItemFlag::ItemIsUserCheckable))) {
                item->setDisabled(true);
            }
        } else {
            this->addTopLevelItem(item);
            if (STRING_IS_TRUE(checkable)) {
                if (STRING_IS_TRUE(checked)) {
                    item->setCheckState(0, Qt::CheckState::Checked);
                } else {
                    item->setCheckState(0, Qt::CheckState::Unchecked);
                }
            } else {
                item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsUserCheckable);
            }
            if (!STRING_IS_ENABLE(enable))
                item->setDisabled(true);
        }
        item->setExpanded(true);

        InitXml(node, item);

        node = node.nextSiblingElement();
    }
}

void QXmlTreeWidget2::SaveXmlConfig(QDomElement& element,
                                    QTreeWidgetItem *parent) {
    int countChild = parent->childCount();

    QString name = parent->text(0);
    QString value = parent->text(1);
    auto data = parent->data(1, Qt::UserRole + 1).value<NodeData>();

    auto node = mDoc.createElement("Param");
    node.setAttribute("name", name);
    node.setAttribute("id", data._id);
    node.setAttribute("valueType", data._valueType);
    node.setAttribute("value", value);
    node.setAttribute("valueRange", data._valueRange);
    node.setAttribute("checkable", data._checkable);
    node.setAttribute("checked",
                       QString::number(parent->checkState(0) != Qt::Unchecked)
                           .toStdString()
                           .c_str());
    node.setAttribute("enable",
                       QString::number(data._enable).toStdString().c_str());

    element.appendChild(node);

    for (int i = 0; i < countChild; i++) {
        QTreeWidgetItem *childItem = parent->child(i);
        SaveXmlConfig(node, childItem);
    }
}

void QXmlTreeWidget2::setNodeEditable(bool newNodeEditable)
{
    if (mNodeEditable == newNodeEditable)
        return;
    mNodeEditable = newNodeEditable;
    emit nodeEditableChanged();
}

bool QXmlTreeWidget2::nodeEditable() const
{
    return mNodeEditable;
}

bool QXmlTreeWidget2::editable() const { return mEditable; }

void QXmlTreeWidget2::setEditable(bool newEditable) {
    if (mEditable == newEditable)
        return;
    mEditable = newEditable;
    if (mEditable) {
        this->setEditTriggers(QAbstractItemView::DoubleClicked |
                              QAbstractItemView::SelectedClicked);
    } else {
        this->setEditTriggers(NoEditTriggers);
    }
    emit editableChanged();
}

void QXmlTreeWidget2::SlotUpdateChildItemStatus(QTreeWidgetItem *item) {
    int nCount = item->childCount();
    for (int nIndex = 0; nIndex < nCount; ++nIndex) {
        auto child = item->child(nIndex);
        if (child->flags() & Qt::ItemFlag::ItemIsUserCheckable)
            child->setCheckState(0, item->checkState(0));
        child->setDisabled(
            (item->checkState(0) == Qt::CheckState::Unchecked) ||
            !child->data(1, Qt::UserRole + 1).value<NodeData>()._enable);
        if (child->childCount() > 0) {
            SlotUpdateChildItemStatus(child);
        }
    }
}

void QXmlTreeWidget2::SlotOnRightClickedMenu(const QPoint &pos) {
    mpMenu->clear();
    mRightClickedItem = this->itemAt(pos);
    if (mRightClickedItem != nullptr) {
        auto type = mRightClickedItem->data(1, Qt::UserRole + 1)
                        .value<NodeData>()
                        ._valueType;
        if (type == "path") {
            mpMenu->addAction(mpOpenAction);
        }
        mpMenu->addAction(mpCopyAction);
        if(mNodeEditable)
        {
            mpMenu->addAction(mpAddAction);
            mpMenu->addAction(mpDelAction);
        }
    }
    mpMenu->exec(QCursor::pos());
}

void QXmlTreeWidget2::SlotOnOpenAction() {
    if (mRightClickedItem == nullptr)
        return;
    QDir dir(mRightClickedItem->text(1));
    QUrl url(QString("file:///%1").arg(dir.absolutePath()));
    if (!QDesktopServices::openUrl(url)) {
        emit XmlErrorOcurred(u8"打开路径失败");
    }
}

void QXmlTreeWidget2::SlotOnCopyAction() {
    if (mRightClickedItem == nullptr)
        return;
    QApplication::clipboard()->setText(mRightClickedItem->text(1));
}

void QXmlTreeWidget2::SlotOnAddAction() {
    if (mRightClickedItem == nullptr)
        return;
    if (NodeAttributesDlg dlg; dlg.exec() == QDialog::Accepted) {
        auto nodeData = dlg.getData();
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
        item->setText(0, nodeData._name);
        item->setText(1, nodeData._value);

        NodeData data(nodeData._id, nodeData._valueType, nodeData._valueRange,
                      nodeData._checkable, nodeData._enable);
        item->setData(1, Qt::UserRole + 1, QVariant::fromValue(data));
        mNodeMap[nodeData._id] = item;

        mRightClickedItem->addChild(item);
        if (STRING_IS_TRUE(nodeData._checkable)) {
            if (nodeData._checked && ((mRightClickedItem->flags() & Qt::ItemFlag::ItemIsUserCheckable)
                ? (mRightClickedItem->checkState(0) == Qt::CheckState::Checked) : true)) {
                item->setCheckState(0, Qt::CheckState::Checked);
            } else {
                item->setCheckState(0, Qt::CheckState::Unchecked);
            }
        } else {
            item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsUserCheckable);
        }
        if (!nodeData._enable || mRightClickedItem->isDisabled() ||
            (mRightClickedItem->checkState(0) == Qt::Unchecked && (mRightClickedItem->flags() & Qt::ItemIsUserCheckable))) {
            item->setDisabled(true);
        }
    }
}

void QXmlTreeWidget2::SlotOnDelAction() {
    if (mRightClickedItem == nullptr)
        return;
    delete mRightClickedItem;
    mRightClickedItem = nullptr;
}
