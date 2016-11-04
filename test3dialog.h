#ifndef TEST3DIALOG_H
#define TEST3DIALOG_H

#include <QDialog>
#include <QRadioButton>

#define MAX 16

namespace Ui {
class Test3Dialog;
}

class Test3Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Test3Dialog(QString,QString,QString,int,QWidget *parent = 0);
    ~Test3Dialog();
    QString getLegend(void);
    int getResults(void);

public slots:
    void accept(void);

private:
    Ui::Test3Dialog *ui;
    QRadioButton* rb[MAX][7];
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

#endif // TEST3DIALOG_H
