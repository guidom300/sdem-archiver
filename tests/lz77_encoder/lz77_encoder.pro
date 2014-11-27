QT       += testlib

QT       -= gui

TARGET = lz77_encoder_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += lz77_encoder_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
