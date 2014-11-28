QT       += testlib

QT       -= gui

TARGET = canonical_huffman_tree_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += canonical_huffman_tree_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
