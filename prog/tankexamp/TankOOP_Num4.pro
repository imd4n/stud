TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    source.cpp

HEADERS += \
    header.h \
    interfaces.h
LIBS += -lgdi32
