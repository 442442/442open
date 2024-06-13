#ifndef QXMLTREEWIDGET2_H
#define QXMLTREEWIDGET2_H
#pragma once
#include <QTreeWidget>
#include <QVariant>
#include <QtUiPlugin/QDesignerExportWidget>

namespace  tinyxml2{class XMLElement;}

class QDESIGNER_WIDGET_EXPORT QXmlTreeWidget2 : public QTreeWidget
{
    Q_OBJECT
public:
    /// <summary>
    /// 节点数据结构
    /// </summary>
    struct NodeData
    {
        NodeData() = default;
        NodeData(const QString &id, const QString &valueType,
                 const QString &valueRange, const QString &checkable, bool enable)
            : _id(id), _valueType(valueType), _valueRange(valueRange),
            _checkable(checkable), _enable(enable) {}
        QString _id;
        QString _valueType;
        QString _valueRange;
        QString _checkable;
        bool _enable;
    };


    explicit QXmlTreeWidget2(QWidget *parent = nullptr);
    ~QXmlTreeWidget2();
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
    /// <summary>
    /// 更新子节点状态
    /// </summary>
    /// <param name = "item">树item</param>
    /// <param name = "col">列</returns>
    void SlotUpdateChildItemStatus(QTreeWidgetItem *item);

    void SlotOnRightClickedMenu(const QPoint &pos);
    void SlotOnOpenAction();
    void SlotOnEditAction();
    void SlotOnCopyAction();

private:
    /// <summary>
    /// 私有重载，迭代初始化树
    /// </summary>
    /// <param name="element">xml元素</param>
    /// <param name="parent">父节点</param>
    /// <param name="level">层级</param>
    void InitXml(tinyxml2::XMLElement* element, QTreeWidgetItem* parent);
    /// <summary>
    /// 私有重载，迭代保存树
    /// </summary>
    /// <param name="element">xml元素</param>
    /// <param name="parent">父节点</param>
    void SaveXmlConfig(tinyxml2::XMLElement* element, QTreeWidgetItem* parent);


    QMap<QString, QTreeWidgetItem*> mNodeMap;//id，节点
    QTreeWidgetItem* mDoubleClickedItem{ nullptr };
    QTreeWidgetItem* mRightClickedItem{ nullptr };
    QWidget *mLastEditer{ nullptr };
    QMenu *mpMenu{ nullptr };
    QAction *mpOpenAction{ nullptr };
    QAction *mpEditAction{ nullptr };
    QAction *mpCopyAction{ nullptr };
};
Q_DECLARE_METATYPE(QXmlTreeWidget2::NodeData);

#endif // QXMLTREEWIDGET2_H
