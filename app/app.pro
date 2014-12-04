QT       += core

QT       -= gui

TARGET = app
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cc

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
    utils.h

include(../common.pri)
