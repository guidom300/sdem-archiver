QT       += testlib

QT       -= gui

TARGET = lzss_decoder_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += lzss_decoder_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
