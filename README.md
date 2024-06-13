# 442open

#### 介绍
442的公开仓库
CustomWidgetProjects为自定义控件库项目
其中MyCustomWidget为控件库项目，test为控件测试项目


### 控件库包含

电脑性能监控
拖拽树
xml树（2种）
路径选择框
参数编辑框
halcon显示
halcon仅显示
halcon缩略窗
qgraphicsview显示
qgraphicsview缩略窗（开发中）
qgraphics箭头图元
qgraphics文字图元

#### 安装教程

1.  需要cmake3.16以上，halcon，qt5/qt6

##### 控件库使用说明

1.  加入lib和dll直接使用。或将dll放进qt安装目录下的QtCreator/bin/plugins/designer中，或qt版本号/qt版本/plugins/designer中
可在qtcreator或vs中使用designer直接拖控件使用(需要版本统一)

#### 已知bug

1.	XmlTreeWidget2按特定顺序操作崩溃的的bug
	双击path/file类型参数，双击空白其他非可编辑item，再改变窗口大小

2.	电脑性能监控，qt5版本无法发送硬盘map信号

3.	halcon窗口release下字体显示异常

4.	halcon窗口region点击信息弹出异常

5.	qgraphics文字图元AttachToArrow特定情况下不进行重绘
