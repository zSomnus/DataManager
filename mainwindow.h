#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidget;
class QComboBox;

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

    void on_actionSave_triggered();

    // Right click menu
    void on_actionDeleteTriggered();

private:
    void setValueAt(int i, int j, const QString &value);
    QString getValueAt(int i, int j);
    Ui::MainWindow *ui;
    QTableWidget *table;
    int selected_row;
    QComboBox *position_box;

    int Flag_IsOpen = 0;       //标记：判断是否打开或创建了一个文件
    int Flag_IsNew = 0;        //标记：如果新建了文件就为1，初始值为0
    QString Last_FileName;     //最后一次保存的文件的名字
    QString Last_FileContent;  //最后一次保存文件的内容
};
#endif // MAINWINDOW_H
