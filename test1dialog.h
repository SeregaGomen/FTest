#ifndef TEST1DIALOG_H
#define TEST1DIALOG_H

#include <QDialog>
#include <QRadioButton>

const int N = 21; // Кол-во вопросов

namespace Ui {
class Test1Dialog;
}

class Test1Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test1Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test1Dialog();
    QString getLegend(void);
    int getResults(void);

public slots:
    void accept(void);

private:
    Ui::Test1Dialog *ui;
    QRadioButton* rb[N][3];
    QString name;
    QString group;
    QString dt;
    int course;
    int idStudent;
    int res;
    void loadData(void);
    void setupTest(void);
    bool calcRes(int&,int*);
};

#endif // TEST1DIALOG_H
