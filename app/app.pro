QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app

CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    decoder_progress_dialog.cpp \
    encoder_progress_dialog.cpp \
    main.cc \
    mainwindow.cpp \
    parallel.cc

HEADERS += \
    bit_reader.h \
    bit_writer.h \
    buffers.h \
    canonical_huffman_tree.h \
    decoder_progress_dialog.h \
    decoder_wrapper.h \
    encoder_progress_dialog.h \
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
    mainwindow.h \
    parallel.h \
    splitter.h \
    tree_node.h \
    utils.h \
    worker.h

FORMS += \
    decoder_progress_dialog.ui \
    encoder_progress_dialog.ui \
    mainwindow.ui

include(../common.pri)

CONFIG += release
