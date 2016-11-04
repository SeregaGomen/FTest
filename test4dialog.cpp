#include <QSqlQuery>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include "test4dialog.h"
#include "ui_test4dialog.h"

Test4Dialog::Test4Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test4Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Оцінка рівня сформованості самооцінного компонента - " + name);
    setupTest();
    loadData();
}

Test4Dialog::~Test4Dialog()
{
    delete ui;
}

void Test4Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test4 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < MAX1; i++)
            {
                q = query.value(3 + i).toInt();
                switch (q)
                {
                    case 1:
                        rb[i][0]->setChecked(true);
                        break;
                    case 2:
                        rb[i][1]->setChecked(true);
                        break;
                    case 3:
                        rb[i][2]->setChecked(true);
                        break;
                }
            }
    }
}

void Test4Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[MAX1],
        id = 0;
    bool isFind = false;

    if (!calcRes(res,q))
        return;
    if (!idStudent)
    {
        // Сохраняем информацию о тестируемом
        if (!query.exec(QString("INSERT INTO tbl_student (f_name,f_group,f_class) VALUES ('%1','%2',%3)").arg(name.toUpper()).arg(group.toUpper()).arg(course)))
        {
            QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
            return;
        }

        query.exec(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
        while (query.next())
            idStudent = query.value(0).toInt();
    }
    // Сохраняем анекту
    query.exec(QString("SELECT id FROM tbl_test4 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test4 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8,f_q9 = %9,f_q10 = %10,f_q11 = %11,f_q12 = %12,f_q13 = %13,f_q14 = %14,f_q15 = %15,f_q16 = %16,f_q17 = %17,f_q18 = %18,f_q19 = %19,f_q20 = %20 WHERE f_student = %21 AND f_dt = '%22'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(q[18]).arg(q[19]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test4 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8,f_q9,f_q10,f_q11,f_q12,f_q13,f_q14,f_q15,f_q16,f_q17,f_q18,f_q19,f_q20) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(q[18]).arg(q[19]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test4 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }


    if (res <= -4)
        resTxt = tr("низький");
    else if (res >= -3 && res <= 3)
        resTxt = tr("занижений");
    else if (res >= 4 && res <= 6)
        resTxt = tr("адекватний");
    else
        resTxt = tr("завищенний");
    QMessageBox::information(this, tr("Результат"),tr("Рівень самооцінки: %1").arg(resTxt), QMessageBox::Ok);

    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id4 = %1,f_res4 = %2,f_legend4 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id4,f_res4,f_legend4) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test4Dialog::calcRes(int& res,int* q)
{
    int r1 = 0,
        r2 = 0;

    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < MAX1; i++)
    {
        q[i] = 0;

        if (!rb[i][0]->isChecked() && !rb[i][1]->isChecked() && !rb[i][2]->isChecked())
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }

        if (rb[i][0]->isChecked())
        {
            q[i] = 1;
            if ((i + 1)%2 == 0)
                r1++;
            else
                r2++;
        }
        else if (rb[i][1]->isChecked())
            q[i] = 2;
        else if (rb[i][2]->isChecked())
            q[i] = 3;
    }
    res = r2 - r1;
    return true;
}

QString Test4Dialog::getLegend(void)
{
    QString resTxt;

    if (res <= -4)
        resTxt = tr("низький");
    else if (res >= -3 && res <= 3)
        resTxt = tr("занижений");
    else if (res >= 4 && res <= 6)
        resTxt = tr("адекватний");
    else
        resTxt = tr("завищенний");
    return resTxt;
}

int Test4Dialog::getResults(void)
{
    return res;
}

void Test4Dialog::setupTest(void)
{
    QVBoxLayout *v = new QVBoxLayout();
    QHBoxLayout *hbox,
                *h;
    QLabel* l;
    QGroupBox *groupBox;
    QString qLegend[] = { "+", "-", "?" },
            qTitle[] = {
        "1. Звичайно я розраховую на успіх у своїх справах.",
        "2. Більшу частину часу я знаходжусь у пригніченому стані.",
        "3. Більшість ровесників зі мною радяться.",
        "4. У мене не має не впевненості в собі.",
        "5. Я настільки ж здібний і винахідливий як більшість оточуючих мене людей.",
        "6. Часом я почуваю себе нікому не потрібним",
        "7. Я все роблю добре (будь-яку справу).",
        "8. Мені здається, що я нічого не досягну в майбутньому.",
        "9. У будь-якій справі я вважаю себе правим.",
        "10. Я роблю багато такого, про що потім жалкую.",
        "11. Коли я дізнаюсь про успіхи кого-небудь, кого я знаю, то сприймаю це як власну поразку",
        "12. Мені здається, що всі навколо дивляться на мене з осудом.",
        "13. Мене мало турбують можливі невдачі.",
        "14. Мені здається, що успішному виконанню доручень чи справ мені заважають перешкоди, яких мені не подолати.",
        "15. Я рідко жалкую про те, що вже зробив.",
        "16. Оточуючі мене люди більш привабливі, ніж я.",
        "17. Я думаю, що я постійно комусь необхідний.",
        "18. Мені здається,що я навчаюся гірше за інших.",
        "19. Мені частіше щастить, ніж не щастить.",
        "20. У житті я завжди чогось боюся."
    };

    ui->scrollArea->setWidgetResizable(true);

    v->setContentsMargins(5,5,5,0);
    v->setSpacing(0);

    ui->scrollArea->widget()->setLayout(v);
    ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setAlignment(Qt::AlignTop);

    l = new QLabel(tr("Пропонуємо певну кількість тверджень, навпроти яких потрібно поставити один з трьох варіантів відповідей: «так» (+), «ні» (-), «не знаю» (?), \nякі відповідають Вашій власній поведінці в аналогічній ситуації."));
    l->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; }");
    v->addWidget(l);
    for (int i = 0; i < MAX1; i++)
    {
//        groupBox = new QGroupBox(QString("%1").arg(i + 1));
        groupBox = new QGroupBox();
        groupBox->setStyleSheet("QGroupBox { font-size: 12px; font-weight: bold; }");

//        groupBox->setMinimumHeight(100);
        groupBox->setMaximumWidth(1000);
        groupBox->setMinimumWidth(1000);

        h = new QHBoxLayout();
        l = new QLabel(qTitle[i]);
        l->setMaximumWidth(800);
        l->setMinimumWidth(800);

        h->addWidget(l);

        for (int j = 0; j < 3; j++)
        {
            rb[i][j] = new QRadioButton(QString("%1").arg(qLegend[j]));
            h->addWidget(rb[i][j]);
        }
        h->addStretch(1);
        groupBox->setLayout(h);


        hbox = new QHBoxLayout();
        hbox->addWidget(groupBox, 0, Qt::AlignLeft);
        hbox->setAlignment(Qt::AlignTop);
        v->addLayout(hbox);
    }
}
