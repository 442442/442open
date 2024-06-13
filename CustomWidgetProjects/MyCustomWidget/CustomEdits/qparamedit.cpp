#include "qparamedit.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QLayout>

QParamEdit::QParamEdit(QWidget *parent)
    : QParamEdit("param", 0, "unit", parent)
{}

QParamEdit::QParamEdit(QString paramName, double value, QString unit, QWidget* parent)
    : QWidget(parent), mMouseEnableFlag(false)
{
    mpLayout = new QHBoxLayout(this);
    mpParamLabel = new QLabel(paramName);
    mpSpinBox = new QDoubleSpinBox();
    mpSpinBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mpUnit = new QLabel(unit);
    mpSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding);

    mpSpinBox->setRange(-1, 9999999);
    mpSpinBox->setValue(value);
    mpSpinBox->setDecimals(4);
    mpSpinBox->installEventFilter(this);

    mpLayout->addWidget(mpParamLabel);
    mpLayout->addItem(mpSpacer);
    mpLayout->addWidget(mpSpinBox);
    mpLayout->addWidget(mpUnit);
    mpLayout->setContentsMargins(0, 0, 10, 0);
}

QParamEdit::~QParamEdit()
{
    if (mpParamLabel != nullptr)
    {
        delete mpParamLabel;
        mpParamLabel = nullptr;
    }
    if (mpSpinBox != nullptr)
    {
        delete mpSpinBox;
        mpSpinBox = nullptr;
    }
    if (mpUnit != nullptr)
    {
        delete mpUnit;
        mpUnit = nullptr;
    }
}

void QParamEdit::SetParam(double param)
{
    mpSpinBox->setValue(param);
}

double QParamEdit::GetParam() const
{
    return mpSpinBox->value();
}

void QParamEdit::SetParamName(const QString& name)
{
    mpParamLabel->setText(name);
}

QString QParamEdit::GetParamName() const
{
    return mpParamLabel->text();
}

void QParamEdit::SetParamUnit(const QString& unit)
{
    mpUnit->setText(unit);
}

QString QParamEdit::GetParamUnit() const
{
    return mpUnit->text();
}

double QParamEdit::GetSingleStep() const
{
    return mpSpinBox->singleStep();
}

void QParamEdit::SetSingleStep(double val)
{
    mpSpinBox->setSingleStep(val);
}

double QParamEdit::GetMinimum() const
{
    return mpSpinBox->minimum();
}

void QParamEdit::SetMinimum(double min)
{
    mpSpinBox->setMinimum(min);
}

double QParamEdit::GetMaximum() const
{
    return mpSpinBox->maximum();
}

void QParamEdit::SetMaximum(double max)
{
    mpSpinBox->setMaximum(max);
}

void QParamEdit::SetRange(double min, double max)
{
    mpSpinBox->setRange(min, max);
}

bool QParamEdit::getMouseEnableFlag() const
{
    return mMouseEnableFlag;
}

void QParamEdit::setMouseEnableFlag(bool mouseEnableFlag)
{
    mMouseEnableFlag = mouseEnableFlag;
}

 QString QParamEdit::getInfoStyleSheet() const
 {
     return mpParamLabel->styleSheet();
 }

void QParamEdit::setInfoStyleSheet(const QString &styleSheet)
{
    mpParamLabel->setStyleSheet(styleSheet);
}

QString QParamEdit::getUnitStyleSheet() const
{
    return mpUnit->styleSheet();
}

void QParamEdit::setUnitStyleSheet(const QString &styleSheet)
{
    mpUnit->setStyleSheet(styleSheet);
}

QString QParamEdit::getSpinStyleSheet() const
{
    return mpSpinBox->styleSheet();
}

void QParamEdit::setSpinStyleSheet(const QString &styleSheet)
{
    mpSpinBox->setStyleSheet(styleSheet);
}

bool QParamEdit::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->inherits("QAbstractSpinBox"))
    {
        if(event->type() == QEvent::Wheel && !mMouseEnableFlag)
            return true;
    }
    return QWidget::eventFilter(obj, event);
}
