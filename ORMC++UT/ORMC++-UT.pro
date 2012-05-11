#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T23:48:23
#
#-------------------------------------------------

QT       += testlib sql

QT       -= gui

TARGET = tst_ormctest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../ORMC++/include/

LIBS += -L../ORMCore-build-desktop -lORMC-Core

SOURCES += tst_ormctest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
