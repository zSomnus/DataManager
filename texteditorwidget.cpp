#include "texteditorwidget.h"
#include "ui_texteditorwidget.h"
#include "mainwindow.h"

TextEditorWidget::TextEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditorWidget)
{
    ui->setupUi(this);
}

TextEditorWidget::~TextEditorWidget()
{
    delete ui;
}

void TextEditorWidget::closeEvent(QCloseEvent *)
{
    long_text = ui->textEdit->toPlainText();
    long_text.replace("\"", "\"\"");
    long_text.prepend('"');
    long_text.append('"');

    setAttribute(Qt::WA_DeleteOnClose);
    emit ExitWin();
}

void TextEditorWidget::setLongText(QString &value)
{
    if(value.length() > 1) {
        if(value[0] == '"') {
            value.remove(0, 1);
            value.chop(1);
            value.replace("\"\"", "\"");
        }
    }
    ui->textEdit->setText(value);
}
