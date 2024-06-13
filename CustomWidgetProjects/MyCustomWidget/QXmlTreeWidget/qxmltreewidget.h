#ifndef QXMLTREEWIDGET_H
#define QXMLTREEWIDGET_H
#pragma once
#include <QTreeWidget>
#include <QMap>
#include <QtUiPlugin/QDesignerExportWidget>

namespace  tinyxml2{class XMLElement;}
/// <summary>
/// xml树
/// 节点data:节点层级 Qt::UserRole(0x0101) 叶子节点id Qt::UserRole(0x0102)
///          节点类型 Qt::UserRole(0x0103) 0：文本 1：数值 其他：不可编辑
///          数值最大值 Qt::UserRole(0x0104) 最小值 Qt::UserRole(0x0105)
/// </summary>
class QDESIGNER_WIDGET_EXPORT QXmlTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QXmlTreeWidget(QWidget *parent = nullptr);
    ~QXmlTreeWidget();

    /// <summary>
    /// 初始化树
    /// </summary>
    /// <param name = "path">xml文件路径</param>
    /// <returns>成功/失败</returns>
    bool InitFromXmlConfig(const QString& path);
    /// <summary>
    /// 初始化树
    /// </summary>
    /// <param name = "xml">xml字符串</param>
    /// <returns>成功/失败</returns>
    bool InitFromXmlStr(const QString& xml);
    /// <summary>
    /// 保存配置
    /// </summary>
    /// <param name = "path">xml文件路径</param>
    /// <returns>成功/失败</returns>
    bool SaveXmlConfig(const QString& path);
    /// <summary>
    /// 清除所有节点
    /// </summary>
    void ClearTree();
    /// <summary>
    /// 获取节点值
    /// </summary>
    /// <param name = "id">节点id</param>
    /// <returns>节点值</returns>
    virtual QString GetNodeValue(const QString& id);
    /// <summary>
    /// 设定节点值
    /// </summary>
    /// <param name = "id">节点id</param>
    /// <param name = "value">节点值</returns>
    virtual void SetNodeValue(const QString& id, const QString& value);

signals:
    /// <summary>
    /// xml异常
    /// </summary>
    /// <param name = "e">异常信息</param>
    void XmlErrorOcurred(const QString& e);

private slots:
    /// <summary>
    /// 鼠标双击事件
    /// </summary>
    /// <param name = "item">树item</param>
    /// <param name = "col">列</returns>
    virtual void SlotOnDoubleClickedItem(QTreeWidgetItem* item, int col);

private:
    /// <summary>
    /// 私有重载，迭代初始化树
    /// </summary>
    /// <param name="element">xml元素</param>
    /// <param name="parent">父节点</param>
    /// <param name="level">层级</param>
    void InitXml(tinyxml2::XMLElement* element, QTreeWidgetItem* parent, int level);
    /// <summary>
    /// 初始化处理叶子节点
    /// </summary>
    /// <param name="element">xml元素叶子</param>
    /// <param name="parent">父节点</param>
    virtual void InitWithLeafNode(tinyxml2::XMLElement* element, QTreeWidgetItem* parent);
    /// <summary>
    /// 私有重载，迭代保存树
    /// </summary>
    /// <param name="element">xml元素</param>
    /// <param name="parent">父节点</param>
    void SaveXmlConfig(tinyxml2::XMLElement* element, QTreeWidgetItem* parent);
    /// <summary>
    /// 保存处理叶子节点
    /// </summary>
    /// <param name="element">xml元素叶子</param>
    /// <param name="child">叶节点</param>
    virtual void SaveWithLeafNode(tinyxml2::XMLElement* element, QTreeWidgetItem* child);

    void updateParentItemStatus(QTreeWidgetItem *item);
    void updateChildItemStatus(QTreeWidgetItem *item);

    QMap<QString, QTreeWidgetItem*> mNodeMap;//id，节点
    QTreeWidgetItem* mDoubleClickedItem;
};

#endif // QXMLTREEWIDGET_H
