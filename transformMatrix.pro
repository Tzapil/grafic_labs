#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T09:02:26
#
#-------------------------------------------------

QT       += core gui

TARGET = transformMatrix
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mywidget.cpp \
    myframe.cpp \
    atransform.cpp \
    affinetransform.cpp \
    perspectivetransform.cpp

HEADERS  += mainwindow.h \
    mywidget.h \
    myframe.h \
    atransform.h \
    affinetransform.h \
    perspectivetransform.h

QMAKE_CXXFLAGS += -std=gnu++0x
