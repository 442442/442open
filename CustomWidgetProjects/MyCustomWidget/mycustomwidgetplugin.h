#pragma once
#if QT_VERSION >= 0x050600
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#else
#include <QDesignerCustomWidgetInterface>
#endif

class MyCustomWidgetPlugin : public QDesignerCustomWidgetInterface
{
public:
    virtual QString group() const override { return u8"442的控件们"; }
    virtual bool isContainer() const override { return false; }
    virtual bool isInitialized() const override { return true; }

    virtual QIcon icon() const override { return m_icon; }
    virtual QString codeTemplate() const override { return m_codeTemplate; }
    virtual QString domXml() const override { return m_domXml; }
    virtual QString includeFile() const override { return m_include; }
    virtual QString name() const override { return m_name; }
    virtual QString toolTip() const override{ return m_toolTip; }
    virtual QString whatsThis() const override { return m_whatsThis; }

protected:
    QString m_name;
    QString m_include;
    QString m_toolTip;
    QString m_whatsThis;
    QString m_domXml;
    QString m_codeTemplate;
    QIcon m_icon;
};

#ifdef BUILD_WITH_HALCON
class QHalconDisplayPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QHalconDisplayPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QHalconDisplayMiniPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QHalconDisplayMiniPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QHalconMiniHudPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QHalconMiniHudPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};
#endif

class QParamEditPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QParamEditPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QTextParamEditPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QTextParamEditPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QPathEditPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QPathEditPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QFileEditPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QFileEditPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget *createWidget(QWidget* parent) override;
};

class QXmlTreeWidget2Plugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QXmlTreeWidget2Plugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QDragTreeWidgetPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QDragTreeWidgetPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QZoomGraphicViewPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QZoomGraphicViewPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QGraphicViewMiniHudPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QGraphicViewMiniHudPlugin(QObject* parent = Q_NULLPTR);
    virtual QWidget* createWidget(QWidget* parent) override;
};

class QCpMonitorPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QCpMonitorPlugin(QObject *parent = nullptr);
    virtual QWidget *createWidget(QWidget *parent) override;
};

class QCodeEditPlugin : public QObject, public MyCustomWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    QCodeEditPlugin(QObject *parent = nullptr);
    virtual QWidget *createWidget(QWidget *parent) override;
};
