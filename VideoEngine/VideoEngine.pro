
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
    Request-Video-Capture-Buffer.cpp \
    Temporary-Video-Capture-Buffer.cpp \
    V4l2Buffer.cpp \
    Video-Buffer.cpp \
    Video-Device-Handler.cpp

HEADERS  += \
    Device-Buffers-Control.h \
    MainWindow.h \
    ImageWidget.h \
    CameraCaptureThread.h \
    Request-Video-Capture-Buffer.h \
    Temporary-Video-Capture-Buffer.h \
    V4l2Buffer.h \
    Video-Buffer.h \
    Video-Device-Handler.h

FORMS    += \
    MainWindow.ui

INCLUDEPATH += /home/janusz/Downloads/v4l-utils-bin/include
LIBS += -L"/tmp/v4l-utils-bin/lib/"

unix:!macx: LIBS += -L$$PWD/../EquinoxLogger/Build/ -lEquinoxLogger

INCLUDEPATH += $$PWD/../EquinoxLogger
DEPENDPATH += $$PWD/../EquinoxLogger

unix:!macx: PRE_TARGETDEPS += $$PWD/../EquinoxLogger/Build/libEquinoxLogger.a
