#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T20:27:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Trabalho_TR2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        proxy.cpp \
        HTTP_Parser.cpp \
        TCP_Client.cpp \
        Spider.cpp \

HEADERS += \
        mainwindow.h \
        proxy.h \
        HTTP_Parser.h \
        TCP_Client.h \
        Spider.h

FORMS += \
        ui/mainwindow.ui
