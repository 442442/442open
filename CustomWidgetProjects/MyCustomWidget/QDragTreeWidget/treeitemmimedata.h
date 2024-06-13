#ifndef TREEITEMMIMEDATA_H
#define TREEITEMMIMEDATA_H
#pragma once
#include <qmimedata.h>

class QTreeWidgetItem;
class TreeItemMimeData : public QMimeData {
  Q_OBJECT
public:
  TreeItemMimeData() : QMimeData() { m_pDragItem = NULL; }

  ~TreeItemMimeData() {}

  void SetDragData(QString mimeType, QTreeWidgetItem *pItem) {
    m_format << mimeType;
    m_pDragItem = pItem;
  }

  QStringList formats() const { return m_format; }

  const QTreeWidgetItem *DragItemData() const { return m_pDragItem; }

protected:
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
  QVariant retrieveData(const QString &mimetype,
                        QVariant::Type preferredType) const {
    if (mimetype == "ItemMimeData") {
      return m_pDragItem;
    } else {
      return QMimeData::retrieveData(mimetype, preferredType);
    }
  }
#else

#endif
private:
  const QTreeWidgetItem *m_pDragItem;
  QStringList m_format;
};

#endif // TREEITEMMIMEDATA_H
