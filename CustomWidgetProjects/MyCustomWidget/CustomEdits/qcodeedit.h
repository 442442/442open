#ifndef QCODEEDIT_H
#define QCODEEDIT_H

#include <QPlainTextEdit>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT QCodeEdit : public QPlainTextEdit
{
    Q_OBJECT

    Q_PROPERTY(QColor highlightLineColor READ getHighlightLineColor WRITE setHighlightLineColor NOTIFY highlightLineColorChanged FINAL)

public:
    QCodeEdit(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    QColor getHighlightLineColor() const;
    void setHighlightLineColor(const QColor &newHighlightLineColor);

signals:
    void highlightLineColorChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    QColor highlightLineColor;

};

#endif // QCODEEDIT_H
