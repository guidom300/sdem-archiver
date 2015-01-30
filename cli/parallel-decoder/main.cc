#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include "splitter.h"
#include "lzss_decoder.h"
#include "decoder_wrapper.h"
#include "huffman_decoder_stack.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " input_file output_file\n";
    return 1;
  }

  ifstream input_file(argv[1], ios::binary);
  ofstream output_file(argv[2], ios::binary | ios::trunc);

  input_file >> noskipws;

  Splitter<HuffmanDecoderStack<char>, DecoderWrapper<LZSSDecoder<12, 4>>>
      splitter;

  splitter(std::istreambuf_iterator<char>(input_file.rdbuf()),
           std::istreambuf_iterator<char>(),
           std::ostreambuf_iterator<char>(output_file),
           8,
           32 * 1024);

  return 0;
}
