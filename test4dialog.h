#ifndef TEST4DIALOG_H
#define TEST4DIALOG_H

#include <QDialog>
#include <QRadioButton>

#define MAX1 20

namespace Ui {
class Test4Dialog;
}

class Test4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test4Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test4Dialog();
    QString getLegend(void);
    int getResults(void);

public slots:
    void accept(void);

private:
    Ui::Test4Dialog *ui;
    QRadioButton* rb[MAX1][3];
    QString name;
    QString group;
    QString dt;
    int course;
    int idStudent;
    int res;
    void loadData(void);
    bool calcRes(int&,int*);
    void setupTest(void);
};

#endif // TEST4DIALOG_H
