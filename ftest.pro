#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T09:42:18
#
#-------------------------------------------------
#QMAKE_CXXFLAGS += -std=c++0x
#DEFINES += __GXX_EXPERIMENTAL_CXX0X__

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ftest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    test1dialog.cpp \
    test2dialog.cpp \
    getstudentdialog.cpp \
    studentdialog.cpp \
    resultdialog.cpp \
    tabledialog.cpp \
    exportdialog.cpp \
    test3dialog.cpp \
    test4dialog.cpp

HEADERS  += mainwindow.h \
    test1dialog.h \
    test2dialog.h \
    getstudentdialog.h \
    studentdialog.h \
    resultdialog.h \
    tabledialog.h \
    exportdialog.h \
    test3dialog.h \
    test4dialog.h

FORMS    += mainwindow.ui \
    test1dialog.ui \
    test2dialog.ui \
    getstudentdialog.ui \
    studentdialog.ui \
    resultdialog.ui \
    tabledialog.ui \
    exportdialog.ui \
    test3dialog.ui \
    test4dialog.ui

RESOURCES += \
    ftest.qrc

#DISTFILES += \
#    ftest.rc


win32 {

    RC_FILE += ftest.rc
}


