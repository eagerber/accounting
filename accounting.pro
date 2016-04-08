#-------------------------------------------------
#
# Project created by QtCreator 2016-02-11T10:41:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = accounting
TEMPLATE = app

QT += sql

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    db.cpp \
    db-settings-form.cpp \
    db-constdef.cpp \
    statisticsfrom.cpp \
    qcustomplot.cpp \
    sumoncategoriesplot.cpp \
    consumptionbymonth.cpp

HEADERS  += mainwindow.h \
    db.h \
    db-settings-form.h \
    db-constdef.h \
    statisticsfrom.h \
    qcustomplot.h \
    sumoncategoriesplot.h \
    consumptionbymonth.h

FORMS    += mainwindow.ui \
    dbsettingsform.ui \
    statisticsfrom.ui

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

#QMAKE_POST_LINK=$(COPY_DIR) $$PWD/db-scripts $$OUT_PWD/$$DESTDIR
