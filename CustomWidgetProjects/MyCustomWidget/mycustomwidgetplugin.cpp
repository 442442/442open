#pragma once
#include "mycustomwidgetplugin.h"
#include <QtPlugin>
#include "Q442CustomWidget.h"
#include "Q442Halconwidget.h"

#ifdef BUILD_WITH_HALCON
QHalconDisplayPlugin::QHalconDisplayPlugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QHalconDisplay");
    m_include=QLatin1String("QHalconDisplay/qhalcondisplay.h");
    m_toolTip=QString(u8"halcon显示控件");
    m_domXml="<widget class=\"QHalconDisplay\" name=\"qHalconDisplay\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>300</width>\n"
            "   <height>200</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
    m_icon = QIcon("://icon/halcondispicon.png");
}

QWidget* QHalconDisplayPlugin::createWidget(QWidget* parent)
{
    return new QHalconDisplay(parent);
}

QHalconDisplayMiniPlugin::QHalconDisplayMiniPlugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QHalconDisplayMini");
    m_include=QLatin1String("QHalconDisplayMini/qhalcondisplaymini.h");
    m_toolTip=QString(u8"halcon迷你显示控件(仅显示无其他功能)");
    m_domXml="<widget class=\"QHalconDisplayMini\" name=\"qHalconDisplayMini\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>300</width>\n"
            "   <height>200</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
    m_icon = QIcon("://icon/halconminiicon.png");
}

QWidget* QHalconDisplayMiniPlugin::createWidget(QWidget* parent)
{
    return new QHalconDisplayMini(parent);
}

QHalconMiniHudPlugin::QHalconMiniHudPlugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QHalconMiniHud");
    m_include=QLatin1String("QHalconMiniHud/qhalconminihud.h");
    m_toolTip=QString(u8"halcon缩略图显示(双击放大)");
    m_domXml="<widget class=\"QHalconMiniHud\" name=\"qHalconMiniHud\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>300</width>\n"
            "   <height>200</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
    m_icon = QIcon("://icon/halconhudicon.png");
}

QWidget* QHalconMiniHudPlugin::createWidget(QWidget* parent)
{
    return new QHalconMiniHud(parent);
}

#endif

QParamEditPlugin::QParamEditPlugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QParamEdit");
    m_include=QLatin1String("CustomEdits/qparamedit.h");
    m_toolTip=QString(u8"参数控件(参数名 值 单位)");
    m_domXml="<widget class=\"QParamEdit\" name=\"qParamEdit\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>150</width>\n"
            "   <height>20</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
    m_icon = QIcon("://icon/lineedit.png");
}

QWidget* QParamEditPlugin::createWidget(QWidget* parent)
{
    return new QParamEdit(parent);
}

QTextParamEditPlugin::QTextParamEditPlugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QTextParamEdit");
    m_include=QLatin1String("CustomEdits/qtextparamedit.h");
    m_toolTip=QString(u8"参数控件(参数 文本)");
    m_domXml="<widget class=\"QTextParamEdit\" name=\"qTextParamEdit\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>150</width>\n"
            "   <height>20</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
    m_icon = QIcon("://icon/spinbox.png");
}

QWidget* QTextParamEditPlugin::createWidget(QWidget* parent)
{
    return new QTextParamEdit(parent);
}

QPathEditPlugin::QPathEditPlugin(QObject *parent):QObject(parent)
{
    m_name = QLatin1String("QPathEdit");
    m_include=QLatin1String("CustomEdits/qpathedit.h");
    m_toolTip=QString(u8"路径选择控件");
    m_domXml="<widget class=\"QPathEdit\" name=\"qPathEdit\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>150</width>\n"
               "   <height>20</height>\n"
               "  </rect>\n"
               " </property>\n"
               "</widget>\n";
    m_icon = QIcon("://icon/lineedit.png");
}

QWidget *QPathEditPlugin::createWidget(QWidget *parent)
{
    return new QPathEdit(parent);
}

QFileEditPlugin::QFileEditPlugin(QObject *parent):QObject(parent)
{
    m_name = QLatin1String("QFileEdit");
    m_include=QLatin1String("CustomEdits/qpathedit.h");
    m_toolTip=QString(u8"路径选择控件");
    m_domXml="<widget class=\"QFileEdit\" name=\"qFileEdit\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>150</width>\n"
               "   <height>20</height>\n"
               "  </rect>\n"
               " </property>\n"
               "</widget>\n";
    m_icon = QIcon("://icon/lineedit.png");
}

QWidget *QFileEditPlugin::createWidget(QWidget *parent)
{
    return new QFileEdit(parent);
}

QXmlTreeWidgetPlugin::QXmlTreeWidgetPlugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QXmlTreeWidget");
    m_include=QLatin1String("QXmlTreeWidget/qxmltreewidget.h");
    m_toolTip=QString(u8"xml树(算法参数树)");
    m_domXml="<widget class=\"QXmlTreeWidget\" name=\"qXmlTreeWidget\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>256</width>\n"
            "   <height>192</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
    m_icon = QIcon("://icon/xmltreeicon.png");
}

QWidget* QXmlTreeWidgetPlugin::createWidget(QWidget* parent)
{
    return new QXmlTreeWidget(parent);
}

QXmlTreeWidget2Plugin::QXmlTreeWidget2Plugin(QObject* parent):QObject(parent)
{
    m_name = QLatin1String("QXmlTreeWidget2");
    m_include=QLatin1String("QXmlTreeWidget/qxmltreewidget2.h");
    m_toolTip=QString(u8"xml树(系统参数树)");
    m_domXml="<widget class=\"QXmlTreeWidget2\" name=\"qXmlTreeWidget2\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>256</width>\n"
               "   <height>192</height>\n"
               "  </rect>\n"
               " </property>\n"
               "</widget>\n";
    m_icon = QIcon("://icon/xmltreeicon.png");
}

QWidget* QXmlTreeWidget2Plugin::createWidget(QWidget* parent)
{
    return new QXmlTreeWidget2(parent);
}

QDragTreeWidgetPlugin::QDragTreeWidgetPlugin(QObject *parent):QObject(parent)
{
    m_name = QLatin1String("QDragTreeWidget");
    m_include=QLatin1String("QDragTreeWidget/qdragtreewidget.h");
    m_toolTip=QString(u8"拖拽树");
    m_domXml="<widget class=\"QDragTreeWidget\" name=\"qDragTreeWidget\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>256</width>\n"
            "   <height>192</height>\n"
            "  </rect>\n"
            " </property>\n"
             "</widget>\n";
    m_icon = QIcon("://icon/dragtreeicon.png");
}

QWidget *QDragTreeWidgetPlugin::createWidget(QWidget *parent)
{
    return new QDragTreeWidget(parent);
}

QZoomGraphicViewPlugin::QZoomGraphicViewPlugin(QObject *parent):QObject(parent)
{
    m_name = QLatin1String("QZoomGraphicView");
    m_include = QLatin1String("QZoomGraphicView/qzoomgraphicview.h");
    m_toolTip = QString(u8"能放大的QGraphicsView,附带箭头，文字，图片图元");
    m_domXml = "<widget class=\"QZoomGraphicView\" name=\"qZoomGraphicView\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>300</width>\n"
               "   <height>200</height>\n"
               "  </rect>\n"
               " </property>\n"
               "</widget>\n";
    m_icon = QIcon("://icon/widget.png");
}

QWidget *QZoomGraphicViewPlugin::createWidget(QWidget *parent)
{
    return new QZoomGraphicView(parent);
}

QGraphicViewMiniHudPlugin::QGraphicViewMiniHudPlugin(QObject *parent):QObject(parent)
{
    m_name = QLatin1String("QGrahpicViewMiniHud");
    m_include = QLatin1String("QGraphicMiniHud/qgrahpicviewminihud.h");
    m_toolTip = QString(u8"QGraphic缩略图显示(双击放大)");
    m_domXml = "<widget class=\"QGrahpicViewMiniHud\" name=\"qGrahpicViewMiniHud\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>300</width>\n"
               "   <height>200</height>\n"
               "  </rect>\n"
               " </property>\n"
               "</widget>\n";
    m_icon = QIcon("://icon/widget.png");
}

QWidget *QGraphicViewMiniHudPlugin::createWidget(QWidget *parent)
{
    return new QGrahpicViewMiniHud(parent);
}

QCpMonitorPlugin::QCpMonitorPlugin(QObject *parent) : QObject(parent)
{
    m_name = QLatin1String("QCpMonitor");
    m_include=QLatin1String("QCpMonitor/qcpmonitor.h");
    m_toolTip=QString(u8"电脑性能监控");
    m_domXml="<widget class=\"QCpMonitor\" name=\"qCpMonitor\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>200</width>\n"
               "   <height>50</height>\n"
               "  </rect>\n"
               " </property>\n"
               "</widget>\n";
    m_icon = QIcon("://icon/widget.png");
}

QWidget *QCpMonitorPlugin::createWidget(QWidget *parent)
{
    return new QCpMonitor(parent);
}
