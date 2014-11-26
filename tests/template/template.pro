QT       += testlib

QT       -= gui

TARGET = template_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += template_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
