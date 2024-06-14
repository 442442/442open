#ifndef QPARAMEDIT_H
#define QPARAMEDIT_H
#pragma once
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>
#include <QEvent>

class QLabel;
class QDoubleSpinBox;
class QHBoxLayout;
class QDESIGNER_WIDGET_EXPORT QParamEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(double value READ GetParam WRITE SetParam)
    Q_PROPERTY(QString param READ GetParamName WRITE SetParamName)
    Q_PROPERTY(QString unit READ GetParamUnit WRITE SetParamUnit)
    Q_PROPERTY(double step READ GetSingleStep WRITE SetSingleStep)
    Q_PROPERTY(double min READ GetMinimum WRITE SetMinimum)
    Q_PROPERTY(double max READ GetMaximum WRITE SetMaximum)

    Q_PROPERTY(bool mouseEnable READ getMouseEnableFlag WRITE setMouseEnableFlag)

    Q_PROPERTY(QString infoStyleSheet READ getInfoStyleSheet WRITE setInfoStyleSheet)
    Q_PROPERTY(QString unitStyleSheet READ getUnitStyleSheet WRITE setUnitStyleSheet)
    Q_PROPERTY(QString spinStyleSheet READ getSpinStyleSheet WRITE setSpinStyleSheet)

public:
    explicit QParamEdit(QWidget *parent = 0);
    explicit QParamEdit(QString paramName, double value, QString unit, QWidget* parent = nullptr);
    ~QParamEdit();

    void SetParam(double param);
    double GetParam() const;

    void SetParamName(const QString& name);
    QString GetParamName() const;

    void SetParamUnit(const QString& unit);
    QString GetParamUnit() const;

    double GetSingleStep() const;
    void SetSingleStep(double val);

    double GetMinimum() const;
    void SetMinimum(double min);

    double GetMaximum() const;
    void SetMaximum(double max);

    bool getMouseEnableFlag() const;
    /// <summary>
    /// 是否使用（spinbox）鼠标事件
    /// </summary>
    /// <param name="mouseEnableFlag"></param>
    void setMouseEnableFlag(bool mouseEnableFlag);

    QString getInfoStyleSheet() const;
    void setInfoStyleSheet(const QString& styleSheet);

    QString getUnitStyleSheet() const;
    void setUnitStyleSheet(const QString& styleSheet);

    QString getSpinStyleSheet() const;
    void setSpinStyleSheet(const QString& styleSheet);

    void SetRange(double min, double max);

    QLabel* mpParamLabel;
    QDoubleSpinBox *mpSpinBox;
    QLabel* mpUnit;

private:
    bool eventFilter(QObject *obj, QEvent *event);

    QHBoxLayout* mpLayout;
    bool mMouseEnableFlag;
};

#endif // QPARAMEDIT_H
