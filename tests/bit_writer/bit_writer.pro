QT       += testlib

QT       -= gui

TARGET = bit_writer_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += bit_writer_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
