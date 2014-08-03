#-------------------------------------------------
#
# Project created by QtCreator 2014-07-22T11:06:36
#
#-------------------------------------------------

QT += core gui widgets concurrent

TARGET = DocumentADC

TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           image.cpp \
           imageslist.cpp

HEADERS += mainwindow.h \
           image.h \
           imageslist.h

OTHER_FILES += .gitignore \
               LICENSE \
               README.md

CONFIG += c++11
