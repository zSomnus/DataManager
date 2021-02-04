#ifndef TEXTEDITORWIDGET_H
#define TEXTEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class TextEditorWidget;
}

class TextEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditorWidget(QWidget *parent = nullptr);
    ~TextEditorWidget();
    QString long_text;

    void closeEvent(QCloseEvent *);

    void setLongText(QString &value);

private:
    Ui::TextEditorWidget *ui;

signals:
    void ExitWin();
};

#endif // TEXTEDITORWIDGET_H
