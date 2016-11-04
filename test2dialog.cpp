#include <QLabel>
#include <QSqlQuery>
#include <QGroupBox>
#include <QMessageBox>
#include "test2dialog.h"
#include "ui_test2dialog.h"

Test2Dialog::Test2Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test2Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Оцінка стану професійної підготовки майбутніх фахівців з туризму до анімаційної діяльності - " + name);
    setupTest();
    loadData();
}

Test2Dialog::~Test2Dialog()
{
    delete ui;
}

void Test2Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test2 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < M; i++)
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
                    case 4:
                        rb[i][3]->setChecked(true);
                        break;
                }
            }
    }
}

void Test2Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[M],
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
    query.exec(QString("SELECT id FROM tbl_test2 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test2 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8,f_q9 = %9,f_q10 = %10,f_q11 = %11,f_q12 = %12 WHERE f_student = %13 AND f_dt = '%14'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test2 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8,f_q9,f_q10,f_q11,f_q12) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test2 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }

    if (res < 4)
        resTxt = tr("недостатній");
    else if (res >= 4 && res <= 5)
        resTxt = tr("допустимий ");
    else if (res >= 6 && res <= 7)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");
    QMessageBox::information(this, tr("Результат"),tr("Рівень сформованості умінь: %1").arg(resTxt), QMessageBox::Ok);


    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id2 = %1,f_res2 = %2,f_legend2 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id2,f_res2,f_legend2) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test2Dialog::calcRes(int& res,int* q)
{
    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < M; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
        {
            q[i] = 1;
            res++;
        }
        else if (rb[i][1]->isChecked())
        {
            q[i] = 2;
            res++;
        }
        else if (rb[i][2]->isChecked())
        {
            q[i] = 3;
//            res += 2;
        }
        else if (rb[i][3]->isChecked())
        {
            q[i] = 4;
//            res += 3;
        }
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
    }
    return true;
}

QString Test2Dialog::getLegend(void)
{
    QString resTxt;

    if (res < 4)
        resTxt = tr("недостатній");
    else if (res >= 4 && res <= 5)
        resTxt = tr("допустимий ");
    else if (res >= 6 && res <= 7)
        resTxt = tr("достатній");
    else
        resTxt = tr("професійний");

    return resTxt;
}

int Test2Dialog::getResults(void)
{
    return res;
}

void Test2Dialog::setupTest(void)
{
    QVBoxLayout *v = new QVBoxLayout();
    QHBoxLayout *hbox,
                *h;
    QLabel* l;
    QGroupBox *groupBox;
    QString qTitle[] = {
        "1. Уміння організовувати і проводити змагання за різними видами спортивних та рухливих ігор.",
        "2. Уміння вести у ролі ведучого спортивно-масові заходи.",
        "3. Уміння організовувати і проводити фізкльтурно-оздоровчі свята та фестивалі у літніх дитячо-оздоровчих таборах.",
        "4. Уміння працювати в команді.",
        "5. Уміння організовувати спортивно-оздоровчу туристичну діяльність.",
        "6. Уміння використовувати під час професійної діяльності новітні досягнення, сучасні засоби та обладнання.",
        "7. Уміння організовувати спортивно-оздоровчу діяльність з позиції сучасних досягнень психолого-педагогічної науки \nта практики, розробляти індивідуальні та групові програми для різних груп населення.",
        "8. Уміння розробляти, організовувати та проводити видовищно-розважальні заходи, використовувати на практиці знання \nз рекреації, мистецтвознавства, культурології.",
        "9. Уміння визначати загальні, певні цілі та завдання щодо екскурсійної діяльності, проводити методико-екскурсійну \nроботу, конструювати цикли музейно-екскурсійного обслуговування, складати програми туристичної \nекскурсійної діяльності.",
        "10. Уміння проводити заняття з різних видів аеробіки.",
        "11. Уміння вільно спілкуватися іноземною мовою.",
        "12. Уміння працювати з різними віковими групами туристів.",
    };

    ui->scrollArea->setWidgetResizable(true);

    v->setContentsMargins(5,5,5,0);
    v->setSpacing(0);

    ui->scrollArea->widget()->setLayout(v);
    ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setAlignment(Qt::AlignTop);

    l = new QLabel(tr("Оцініть свої уміння в балах від 2 до 5, де 5 – вищий бал, 2 – низький бал."));
    l->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; }");
    v->addWidget(l);
    for (int i = 0; i < M; i++)
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

        for (int j = 0; j < 4; j++)
        {
            rb[i][j] = new QRadioButton(QString("%1").arg(5 - j));
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
