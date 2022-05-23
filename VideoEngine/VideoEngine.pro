#-------------------------------------------------
#
# Project created by QtCreator 2016-06-07T18:10:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoEngine.x86
TEMPLATE = app
CONFIG += c++11
LIBS += -lv4lconvert


SOURCES +=\
    source/Camera-Capture-Thread.cpp \
    source/Convert-Data.cpp \
    source/Copy-Buffer.cpp \
    source/Device-Buffers-Control.cpp \
    source/Image-Handler.cpp \
    source/ImageWidget.cpp \
    source/Request-Video-Capture-Buffer.cpp \
    source/Stream-Data-Format.cpp \
    source/Temporary-Video-Capture-Buffer.cpp \
    source/V4l2Buffer.cpp \
    source/Video-Buffer.cpp \
    source/Video-Device-Handler.cpp \
    source/camviewer.cpp \
    source/main.cpp \
    source/mainwindow.cpp

HEADERS  +=\
    headers/Camera-Capture-Thread.h \
    headers/Convert-Data.h \
    headers/Copy-Buffer.h \
    headers/Device-Buffers-Control.h \
    headers/Image-Handler.h \
    headers/ImageWidget.h \
    headers/Request-Video-Capture-Buffer.h \
    headers/Stream-Data-Format.h \
    headers/Temporary-Video-Capture-Buffer.h \
    headers/V4l2Buffer.h \
    headers/Video-Buffer.h \
    headers/Video-Device-Handler.h \
    headers/camviewer.h \
    headers/mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib

INCLUDEPATH += /home/janusz/Downloads/v4l-utils-bin/include
LIBS += -L"/tmp/v4l-utils-bin/lib/"


QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig


unix:!macx: LIBS += -L$$PWD/../EquinoxLogger/Build/ -lEquinoxLogger

INCLUDEPATH += $$PWD/../EquinoxLogger
DEPENDPATH += $$PWD/../EquinoxLogger

unix:!macx: PRE_TARGETDEPS += $$PWD/../EquinoxLogger/Build/libEquinoxLogger.a
