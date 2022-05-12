
TARGET = VideoEngine
TEMPLATE = app
QT += core gui widgets
CONFIG += c++11

LIBS += -lv4lconvert

SOURCES += main.cpp \
    MainWindow.cpp \
    ImageWidget.cpp \
    CameraCaptureThread.cpp

HEADERS  += \
    MainWindow.h \
    ImageWidget.h \
    CameraCaptureThread.h

FORMS    += \
    MainWindow.ui

INCLUDEPATH += /home/janusz/Downloads/v4l-utils-bin/include
LIBS += -L"/tmp/v4l-utils-bin/lib/"
