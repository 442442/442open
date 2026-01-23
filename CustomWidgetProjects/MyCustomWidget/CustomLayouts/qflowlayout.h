#pragma once
#include <QLayout>
#include <QWidget>
#include <QRect>
#include <QStyle>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT QFlowLayout : public QLayout
{
    Q_OBJECT

public:
    explicit QFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit QFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~QFlowLayout();

	void addItem(QLayoutItem* item) override;
	int horizontalSpacing() const;
    void setHorizontalSpacing(int hSpace);
	int verticalSpacing() const;
    void setVerticalSpacing(int vSpace);
	Qt::Orientations expandingDirections() const override;
	bool hasHeightForWidth() const override;
	int heightForWidth(int) const override;
	int count() const override;
	QLayoutItem* itemAt(int index) const override;
	QSize minimumSize() const override;
	void setGeometry(const QRect& rect) override;
	QSize sizeHint() const override;
	QLayoutItem* takeAt(int index) override;

private:
	int doLayout(const QRect& rect, bool testOnly) const;
	int smartSpacing(QStyle::PixelMetric pm) const;

	QList<QLayoutItem*> itemList;
	int m_hSpace;
	int m_vSpace;
};

class QDESIGNER_WIDGET_EXPORT QFlowLayoutWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int layoutLeftMargin READ getLayoutLeftMargin WRITE setLayoutLeftMargin FINAL)
    Q_PROPERTY(int layoutRightMargin READ getLayoutRightMargin WRITE setLayoutRightMargin FINAL)
    Q_PROPERTY(int layoutTopMargin READ getLayoutTopMargin WRITE setLayoutTopMargin FINAL)
    Q_PROPERTY(int layoutBottomMargin READ getLayoutBottomMargin WRITE setLayoutBottomMargin FINAL)
    Q_PROPERTY(int layoutHSpacing READ getLayoutHSpacing WRITE setLayoutHSpacing FINAL)
    Q_PROPERTY(int layoutVSpacing READ getLayoutVSpacing WRITE setLayoutVSpacing FINAL)

public:
    explicit QFlowLayoutWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    int getLayoutLeftMargin() const;
    void setLayoutLeftMargin(int newLayoutLeftMargin);

    int getLayoutRightMargin() const;
    void setLayoutRightMargin(int newLayoutRightMargin);

    int getLayoutTopMargin() const;
    void setLayoutTopMargin(int newLayoutTopMargin);

    int getLayoutBottomMargin() const;
    void setLayoutBottomMargin(int newLayoutBottomMargin);

    int getLayoutHSpacing() const;
    void setLayoutHSpacing(int newLayoutHSpacing);

    int getLayoutVSpacing() const;
    void setLayoutVSpacing(int newLayoutVSpacing);

};
