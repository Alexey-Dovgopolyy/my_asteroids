QT += core
QT -= gui

TARGET = asteroids
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    gamestate.cpp \
    aircraft.cpp \
    utility.cpp \
    application.cpp \
    world.cpp \
    playersinput.cpp



HEADERS += \
    gamestate.h \
    aircraft.h \
    utility.h \
    application.h \
    world.h \
    playersinput.h


LIBS += -LD:\SFML-2.4.0\lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += D:\SFML-2.4.0\include
DEPENDPATH += D:\SFML-2.4.0\include
