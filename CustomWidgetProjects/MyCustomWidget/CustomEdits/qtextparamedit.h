#ifndef QTEXTPARAMEDIT_H
#define QTEXTPARAMEDIT_H
#pragma once
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

class QLabel;
class QLineEdit;
class QHBoxLayout;
class QDESIGNER_WIDGET_EXPORT QTextParamEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value READ getParam WRITE setParam)
    Q_PROPERTY(QString param READ getParamName WRITE setParamName)
    Q_PROPERTY(QString labelStyleSheet READ getLabelStyleSheet WRITE setLabelStyleSheet)
    Q_PROPERTY(QString editStyleSheet READ getEditStyleSheet WRITE setEditStyleSheet)

public:
    explicit QTextParamEdit(QWidget *parent = 0)
        :QTextParamEdit("param","text",parent){};
    explicit QTextParamEdit(QString paramName, QString value, QWidget* parent = nullptr);
    ~QTextParamEdit();

    void setParam(const QString& param);
    QString getParam() const;

    void setParamName(const QString& name);
    QString getParamName() const;

    QString getLabelStyleSheet() const;
    void setLabelStyleSheet(const QString& styleSheet);

    QString getEditStyleSheet() const;
    void setEditStyleSheet(const QString& styleSheet);

    QLabel* mpParamLabel;
    QLineEdit* mpLineEdit;

private:
    QHBoxLayout* mpLayout;
};

#endif // QTEXTPARAMEDIT_H
