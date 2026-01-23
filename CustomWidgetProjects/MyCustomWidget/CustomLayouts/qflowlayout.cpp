#include "qflowlayout.h"
#include <QtWidgets>

QFlowLayout::QFlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

QFlowLayout::QFlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

QFlowLayout::~QFlowLayout()
{
    QLayoutItem* item;
    while ((item = takeAt(0)))
        delete item;
}

void QFlowLayout::addItem(QLayoutItem* item)
{
    itemList.append(item);
}

int QFlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return m_hSpace;
    }
    else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

void QFlowLayout::setHorizontalSpacing(int hSpace)
{
    m_hSpace = hSpace;
}

int QFlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return m_vSpace;
    }
    else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

void QFlowLayout::setVerticalSpacing(int vSpace)
{
    m_vSpace = vSpace;
}

int QFlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem* QFlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem* QFlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations QFlowLayout::expandingDirections() const
{
    return { };
}

bool QFlowLayout::hasHeightForWidth() const
{
    return true;
}

int QFlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void QFlowLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize QFlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize QFlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem* item : std::as_const(itemList))
        size = size.expandedTo(item->minimumSize());

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int QFlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for (QLayoutItem* item : std::as_const(itemList)) {
        const QWidget* wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int QFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject* parent = this->parent();
    if (!parent) {
        return -1;
    }
    else if (parent->isWidgetType()) {
        QWidget* pw = static_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    else {
        return static_cast<QLayout*>(parent)->spacing();
    }
}

QFlowLayoutWidget::QFlowLayoutWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent,f)
{
    this->setLayout(new QFlowLayout(this));
}

int QFlowLayoutWidget::getLayoutLeftMargin() const
{
    return this->layout()->contentsMargins().left();
}

void QFlowLayoutWidget::setLayoutLeftMargin(int newLayoutLeftMargin)
{
    QMargins margins = this->layout()->contentsMargins();
    margins.setLeft(newLayoutLeftMargin);
    this->layout()->setContentsMargins(margins);
}

int QFlowLayoutWidget::getLayoutRightMargin() const
{
    return this->layout()->contentsMargins().right();
}

void QFlowLayoutWidget::setLayoutRightMargin(int newLayoutRightMargin)
{
    QMargins margins = this->layout()->contentsMargins();
    margins.setRight(newLayoutRightMargin);
    this->layout()->setContentsMargins(margins);
}

int QFlowLayoutWidget::getLayoutTopMargin() const
{
    return this->layout()->contentsMargins().top();
}

void QFlowLayoutWidget::setLayoutTopMargin(int newLayoutTopMargin)
{
    QMargins margins = this->layout()->contentsMargins();
    margins.setTop(newLayoutTopMargin);
    this->layout()->setContentsMargins(margins);
}

int QFlowLayoutWidget::getLayoutBottomMargin() const
{
    return this->layout()->contentsMargins().bottom();
}

void QFlowLayoutWidget::setLayoutBottomMargin(int newLayoutBottomMargin)
{
    QMargins margins = this->layout()->contentsMargins();
    margins.setBottom(newLayoutBottomMargin);
    this->layout()->setContentsMargins(margins);
}

int QFlowLayoutWidget::getLayoutHSpacing() const
{
    auto layout = qobject_cast<QFlowLayout*>(this->layout());
    return layout == nullptr ? -1 : layout->horizontalSpacing();
}

void QFlowLayoutWidget::setLayoutHSpacing(int newLayoutHSpacing)
{
    auto layout = qobject_cast<QFlowLayout*>(this->layout());
    if(layout)
        layout->setHorizontalSpacing(newLayoutHSpacing);
}

int QFlowLayoutWidget::getLayoutVSpacing() const
{
    auto layout = qobject_cast<QFlowLayout*>(this->layout());
    return layout == nullptr ? -1 : layout->verticalSpacing();
}

void QFlowLayoutWidget::setLayoutVSpacing(int newLayoutVSpacing)
{
    auto layout = qobject_cast<QFlowLayout*>(this->layout());
    if(layout)
        layout->setVerticalSpacing(newLayoutVSpacing);
}
