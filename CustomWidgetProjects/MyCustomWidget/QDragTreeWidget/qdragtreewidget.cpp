#include "qdragtreewidget.h"
#include "QDragTreeWidget/treeitemmimedata.h"
#include <QApplication>
#include <QDrag>
#include <QHeaderView>
#include <QMouseEvent>

QDragTreeWidget::QDragTreeWidget(QWidget* parent)
    :QTreeWidget(parent), m_Behavior(InteractionDragBehavior::DragToCopy)
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setAcceptDrops(true);
    this->setDragEnabled(true);
}

void QDragTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_startDragPoint = event->pos();
    }
    QTreeWidget::mousePressEvent(event);
}

void QDragTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int dragDistance = (event->pos() - m_startDragPoint).manhattanLength();
        if (dragDistance > QApplication::startDragDistance())
        {
            QTreeWidgetItem *item = currentItem();
            if (item)
            {
                TreeItemMimeData *mimeData = new TreeItemMimeData;
                mimeData->SetDragData("ItemMimeData",item);

                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                auto headerRect = this->header()->rect();
                auto itemRect = visualItemRect(item);
                auto realRect = QRect(itemRect.x(), itemRect.y() + headerRect.height(), itemRect.width(), itemRect.height());
                drag->setPixmap(this->grab(realRect));
                if(drag->exec(Qt::CopyAction|Qt::MoveAction,Qt::MoveAction) == Qt::MoveAction)
                    delete item;
            }
        }
    }
    QTreeWidget::mouseMoveEvent(event);
}

void QDragTreeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeWidget::mouseReleaseEvent(event);
}

void QDragTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget *source =  qobject_cast<QDragTreeWidget *>(event->source());
    TreeItemMimeData *pMimeData = (TreeItemMimeData *)(event->mimeData());
    if (pMimeData->hasFormat("ItemMimeData"))
    {
        if (source != this)
            if(m_Behavior == InteractionDragBehavior::DragToCopy)
                event->setDropAction(Qt::CopyAction);
            else
                event->setDropAction(Qt::MoveAction);
        else
            event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void QDragTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget *source =  qobject_cast<QDragTreeWidget *>(event->source());
    TreeItemMimeData *pMimeData = (TreeItemMimeData *)(event->mimeData());
    if (source == this && pMimeData->hasFormat("ItemMimeData"))
    {
        const QTreeWidgetItem *item = pMimeData->DragItemData();
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
        QTreeWidgetItem *currentItem = this->itemAt(event->pos());
#else
        QTreeWidgetItem *currentItem = this->itemAt(event->position().toPoint());
#endif
        if(currentItem){
            if (isRelated(item, currentItem))           //不允许拖回到原来的item
            {
                event->ignore();
            }
            else
            {
                setCurrentItem(currentItem);
                event->setDropAction(Qt::MoveAction);
                event->accept();
            }
        }
    }
    else if(source != this && pMimeData->hasFormat("ItemMimeData"))
    {
        if(m_Behavior == InteractionDragBehavior::DragToCopy)
            event->setDropAction(Qt::CopyAction);
        else
            event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void QDragTreeWidget::dropEvent(QDropEvent *event)
{
    QWidget *source =  qobject_cast<QDragTreeWidget *>(event->source());
    const TreeItemMimeData *pMimeData = (const TreeItemMimeData *)(event->mimeData());
    if (pMimeData->hasFormat("ItemMimeData"))
    {
        const QTreeWidgetItem *item = pMimeData->DragItemData();
        QTreeWidgetItem *pItem = item->clone();
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
        QTreeWidgetItem *currentItem = this->itemAt(event->pos());
#else
        QTreeWidgetItem *currentItem = this->itemAt(event->position().toPoint());
#endif
        if (currentItem)
        {
            currentItem->addChild(pItem);
            pItem->setExpanded(1);
        }
        else
        {
            this->addTopLevelItem(pItem);
            pItem->setExpanded(1);
        }
        if (source != this)
        {
            if(m_Behavior == InteractionDragBehavior::DragToCopy)
                event->setDropAction(Qt::CopyAction);
            else
                event->setDropAction(Qt::MoveAction);
        }
        else
        {
            event->setDropAction(Qt::MoveAction);
        }
        event->accept();
    }
}

void QDragTreeWidget::keyPressEvent(QKeyEvent *event)
{
    return QTreeWidget::keyPressEvent(event);
}

void QDragTreeWidget::keyReleaseEvent(QKeyEvent *event)
{
    return QTreeWidget::keyPressEvent(event);
}

bool QDragTreeWidget::isRelated(const QTreeWidgetItem* parent, const QTreeWidgetItem* child)
{
    if(parent == child)
        return true;
    else
    {
        int childCount = parent->childCount();
        if(childCount > 0)
        {
            for (int i = 0; i < childCount; i++)
            {
                auto item = parent->child(i);
                if(isRelated(item, child))
                    return true;
            }
        }
    }
    return false;
}

QDragTreeWidget::InteractionDragBehavior QDragTreeWidget::Behavior() const
{
    return m_Behavior;
}

void QDragTreeWidget::setBehavior(const InteractionDragBehavior &Behavior)
{
    m_Behavior = Behavior;
}
