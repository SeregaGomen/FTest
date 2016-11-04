#include <QListWidgetItem>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include "studentdialog.h"
#include "ui_studentdialog.h"

StudentDialog::StudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentDialog)
{
    ui->setupUi(this);
    initDialog();
}

StudentDialog::~StudentDialog()
{
    delete ui;
}

void StudentDialog::slotCheckedBegin(bool isChecked)
{
    ui->lwGroupBegin->setEnabled(!isChecked);
}

void StudentDialog::slotCheckedCurrent(bool isChecked)
{
    ui->lwGroupCurrent->setEnabled(!isChecked);
}

void StudentDialog::initDialog(void)
{
    QSqlQuery query;
    QListWidgetItem *item1,
                    *item2;

    ui->deCurrent->setDateTime(QDateTime::currentDateTime());
    ui->deBegin->setDateTime(QDateTime::currentDateTime());
    ui->lwGroupBegin->setEnabled(false);
    ui->lwGroupCurrent->setEnabled(false);
    connect(ui->cbGroupBegin, SIGNAL(clicked(bool)), this, SLOT(slotCheckedBegin(bool)));
    connect(ui->cbGroupCurrent, SIGNAL(clicked(bool)), this, SLOT(slotCheckedCurrent(bool)));

    if (!query.exec(QString("SELECT DISTINCT f_group FROM tbl_student")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
        qDebug() << query.lastError();
        return;
    }
    while (query.next())
    {
        item1 = new QListWidgetItem(query.value(0).toString(), ui->lwGroupBegin);
        item1->setFlags(item1->flags() | Qt::ItemIsUserCheckable);
        item1->setCheckState(Qt::Unchecked);

        item2 = new QListWidgetItem(query.value(0).toString(), ui->lwGroupCurrent);
        item2->setFlags(item2->flags() | Qt::ItemIsUserCheckable);
        item2->setCheckState(Qt::Unchecked);
    }

}

int StudentDialog::getCurrentTest(void)
{
    return ui->cbTest->currentIndex();
}

QString StudentDialog::getBeginDate(void)
{
    return ui->deBegin->text();
}

QString StudentDialog::getCurrentDate(void)
{
    return ui->deCurrent->text();
}

QStringList StudentDialog::getBeginGroups(void)
{
    QStringList lst;

    if (ui->cbGroupBegin->isChecked())
    {
        for (int i = 0; i < ui->lwGroupBegin->count(); i++)
            lst << ui->lwGroupBegin->item(i)->text();
    }
    else
    {
        for (int i = 0; i < ui->lwGroupBegin->count(); i++)
            if (ui->lwGroupBegin->item(i)->checkState() == Qt::Checked)
                lst << ui->lwGroupBegin->item(i)->text();
    }
    return lst;
}

QStringList StudentDialog::getCurrentGroups(void)
{
    QStringList lst;

    if (ui->cbGroupCurrent->isChecked())
    {
        for (int i = 0; i < ui->lwGroupCurrent->count(); i++)
            lst << ui->lwGroupCurrent->item(i)->text();
    }
    else
    {
        for (int i = 0; i < ui->lwGroupCurrent->count(); i++)
            if (ui->lwGroupCurrent->item(i)->checkState() == Qt::Checked)
                lst << ui->lwGroupCurrent->item(i)->text();
    }
    return lst;
}
