#include <QRadioButton>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QGroupBox>
#include <QSqlError>
#include "test1dialog.h"
#include "ui_test1dialog.h"

Test1Dialog::Test1Dialog(QString d,QString n,QString g,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test1Dialog)
{
    ui->setupUi(this);
    name = n;
    group = g;
    course = c;
    dt = d;
    idStudent = 0;
    setWindowTitle("Тест оцінки рівня сформованості когнітивного компонента - " + name);
    setupTest();
    loadData();

}

Test1Dialog::~Test1Dialog()
{
    delete ui;
}

void Test1Dialog::loadData(void)
{
    QSqlQuery query(QString("SELECT id FROM tbl_student WHERE f_name = '%1' AND f_group = '%2' AND f_class = %3").arg(name.toUpper()).arg(group.toUpper()).arg(course));
    int q;

    while (query.next())
        idStudent = query.value(0).toInt();
    if (idStudent)
    {
        // Такая анкета уже есть, загружаем ее
        query.exec(QString("SELECT * FROM tbl_test1 WHERE f_student = %1 AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            for (int i = 0; i < N; i++)
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

void Test1Dialog::accept(void)
{
    QSqlQuery query;
    QString resTxt,
            sql;
    int id = 0,
        q[N];
    bool isFind = false;

    if (!calcRes(res, q))
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
    query.exec(QString("SELECT id FROM tbl_test1 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
    {
        id = query.value(0).toInt();
        isFind = true;
    }
    if (isFind)
        sql = QString("UPDATE tbl_test1 SET f_q1 = %1,f_q2 = %2,f_q3 = %3,f_q4 = %4,f_q5 = %5,f_q6 = %6,f_q7 = %7,f_q8 = %8,f_q9 = %9,f_q10 = %10,f_q11 = %11,f_q12 = %12,f_q13 = %13,f_q14 = %14,f_q15 = %15,f_q16 = %16,f_q17 = %17,f_q18 = %18,f_q19 = %19,f_q20 = %20,f_q21 = %21 WHERE f_student = %22 AND f_dt = '%23'").arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(q[18]).arg(q[19]).arg(q[20]).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_test1 (f_student,f_dt,f_q1,f_q2,f_q3,f_q4,f_q5,f_q6,f_q7,f_q8,f_q9,f_q10,f_q11,f_q12,f_q13,f_q14,f_q15,f_q16,f_q17,f_q18,f_q19,f_q20,f_q21) VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22,%23)").arg(idStudent).arg(dt).arg(q[0]).arg(q[1]).arg(q[2]).arg(q[3]).arg(q[4]).arg(q[5]).arg(q[6]).arg(q[7]).arg(q[8]).arg(q[9]).arg(q[10]).arg(q[11]).arg(q[12]).arg(q[13]).arg(q[14]).arg(q[15]).arg(q[16]).arg(q[17]).arg(q[18]).arg(q[19]).arg(q[20]);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        qDebug() << query.lastError();
        return;
    }
    // Определяем номер анкеты
    if (!id)
    {
        query.exec(QString("SELECT id FROM tbl_test1 WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
        while (query.next())
            id = query.value(0).toInt();
    }

    if (res < 10)
        resTxt = tr("недостатній");
    else if (res >= 10 && res <= 14)
        resTxt = tr("допустимий");
    else if (res >= 15 && res <= 19)
        resTxt = tr("достатній");
    else
        resTxt = tr("високий");
    QMessageBox::information(this, tr("Результат"),tr("Рівень сформованості теоретичних знань з анімаційної діяльності: %1").arg(resTxt), QMessageBox::Ok);

    // Сохраняем результаты
    isFind = false;
    query.exec(QString("SELECT * FROM tbl_results WHERE f_student = '%1' AND f_dt = '%2'").arg(idStudent).arg(dt));
    while (query.next())
        isFind = true;
    if (isFind)
        sql = QString("UPDATE tbl_results SET f_id1 = %1,f_res1 = %2,f_legend1 = '%3' WHERE f_student = '%4' AND f_dt = '%5'").arg(id).arg(res).arg(resTxt).arg(idStudent).arg(dt);
    else
        sql = QString("INSERT INTO tbl_results (f_student,f_dt,f_id1,f_res1,f_legend1) VALUES (%1,'%2',%3,%4,'%5')").arg(idStudent).arg(dt).arg(id).arg(res).arg(resTxt);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка запису бази даних!"), QMessageBox::Ok);
        qDebug() << query.lastError();
        return;
    }
    QDialog::accept();
}

bool Test1Dialog::calcRes(int& res, int* q)
{
    // 1 -  а; 2 – а; 3 – б; 4 – в; 5 – а; 6 – в; 7 – б; 8 – б; 9 – в; 10 – а; 11 – а; 12 – в; 13 – а; 14 – а; 15 – б; 16 – в; 17 – б; 18 – а; 19 – в; 20 – б; 21 – а.
    int t[N] = { 0, 0, 1, 2, 0, 2, 1, 1, 2, 0, 0, 2, 0, 0, 1, 2, 1, 0, 2, 1, 0 };


    res = 0;
    // Проверка на наличие ответов на все вопросы анкеты
    for (int i = 0; i < N; i++)
    {
        q[i] = 0;
        if (rb[i][0]->isChecked())
        {
            q[i] = 1;
            if (t[i] == 0)
                res++;
        }
        else if (rb[i][1]->isChecked())
        {
            q[i] = 2;
            if (t[i] == 1)
                res++;
        }
        else if (rb[i][2]->isChecked())
        {
            q[i] = 3;
            if (t[i] == 2)
                res++;
        }
        if (q[i] == 0)
        {
            QMessageBox::information(this, tr("Помилка"), tr("Не задана відповіднь на питання: %1!").arg(i + 1));
            return false;
        }

    }
    return true;
}

QString Test1Dialog::getLegend(void)
{
    QString resTxt;

    if (res < 10)
        resTxt = tr("недостатній");
    else if (res >= 10 && res <= 14)
        resTxt = tr("допустимий");
    else if (res >= 15 && res <= 19)
        resTxt = tr("достатній");
    else
        resTxt = tr("високий");

    return resTxt;
}

int Test1Dialog::getResults(void)
{
    return res;
}


void Test1Dialog::setupTest(void)
{
    QVBoxLayout *v = new QVBoxLayout(),
                *vbox;
    QHBoxLayout *hbox;
    QGroupBox *groupBox;
    QString qTitle[] = {
        "1. Яке з наведених визначень загального поняття “анімація” є правильним:",
        "2. Яке з наведених нижче визначень щодо поняття “аніматор” є правильним:",
        "3. Яке з наведених нижче визначень щодо поняття “туристична анімація” є правильним:",
        "4. Яке з наведених визначень щодо поняття “анімаційна програма” є правильним:",
        "5. Яке з наведених визначень щодо поняття “дозвілля” є правильним:",
        "6. Яке з наведених визначень щодо поняття “масові фізкультурно-спортивні заходи” є правильним:",
        "7. Яке визначення має вид дозвільної діяльності, спрямований на відновлення духовних та фізичних сил людини:",
        "8. Які ігри побудовані на динаміці:",
        "9. Який метод здійснення анімаційної програми реалізується через костюми, особливу мову спілкування, традиції, обряди, різноманітні життєві сюжети:",
        "10. Яке з наведених визначень щодо поняття “подієвий туризм” є правильним:",
        "11. Яке з наведених визначень щодо поняття “додаткові анімаційні послуги” є правильним:",
        "12. Оздоровча функція рекреаційної туристичної анімації спрямована на:",
        "13. Яке з наведених нижче визначень щодо поняття “конфлікт” є правильним:",
        "14. Предмет конфлікту – це:",
        "15. Конфліктологія –це дисципліна, що вивчає:",
        "16. Які заняття допомагають підготувати м’язи до більш важких вправ:",
        "17. Степ-аеробіка це:",
        "18. Комплекс спеціальних вправ на воді з супроводом ритмічної музики – це:",
        "19. Короткий виклад змісту п’єси, сюжет, за яким створюється вистава, театральна імпровізація, масові видовища – це:",
        "20. Процес відновлення психофізичного балансу людського організму− це:",
        "21. Спортивний напрям анімаційних програм− це:"

    },
    qAnswer[][3] = {
        {
            "різновид туристичної діяльності, здійснюваної на туристичному підприємстві , що залучає туристів до різноманітних заходів через участь у спеціально розроблених програмах дозвілля;",
            "комплекс послуг по розміщенню, перевезенню, харчуванню туристів; екскурсійні послуги, а також послуги гідів-перекладачів;",
            "пасивний відпочинок, вид туризму, орієнтований на спокійну, не напружену за фізичним навантаженням програму туру або туристичної мандрівки?"
        },
        {
            "особа, яка повинна бути освіченою з багатьох сфер та галузей знань, а також володіти навичками щодо організації та проведення більшості видів анімаційних програм;",
            "особа, яка здійснює діяльність з надання різноманітних туристичних послуг відповідно до вимог законів України;",
            "особа, яка здійснює послуги з організації та проведення різноманітних заходів дозвілля?"
        },
        {
            "комплекс туристичних послуг, необхідних для задоволення потреб туриста під час його подорожі;",
            "послуга, завдяки якій турист залучається до активних дій, заснована на особистих контактах аніматора з туристом, спільній участі аніматора й туриста у розвагах,\nпропонованих анімаційною програмою туристичного комплексу;",
            "діяльність з надання різноманітних туристичних послуг відповідно до вимог законів України?"
        },
        {
            "навчальні програми, які допомагають туристам отримати різні уміння та навички;",
            "план-сценарій проведення фізкультурно-оздоровчого заходу;",
            "об’єднаний загальною метою або ідеєю план проведення туристичних, спортивно-оздоровчих, культурно-масових, пізнавальних та аматорських занять під час дозвілля?"
        },
        {
            "позаробочий час, що залишається людині з вираховуванням часу на побутові справи;",
            "вечірня пора доби;",
            "відпустка?"
        },
        {
            "специфічна форма суперництва, кожний учасник якого за допомогою певних прийомів в рівних для всіх умовах прагне досягти найкращого результату, переграти і перемогти решту\nучасників;",
            "діяльність, спрямована на отримання спортивних розрядів;",
            "ігрова діяльність, учасники якої з урахуванням віку, статі, рівня фізичної підготовленості, мають однакові можливості в досягненні перемоги у вправах?"
        },
        {
            "туристична анімація;",
            "рекреаційна анімація;",
            "готельна анімація?"
        },
        {
            "рухливі, інтелектуальні;",
            "рухливі, спортивні;",
            "ділові, інтелектуальні?"
        },
        {
            "метод змагання;",
            "метод рівноправного духовного контакту;",
            "метод театралізації?"
        },
        {
            "вид туризму, орієнтований на відвідування місцевості в певний час, пов'язаний з якоюсь особливою подією;",
            "діяльність з формування, просування та реалізації анімаційних програм різного напряму, що забезпечують цікаві, розвивальні та духовно збагачувальні дозвільні програми туристичного\nвідпочинку;",
            "складна структура та різноманітні форми прояву туристичної діяльності?"
        },
        {
            "програми, призначені для “підтримки” основних туристичних послуг, обумовлені переїздами, затримками в дорозі, несприятливими погодними умовами;",
            "цільові туристичні поїздки заради анімаційної програми або безперервного анімаційного процесу;",
            "сукупність соціальних закладів, об’єктів та споруд для забезпечення ефективної анімаційної діяльності?"
        },
        {
            "можливість переходу від повсякденного стану до вільного, дозвільного;",
            "створення позитивних емоцій та стимулювання психічної стабільності;",
            "відновлення та розвиток фізичних сил людини."
        },
        {
            "багаторівневе, багатомірне та багатофункціональне соціально-психологічне явище;",
            "психологічно напружена ситуація;",
            "соціально й психологічно не стабільна ситуація."
        },
        {
            "те, через що виник конфлікт;",
            "те, що застосовували для протиправних дій під час конфлікту;",
            "учасники конфлікту."
        },
        {
            "причини конфліктів;",
            "закономірності виникнення, розвитку, вирішення та завершення конфліктів будь-якого рівня;",
            "складні соціально-психологічні відносини між людьми."
        },
        {
            "загально-організуючі вправи різного напряму;",
            "зміцнювальні вправи;",
            "загально-розвивальні вправи."
        },
        {
            "новий різновид аеробіки, який базується на рухах карате;",
            "оздоровча аеробіка з використанням спеціальних платформ;",
            "командна гра, що проводиться за загально прийнятими правилами."
        },
        {
            "аква-аеробіка;",
            "водне поло;",
            "синхронне плавання."
        },
        {
            "план; ",
            "сценарний план;",
            "сценарій."
        },
        {
            "анімація;",
            "рекреація;",
            "дозвілля."
        },
        {
            "спортивно-оздоровча анімація;",
            "туристично-оздоровча анімація;",
            "оздоровчо-пізнавальна анімація."
        },
    };

    ui->scrollArea->setWidgetResizable(true);

    v->setContentsMargins(5,5,5,0);
    v->setSpacing(0);

    ui->scrollArea->widget()->setLayout(v);
    ui->scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->scrollArea->setAlignment(Qt::AlignTop);

    for (int i = 0; i < N; i++)
    {
        groupBox = new QGroupBox(qTitle[i]);
        groupBox->setStyleSheet("QGroupBox { font-size: 12px; font-weight: bold; }");

//        groupBox->setMinimumHeight(100);
        groupBox->setMaximumWidth(1250);
        groupBox->setMinimumWidth(1250);

        vbox = new QVBoxLayout();
        for (int j = 0; j < 3; j++)
        {
            rb[i][j] = new QRadioButton((qAnswer[i][j]));
            vbox->addWidget(rb[i][j]);
        }
        vbox->addStretch(1);
        groupBox->setLayout(vbox);



        hbox = new QHBoxLayout();
        hbox->addWidget(groupBox, 0, Qt::AlignLeft);
        hbox->setAlignment(Qt::AlignTop);
        v->addLayout(hbox);
    }
}
