QT       += testlib

QT       -= gui

TARGET = huffman_tree_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += huffman_tree_test.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


include(../tests.pri)
