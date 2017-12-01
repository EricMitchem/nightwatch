#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T22:58:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ../src/main.cpp

HEADERS +=

CONFIG(debug, debug|release) {
    BUILD_DIR = ../build/qtcreator/debug
    DESTDIR   = ../bin/qtcreator/debug
    TARGET    = nightwatchd
}

CONFIG(release, debug|release) {
    BUILD_DIR = ../build/qtcreator/release
    DESTDIR   = ../bin/qtcreator/release
    TARGET    = nightwatch
}

CONFIG -= debug_and_release debug_and_release_target

MOC_DIR     = $$BUILD_DIR
OBJECTS_DIR = $$BUILD_DIR
RCC_DIR     = $$BUILD_DIR
UI_DIR      = $$BUILD_DIR

#RESOURCES += \
#    ../assets/nightwatch.qrc

#RC_FILE = ../assets/nightwatch.rc
