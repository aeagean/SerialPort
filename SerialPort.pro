#**********************************************************
#Author: Qt君
#微信公众号: Qt君
#QQ群: 732271126
#LICENSE: MIT
#**********************************************************
QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SerialPort
TEMPLATE = app

win32: RC_ICONS += app.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
