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
    emit ExitWin();
}

void TextEditorWidget::setLongText(QString &value)
{
    ui->textEdit->setText(value);
}
