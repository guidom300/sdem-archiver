#ifndef PARALLEL_H
#define PARALLEL_H

#include <fstream>
#include <ios>
#include <iterator>
#include "splitter.h"
#include "worker.h"
#include "lzss_encoder.h"
#include "encoder_wrapper.h"
#include "huffman_encoder_stack.h"
#include "lzss_decoder.h"
#include "decoder_wrapper.h"
#include "huffman_decoder_stack.h"

template <typename SplitterType>
static void process_in_parallel(const char* input,
                                const char* output,
                                size_t threads,
                                SplitterType& splitter) {
  std::ifstream input_file(input, std::ios::binary);
  std::ofstream output_file(output, std::ios::binary | std::ios::trunc);

  input_file >> std::noskipws;

  splitter(std::istreambuf_iterator<char>(input_file.rdbuf()),
           std::istreambuf_iterator<char>(),
           std::ostreambuf_iterator<char>(output_file),
           threads,
           64 * 1000);

  input_file.close();
  output_file.close();
}

void encode_in_parallel(const char* input, const char* output, size_t threads) {
  Splitter<Worker<EncoderWrapper<LZSSEncoder<12, 4>>>,
           HuffmanEncoderStack<char>> splitter;

  process_in_parallel(input, output, threads, splitter);
}

void decode_in_parallel(const char* input, const char* output, size_t threads) {
  Splitter<HuffmanDecoderStack<char>, DecoderWrapper<LZSSDecoder<12, 4>>>
      splitter;

  process_in_parallel(input, output, threads, splitter);
}

#endif /* PARALLEL_H */
