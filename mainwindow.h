#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidget;

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

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_actionDeleteTriggered();
private:
    void setValueAt(int i, int j, const QString &value);
    QString getValueAt(int i, int j);
    Ui::MainWindow *ui;
    QTableWidget *table;
    int selected_row;
};
#endif // MAINWINDOW_H
