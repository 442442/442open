# 442open

#### Description
442's open sources repository.
CustomWidgetProjects is a project about Qt custom widgets.
MyCustomWidget is a project of Qt Designer custom widgets plugin.
test is a project of widget testing. 

#### What's in Custom Widgets

Computer Performance Monitor
Drag Tree
Xml Tree
Path Selector
File Selector
Parameter Editor
Halcon Display
Halcon Image Display Only
Halcon Display Minihud(double click to open in a new dialog)
Zoomable QGraphicsView
Zoomable Custom QGraphicsItems(arrow,rect,text,pixmap)


#### Installation

1.  cmake_minimum_required 3.16 halcon qt5/qt6

#### Bugs Remain

1.  XmlTreeWidget2 :crash on some specific situation.
2.  Computer Performance Monitor: build with qt5.14.2 can not send QMap signal
3.  Halcon build release:setting text font do not work

4.  Halcon region info tips abnormal

5.  Graphics text item would not repaint with AttachToArrow on some specific situation.
