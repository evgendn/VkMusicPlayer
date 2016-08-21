#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T21:45:18
#
#-------------------------------------------------

QT       += core gui webenginewidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VkMusicPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vkauth.cpp \
    vkaudio.cpp

HEADERS  += mainwindow.h \
    vkauth.h \
    vkaudio.h

FORMS    += mainwindow.ui
