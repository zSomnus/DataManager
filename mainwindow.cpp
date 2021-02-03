#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = ui->tableWidget;
    setWindowTitle("Data Manager");

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
