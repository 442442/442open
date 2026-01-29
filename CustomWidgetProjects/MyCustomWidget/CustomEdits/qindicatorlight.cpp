#include "qindicatorlight.h"
#include <QPainter>

QIndicatorLight::QIndicatorLight(QWidget* parent) : QLabel(parent)
{
    this->setAlignment(Qt::AlignCenter);
}

QColor QIndicatorLight::normalColor() const
{
    return mNormalColor;
}

void QIndicatorLight::setNormalColor(const QColor &newNormalColor)
{
    if (mNormalColor == newNormalColor)
        return;
    mNormalColor = newNormalColor;
    emit normalColorChanged();
}

void QIndicatorLight::resetNormalColor()
{
    setNormalColor(Qt::green); // TODO: Adapt to use your actual default value
}

QColor QIndicatorLight::alarmColor() const
{
    return mAlarmColor;
}

void QIndicatorLight::setAlarmColor(const QColor &newAlarmColor)
{
    if (mAlarmColor == newAlarmColor)
        return;
    mAlarmColor = newAlarmColor;
    emit alarmColorChanged();
}

void QIndicatorLight::resetAlarmColor()
{
    setAlarmColor(Qt::yellow); // TODO: Adapt to use your actual default value
}

QColor QIndicatorLight::abnormalColor() const
{
    return mAbnormalColor;
}

void QIndicatorLight::setAbnormalColor(const QColor &newAbnormalColor)
{
    if (mAbnormalColor == newAbnormalColor)
        return;
    mAbnormalColor = newAbnormalColor;
    emit abnormalColorChanged();
}

void QIndicatorLight::resetAbnormalColor()
{
    setAbnormalColor(Qt::red); // TODO: Adapt to use your actual default value
}

QColor QIndicatorLight::normalTextColor() const
{
    return mNormalTextColor;
}

void QIndicatorLight::setNormalTextColor(const QColor &newNormalTextColor)
{
    if (mNormalTextColor == newNormalTextColor)
        return;
    mNormalTextColor = newNormalTextColor;
    emit normalTextColorChanged();
}

void QIndicatorLight::resetNormalTextColor()
{
    setNormalTextColor(Qt::black); // TODO: Adapt to use your actual default value
}

QColor QIndicatorLight::alarmTextColor() const
{
    return mAlarmTextColor;
}

void QIndicatorLight::setAlarmTextColor(const QColor &newAlarmTextColor)
{
    if (mAlarmTextColor == newAlarmTextColor)
        return;
    mAlarmTextColor = newAlarmTextColor;
    emit alarmTextColorChanged();
}

void QIndicatorLight::resetAlarmTextColor()
{
    setAlarmTextColor(Qt::black); // TODO: Adapt to use your actual default value
}

QColor QIndicatorLight::abnormalTextColor() const
{
    return mAbnormalTextColor;
}

void QIndicatorLight::setAbnormalTextColor(const QColor &newAbnormalTextColor)
{
    if (mAbnormalTextColor == newAbnormalTextColor)
        return;
    mAbnormalTextColor = newAbnormalTextColor;
    emit abnormalTextColorChanged();
}

void QIndicatorLight::resetAbnormalTextColor()
{
    setAbnormalTextColor(Qt::white); // TODO: Adapt to use your actual default value
}

QString QIndicatorLight::normalText() const
{
    return mNormalText;
}

void QIndicatorLight::setNormalText(const QString &newNormalText)
{
    if (mNormalText == newNormalText)
        return;
    mNormalText = newNormalText;
    emit normalTextChanged();
}

QString QIndicatorLight::alarmText() const
{
    return mAlarmText;
}

void QIndicatorLight::setAlarmText(const QString &newAlarmText)
{
    if (mAlarmText == newAlarmText)
        return;
    mAlarmText = newAlarmText;
    emit alarmTextChanged();
}

QString QIndicatorLight::abnormalText() const
{
    return mAbnormalText;
}

void QIndicatorLight::setAbnormalText(const QString &newAbnormalText)
{
    if (mAbnormalText == newAbnormalText)
        return;
    mAbnormalText = newAbnormalText;
    emit abnormalTextChanged();
}

void QIndicatorLight::setNormalStatus()
{
    setStatus(IndicatorStatus::Normal);
}

void QIndicatorLight::setAlarmStatus()
{
    setStatus(IndicatorStatus::Alarm);
}

void QIndicatorLight::setAbnormalStatus()
{
    setStatus(IndicatorStatus::Abnormal);
}

QIndicatorLight::IndicatorStatus QIndicatorLight::status() const
{
    return mStatus;
}

void QIndicatorLight::setStatus(QIndicatorLight::IndicatorStatus newStatus)
{
    if(mStatus == newStatus)
        return;
    mStatus = newStatus;
    emit statusChanged();
    repaint();
}

void QIndicatorLight::setStatusWithText(QIndicatorLight::IndicatorStatus newStatus, const QString &text)
{
    if(mStatus != newStatus)
    {
        mStatus = newStatus;
        emit statusChanged();
    }

    switch (mStatus) {
    case IndicatorStatus::Normal:
        mNormalText = text;
        break;
    case IndicatorStatus::Alarm:
        mAlarmText = text;
        break;
    case IndicatorStatus::Abnormal:
        mAbnormalText = text;
        break;
    default:
        break;
    }
    repaint();
}

void QIndicatorLight::paintEvent(QPaintEvent *event)
{
    // 1. 创建画家对象，指定绘制设备为当前标签
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿，让圆角更平滑

    // 2. 根据状态获取对应的背景色和文字色
    QColor bgColor, textColor;
    QString text;
    switch (mStatus) {
    case IndicatorStatus::Normal:
        bgColor = mNormalColor;
        textColor = mNormalTextColor;
        text = mNormalText;
        break;
    case IndicatorStatus::Alarm:
        bgColor = mAlarmColor;
        textColor = mAlarmTextColor;
        text = mAlarmText;
        break;
    case IndicatorStatus::Abnormal:
        bgColor = mAbnormalColor;
        textColor = mAbnormalTextColor;
        text = mAbnormalText;
        break;
    default:
        bgColor = mNormalColor;
        textColor = mNormalTextColor;
        text = mNormalText;
        break;
    }

    // 3. 绘制背景（填充整个标签区域，可加圆角）
    painter.setBrush(bgColor);          // 设置画刷（背景色）
    painter.setPen(Qt::NoPen);          // 去掉边框（如需边框可设置QPen）
    // 绘制圆角矩形（x, y, 宽, 高, 圆角x半径, 圆角y半径）
    painter.drawRoundedRect(rect(), 4, 4);

    // 4. 绘制文字
    painter.setPen(textColor);          // 设置画笔（文字色）
    painter.setFont(font());            // 使用标签的字体

    // 获取文字的矩形区域，实现居中对齐（和QLabel的alignment一致）
    QFontMetrics fm(painter.font());
    QRect textRect = fm.boundingRect(rect(), alignment(), text);
    painter.drawText(textRect, alignment(), text);
}
