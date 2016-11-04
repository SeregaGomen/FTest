#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();
    QMap<QString,bool> getMap(void);

private slots:
    void slotCBClicked(bool);

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
