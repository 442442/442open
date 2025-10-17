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
#ifdef BUILD_WITH_VTK
#include "QVtkPointCloudWidget/QVtkPointCloudWidget.h"
#endif
//using namespace HalconCpp;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *playout = new QHBoxLayout(ui->tab);

    // xml树demo
    {
        QXmlTreeWidget2 *xmltree = new QXmlTreeWidget2;
        xmltree->InitFromXmlConfig(
           "D:\\Git\\QtPluginSystem\\x64\\Release\\config\\SystemConfig.xml");
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
        auto btn = new QPushButton;
        playout->addWidget(btn);
        QDragTreeWidget *tree = new QDragTreeWidget;
        playout->addWidget(tree);
        tree->setBehavior(QDragTreeWidget::InteractionDragBehavior::DragToMove);
        tree = new QDragTreeWidget;
        playout->addWidget(tree);
        tree->setBehavior(QDragTreeWidget::InteractionDragBehavior::DragToCopy);

        QList<QTreeWidgetItem *> list;
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, "parent");
        item->setData(0, QDragTreeWidget::ItemLevel, 1);
        list.append(item);

        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0, "child1");
        child->setData(0, QDragTreeWidget::ItemLevel, 2);
        item->addChild(child);
        child = new QTreeWidgetItem();
        child->setText(0, "child2");
        child->setData(0, QDragTreeWidget::ItemLevel, 2);
        item->addChild(child);

        item = new QTreeWidgetItem();
        item->setText(0, "parent2");
        item->setData(0, QDragTreeWidget::ItemLevel, 1);
        list.append(item);

        child = new QTreeWidgetItem();
        child->setText(0, "child3");
        child->setData(0, QDragTreeWidget::ItemLevel, 2);
        item->addChild(child);
        child = new QTreeWidgetItem();
        child->setText(0, "child4");
        child->setData(0, QDragTreeWidget::ItemLevel, 2);
        item->addChild(child);

        tree->addTopLevelItems(list);


        connect(btn,&QPushButton::clicked,this,[=]{ qDebug()<<tree->IsValid();});
        connect(tree,&QDragTreeWidget::itemDragged,this,[=]{ qDebug()<<"drag";});
        connect(tree,&QDragTreeWidget::itemDropped,this,[=]{ qDebug()<<"drop";});
    }

    // 放大graphicsview demo
    {
        //QImageReader::setAllocationLimit(500);
        QImage img;
        img.load("D:\\taocibian\\baidian2\\20240522170243459.jpg", "JPG");

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

        QGraphicCircleItem *item2 = new QGraphicCircleItem(0, 0, 200, 100);
        item2->setPos(0, 0);
        item2->SetColor(Qt::blue);
        item2->SetLineWidth(3);
        ui->qZoomGraphicView->scene()->addItem(item2);
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
            new QGraphicArrowItem(QPointF(1, 1000.5), QPointF(1, 2050.2));
        arrow->SetLineWidth(3);
        // arrow->SetColor(Qt::red);
        // arrow->SetArrowType(QGraphicArrorItem::ArrowType::DoubleArrow);
        ui->qZoomGraphicView->scene()->addItem(arrow);
        // arrow->setFlag(QGraphicsItem::ItemIgnoresTransformations);

        auto rect = new QGraphicRectItem(QRectF(7500, 3000, 100, 100));
        rect->SetColor(Qt::red);
        rect->SetLineWidth(3);
        ui->qZoomGraphicView->scene()->addItem(rect);

        auto ptext = new QGraphicNoticeText();
        QFont font("黑体", 9, 1);
        QBrush brush(Qt::red);
        ptext->setText("123");
         ptext->setPos(10,10);
        ptext->setFont(font);
        ptext->setBrush(brush);
        ptext->AttachToCustomItem(item2);
        ui->qZoomGraphicView->scene()->addItem(ptext);

        connect(ui->pushButton, &QPushButton::clicked, this, [=] {
            arrow->SetColor(Qt::red);
            arrow->SetLineItem(QPointF(250.2, 2800), QPointF(1250.2, 2800));
            arrow->SetLineWidth(3);
            arrow->SetArrowType(QGraphicArrowItem::ArrowType::NoArrow);
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
        ui->horizontalLayout_4->addWidget(pdisp);
        connect(ui->pushButton_2, &QPushButton::clicked,this,[=]{
            HObject region;
            GenEmptyObj(&region);
            pdisp->MyDrawRect1(region);
        });
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
            if (index == 5) {
                try {
                    HalconCpp::HObject himg;
                    HalconCpp::ReadImage(&himg, "D:\\taocibian\\baidian2\\20240522170243459.jpg");
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
        QGrahpicViewMiniHud* hud1=new QGrahpicViewMiniHud;
        //QGrahpicViewMiniHud* hud2=new QGrahpicViewMiniHud;
        //QGrahpicViewMiniHud* hud3=new QGrahpicViewMiniHud;
        ui->horizontalLayout_2->addWidget(hud1);
        //ui->horizontalLayout_2->addWidget(hud2);
        //ui->horizontalLayout_2->addWidget(hud3);
        hud1->SetHighLight();
        // hud->SetHighLight();
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
            if (index == 4) {
                auto ptext = new QGraphicNoticeText("132");
                QFont font("黑体", 9, 1);
                QBrush brush(Qt::red);
                ptext->setPos(QPoint(100,100));

                QImage img;
                img.load("D:\\mifengding\\24740_00_01_28_173_0ALCB011G0000DE480203673_0ALCB011G0000DE480203675.tiff");
                hud1->SetImage(QPixmap::fromImage(img),"1233333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333",QList<QGraphicsItem*>()<<ptext);

                //hud2->SetText("123");
                //hud2->SetImage(QPixmap::fromImage(img));

            }
        });
        this->setStyleSheet("QGrahpicViewMiniHud > QLabel[Selected=\"0\"]{color: black;}"
                            "QGrahpicViewMiniHud > QLabel[Selected=\"1\"]{color: red;}"
                            "QGrahpicViewMiniHud > QLabel{font-size:15px;border: 1px solid orange;}");
    }
#ifdef BUILD_WITH_VTK
    {
        QVtkPointCloudWidget *pdisp = new QVtkPointCloudWidget;
        ui->horizontalLayout_4->addWidget(pdisp);
        //pdisp->setDisplaySample(50);
        connect(ui->pushButton_2, &QPushButton::clicked,this,[=]{
            auto ptr=pdisp->ReadTxtPointCloud(u8"D:\\管座\\cloud_csv_to_txt4.txt");
            //auto ptr=pdisp->ReadTxtPointCloud(u8"D:\\管座\\cloud_csv_to_txt4.txt");
            pdisp->MeshCloud(ptr, 0.3);
            //pdisp->AddCloud(ptr);
        });
    }
#endif
    {
        auto* codeEdit=new QCodeEdit;
        codeEdit->setStyleSheet("#LineNumberArea{background-color:lightgray;}");
        ui->horizontalLayout_4->addWidget(codeEdit);
    }
}

MainWindow::~MainWindow() { delete ui; }
