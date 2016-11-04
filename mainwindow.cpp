#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QFile>
#include <QResource>
#include <QSqlError>
#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <math.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "getstudentdialog.h"
#include "test1dialog.h"
#include "test2dialog.h"
#include "test3dialog.h"
#include "test4dialog.h"
#include "studentdialog.h"
#include "resultdialog.h"
#include "tabledialog.h"
#include "exportdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();
    setupLanguage();
    openDB();
#ifndef Q_OS_LINUX
    setWindowIcon(QIcon(":/images/app.ico"));
//    #else
//        setWindowIcon(QIcon(":/images/main.png"));
#endif

    sdlg = new GetStudentDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings(void)
{
    QSettings settings("ZNU","FTEST");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    QString path = settings.value("path").toString();
    int states = settings.value("state").toInt();

    if ((Qt::WindowStates)states == Qt::WindowMaximized)
        this->setWindowState(Qt::WindowMaximized);
    else
    {
        move(pos);
        resize(size);
    }
    setWindowFilePath(path);
}

void MainWindow::writeSettings(void)
{
    QSettings settings("ZNU","FTEST");

    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("path",windowFilePath());
    settings.setValue("state",(int)windowState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    closeDB();
    writeSettings();
    event->accept();
}

bool MainWindow::checkDB(QString fileName)
{
    return QFile::exists(fileName);
}

bool MainWindow::createDB(QString fileName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    setCursor(Qt::WaitCursor);
    if (!db.open())
    {
        setCursor(Qt::ArrowCursor);
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        return false;
    }
    setCursor(Qt::ArrowCursor);

    QSqlQuery query;

    // Создание таблиц
    // Student
    if (!query.exec(QString("CREATE TABLE tbl_student (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, f_name TEXT, f_group TEXT, f_class INTEGER)")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        qDebug() << db.lastError();
        db.close();
        return false;
    }
    // Test1
    if (!query.exec(QString("CREATE TABLE tbl_test1 (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, f_student INTEGER NOT NULL, f_dt TEXT, f_q1 INTEGER, f_q2 INTEGER, f_q3 INTEGER, f_q4 INTEGER, f_q5 INTEGER, f_q6 INTEGER, f_q7 INTEGER, f_q8 INTEGER, f_q9 INTEGER, f_q10 INTEGER, f_q11 INTEGER, f_q12 INTEGER, f_q13 INTEGER, f_q14 INTEGER, f_q15 INTEGER, f_q16 INTEGER, f_q17 INTEGER, f_q18 INTEGER, f_q19 INTEGER, f_q20 INTEGER, f_q21 INTEGER)")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        qDebug() << db.lastError();
        db.close();
        return false;
    }
    // Test2
    if (!query.exec(QString("CREATE TABLE tbl_test2 (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, f_student INTEGER NOT NULL, f_dt TEXT, f_q1 INTEGER, f_q2 INTEGER, f_q3 INTEGER, f_q4 INTEGER, f_q5 INTEGER, f_q6 INTEGER, f_q7 INTEGER, f_q8 INTEGER, f_q9 INTEGER, f_q10 INTEGER, f_q11 INTEGER, f_q12 INTEGER)")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        qDebug() << db.lastError();
        db.close();
        return false;
    }
    // Test3
    if (!query.exec(QString("CREATE TABLE tbl_test3 (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, f_student INTEGER NOT NULL, f_dt TEXT, f_q1 INTEGER, f_q2 INTEGER, f_q3 INTEGER, f_q4 INTEGER, f_q5 INTEGER, f_q6 INTEGER, f_q7 INTEGER, f_q8 INTEGER, f_q9 INTEGER, f_q10 INTEGER, f_q11 INTEGER, f_q12 INTEGER, f_q13 INTEGER, f_q14 INTEGER, f_q15 INTEGER, f_q16 INTEGER)")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        qDebug() << db.lastError();
        db.close();
        return false;
    }
    // Test4
    if (!query.exec(QString("CREATE TABLE tbl_test4 (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, f_student INTEGER NOT NULL, f_dt TEXT, f_q1 INTEGER, f_q2 INTEGER, f_q3 INTEGER, f_q4 INTEGER, f_q5 INTEGER, f_q6 INTEGER, f_q7 INTEGER, f_q8 INTEGER, f_q9 INTEGER, f_q10 INTEGER, f_q11 INTEGER, f_q12 INTEGER, f_q13 INTEGER, f_q14 INTEGER, f_q15 INTEGER, f_q16 INTEGER, f_q17 INTEGER, f_q18 INTEGER, f_q19 INTEGER, f_q20 INTEGER)")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        qDebug() << db.lastError();
        db.close();
        return false;
    }
    // Results
    if (!query.exec(QString("CREATE TABLE tbl_results (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, f_student INTEGER NOT NULL, f_dt TEXT, f_id1 INTEGER, f_res1 INTEGER, f_legend1 TEXT, f_id2 INTEGER, f_res2 INTEGER, f_legend2 TEXT, f_id3 INTEGER, f_res3 INTEGER, f_legend3 TEXT, f_id4 INTEGER, f_res4 INTEGER, f_legend4 TEXT)")))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка створення бази даних!"), QMessageBox::Ok);
        qDebug() << db.lastError();
        db.close();
        return false;
    }

    db.close();
    return true;
}

void MainWindow::openDB(void)
{
    // Проверка наличия БД
    if (!checkDB("ftest.db"))
        if (!createDB("ftest.db"))
            return;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ftest.db");

    setCursor(Qt::WaitCursor);
    if (!db.open())
    {
        setCursor(Qt::ArrowCursor);
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка відкриття бази даних!"), QMessageBox::Ok);
        return;
    }
    setCursor(Qt::ArrowCursor);
}

void MainWindow::closeDB(void)
{
    db.close();
}

void MainWindow::setupLanguage(void)
{
    QString translatorFileName = QLatin1String("qt_") + QLocale::system().name();
    QTranslator* dlgTranslator = new QTranslator(qApp);

    QTextCodec* codec =  QTextCodec::codecForName("UTF-8");
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
#endif
    QTextCodec::setCodecForLocale(codec);

    // Локализация (стандартных диалогов, e.t.c, ...)
    if (dlgTranslator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        qApp->installTranslator(dlgTranslator);

}

void MainWindow::slotTest1(void)
{
   Test1Dialog *dlg;

   sdlg->initDialog();
   if (sdlg->exec() != QDialog::Accepted)
   {
       delete sdlg;
       return;
   }

   dlg = new Test1Dialog(sdlg->getDate(),sdlg->getStudentName(),sdlg->getStudentGroup(),sdlg->getStudentClass(),this);
   dlg->exec();
   delete dlg;
}

void MainWindow::slotTest2(void)
{
    Test2Dialog *dlg;

    sdlg->initDialog();
    if (sdlg->exec() != QDialog::Accepted)
    {
        delete sdlg;
        return;
    }

    dlg = new Test2Dialog(sdlg->getDate(),sdlg->getStudentName(),sdlg->getStudentGroup(),sdlg->getStudentClass(),this);
    dlg->exec();
    delete dlg;
}

void MainWindow::slotTest3(void)
{
    Test3Dialog *dlg;

    sdlg->initDialog();
    if (sdlg->exec() != QDialog::Accepted)
    {
        delete sdlg;
        return;
    }

    dlg = new Test3Dialog(sdlg->getDate(),sdlg->getStudentName(),sdlg->getStudentGroup(),sdlg->getStudentClass(),this);
    dlg->exec();
    delete dlg;
}

void MainWindow::slotTest4(void)
{
    Test4Dialog *dlg;

    sdlg->initDialog();
    if (sdlg->exec() != QDialog::Accepted)
    {
        delete sdlg;
        return;
    }

    dlg = new Test4Dialog(sdlg->getDate(),sdlg->getStudentName(),sdlg->getStudentGroup(),sdlg->getStudentClass(),this);
    dlg->exec();
    delete dlg;
}

void MainWindow::slotStudent(void)
{
    StudentDialog *dlg = new StudentDialog(this);

    if (dlg->exec() != QDialog::Accepted)
    {
        delete dlg;
        return;
    }
    calcStudent(dlg->getCurrentTest(),dlg->getBeginDate(),dlg->getCurrentDate(),dlg->getBeginGroups(),dlg->getCurrentGroups());
}

void MainWindow::calcStudent(int currentTest,QString beginDate,QString currentDate,QStringList beginGroups,QStringList currentGroups)
{
    ResultDialog* dlg;
    QSqlQuery beginQuery,
              currentQuery;
    QString beginPredicate,
            currentPredicate;
    int n1 = 0,
        n2 = 0,
        index = 0;
    float m1 = 0,
          m2 = 0,
          s1 = 0,
          sx1,
          sx2,
          t = 0,
          v = 0,
          s2 = 0;


    // Формирование предикатов для отбора выбранных групп
    for (int i = 0; i < beginGroups.size(); i++)
    {
        beginPredicate += ("'" + beginGroups.at(i) + "'");
        if (i < beginGroups.size() - 1)
            beginPredicate += ",";
    }
    for (int i = 0; i < currentGroups.size(); i++)
    {
        currentPredicate += ("'" + currentGroups.at(i) + "'");
        if (i < currentGroups.size() - 1)
            currentPredicate += ",";
    }


    if (!beginQuery.exec(QString("SELECT f_res%1 FROM tbl_results,tbl_student,tbl_test%1 WHERE tbl_test%1.id = tbl_results.f_id%1 AND tbl_student.id = tbl_results.f_student AND tbl_results.f_dt='%2' AND tbl_student.f_group IN (%3)").arg(currentTest + 1).arg(beginDate).arg(beginPredicate)))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
        qDebug() << beginQuery.lastError();
        return;
    }
    if (!currentQuery.exec(QString("SELECT f_res%1 FROM tbl_results,tbl_student,tbl_test%1 WHERE tbl_test%1.id = tbl_results.f_id%1 AND tbl_student.id = tbl_results.f_student AND tbl_results.f_dt='%2' AND tbl_student.f_group IN (%3)").arg(currentTest + 1).arg(currentDate).arg(currentPredicate)))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
        qDebug() << currentQuery.lastError();
        return;
    }
    while (beginQuery.next())
    {
        n1++;
        m1 += beginQuery.value(0).toFloat();
    }
    if (!n1)
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Вибрані групи %1 не тестувалися!").arg(beginDate), QMessageBox::Ok);
        return;
    }
    m1 /= float(n1);

    while (currentQuery.next())
    {
        n2++;
        m2 += currentQuery.value(0).toFloat();
    }
    if (!n2)
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Вибрані групи %1 не тестувалися!").arg(currentDate), QMessageBox::Ok);
        return;
    }
    m2 /= float(n2);

    if (!beginQuery.exec(QString("SELECT f_res%1 FROM tbl_results,tbl_student,tbl_test%1 WHERE tbl_test%1.id = tbl_results.f_id%1 AND tbl_student.id = tbl_results.f_student AND tbl_results.f_dt='%2' AND tbl_student.f_group IN (%3)").arg(currentTest + 1).arg(beginDate).arg(beginPredicate)))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
        qDebug() << beginQuery.lastError();
        return;
    }
    while (beginQuery.next())
        s1 += (m1 - beginQuery.value(0).toFloat())*(m1 - beginQuery.value(0).toFloat());
    s1 /= float(n1);
    s1 = sqrt(s1);
    sx1 = s1/sqrt(float(n1));

    if (!currentQuery.exec(QString("SELECT f_res%1 FROM tbl_results,tbl_student,tbl_test%1 WHERE tbl_test%1.id = tbl_results.f_id%1 AND tbl_student.id = tbl_results.f_student AND tbl_results.f_dt='%2' AND tbl_student.f_group IN (%3)").arg(currentTest + 1).arg(currentDate).arg(currentPredicate)))
    {
        QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
        qDebug() << currentQuery.lastError();
        return;
    }
    while (currentQuery.next())
        s2 += (m2 - currentQuery.value(0).toFloat())*(m2 - currentQuery.value(0).toFloat());
    s2 /= float(n2);
    s2 = sqrt(s2);
    sx2 = s2/sqrt(float(n2));

    if (n1 == n2 && s1 != s2)
    {
        t = fabs(m1 - m2)/sqrt(s1*s1 + s2*s2)*sqrt(float(n1));
        v = 2*n1 - 2;
    }
    if (n1 != n2 && s1 != s2)
    {
        t = fabs(m1 - m2)/sqrt(s1*s1/float(n1) + s2*s2/float(n2));
        v = n1 + n2 - 2;
    }
    if (n1 != n2 && s1 == s2)
    {
        t = fabs(m1 - m2)/s1/sqrt(1.0/float(n1) + 1.0/float(n2));
        v = n1 + n2 - 2;
    }

    if (v <= 1.5)
        index = 0;
    else if (v > 1.5 && v <= 2.5)
        index = 1;
    else if (v > 2.5 && v <= 3.5)
        index = 2;
    else if (v > 3.5 && v <= 4.5)
        index = 3;
    else if (v > 4.5 && v <= 5.5)
        index = 4;
    else if (v > 5.5 && v <= 6.5)
        index = 5;
    else if (v > 6.5 && v <= 7.5)
        index = 6;
    else if (v > 7.5 && v <= 8.5)
        index = 7;
    else if (v > 8.5 && v <= 9.5)
        index = 8;
    else if (v > 9.5 && v <= 15)
        index = 9;
    else if (v > 15 && v <= 25)
        index = 10;
    else if (v > 25 && v <= 45)
        index = 11;
    else if (v > 45)
        index = 12;

    dlg = new ResultDialog(currentTest,beginDate,currentDate,n1,n2,index,m1,m2,s1,s2,sx1,sx2,t,v);
    dlg->exec();
    //(new ResultForm("Обрані групи", wf.GetTestName(), dt1.ToString().Substring(0, 10), dt2.ToString().Substring(0, 10), n1.ToString(), n2.ToString(), string.Format("{0:F}", M1), string.Format("{0:F}", M2), string.Format("{0:F}", S1), string.Format("{0:F}", S2), string.Format("{0:F}", T), V.ToString(), string.Format("{0:F}", Sx1), string.Format("{0:F}", Sx2), TS, Index)).ShowDialog();
}

void MainWindow::slotTable(void)
{
    TableDialog* dlg = new TableDialog(this);

    dlg->show();
}

void MainWindow::slotExport(void)
{
    ExportDialog* dlg = new ExportDialog(this);
    QString fileName;

    if (dlg->exec() == QDialog::Accepted)
    {
        fileName = QFileDialog::getSaveFileName(this,tr("Вибір файлу для експорту"),windowFilePath(),tr("Excel XML (*.xml)"));
        if (!fileName.isEmpty())
        {
            if (!fileName.endsWith(".xml"))
                fileName += ".xml";
            export2XML(fileName,dlg->getMap());
        }
    }
    delete dlg;
}

// https://technet.microsoft.com/ru-ru/magazine/2006.01.blogtales(en-us).aspx
// http://habrahabr.ru/post/235973/
void MainWindow::export2XML(QString fileName,QMap<QString,bool> map)
{
    QSqlQuery query;
    QFile file(fileName);
    QXmlStreamWriter stream;
    QString title[] = {
                        "Когнитивний тест",
                        "Діяльнісний тест",
                        "Мотиваційний тест",
                        "Тест самооціки"
                      };

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(NULL, QObject::tr("Помилка"), QObject::tr("Помилка відкриття файлу %1").arg(fileName));
        return;
    }
    QApplication::setOverrideCursor(Qt::BusyCursor);
    stream.setDevice(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();


    stream.writeStartElement("ss:Workbook");
    stream.writeAttribute("xmlns:ss", "urn:schemas-microsoft-com:office:spreadsheet");
    stream.writeStartElement("ss:Styles");
        stream.writeStartElement("ss:Style");
            stream.writeAttribute("ss:ID","1");
            stream.writeStartElement("ss:Font");
                stream.writeAttribute("ss:Bold","1");
            stream.writeEndElement(); // Font

        stream.writeEndElement(); // Style
    stream.writeEndElement(); // Styles

    for (int i = 0; i < 4; i++)
    {
        if (map[QString("Test%1").arg(i + 1)] == false)
            continue;

        stream.writeStartElement("ss:Worksheet");
            stream.writeAttribute("ss:Name", title[i]);
                stream.writeStartElement("ss:Table");

                stream.writeStartElement("ss:Column");
                    stream.writeAttribute("ss:Width","200");
                stream.writeEndElement(); // Column
                stream.writeStartElement("ss:Column");
                    stream.writeAttribute("ss:Width","60");
                stream.writeEndElement(); // Column
                stream.writeStartElement("ss:Column");
                    stream.writeAttribute("ss:Width","60");
                stream.writeEndElement(); // Column
                stream.writeStartElement("ss:Column");
                    stream.writeAttribute("ss:Width","60");
                stream.writeEndElement(); // Column
                stream.writeStartElement("ss:Column");
                    stream.writeAttribute("ss:Width","60");
                stream.writeEndElement(); // Column
                stream.writeStartElement("ss:Column");
                    stream.writeAttribute("ss:Width","120");
                stream.writeEndElement(); // Column

                    // Заголовок
                    stream.writeStartElement("ss:Row");
                        stream.writeAttribute("ss:StyleID","1");
                        if (map["Name"] == true)
                        {
                            stream.writeStartElement("ss:Cell");
                                stream.writeStartElement("ss:Data");
                                    stream.writeAttribute("ss:Type","String");
                                    stream.writeCDATA("ПІБ");
                                stream.writeEndElement(); // Data
                            stream.writeEndElement(); // Cell
                        }
                        if (map["Group"] == true)
                        {
                            stream.writeStartElement("ss:Cell");
                                stream.writeStartElement("ss:Data");
                                    stream.writeAttribute("ss:Type","String");
                                    stream.writeCDATA("Група");
                                stream.writeEndElement(); // Data
                            stream.writeEndElement(); // Cell
                        }
                        if (map["Class"] == true)
                        {
                            stream.writeStartElement("ss:Cell");
                                stream.writeStartElement("ss:Data");
                                    stream.writeAttribute("ss:Type","String");
                                    stream.writeCDATA("Курс");
                                stream.writeEndElement(); // Data
                            stream.writeEndElement(); // Cell
                        }
                        if (map["Date"] == true)
                        {
                            stream.writeStartElement("ss:Cell");
                                stream.writeStartElement("ss:Data");
                                    stream.writeAttribute("ss:Type","String");
                                    stream.writeCDATA("Дата");
                                stream.writeEndElement(); // Data
                            stream.writeEndElement(); // Cell
                        }
                        if (map["Result"] == true)
                        {
                            stream.writeStartElement("ss:Cell");
                                stream.writeStartElement("ss:Data");
                                    stream.writeAttribute("ss:Type","String");
                                    stream.writeCDATA("Результат");
                                stream.writeEndElement(); // Data
                            stream.writeEndElement(); // Cell
                        }
                        if (map["Legend"] == true)
                        {
                            stream.writeStartElement("ss:Cell");
                                stream.writeStartElement("ss:Data");
                                    stream.writeAttribute("ss:Type","String");
                                    stream.writeCDATA("Легенда");
                                stream.writeEndElement(); // Data
                            stream.writeEndElement(); // Cell
                        }
                stream.writeEndElement(); // Row

                // Данные
                if (!query.exec(QString("SELECT f_name,f_group,f_class,f_dt,f_res%1,f_legend%1 FROM tbl_results,tbl_student WHERE tbl_student.id = tbl_results.f_student ").arg(i + 1)))
                {
                    QMessageBox::critical(this, tr("Помилка"),tr("Помилка доступу до бази даних!"), QMessageBox::Ok);
                    qDebug() << query.lastError();
                    file.close();
                    return;
                }
                while (query.next())
                {
                    stream.writeStartElement("ss:Row");
                    if (map["Name"] == true)
                    {
                        stream.writeStartElement("ss:Cell");
                            stream.writeStartElement("ss:Data");
                            stream.writeAttribute("ss:Type","String");
                                stream.writeCDATA(query.value(0).toString());
                            stream.writeEndElement(); // Data
                        stream.writeEndElement(); // Cell
                    }
                    if (map["Group"] == true)
                    {
                        stream.writeStartElement("ss:Cell");
                            stream.writeStartElement("ss:Data");
                            stream.writeAttribute("ss:Type","String");
                                stream.writeCDATA(query.value(1).toString());
                            stream.writeEndElement(); // Data
                        stream.writeEndElement(); // Cell
                    }
                    if (map["Class"] == true)
                    {
                        stream.writeStartElement("ss:Cell");
                            stream.writeStartElement("ss:Data");
                            stream.writeAttribute("ss:Type","String");
                                stream.writeCDATA(query.value(2).toString());
                            stream.writeEndElement(); // Data
                        stream.writeEndElement(); // Cell
                    }
                    if (map["Date"] == true)
                    {
                        stream.writeStartElement("ss:Cell");
                            stream.writeStartElement("ss:Data");
                            stream.writeAttribute("ss:Type","String");
                                stream.writeCDATA(query.value(3).toString());
                            stream.writeEndElement(); // Data
                        stream.writeEndElement(); // Cell
                    }
                    if (map["Result"] == true)
                    {
                        stream.writeStartElement("ss:Cell");
                            stream.writeStartElement("ss:Data");
                            stream.writeAttribute("ss:Type","String");
                                stream.writeCDATA(query.value(4).toString());
                            stream.writeEndElement(); // Data
                        stream.writeEndElement(); // Cell
                    }
                    if (map["Legend"] == true)
                    {
                        stream.writeStartElement("ss:Cell");
                            stream.writeStartElement("ss:Data");
                            stream.writeAttribute("ss:Type","String");
                                stream.writeCDATA(query.value(5).toString());
                            stream.writeEndElement(); // Data
                        stream.writeEndElement(); // Cell
                    }
                    stream.writeEndElement(); // Row
                }


            stream.writeEndElement(); // Table
        stream.writeEndElement(); // Worksheet
    }
    stream.writeEndElement(); // Workbook
    stream.writeEndDocument();
    file.close();
    QApplication::restoreOverrideCursor();
    if (file.error() != QFile::NoError)
    {
        QMessageBox::information(NULL, QObject::tr("Помилка"), QObject::tr("Помилка запису файлу %1").arg(fileName));
        return;
    }
}
