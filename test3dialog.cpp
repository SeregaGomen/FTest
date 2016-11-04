#include <QLabel>
#include <QSqlQuery>
#include <QGroupBox>
#include <QMessageBox>
#include "test3dialog.h"
#include "ui_test3dialog.h"

Test3Dialog::Test3Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test3Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Оцінка рівня сформованості мотиваційного компонента - " + name);
    setupTest();
    loadData();
}

Test3Dialog::~Test3Dialog()
{
    delete ui;
}

void Test3Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test3 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < MAX; i++)
            {
                q = query.value(3 + i).toInt();
                if (q >= 1 && q <= 7)
                    rb[i][q - 1]->setChecked(true);
            }
    }
}

void Test3Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int q[MAX],
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
    query.exec(QString("SELECT id FROM tbl_test3 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test3 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8,f_q9 = %9,f_q10 = %10,f_q11 = %11,f_q12 = %12,f_q13 = %13,f_q14 = %14,f_q15 = %15,f_q16 = %16 WHERE f_student = %17 AND f_dt = '%18'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test3 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8,f_q9,f_q10,f_q11,f_q12,f_q13,f_q14,f_q15,f_q16) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test3 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }

    if (res < 25)
        resTxt = tr("недопустимий");
    else if (res >= 25 && res <= 54)
        resTxt = tr("допустимий ");
    else if (res >= 55 && res <= 83)
        resTxt = tr("достатній  ");
    else
        resTxt = tr("високий ");
    QMessageBox::information(this, tr("Результат"),tr("Рівень сформованості мотиваційного компоненту: %1").arg(resTxt), QMessageBox::Ok);


    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id3 = %1,f_res3 = %2,f_legend3 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id3,f_res3,f_legend3) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        return;
    }

    QDialog::accept();
}

bool Test3Dialog::calcRes(int& res,int* q)
{
    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < MAX; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
        {
            q[i] = 1;
            res += 1;
        }
        else if (rb[i][1]->isChecked())
        {
            q[i] = 2;
            res += 2;
        }
        else if (rb[i][2]->isChecked())
        {
            q[i] = 3;
            res += 3;
        }
        else if (rb[i][3]->isChecked())
        {
            q[i] = 4;
            res += 4;
        }
        else if (rb[i][4]->isChecked())
        {
            q[i] = 5;
            res += 5;
        }
        else if (rb[i][5]->isChecked())
        {
            q[i] = 6;
            res += 6;
        }
        else if (rb[i][6]->isChecked())
        {
            q[i] = 7;
            res += 7;
        }
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }
    }
    return true;
}

QString Test3Dialog::getLegend(void)
{
    QString resTxt;

    if (res < 25)
        resTxt = tr("недопустимий");
    else if (res >= 25 && res <= 54)
        resTxt = tr("допустимий ");
    else if (res >= 55 && res <= 83)
        resTxt = tr("достатній  ");
    else
        resTxt = tr("високий ");

    return resTxt;
}

int Test3Dialog::getResults(void)
{
    return res;
}

void Test3Dialog::setupTest(void)
{
    QVBoxLayout *v = new QVBoxLayout();
    QHBoxLayout *hbox,
                *h;
    QLabel* l;
    QGroupBox *groupBox;
    QString qTitle[] = {
        "1. Стати висококваліфікованим фахівцем.",
        "2. Отримати диплом про вищу освіту.",
        "3. Успішно продовжувати навчання на наступних курсах.",
        "4. Успішно навчатися, здавати екзамени на оцінку «добре» та «відмінно».",
        "5. Постійно отримувати стипендію.",
        "6. Отримати глибокі знання з майбутньої професійної діяльності.",
        "7. Бути постійно готовим до наступних занять.",
        "8. Не «запускати» предмети навчального циклу.",
        "9. Не відставати від сокурсників.",
        "10. Забезпечити успішність майбутньої професійної діяльності.",
        "11. Виконувати педагогічні вимоги.",
        "12. Досягти поваги з боку викладачів.",
        "13. Бути прикладом для сокурсників.",
        "14. Досягти схвалення батьків та оточуючих.",
        "15. Уникнути покарання за погане навчання.",
        "16. Отримати інтелектуальне задоволення."
    };

    ui->scrollArea->setWidgetResizable(true);

    v->setContentsMargins(5,5,5,0);
    v->setSpacing(0);

    ui->scrollArea->widget()->setLayout(v);
    ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setAlignment(Qt::AlignTop);

    l = new QLabel(tr("Уважно прочитайте наведені в списку мотиви навчальної діяльності. Оцініть мотиви за значущістю для Вас за 7-бальною шкалою. \nПри цьому − 1бал – мінімальна значущість мотиву, 7 балів – максимальна. Оцініть усі наведені в списку мотиви, не пропустивши жодного!"));
    l->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; }");
    v->addWidget(l);
    for (int i = 0; i < MAX; i++)
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

        for (int j = 0; j < 7; j++)
        {
            rb[i][j] = new QRadioButton(QString("%1").arg(j + 1));
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
