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
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    const int rowCount = mModel->rowCount();
    const int colCount = mModel->columnCount();

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
    if(!mModel->item(i, j)) {
        mModel->setItem(i, j, new QStandardItem(value));
    } else {
        mModel->item(i, j)->setText(value);
    }
}

QString MainWindow::getValueAt(int i, int j)
{
    if(!mModel->item(i, j)) {
        return "";
    }
    return mModel->item(i, j)->text();
}
