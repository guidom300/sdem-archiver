QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app

CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cc \
    mainwindow.cpp \
success.cpp

HEADERS += \
    bit_writer.h \
    canonical_huffman_tree.h \
    huffman_encoder.h \
    huffman_tree.h \
    huffman_tree_base.h \
    huffman_tree_node.h \
    lz77_boyer_moore_dictionary.h \
    lz77_encoder.h \
    lz77_naive_dictionary.h \
    tree_node.h \
    utils.h \
    mainwindow.h \
success.h

include(../common.pri)

FORMS += \
    mainwindow.ui \
    success.ui
