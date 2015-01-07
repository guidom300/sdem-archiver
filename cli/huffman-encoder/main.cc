#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
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

  HuffmanEncoderStack<char> encoder_stack;
  encoder_stack(input_file, output_file);

  return 0;
}
