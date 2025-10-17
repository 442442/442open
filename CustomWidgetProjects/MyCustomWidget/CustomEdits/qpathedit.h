#ifndef QPATHEDIT_H
#define QPATHEDIT_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QDESIGNER_WIDGET_EXPORT QPathEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QPathEdit(QWidget *parent = nullptr) : QPathEdit("", parent) {}
    explicit QPathEdit(const QString &path, QWidget *parent = nullptr);
    ~QPathEdit();

    void setText(const QString& text);
    QString text() const;

signals:
    void editingFinished();
private:
    QHBoxLayout *mpLayout;
    QLineEdit* mpEdit;
    QPushButton* mpButton;
};

class QDESIGNER_WIDGET_EXPORT QFileEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QFileEdit(QWidget *parent = nullptr) : QFileEdit("", parent) {}
    explicit QFileEdit(const QString& path,QWidget *parent = nullptr);
    ~QFileEdit();

    void setText(const QString& text);
    QString text() const;

signals:
    void editingFinished();
private:
    QHBoxLayout *mpLayout;
    QLineEdit* mpEdit;
    QPushButton* mpButton;
};

#endif // QPATHEDIT_H
