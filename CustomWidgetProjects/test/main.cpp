#include "mainwindow.h"
#include "windoweffect.h"
#include <QApplication>
#include <QRandomGenerator>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QWidget w2;
    w2.setAttribute(Qt::WA_TranslucentBackground);

    //标题栏变色
#if 0
    QTimer timer(&w);
    QApplication::connect(&timer, &QTimer::timeout, &w, [&w] {
        WindowEffect::setTitleBarColor((HWND)w.winId(),
                                       QRandomGenerator::global()->bounded(256),
                                       QRandomGenerator::global()->bounded(256),
                                       QRandomGenerator::global()->bounded(256));
    });
    timer.start(1000);
#endif
    //云母窗口
    //WindowEffect::setMicaEffect((HWND)w.winId());

    //云母Alt窗口
    //WindowEffect::setMicaAltEffect((HWND)w.winId());

    //亚克力窗口
    //WindowEffect::setAcrylicEffect((HWND)w.winId());

    //win11下调整毛玻璃窗口会卡顿
    //win11下无法使标题栏也生效
    //推荐亚克力窗口

    //毛玻璃窗口(Areo)
    WindowEffect::setAreoEffect((HWND)w2.winId());

    //带颜色的毛玻璃窗口(非Qt事件绘制)
    //透明度值较高时效果和亚克力类似;
    //WindowEffect::setAreoEffect((HWND)w2.winId(),ABGR(100,201,174,255));//使用ABGR宏生成颜色，或直接传入16进制颜色值

    w.show();
    //w2.show();
    return a.exec();
}
