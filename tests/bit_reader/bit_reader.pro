QT       += testlib

QT       -= gui

TARGET = bit_reader_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += bit_reader_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
