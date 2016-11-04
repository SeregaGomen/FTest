#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class GetStudentDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotTest1(void);
    void slotTest2(void);
    void slotTest3(void);
    void slotTest4(void);
    void slotStudent(void);
    void slotTable(void);
    void slotExport(void);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    GetStudentDialog* sdlg;
    void readSettings(void);
    void writeSettings(void);
    void openDB(void);
    void closeDB(void);
    void setupLanguage(void);
    void calcStudent(int,QString,QString,QStringList,QStringList);
    void export2XML(QString,QMap<QString,bool>);
    bool checkDB(QString);
    bool createDB(QString);
};

#endif // MAINWINDOW_H
