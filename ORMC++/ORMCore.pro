#-------------------------------------------------
#
# Project created by QtCreator 2011-08-30T14:28:00
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = ORMC-Core
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += include/

CONFIG += warn_on

SOURCES += \
    src/ocdbmanager.cpp \
    src/ocexceptionshelper.cpp \
    src/oclogger.cpp \
    src/ocmodel.cpp \
    src/ocobject.cpp \
    src/ocparameter.cpp

HEADERS += \
    include/ocapplication.h \
    include/ocapplicationmanager.h \
    include/ocdbheaders.h \
    include/ocdbmanager.h \
    include/ocdbobject.h \
    include/ocexception.h \
    include/ocexceptionshelper.h \
    include/ocfileexception.h \
    include/oclogger.h \
    include/ocmodel.h \
    include/ocobject.h \
    include/ocsqlexception.h \
    include/ocutils.h \
    include/ocparameter.h
    #INSTALLS += target


PLUGiNS = ./plugins
