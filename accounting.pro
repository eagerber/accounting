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

INCLUDEPATH += ../utils

Release:LIBS += -L../_build/release -ldatabase
Debug:LIBS   += -L../_build/debug -ldatabase

SOURCES += main.cpp\
    db-settings-form.cpp \
    qcustomplot.cpp \
    statistics-from.cpp \
    plots/consumption-by-month.cpp \
    plots/acc-plot.cpp \
    plots/sum-on-plot.cpp \
    queries.cpp \
    accounting-mainwindow.cpp

HEADERS  += \
    db-settings-form.h \
    qcustomplot.h \
    statistics-from.h \
    plots/consumption-by-month.h \
    plots/acc-plot.h \
    plots/sum-on-plot.h \
    queries.h \
    accounting-mainwindow.h

FORMS    += \
    dbsettingsform.ui \
    statisticsfrom.ui \
    accounting-mainwindow.ui

Release:DESTDIR = ../_build/release
Release:OBJECTS_DIR = ../_build/release/.obj
Release:MOC_DIR = ../_build/release/.moc
Release:RCC_DIR = ../_build/release/.rcc
Release:UI_DIR = ../_build/release/.ui

Debug:DESTDIR = ../_build/debug
Debug:OBJECTS_DIR = ../_build/debug/.obj
Debug:MOC_DIR = ../_build/debug/.moc
Debug:RCC_DIR = ../_build/debug/.rcc
Debug:UI_DIR = ../_build/debug/.ui

#QMAKE_POST_LINK=$(COPY_DIR) $$PWD/db-scripts $$OUT_PWD/$$DESTDIR
