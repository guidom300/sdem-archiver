#include "parallel.h"
#include "splitter.h"
#include "worker.h"
#include "lzss_encoder.h"
#include "encoder_wrapper.h"
#include "huffman_encoder_stack.h"
#include "lzss_decoder.h"
#include "decoder_wrapper.h"
#include "huffman_decoder_stack.h"

void encode_in_parallel(const char* input,
                        const char* output,
                        size_t threads,
                        bool boyer_moore) {
  if (boyer_moore) {
    Splitter<Worker<EncoderWrapper<LZSSEncoder<12, 4>>>,
             HuffmanEncoderStack<char>> splitter;

    process_in_parallel(input, output, threads, splitter);
  } else {
    Splitter<Worker<EncoderWrapper<NaiveLZSSEncoder<12, 4>>>,
             HuffmanEncoderStack<char>> splitter;

    process_in_parallel(input, output, threads, splitter);
  }
}

void decode_in_parallel(const char* input, const char* output, size_t threads) {
  Splitter<HuffmanDecoderStack<char>, DecoderWrapper<LZSSDecoder<12, 4>>>
      splitter;

  process_in_parallel(input, output, threads, splitter);
}
