#ifndef QINDICATORLIGHT_H
#define QINDICATORLIGHT_H

#include <QLabel>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT QIndicatorLight : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor RESET resetNormalColor NOTIFY normalColorChanged FINAL)
    Q_PROPERTY(QColor alarmColor READ alarmColor WRITE setAlarmColor RESET resetAlarmColor NOTIFY alarmColorChanged FINAL)
    Q_PROPERTY(QColor abnormalColor READ abnormalColor WRITE setAbnormalColor RESET resetAbnormalColor NOTIFY abnormalColorChanged FINAL)

    Q_PROPERTY(QColor normalTextColor READ normalTextColor WRITE setNormalTextColor RESET resetNormalTextColor NOTIFY normalTextColorChanged FINAL)
    Q_PROPERTY(QColor alarmTextColor READ alarmTextColor WRITE setAlarmTextColor RESET resetAlarmTextColor NOTIFY alarmTextColorChanged FINAL)
    Q_PROPERTY(QColor abnormalTextColor READ abnormalTextColor WRITE setAbnormalTextColor RESET resetAbnormalTextColor NOTIFY abnormalTextColorChanged FINAL)

    Q_PROPERTY(QString normalText READ normalText WRITE setNormalText NOTIFY normalTextChanged FINAL)
    Q_PROPERTY(QString alarmText READ alarmText WRITE setAlarmText NOTIFY alarmTextChanged FINAL)
    Q_PROPERTY(QString abnormalText READ abnormalText WRITE setAbnormalText NOTIFY abnormalTextChanged FINAL)

    Q_PROPERTY(QIndicatorLight::IndicatorStatus status READ status WRITE setStatus NOTIFY statusChanged FINAL)

public:
    enum IndicatorStatus
    {
        Normal,
        Alarm,
        Abnormal
    };
    Q_ENUM(IndicatorStatus)

    QIndicatorLight(QWidget* parent = nullptr);

    QColor normalColor() const;
    void setNormalColor(const QColor &newNormalColor);
    void resetNormalColor();

    QColor alarmColor() const;
    void setAlarmColor(const QColor &newAlarmColor);
    void resetAlarmColor();

    QColor abnormalColor() const;
    void setAbnormalColor(const QColor &newAbnormalColor);
    void resetAbnormalColor();

    QColor normalTextColor() const;
    void setNormalTextColor(const QColor &newNormalTextColor);
    void resetNormalTextColor();

    QColor alarmTextColor() const;
    void setAlarmTextColor(const QColor &newAlarmTextColor);
    void resetAlarmTextColor();

    QColor abnormalTextColor() const;
    void setAbnormalTextColor(const QColor &newAbnormalTextColor);
    void resetAbnormalTextColor();

    QString normalText() const;
    void setNormalText(const QString &newNormalText);

    QString alarmText() const;
    void setAlarmText(const QString &newAlarmText);

    QString abnormalText() const;
    void setAbnormalText(const QString &newAbnormalText);

    QIndicatorLight::IndicatorStatus status() const;
    void setStatus(QIndicatorLight::IndicatorStatus newStatus);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setNormalStatus();
    void setAlarmStatus();
    void setAbnormalStatus();

    void setStatusWithText(QIndicatorLight::IndicatorStatus newStatus, const QString& text);

public:
    void paintEvent(QPaintEvent *event) override;

signals:
    void normalColorChanged();
    void alarmColorChanged();
    void abnormalColorChanged();

    void normalTextColorChanged();
    void alarmTextColorChanged();
    void abnormalTextColorChanged();

    void normalTextChanged();
    void alarmTextChanged();
    void abnormalTextChanged();

    void statusChanged();

private:
    QColor mNormalColor{Qt::green};
    QColor mAlarmColor{Qt::yellow};
    QColor mAbnormalColor{Qt::red};

    QColor mNormalTextColor{Qt::black};
    QColor mAlarmTextColor{Qt::black};
    QColor mAbnormalTextColor{Qt::white};

    QString mNormalText{ u8"正常" };
    QString mAlarmText{ u8"警告" };
    QString mAbnormalText{ u8"异常" };

    QIndicatorLight::IndicatorStatus mStatus{IndicatorStatus::Normal};
};
#endif // QINDICATORLIGHT_H
