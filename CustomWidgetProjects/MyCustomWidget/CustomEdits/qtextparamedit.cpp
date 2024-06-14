#include "qtextparamedit.h"
#include <QLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QLineEdit>

QTextParamEdit::QTextParamEdit(QString paramName, QString value, QWidget* parent) :
    QWidget(parent)
{
    mpLayout = new QHBoxLayout(this);
    mpParamLabel = new QLabel(paramName);
    mpLineEdit = new QLineEdit(value);

    mpParamLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mpLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    mpLayout->addWidget(mpParamLabel);
    mpLayout->addWidget(mpLineEdit);

    mpLayout->setContentsMargins(0, 0, 20, 0);
}

QTextParamEdit::~QTextParamEdit()
{
    if (mpParamLabel != nullptr)
    {
        delete mpParamLabel;
        mpParamLabel = nullptr;
    }
    if (mpLineEdit != nullptr)
    {
        delete mpLineEdit;
        mpLineEdit = nullptr;
    }
}

void QTextParamEdit::setParam(const QString& param)
{
    mpLineEdit->setText(param);
}

QString QTextParamEdit::getParam() const
{
    return mpLineEdit->text();
}

void QTextParamEdit::setParamName(const QString& name)
{
    mpParamLabel->setText(name);
}

QString QTextParamEdit::getParamName() const
{
    return mpParamLabel->text();
}

QString QTextParamEdit::getLabelStyleSheet() const
{
    return mpParamLabel->styleSheet();
}

void QTextParamEdit::setLabelStyleSheet(const QString &styleSheet)
{
    mpParamLabel->setStyleSheet(styleSheet);
}

QString QTextParamEdit::getEditStyleSheet() const
{
    return mpLineEdit->styleSheet();
}

void QTextParamEdit::setEditStyleSheet(const QString &styleSheet)
{
    mpLineEdit->setStyleSheet(styleSheet);
}
