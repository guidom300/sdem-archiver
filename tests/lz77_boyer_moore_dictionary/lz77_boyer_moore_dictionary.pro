QT       += testlib

QT       -= gui

TARGET = lz77_boyer_moore_dictionary_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += lz77_boyer_moore_dictionary_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
