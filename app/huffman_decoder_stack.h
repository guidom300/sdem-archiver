#ifndef HUFFMAN_DECODER_STACK_H
#define HUFFMAN_DECODER_STACK_H

#include <cmath>
#include <type_traits>
#include "canonical_huffman_tree.h"
#include "huffman_decoder.h"
#include "bit_reader.h"

template <typename T>
struct HuffmanDecoderStack {
  template <typename InputIterator, typename OutputIterator>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator) {
    CanonicalHuffmanTree<T> tree(begin, end);
    HuffmanDecoder<T> decoder(tree);
    decoder(begin, end, output_iterator, std::is_pointer<InputIterator>::value);
  }

  template <typename InputIterator>
  static size_t prepare_input_buffer(InputIterator& begin,
                                     InputIterator end,
                                     T* input_buffer,
                                     size_t chunk_size);
};

template <>
template <typename InputIterator>
size_t HuffmanDecoderStack<char>::prepare_input_buffer(InputIterator& begin,
                                                       InputIterator end,
                                                       char* input_buffer,
                                                       size_t chunk_size) {
  size_t current_chunk_size = 0;

  for (; current_chunk_size < 256 && begin != end;
       ++current_chunk_size, ++begin) {
    input_buffer[current_chunk_size] = *begin;
  }

  size_t bits_to_decode;
  BitReader<InputIterator> bit_reader(begin, end);
  bit_reader.read(bits_to_decode);

  *(reinterpret_cast<size_t*>(input_buffer + current_chunk_size)) =
      bits_to_decode;

  current_chunk_size += sizeof(bits_to_decode);

  size_t bytes_to_decode = std::ceil(bits_to_decode / 8.0);
  for (begin = bit_reader.next(); bytes_to_decode--;
       ++begin, ++current_chunk_size) {
    input_buffer[current_chunk_size] = *begin;
  }

  return current_chunk_size;
}

#endif /* HUFFMAN_DECODER_STACK_H */
