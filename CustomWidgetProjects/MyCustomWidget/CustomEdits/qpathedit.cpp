#include "qpathedit.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

QPathEdit::QPathEdit(const QString &path, QWidget *parent)
    : QWidget{parent}, mpLayout{new QHBoxLayout(this)},
    mpEdit{new QLineEdit(path)},
    mpButton{new QPushButton(u8"选择")}
{
    mpLayout->addWidget(mpEdit);
    mpLayout->addWidget(mpButton);
    mpLayout->setSpacing(0);
    mpLayout->setContentsMargins(0, 0, 0, 0);
    connect(mpButton,&QPushButton::clicked,this,[this]{
        QString savePath = QFileDialog::getExistingDirectory(this, u8"选择路径", mpEdit->text());
        if (!savePath.isEmpty())
        {
            mpEdit->setText(savePath);
        }
        emit editingFinished();
    });
    connect(mpEdit, &QLineEdit::editingFinished, this,
            &QPathEdit::editingFinished);
}

QPathEdit::~QPathEdit()
{
    delete mpEdit;
    mpEdit = nullptr;
    delete mpButton;
    mpButton = nullptr;
}

void QPathEdit::setText(const QString &text)
{
    mpEdit->setText(text);
}

QString QPathEdit::text() const
{
    return mpEdit->text();
}

QFileEdit::QFileEdit(const QString &path, QWidget *parent)
    : QWidget{parent}, mpLayout{new QHBoxLayout(this)},
    mpEdit{new QLineEdit(path)},
    mpButton{new QPushButton(u8"选择")}
{
    mpLayout->addWidget(mpEdit);
    mpLayout->addWidget(mpButton);
    mpLayout->setSpacing(0);
    mpLayout->setContentsMargins(0, 0, 0, 0);
    connect(mpButton,&QPushButton::clicked,this,[this]{
        QString savePath = QFileDialog::getOpenFileName(this, u8"选择路径", mpEdit->text());
        if (!savePath.isEmpty())
        {
            mpEdit->setText(savePath);
        }
        emit editingFinished();
    });
    connect(mpEdit, &QLineEdit::editingFinished, this,
            &QFileEdit::editingFinished);
}

QFileEdit::~QFileEdit()
{
    delete mpEdit;
    mpEdit = nullptr;
    delete mpButton;
    mpButton = nullptr;
}

void QFileEdit::setText(const QString &text)
{
    mpEdit->setText(text);
}

QString QFileEdit::text() const
{
    return mpEdit->text();
}
