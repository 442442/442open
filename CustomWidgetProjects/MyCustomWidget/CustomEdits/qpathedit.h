#ifndef QPATHEDIT_H
#define QPATHEDIT_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QPathEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QPathEdit(const QString& path,QWidget *parent = nullptr);
    ~QPathEdit();

    QString text() const;

signals:
    void editingFinished();
private:
    QHBoxLayout *mpLayout;
    QLineEdit* mpEdit;
    QPushButton* mpButton;
};

class QFileEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QFileEdit(const QString& path,QWidget *parent = nullptr);
    ~QFileEdit();

    QString text() const;

signals:
    void editingFinished();
private:
    QHBoxLayout *mpLayout;
    QLineEdit* mpEdit;
    QPushButton* mpButton;
};

#endif // QPATHEDIT_H
