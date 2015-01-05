#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include "huffman_tree.h"
#include "huffman_encoder.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " input_file output_file\n";
    return 1;
  }

  ifstream input_file(argv[1], ios::binary);
  ofstream output_file(argv[2], ios::binary | ios::trunc);

  input_file >> noskipws;

  // Build a Huffman tree
  HuffmanTree<char> tree(std::istreambuf_iterator<char>(input_file.rdbuf()),
                         std::istreambuf_iterator<char>());

  // Create a canonical Huffman encoder
  HuffmanEncoder<char> encoder(tree);
  encoder.make_canonical();

  // Dump the canonical codebook
  encoder.dump_codebook(std::ostreambuf_iterator<char>(output_file));

  // Encode the input file
  input_file.seekg(0);
  encoder(std::istreambuf_iterator<char>(input_file.rdbuf()),
          std::istreambuf_iterator<char>(),
          std::ostreambuf_iterator<char>(output_file));

  return 0;
}
