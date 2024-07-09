#include "qzoomgraphicdlg.h"
#include "ui_qzoomgraphicdlg.h"
#include "QZoomGraohicItems/qgraphicimgitem.h"
#include "MyListener.h"

QZoomGraphicDlg::QZoomGraphicDlg(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::QZoomGraphicDlg)
{
	ui->setupUi(this);
	//this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
}

QZoomGraphicDlg::~QZoomGraphicDlg()
{
	delete ui;
	delete mpListener;
}

void QZoomGraphicDlg::InitSceneSize(int width, int height)
{
	ui->qZoomGraphicView->InitSceneSize(width, height);
}

void QZoomGraphicDlg::AddImage(const QPixmap& pixmap)
{
	QGraphicImgItem* img = new QGraphicImgItem(pixmap);
	mpListener = new MyListener;
	img->AddPixelListener(mpListener);
	connect(mpListener, &MyListener::NoticeColorStr, ui->label, &QLabel::setText);
	ui->qZoomGraphicView->scene()->addItem(img);
}

void QZoomGraphicDlg::AddGraphicsItem(QGraphicsItem* item)
{
	ui->qZoomGraphicView->scene()->addItem(item);
}

void QZoomGraphicDlg::Update()
{
	ui->qZoomGraphicView->Update();
}

void QZoomGraphicDlg::showEvent(QShowEvent*)
{
	ui->qZoomGraphicView->Restore();
}

