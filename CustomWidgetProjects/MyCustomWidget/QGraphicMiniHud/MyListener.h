#ifndef MYLISTENER_H
#define MYLISTENER_H

#include "QZoomGraphicView/qpixellistener.h"
#include <QObject>

class MyListener : public QObject, public QPixelListener
{
	Q_OBJECT
public:
	void OnPixelInfo(const QPointF& pos, const QColor& color) override
	{
		int r = 0, g = 0, b = 0, a = 0;
		color.getRgb(&r, &g, &b, &a);
		int gray = r * 0.299 + g * 0.587 + b * 0.114;
		QString str =
			QString("<font color=\'black\'>position(%1,%2)\t</font>"
				"<font color=\'gray\'>gray:%3\t</font>"
				"<font color=\'red\'>red:%4\t</font>"
				"<font color=\'green\'>green:%5\t</font>"
				"<font color=\'blue\'>blue:%6\t</font>"
				"<font color=\'black\'>alpha:%7\t</font>")
			.arg(QString::number(pos.x(), 'f', 3),
				QString::number(pos.y(), 'f', 3), QString::number(gray),
				QString::number(r), QString::number(g), QString::number(b),
				QString::number(a));
		emit NoticeColorStr(str);
	}
signals:
	void NoticeColorStr(const QString& str);
};

#endif // MYLISTENER_H
