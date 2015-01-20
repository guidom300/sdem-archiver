QT       += testlib

QT       -= gui

TARGET = lzss_encoder_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += lzss_encoder_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
