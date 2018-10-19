#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T02:29:57
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeminarskiGA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    disc.cpp \
    algorithmview.cpp \
    convexhullalgorithm.cpp \
    edge.cpp

HEADERS  += mainwindow.h \
    disc.h \
    supportline.h \
    algorithmview.h \
    convexhullalgorithm.h \
    edge.h

FORMS    += mainwindow.ui
