QT       += core

QT       -= gui

TARGET = app
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cc

HEADERS += \
    bit_writer.h \
    lz77_naive_dictionary.h \
    utils.h \
    huffman_tree_node.h \
    huffman_tree_base.h \
    huffman_tree.h \
    huffman_encoder.h

include(../common.pri)
