#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QStandardItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionCreate_New_File_triggered();

    void on_actionOpen_triggered();

    void on_actionSaveAs_triggered();

private:
    void setValueAt(int i, int j, const QString &value);
    QString getValueAt(int i, int j);
    Ui::MainWindow *ui;
    QStandardItemModel *mModel;
};
#endif // MAINWINDOW_H
