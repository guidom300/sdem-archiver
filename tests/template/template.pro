TEMPLATE = app

QT += testlib

CONFIG += testcase
CONFIG -= app_bundle

HEADERS += test.h

SOURCES += test.cpp

unix: LIBS += -L$$OUT_PWD/../../lib/ -llib

INCLUDEPATH += $$PWD/../../lib
DEPENDPATH += $$PWD/../../lib

unix: PRE_TARGETDEPS += $$OUT_PWD/../../lib/liblib.a

include ($$PWD/../../common.pri)
