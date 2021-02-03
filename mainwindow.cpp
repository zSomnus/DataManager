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
    mModel = new QStandardItemModel(this);
    ui->tableView->setModel(mModel);
    setCentralWidget(ui->tableView);
    setWindowTitle("Data Manager");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionCreate_New_File_triggered()
{
    const int rowCount = 5;
    const int colCount = 5;
    mModel->setRowCount(rowCount);
    mModel->setColumnCount(colCount);
}

void MainWindow::on_actionOpen_triggered()
{
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
        mModel->setRowCount(i);
        auto line = xin.readLine();
        auto values = line.split("\t");
        const int colCount = values.size();
        mModel->setColumnCount(colCount);

        for(int j=0; j < colCount; ++j) {
            setValueAt(i, j, values.at(j));
        }

        ++i;
    }

    file.close();
}

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::setValueAt(int i, int j, const QString &value)
{
    if(!mModel->item(i, j)) {
        mModel->setItem(i, j, new QStandardItem(value));
    } else {
        mModel->item(i, j)->setText(value);
    }
}
