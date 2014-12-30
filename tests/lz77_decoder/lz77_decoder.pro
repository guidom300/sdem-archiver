QT       += testlib

QT       -= gui

TARGET = lz77_decoder_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += lz77_decoder_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
