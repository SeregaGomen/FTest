#include <QSqlQuery>
#include <QCompleter>
#include <QStringList>
#include <QPushButton>
#include "getstudentdialog.h"
#include "ui_getstudentdialog.h"

GetStudentDialog::GetStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetStudentDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->deDate->setDateTime(QDateTime::currentDateTime());
    connect(ui->leName, SIGNAL(textChanged(const QString&)), this, SLOT(slotChangeText(const QString&)));
    connect(ui->leGroup, SIGNAL(textChanged(const QString&)), this, SLOT(slotChangeText(const QString&)));

    initDialog();
}

GetStudentDialog::~GetStudentDialog()
{
    delete ui;
}

QString GetStudentDialog::getStudentName(void)
{
    return ui->leName->text();
}

QString GetStudentDialog::getStudentGroup(void)
{
    return ui->leGroup->text();
}

QString GetStudentDialog::getDate(void)
{
    return ui->deDate->text();
}

int GetStudentDialog::getStudentClass(void)
{
    return ui->sbClass->value();
}

void GetStudentDialog::slotChangeText(const QString&)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->leName->text().length() && ui->leGroup->text().length());
}

void GetStudentDialog::initDialog(void)
{
    QSqlQuery query;
    QCompleter *completer1,
               *completer2;
    QStringList nameList,
                groupList;

    ui->leName->setFocus();


    if (query.exec(QString("SELECT DISTINCT f_name FROM tbl_student ORDER BY f_name")))
    {
        while (query.next())
            nameList << query.value(0).toString();

        completer1 = new QCompleter(nameList);
        completer1->setMaxVisibleItems(5);
        completer1->setCaseSensitivity(Qt::CaseInsensitive);
        ui->leName->setCompleter(completer1);
    }
    if (query.exec(QString("SELECT DISTINCT f_group FROM tbl_student ORDER BY f_name")))
    {
        while (query.next())
            groupList << query.value(0).toString();

        completer2 = new QCompleter(groupList);
        completer2->setMaxVisibleItems(5);
        completer2->setCaseSensitivity(Qt::CaseInsensitive);
        ui->leGroup->setCompleter(completer2);
    }
//    ui->leName->setText("");
}

void GetStudentDialog::initDialog(QString name,QString group,int course,QString dt)
{
    initDialog();
    ui->leName->setText(name);
    ui->leGroup->setText(group);
    ui->sbClass->setValue(course);
    ui->deDate->setDateTime(QDateTime::fromString(dt,"dd.MM.yyyy"));
}
