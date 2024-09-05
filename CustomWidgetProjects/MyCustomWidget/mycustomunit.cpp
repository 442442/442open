#include "mycustomunit.h"
#include "mycustomwidgetplugin.h"

MyCustomUnit::MyCustomUnit(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new QParamEditPlugin(this));
    m_widgets.append(new QTextParamEditPlugin(this));
    m_widgets.append(new QPathEditPlugin(this));
    m_widgets.append(new QFileEditPlugin(this));
#ifdef BUILD_WITH_HALCON
    m_widgets.append(new QHalconDisplayPlugin(this));
    m_widgets.append(new QHalconDisplayMiniPlugin(this));
    m_widgets.append(new QHalconMiniHudPlugin(this));
#endif
    m_widgets.append(new QXmlTreeWidgetPlugin(this));
    m_widgets.append(new QXmlTreeWidget2Plugin(this));
    m_widgets.append(new QDragTreeWidgetPlugin(this));
    m_widgets.append(new QZoomGraphicViewPlugin(this));
    m_widgets.append(new QGraphicViewMiniHudPlugin(this));
    m_widgets.append(new QCpMonitorPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> MyCustomUnit::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mycustomunitplugin, MyCustomUnit)
#endif // QT_VERSION < 0x050000
