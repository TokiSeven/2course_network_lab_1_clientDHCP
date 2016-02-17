#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T03:50:20
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dhcp_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dhcp_client.cpp \
    dhcp_packet.cpp

HEADERS  += mainwindow.h \
    dhcp_client.h \
    dhcp_packet.h

FORMS    += mainwindow.ui
