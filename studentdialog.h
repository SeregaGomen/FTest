#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>

namespace Ui {
class StudentDialog;
}

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentDialog(QWidget *parent = 0);
    ~StudentDialog();
    int getCurrentTest(void);
    QString getBeginDate(void);
    QString getCurrentDate(void);
    QStringList getBeginGroups(void);
    QStringList getCurrentGroups(void);

private slots:
    void slotCheckedBegin(bool);
    void slotCheckedCurrent(bool);

private:
    Ui::StudentDialog *ui;
    void initDialog(void);
 };

#endif // STUDENTDIALOG_H
