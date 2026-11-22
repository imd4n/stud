TEMPLATE = app
TARGET = PlaneSim
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    planeMain.cpp \
    plane.cpp

HEADERS += \
    plane.h

LIBS += -lgdi32
