#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QComboBox>
#include <QMessageBox>
#include <QTextEdit>
#include "texteditorwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = ui->tableWidget;
    setCentralWidget(table);
    setWindowTitle("Data Manager");
    position_box = new QComboBox();
    on_actionCreate_New_File_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionCreate_New_File_triggered()
{
    table->clear();
    const int rowCount = 2;
    const int colCount = 5;
    table->setRowCount(rowCount);
    table->setColumnCount(colCount);
    table->setHorizontalHeaderLabels({"Type", "Name", "Position", "Dialog", "Image"});

    Flag_IsNew = 1;
    Flag_IsOpen = 1;
}

void MainWindow::on_actionOpen_triggered()
{
    table->clear();

    auto filename = QFileDialog::getOpenFileName(this, "Open File", QDir::rootPath(), "TSV File (*.tsv)");

    if(filename.isEmpty()) {
        return;
    }

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream xin(&file);
    int i = 0;
    while(!xin.atEnd()) {
        table->setRowCount(i + 1);
        auto line = xin.readLine();
        auto values = line.split("\t");
        const int colCount = values.size();
        table->setColumnCount(colCount);

        for(int j=0; j < colCount; ++j) {
            setValueAt(i, j, values.at(j));
        }

        ++i;
    }

    file.close();
    Flag_IsOpen = 1;
    Flag_IsNew = 0;
    Last_FileName = filename;
}

void MainWindow::on_actionSaveAs_triggered()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save", QDir::rootPath(), "TSV File (*.tsv)");

    if(filename.isEmpty()) {
        return;
    }

    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream xout(&file);
    const int rowCount = table->rowCount();
    const int colCount = table->columnCount();

    for(int i=0; i<rowCount; ++i) {
        xout << getValueAt(i, 0);
        for(int j=1; j<colCount; ++j) {
            xout << "\t" << getValueAt(i, j);
        }
        xout << "\n";
    }
    file.flush();
    file.close();

    Flag_IsNew = 0;
    Last_FileName = filename;
}

void MainWindow::on_actionSave_triggered()
{
    if(Flag_IsNew) {
        on_actionSaveAs_triggered();
    } else {
        QFile file(Last_FileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("Warning"),tr("Fail to open the file"));
            return;
        }
        else
        {
            if(Last_FileName.isEmpty()) {
                return;
            }

            QFile file(Last_FileName);

            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return;
            }

            QTextStream xout(&file);
            const int rowCount = table->rowCount();
            const int colCount = table->columnCount();

            for(int i=0; i<rowCount; ++i) {
                xout << getValueAt(i, 0);
                for(int j=1; j<colCount; ++j) {
                    xout << "\t" << getValueAt(i, j);
                }
                xout << "\n";
            }
            file.flush();
            file.close();
        }
    }
}

void MainWindow::setValueAt(int i, int j, const QString &value)
{
    if(!table->item(i, j)) {
        table->setItem(i, j, new QTableWidgetItem(value));
    } else {
        table->item(i, j)->setText(value);
    }
}

QString MainWindow::getValueAt(int i, int j)
{
    if(!table->item(i, j)) {
        return "";
    }
    return table->item(i, j)->text();
}

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *treeWidgeMenu = new QMenu();

    // Actions
    QAction *edit_long_text = treeWidgeMenu->addAction("Long text...");
    QAction *insert_image = treeWidgeMenu->addAction("Image...");

    treeWidgeMenu->addSeparator();

    QAction *insert_row_above = treeWidgeMenu->addAction("Insert row above");
    QAction *insert_row_below = treeWidgeMenu->addAction("Insert row below");
    QAction *delete_row = treeWidgeMenu->addAction("Delete row");


    QModelIndex index = table->indexAt(pos);    //找到tableview当前位置信息
    selected_row = index.row();
    selected_column = index.column();

    // Connections
    connect(edit_long_text, SIGNAL(triggered()), this, SLOT(on_actionEditLongText()));
    connect(insert_image, SIGNAL(triggered()), this, SLOT(on_actionInsertImage()));

    connect(insert_row_above, SIGNAL(triggered()), this, SLOT(on_actionInsertAboveTriggered()));
    connect(insert_row_below, SIGNAL(triggered()), this, SLOT(on_actionInsertBelowTriggered()));
    connect(delete_row, SIGNAL(triggered()), this, SLOT(on_actionDeleteTriggered()));

    treeWidgeMenu->exec(QCursor::pos());
}

void MainWindow::on_actionEditLongText() {
    text_edit_widget = new TextEditorWidget;
    text_edit_widget->setWindowTitle("Text Editor");
    text_edit_widget->long_text = getValueAt(selected_row, selected_column);
    text_edit_widget->setLongText(text_edit_widget->long_text);
    text_edit_widget->show();
    connect(text_edit_widget,SIGNAL(ExitWin()),this,SLOT(on_closeTextEditor()));
}

void MainWindow::on_actionInsertImage()
{
    QString path = "";

    if(table->itemAt(selected_row, selected_column)) {
        path = getValueAt(selected_row, selected_column);
    } else {
        path = QDir::rootPath();
    }

    auto filename = QFileDialog::getOpenFileName(this, "Select Image", path, "image/jpeg (*.jpeg *.jpg *.jpe);;image/png (*.png)");
    table->setItem(selected_row, selected_column, new QTableWidgetItem(QIcon(filename), filename));
}

void MainWindow::on_closeTextEditor()
{
    table->setItem(selected_row, selected_column, new QTableWidgetItem(text_edit_widget->long_text));
}

void MainWindow::on_actionInsertAboveTriggered()
{
    table->insertRow(selected_row);
}

void MainWindow::on_actionInsertBelowTriggered()
{
    table->insertRow(selected_row + 1);
}

void MainWindow::on_actionDeleteTriggered() {
    //通过sender()得到信号的发送对象，也就是哪个菜单项被单击
    table->removeRow(selected_row);  //删除掉了表格信息
}

