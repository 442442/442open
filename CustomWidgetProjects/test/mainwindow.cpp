#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QImageReader>
#include <QPushButton>
#include <QTimer>
#ifdef BUILD_WITH_HALCON
#include <halconcpp/HalconCpp.h>
#include "Q442Halconwidget.h"
#endif
#include "Q442CustomGraphicItem.h"
#include "Q442CustomWidget.h"

using namespace HalconCpp;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *playout = new QHBoxLayout(ui->tab);

    // xml树demo
    {
        QXmlTreeWidget2 *xmltree = new QXmlTreeWidget2;
        xmltree->InitFromXmlConfig(
            "D:\\Git\\ceramic_edge_detection\\x64\\Release\\config\\SystemConfig."
            "xml");
        // xmltree->InitFromXmlConfig("D:\\Git\\ceramic_edge_detection\\x64\\Release\\config\\ResultTable.xml");
        playout->addWidget(xmltree);
        QPushButton *button = new QPushButton();
        playout->addWidget(button);
        connect(button, &QPushButton::clicked, this,
                [=] { xmltree->SaveXmlConfig("2.xml"); });
    }
    // drag树demo
    {
        playout = new QHBoxLayout(ui->tab_2);
        QDragTreeWidget *tree = new QDragTreeWidget;
        playout->addWidget(tree);
        tree->setBehavior(QDragTreeWidget::InteractionDragBehavior::DragToMove);
        tree = new QDragTreeWidget;
        playout->addWidget(tree);

        QList<QTreeWidgetItem *> list;
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, "parent");
        list.append(item);

        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0, "child1");
        item->addChild(child);
        child = new QTreeWidgetItem();
        child->setText(0, "child2");
        item->addChild(child);

        item = new QTreeWidgetItem();
        item->setText(0, "parent2");
        list.append(item);

        child = new QTreeWidgetItem();
        child->setText(0, "child3");
        item->addChild(child);
        child = new QTreeWidgetItem();
        child->setText(0, "child4");
        item->addChild(child);

        tree->addTopLevelItems(list);
        tree->setBehavior(QDragTreeWidget::InteractionDragBehavior::DragToMove);
    }
    // 放大graphicsview demo
    {
        QImageReader::setAllocationLimit(500);
        QImage img;
        img.load("D:/Git/test1031/merge/NG/1.jpg", "JPG");

        QPixmap pix;
        pix = QPixmap::fromImage(img);
        QGraphicImgItem *pImgItem = new QGraphicImgItem(pix);
        pImgItem->setPos(0, 0);
        ui->qZoomGraphicView->scene()->addItem(pImgItem);
        QPixelListener *pwatcher = new QPixelListener;
        pImgItem->AddPixelListener(pwatcher);

        connect(
            ui->tabWidget, &QTabWidget::currentChanged, this,
            [=](int index) {
                if (index == 3)
                    ui->qZoomGraphicView->InitSceneSize(pix.width(), pix.height());
        },
            Qt::QueuedConnection);

        QGraphicsEllipseItem *item2 = new QGraphicsEllipseItem(0, 0, 200, 100);
        item2->setPos(0, 0);
        item2->setBrush(QBrush(Qt::blue));
        // item2->setFlags(QGraphicsItem::ItemIsSelectable |
        // QGraphicsItem::ItemIsFocusable
        //                | QGraphicsItem::ItemIsMovable);    //设置可拖动
        // ui->qZoomGraphicView->scene()->addItem(item2);
        // 画一个位于scene边缘的椭圆
        QGraphicsEllipseItem *item3 = new QGraphicsEllipseItem(0, 0, 100, 100);
        item3->setPos(200, 200);
        item3->setBrush(QBrush(Qt::red));
        // item3->setFlags(QGraphicsItem::ItemIsSelectable |
        // QGraphicsItem::ItemIsFocusable
        //                | QGraphicsItem::ItemIsMovable);    //设置可拖动
        // ui->qZoomGraphicView->scene()->addItem(item3);

        auto arrow =
            new QGraphicArrorItem(QPointF(50.5, 10.5), QPointF(50.5, 2050.2));
        arrow->SetLineWidth(3);
        // arrow->SetColor(Qt::red);
        // arrow->SetArrowType(QGraphicArrorItem::ArrowType::DoubleArrow);
        ui->qZoomGraphicView->scene()->addItem(arrow);
        // arrow->setFlag(QGraphicsItem::ItemIgnoresTransformations);

        auto rect = new QGraphicRectItem(QRectF(100, 1000, 1000, 1000));
        rect->SetColor(Qt::red);
        rect->SetLineWidth(3);
        ui->qZoomGraphicView->scene()->addItem(rect);

        auto ptext = new QGraphicNoticeText();
        QFont font("黑体", 9, 1);
        QBrush brush(Qt::red);
        ptext->setPlainText("123");
        // ptext->setPos(10,10);
        // ptext->setFont(font);
        // ptext->setBrush(brush);
        ptext->AttachToArrow(arrow);
        ui->qZoomGraphicView->scene()->addItem(ptext);

        connect(ui->pushButton, &QPushButton::clicked, this, [=] {
            arrow->SetColor(Qt::red);
            arrow->SetLineItem(QPointF(250.2, 2800), QPointF(1250.2, 2800));
            arrow->SetLineWidth(3);
            arrow->SetArrowType(QGraphicArrorItem::ArrowType::NoArrow);
            ui->qZoomGraphicView->Update();
        });
    }
    // 性能监控
    {
        ui->qCpMonitor_2->StartMonitor();
        connect(ui->cpBtn, &QPushButton::clicked, this, [this] {
            QCpMonitor::MonitorShowFlags flags = QCpMonitor::MonitorShowFlag::None;
            if (ui->checkBox->checkState())
                flags |= QCpMonitor::MonitorShowFlag::All;
            if (ui->checkBox_2->checkState())
                flags |= QCpMonitor::MonitorShowFlag::CPU;
            if (ui->checkBox_3->checkState())
                flags |= QCpMonitor::MonitorShowFlag::Memory;
            if (ui->checkBox_4->checkState())
                flags |= QCpMonitor::MonitorShowFlag::DiskSpace;
            ui->qCpMonitor_2->setShowFlag(flags);
        });
        ui->qCpMonitor_2->setMemoryWarning(50);
    }
#ifdef BUILD_WITH_HALCON   // halcon显示
    {
        QHalconDisplay *pdisp = new QHalconDisplay;
        // playout=new QHBoxLayout(ui->tab_5);
        // ui->horizontalLayout_2->addWidget(pdisp);
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
            if (index == 4) {
                try {
                    HalconCpp::HObject himg;
                    HalconCpp::ReadImage(&himg, "D:/Git/test1031/merge/NG/1.jpg");
                    HalconCpp::HTuple width, height;
                    HalconCpp::GetImageSize(himg, &width, &height);
                    pdisp->SetImgToDisplay(himg);
                    pdisp->InitHalconWnd(width, height);
                    pdisp->PaintOnHWnd(1);
                } catch (HalconCpp::HException &e) {
                    auto hstring = e.ErrorMessage().TextA();
                    auto herror = e.ErrorCode();
                    qDebug() << hstring << herror;
                }
            }
        });
    }
#endif

    // minihud
    {
        QGrahpicViewMiniHud* hud=new QGrahpicViewMiniHud;
        ui->horizontalLayout_2->addWidget(hud);
        // hud->SetHighLight();
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
            if (index == 4) {
                hud->SetText("123");
                QImage img;
                img.load("D:\\Git\\442open\\CustomWidgetProjects\\test\\1.jpg");
                hud->SetImage(QPixmap::fromImage(img));
                hud->SetHighLight();
            }
        });
    }

}

MainWindow::~MainWindow() { delete ui; }
