#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QItemSelection>

namespace Ui {
class TableDialog;
}



class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QWidget *parent = 0);
    ~TableDialog();

private slots:
    void setEnabledBtn(QItemSelection,QItemSelection);
    void slotFirst(void);
    void slotPrior(void);
    void slotNext(void);
    void slotLast(void);
    void slotTest1(void);
    void slotTest2(void);
    void slotTest3(void);
    void slotTest4(void);
    void slotName(void);
    void showContextMenu(const QPoint&);

private:
    Ui::TableDialog *ui;
    QMenu menu;     // Контекстные меню
    QMenu* delMenu;
    void setupDialog(void);
    void createMenu(void);
    void removeTest(int);
};

#endif // TABLEDIALOG_H
