QT += core gui widgets

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app
TARGET = YutNori

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    yutboard.cpp

HEADERS += \
    mainwindow.h \
    yutboard.h \
    defvar.h \
    initialize.h \
    toss.h \
    select.h \
    move.h \
    movemal.h \
    moveresult.h \
    rollyut.h \
    turncontrol.h

FORMS += \
    mainwindow.ui

RESOURCES += 

CONFIG += c++11

