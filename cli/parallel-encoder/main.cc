#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include "splitter.h"
#include "worker.h"
#include "lzss_encoder.h"
#include "encoder_wrapper.h"
#include "huffman_encoder_stack.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " input_file output_file\n";
    return 1;
  }

  ifstream input_file(argv[1], ios::binary);
  ofstream output_file(argv[2], ios::binary | ios::trunc);

  input_file >> noskipws;

  Splitter<Worker<EncoderWrapper<LZSSEncoder<12, 4>>>,
           HuffmanEncoderStack<char>> splitter;

  splitter(std::istreambuf_iterator<char>(input_file.rdbuf()),
           std::istreambuf_iterator<char>(),
           std::ostreambuf_iterator<char>(output_file),
           8,
           32 * 1024);

  return 0;
}
