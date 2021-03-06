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
    setWindowTitle("Data Manager");
    position_box = new QComboBox();
    on_actionCreate_New_File_triggered();
    table->setShowGrid(true);
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
    auto filename = QFileDialog::getOpenFileName(this, "Open File", QDir::rootPath(), "CSV File (*.csv)");

    if(filename.isEmpty()) {
        return;
    }

    table->clear();

    table->setRowCount(0);
    table->setColumnCount(0);

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream xin(&file);
    int i = 0;
    bool isLongText = false;
    QString temp = "";
    QString current;

    int col = 0;
    int row = 0;

    int maxCol = 0;


    while(!xin.atEnd()) {
        auto line = xin.readLine();

        if(line.isEmpty()) {
            if(isLongText) {
                temp.append('\n');
            } else {
                ++row;
            }

            ++i;
            continue;
        }

        table->setRowCount(row + 1);
        auto values = line.split(",");

        const int colCount = values.size();

        if(colCount > maxCol) {
            maxCol = colCount;
        }

        table->setColumnCount(maxCol);

        for(int j=0; j < colCount; ++j) {
            current = values.at(j);

            if(col + 1 > table->columnCount()) {
                table->setColumnCount(col + 1);
                maxCol = col + 1;
            }

            if(current.length() > 0 && current[0] == '"' && current.left(2) != "\"\"" && !isLongText) {
                isLongText = true;
                qDebug() << current;
            }

            if(isLongText) {
                if(!temp.isEmpty() && temp.right(1) != '\n') {
                    temp.append(',');
                }
                temp.append(current);
                if(current.length() > 1 && current.right(2) != "\"\"" && current.right(1) == '\"') {
                    // Set value here
                    qDebug() << current;
                    setValueAt(row, col, temp);
                    table->item(row, col)->setFlags((Qt::ItemFlags)16);
                    isLongText = false;
                    temp.clear();
                    ++col;
                }
            } else {
                // Set value here
                // Check if this is a image (.png, .jepg, .jpg, .jpe)
                if(current.contains(".png") ||
                        current.contains(".jpg") ||
                        current.contains(".jepg") ||
                        current.contains(".jpe")) {
                    QFileInfo fileInfo(current);
                    if(fileInfo.isFile()) {
                        setValueWithIconAt(row, col, current);
                    }
                } else {
                    setValueAt(row, col, current);
                }
                ++col;
            }
            current.clear();
        }

        if(!isLongText) {
            ++row;
            col = 0;
        } else {
            temp.append('\n');
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
    auto filename = QFileDialog::getSaveFileName(this, "Save", QDir::rootPath(), "CSV File (*.csv)");

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
            xout << "," << getValueAt(i, j);
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
                    xout << "," << getValueAt(i, j);
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

void MainWindow::setValueWithIconAt(int i, int j, const QString &value)
{
    if(table->item(i, j)) {
        table->removeCellWidget(i, j);
    }
    table->setItem(i, j, new QTableWidgetItem(QIcon(value), value));
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

    QAction *clear_cell = treeWidgeMenu->addAction("Clear current cell");

    treeWidgeMenu->addSeparator();

    QAction *insert_row_above = treeWidgeMenu->addAction("Insert row above");
    QAction *insert_row_below = treeWidgeMenu->addAction("Insert row below");
    QAction *delete_row = treeWidgeMenu->addAction("Delete row");


    QModelIndex index = table->indexAt(pos);    //Find current position of tableview
    selected_row = index.row();
    selected_column = index.column();

    // Connections
    connect(edit_long_text, SIGNAL(triggered()), this, SLOT(on_actionEditLongText()));
    connect(insert_image, SIGNAL(triggered()), this, SLOT(on_actionInsertImage()));

    connect(clear_cell, SIGNAL(triggered()), this, SLOT(on_actionClearCell()));
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
    ui->imagePreviewLabel->setPixmap(QPixmap(filename).scaled(ui->imagePreviewLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::on_actionClearCell()
{
    delete  table->item(selected_row, selected_column);
    setValueAt(selected_row, selected_column, "");
}

void MainWindow::on_closeTextEditor()
{
    table->setItem(selected_row, selected_column, new QTableWidgetItem(text_edit_widget->long_text));
    table->item(selected_row, selected_column)->setFlags((Qt::ItemFlags)16);
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
    table->removeRow(selected_row);  // Remove selected row
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    QString current_string = getValueAt(currentRow, currentColumn);
    if(current_string.contains(".png") ||
            current_string.contains(".jpg") ||
            current_string.contains(".jepg") ||
            current_string.contains(".jpe")) {
        QFileInfo fileInfo(current_string);
        if(fileInfo.isFile()) {
            ui->imagePreviewLabel->setPixmap(QPixmap(current_string).scaled(ui->imagePreviewLabel->size(), Qt::KeepAspectRatio));
        }
    } else {
        ui->imagePreviewLabel->setText("No image...");
    }
}
