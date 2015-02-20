QT       += core

QT       -= gui

TARGET = app
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cc

HEADERS += \
    bit_reader.h \
    bit_writer.h \
    buffers.h \
    canonical_huffman_tree.h \
    decoder_wrapper.h \
    encoder_wrapper.h \
    huffman_decoder.h \
    huffman_decoder_stack.h \
    huffman_encoder.h \
    huffman_encoder_stack.h \
    huffman_tree_base.h \
    huffman_tree.h \
    huffman_tree_node.h \
    lz77_boyer_moore_dictionary.h \
    lz77_data.h \
    lz77_decoder.h \
    lz77_encoder.h \
    lz77_naive_dictionary.h \
    lzss_decoder.h \
    lzss_encoder.h \
    parallel.h \
    splitter.h \
    tree_node.h \
    utils.h \
    worker.h

FORMS += \

include(../common.pri)

CONFIG += release
