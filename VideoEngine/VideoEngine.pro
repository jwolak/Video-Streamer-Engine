
TARGET = VideoEngine
TEMPLATE = app
QT += core gui widgets
CONFIG += c++11

LIBS += -lv4lconvert

SOURCES += main.cpp \
    Device-Buffers-Control.cpp \
    MainWindow.cpp \
    ImageWidget.cpp \
    CameraCaptureThread.cpp \
    Temporary-Video-Capture-Buffer.cpp \
    Video-Device-Handler.cpp

HEADERS  += \
    Device-Buffers-Control.h \
    MainWindow.h \
    ImageWidget.h \
    CameraCaptureThread.h \
    Temporary-Video-Capture-Buffer.h \
    Video-Device-Handler.h

FORMS    += \
    MainWindow.ui

INCLUDEPATH += /home/janusz/Downloads/v4l-utils-bin/include
LIBS += -L"/tmp/v4l-utils-bin/lib/"

unix:!macx: LIBS += -L$$PWD/../EquinoxLogger/Build/ -lEquinoxLogger

INCLUDEPATH += $$PWD/../EquinoxLogger
DEPENDPATH += $$PWD/../EquinoxLogger

unix:!macx: PRE_TARGETDEPS += $$PWD/../EquinoxLogger/Build/libEquinoxLogger.a
