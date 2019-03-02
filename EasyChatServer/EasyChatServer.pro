TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h

LIBS += -L/usr/lib -lboost_system -lboost_thread -lpthread
