#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidget;
class QComboBox;
class TextEditorWidget;

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
    void on_actionEditLongText();

    void on_actionInsertImage();


    // Table action
    void on_actionClearCell();

    void on_actionInsertAboveTriggered();

    void on_actionInsertBelowTriggered();

    void on_actionDeleteTriggered();

    void on_closeTextEditor();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    void setValueAt(int i, int j, const QString &value);
    void setValueWithIconAt(int i, int j, const QString &value);
    QString getValueAt(int i, int j);

    int selected_row;
    int selected_column;

    Ui::MainWindow *ui;
    QTableWidget *table;
    QComboBox *position_box;
    TextEditorWidget *text_edit_widget;

    int Flag_IsOpen = 0;       // Flag: Is the file opend or not
    int Flag_IsNew = 0;        // Flag: if it's new file, set the value to 1, initial number is 0
    QString Last_FileName;     // File name during last save
    QString Last_FileContent;  // File content during last save
};
#endif // MAINWINDOW_H
