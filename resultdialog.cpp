#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(int pi, QString pdt1, QString pdt2, int n1,int n2,int index,float m1, float m2, float s1, float s2, float sx1, float sx2, float t, float v,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    QString title[] = {
                        "Діяльнісно-операційний тест",
                        "Інформаційно-технологічний тест",
                        "Когнітивний тест з маркетингу туризму",
                        "Когнітивний тест з менеджменту",
                        "Особистісно-творчий тест",
                        "Мотиваційний тест"
                      };
    float ts[][4] =  {
                         { 1, 12.71f, 63.66f, 0.0f },
                         { 2, 4.30f, 9.92f, 31.60f },
                         { 3, 3.18f, 5.84f, 12.92f },
                         { 4, 2.78f, 4.60f, 8.61f },
                         { 5, 2.57f, 4.03f, 6.87f },
                         { 6, 2.45f, 3.71f, 5.96f },
                         { 7, 2.37f, 3.50f, 5.41f },
                         { 8, 2.31f, 3.36f, 5.04f },
                         { 9, 2.26f, 3.25f, 4.78f },
                         {10, 2.23f, 3.17f, 4.59f },
                         {20, 2.09f, 2.85f, 3.85f },
                         {30, 2.04f, 2.75f, 3.65f },
                         {60, 2.00f, 2.66f, 3.46f }
                     };


    ui->setupUi(this);

    setWindowTitle(title[pi]);
    ui->lbBeginDate->setText(pdt1);
    ui->lbCurrentDate->setText(pdt2);
    ui->lbBeginN->setText(QString("%1").arg(n1));
    ui->lbCurrentN->setText(QString("%1").arg(n2));
    ui->lbBeginExpect->setText(QString("%1").arg(m1,6,'f',2));
    ui->lbCurrentExpect->setText(QString("%1").arg(m2,6,'f',2));
    ui->lbBeginDeviation->setText(QString("%1").arg(s1,6,'f',2));
    ui->lbCurrentDeviation->setText(QString("%1").arg(s2,6,'f',2));
    ui->lbBeginError->setText(QString("%1").arg(sx1,6,'f',2));
    ui->lbCurrentError->setText(QString("%1").arg(sx2,6,'f',2));
    ui->lbT->setText(QString("%1").arg(t,6,'f',2));
    ui->lbFreedom->setText(QString("%1").arg(v,6,'f',2));


    if (t < ts[index][1])
        ui->lbSummary->setText(tr("Групи статистично не відрізняються! (%1 < %2)").arg(t,6,'f',2).arg(ts[index][1],6,'f',2));
    else
        ui->lbSummary->setText(tr("Групи статистично відрізняються! (%1 > %2)").arg(t,6,'f',2).arg(ts[index][1],6,'f',2));

    for (int i = 0; i < 13; i++)
    {
        ui->twTable->insertRow(i);
        ui->twTable->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(ts[i][0])));
        ui->twTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(ts[i][1],6,'f',2)));
    }
    ui->twTable->setCurrentCell(index,0);
    ui->twTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}
