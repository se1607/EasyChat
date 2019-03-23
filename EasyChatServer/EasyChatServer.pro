TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

SOURCES += main.cpp \
    server.cpp \
    relationalbroker.cpp \
    userbroker.cpp \
    user.cpp \
    conversation.cpp \
    dynamic.cpp

HEADERS += \
    server.h \
    relationalbroker.h \
    userbroker.h \
    user.h \
    conversation.h \
    dynamic.h

LIBS += -L/usr/lib -lboost_system -lboost_thread -lpthread -lmysqlclient
