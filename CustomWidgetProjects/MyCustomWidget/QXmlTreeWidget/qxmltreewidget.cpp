#include "qxmltreewidget.h"
#include <QCoreApplication>
#include "tinyxml2.h"
#include <QHeaderView>
#include <QLineEdit>
#include <QDoubleSpinBox>
#ifdef _DEBUG
#include <QDebug>
#endif
tinyxml2::XMLDocument xmlDocWrite;

const QString XML_COMMENT=u8"name属性:ui显示的文本\nid:参数唯一标识\nvalue:参数值\nenable:参数是否显示\ntype:参数类型: 0:文本 1:数值 其他:不可编辑\nmax:数值类型最大值\nmin:最小值";

QXmlTreeWidget::QXmlTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    this->setAlternatingRowColors(true);
    this->header()->setSectionResizeMode(QHeaderView::Stretch);
    this->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    this->setColumnCount(3);
    this->setHeaderLabels(QStringList()<<u8"名称"<<u8"数据"<<u8"单位");
    connect(this, &QXmlTreeWidget::itemDoubleClicked, this, &QXmlTreeWidget::SlotOnDoubleClickedItem);
}

QXmlTreeWidget::~QXmlTreeWidget()
{

}

bool QXmlTreeWidget::InitFromXmlConfig(const QString& path)
{
    tinyxml2::XMLDocument xmlDocRead;
    auto error = xmlDocRead.LoadFile(path.toStdString().c_str());
    if (error != tinyxml2::XMLError::XML_SUCCESS)
    {
        emit XmlErrorOcurred(tinyxml2::XMLDocument::ErrorIDToName(error));
        return false;
    }
    auto rootEle = xmlDocRead.RootElement();
    if (!rootEle)
    {
        return false;
    }
    InitXml(rootEle, nullptr, 0);
    return true;
}

void QXmlTreeWidget::InitXml(tinyxml2::XMLElement* element, QTreeWidgetItem* parent, int level)
{
    int node_level = level + 1;
    tinyxml2::XMLElement* node = element->FirstChildElement();
    if (!node) {
        // 叶子节点
        QString name = element->GetText();
        parent->setText(1, name);
    }
    if(!node->FirstChildElement())
    {
        InitWithLeafNode(node, parent);
    }
    else
    {
        while (node)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setData(0, Qt::UserRole + 1, node_level);
            if (parent)
            {
                parent->addChild(item);
            }
            else
            {
                this->addTopLevelItem(item);                
            }
            item->setExpanded(true);
            item->setText(0, node->Attribute("name"));
            InitXml(node, item, node_level);
            node = node->NextSiblingElement();
        }
    }
}

bool QXmlTreeWidget::InitFromXmlStr(const QString &xml)
{
    tinyxml2::XMLDocument xmlDocRead;
    auto error = xmlDocRead.Parse(xml.toStdString().c_str());
    if (error != tinyxml2::XMLError::XML_SUCCESS)
    {
        emit XmlErrorOcurred(tinyxml2::XMLDocument::ErrorIDToName(error));
        return false;
    }
    auto rootEle = xmlDocRead.RootElement();
    if (!rootEle)
    {
        return false;
    }
    InitXml(rootEle, nullptr, 0);
    return true;
}

bool QXmlTreeWidget::SaveXmlConfig(const QString& path)
{
    xmlDocWrite.Clear();
    tinyxml2::XMLDeclaration* xmlDeclare = xmlDocWrite.NewDeclaration();
    xmlDocWrite.InsertEndChild(xmlDeclare);

    tinyxml2::XMLComment* xmlComment
            = xmlDocWrite.NewComment(XML_COMMENT.toStdString().c_str());
    xmlDocWrite.InsertEndChild(xmlComment);

    tinyxml2::XMLElement* root = xmlDocWrite.NewElement("Root");
    xmlDocWrite.InsertEndChild(root);

    for (int i = 0; i < this->topLevelItemCount(); i++) {
        QTreeWidgetItem* item = this->topLevelItem(i);
        tinyxml2::XMLElement* element = xmlDocWrite.NewElement(QString("Level%1").arg(item->data(0, Qt::UserRole + 1).toString()).toStdString().c_str());
        element->SetAttribute("name", item->text(0).toStdString().c_str());
        root->InsertEndChild(element);
        SaveXmlConfig(element, item);
    }
#ifdef _DEBUG
    //字符串预览
    tinyxml2::XMLPrinter printer;
    xmlDocWrite.Accept(&printer);
    QString xmlstr = printer.CStr();
    qDebug() << xmlstr;
#endif
    auto ret = xmlDocWrite.SaveFile(path.toStdString().c_str());
    if( ret != tinyxml2::XMLError::XML_SUCCESS)
    {
        emit XmlErrorOcurred(tinyxml2::XMLDocument::ErrorIDToName(ret));
        return false;
    }
    else {
        return true;
    }
}

void QXmlTreeWidget::SaveXmlConfig(tinyxml2::XMLElement* element, QTreeWidgetItem *parent)
{
    int countChild = parent->childCount();
    for (int i = 0; i < countChild; i++)
    {
        QTreeWidgetItem *childItem = parent->child(i);
        if(!childItem->childCount())
        {
            SaveWithLeafNode(element, childItem);
        }
        else
        {
            QString itemTxt = childItem->text(0);
            QString levelNum = childItem->data(0,Qt::UserRole + 1).toString();
            tinyxml2::XMLElement* node = xmlDocWrite.NewElement(QString("Level%1").arg(levelNum).toStdString().c_str());
            node->SetAttribute("name", itemTxt.toStdString().c_str());
            element->InsertEndChild(node);

            SaveXmlConfig(node, childItem);
        }
    }
}

void QXmlTreeWidget::ClearTree()
{
    QTreeWidgetItemIterator it(this);
    while (*it)	delete* it;
}

QString QXmlTreeWidget::GetNodeValue(const QString& id)
{
    QTreeWidgetItem* item = mNodeMap.value(id, nullptr);
    if(item)
    {
        return item->text(1);
    }
    return QString();
}

void QXmlTreeWidget::SetNodeValue(const QString& id, const QString& value)
{
    QTreeWidgetItem* item = mNodeMap.value(id, nullptr);
    if(item)
    {
        item->setText(1, value);
    }
}

void QXmlTreeWidget::SlotOnDoubleClickedItem(QTreeWidgetItem *item, int col)
{
    mDoubleClickedItem = nullptr;
    if (col == 1) {
        mDoubleClickedItem = item;
        if(item->text(col).isEmpty())return;
        auto type = item->data(0, Qt::UserRole + 3).toString();
        if(type == "0")
        {
            QLineEdit* lineEdit = new QLineEdit(this);
            lineEdit->setText(item->text(col));
            connect(lineEdit, &QLineEdit::editingFinished, this, [=]{
                if(mDoubleClickedItem == nullptr) return;
                QLineEdit *edit = qobject_cast<QLineEdit*>(this->itemWidget(mDoubleClickedItem, 1));
                if (!edit) return;
                QString text = edit->text();
                this->removeItemWidget(mDoubleClickedItem, 1);
                mDoubleClickedItem->setText(1, text);
            });
            this->setItemWidget(item, col, lineEdit);
        }
        else if (type == "1")
        {
            QDoubleSpinBox* spin = new QDoubleSpinBox(this);
            spin->setRange(item->data(0, Qt::UserRole + 5).toDouble()
                           , item->data(0,Qt::UserRole + 4).toDouble());
            spin->setDecimals(4);
            spin->setValue(item->text(col).toDouble());
            connect(spin, &QDoubleSpinBox::editingFinished, this, [=]{
                if(mDoubleClickedItem == nullptr) return;
                QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox*>(this->itemWidget(mDoubleClickedItem, 1));
                if (!spin) return;
                auto value = spin->value();
                this->removeItemWidget(mDoubleClickedItem, 1);
                mDoubleClickedItem->setText(1, QString::number(value));
            });
            this->setItemWidget(item, col, spin);
        }
    }
}

void QXmlTreeWidget::InitWithLeafNode(tinyxml2::XMLElement* element, QTreeWidgetItem *parent)
{
    if(!parent) return;
    while (element)
    {
        QString key = element->Value();
        if(key == "id")
        {
            QString id = element->GetText();
            parent->setData(0, Qt::UserRole + 2, id);
            mNodeMap.insert(id, parent);
        }
        if(key == "value")
        {
            parent->setText(1, element->GetText());
        }
        if(key == "enable")
        {
            //parent->setCheckState(0, QString(element->GetText()).toInt() == 1 ? Qt::Checked : Qt::Unchecked);
            parent->setHidden(!QString(element->GetText()).toInt());
        }
        if(key == "unit")
        {
            parent->setText(2, element->GetText());
        }
        if(key == "type")
        {
            QString type = element->GetText();
            parent->setData(0, Qt::UserRole + 3, type);
        }
        if(key == "max")
        {
            QString max = element->GetText();
            parent->setData(0, Qt::UserRole + 4, max);
        }
        if(key == "min")
        {
            QString min = element->GetText();
            parent->setData(0, Qt::UserRole + 5, min);
        }
        element = element->NextSiblingElement();
    }
}

void QXmlTreeWidget::SaveWithLeafNode(tinyxml2::XMLElement* element, QTreeWidgetItem *child)
{
    //param
    QString itemTxt = child->text(0);
    QString levelNum = child->data(0,Qt::UserRole + 1).toString();
    tinyxml2::XMLElement* paramEle = xmlDocWrite.NewElement(QString("Level%1").arg(levelNum).toStdString().c_str());
    paramEle->SetAttribute("name", itemTxt.toStdString().c_str());
    element->InsertEndChild(paramEle);
    //id
    tinyxml2::XMLElement* idEle = xmlDocWrite.NewElement("id");
    idEle->SetText(child->data(0, Qt::UserRole + 2).toString().toStdString().c_str());
    paramEle->InsertEndChild(idEle);
    //value
    tinyxml2::XMLElement* valueEle = xmlDocWrite.NewElement("value");
    valueEle->SetText(child->text(1).toStdString().c_str());
    paramEle->InsertEndChild(valueEle);
    //enable
    tinyxml2::XMLElement* enableEle = xmlDocWrite.NewElement("enable");
    //enableEle->SetText(QString::number(child->checkState(0)&&true).toStdString().c_str());
    enableEle->SetText(QString::number(!child->isHidden()).toStdString().c_str());
    paramEle->InsertEndChild(enableEle);
    //unit
    tinyxml2::XMLElement* unitEle = xmlDocWrite.NewElement("unit");
    unitEle->SetText(child->text(2).toStdString().c_str());
    paramEle->InsertEndChild(unitEle);
    //type 0:文本 1:数字 其他：不能修改
    QString type = child->data(0, Qt::UserRole + 3).toString();
    tinyxml2::XMLElement* typeEle = xmlDocWrite.NewElement("type");
    typeEle->SetText(type.toStdString().c_str());
    paramEle->InsertEndChild(typeEle);
    if(type == "1")
    {
        //max
        tinyxml2::XMLElement* maxEle = xmlDocWrite.NewElement("max");
        maxEle->SetText(child->data(0, Qt::UserRole + 4).toString().toStdString().c_str());
        paramEle->InsertEndChild(maxEle);
        //min
        tinyxml2::XMLElement* minEle = xmlDocWrite.NewElement("min");
        minEle->SetText(child->data(0, Qt::UserRole + 5).toString().toStdString().c_str());
        paramEle->InsertEndChild(minEle);
    }
}

void QXmlTreeWidget::updateParentItemStatus(QTreeWidgetItem* item)
{
    auto parent = item->parent();
    if (Q_NULLPTR == parent) return;
    //先把父节点的状态设置为改变的子节点的状态
    parent->setCheckState(0, item->checkState(0));
    //然后遍历它的子节点，如果有节点的状态和父节点的状态不一致，则是部分选中，否则全为选中或者未选中
    int nCount = parent->childCount();
    for (int nIndex = 0; nIndex < nCount; ++nIndex)
    {
        auto child = parent->child(nIndex);
        if (child->checkState(0) != parent->checkState(0))
        {
            parent->setCheckState(0, Qt::PartiallyChecked);
            break;
        }
    }
    //设置该父节点的父节点，直到根节点
    updateParentItemStatus(item);
}

void QXmlTreeWidget::updateChildItemStatus(QTreeWidgetItem* item)
{
    int nCount = item->childCount();
    for (int nIndex = 0; nIndex < nCount; ++nIndex)
    {
        auto child = item->child(nIndex);
        child->setCheckState(0, item->checkState(0));
        if (child->childCount() > 0)
        {
            updateChildItemStatus(child);
        }
    }
}
