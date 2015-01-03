#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include "lz77_encoder.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " input_file output_file [--naive]\n";
    return 1;
  }

  ifstream input_file(argv[1], ios::binary);
  ofstream output_file(argv[2], ios::binary | ios::trunc);

  input_file >> noskipws;

  if (argc != 4) {
    LZ77Encoder<12, 4> encoder;
    encoder(std::istreambuf_iterator<char>(input_file.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(output_file));
  } else {
    NaiveLZ77Encoder<12, 4> encoder;
    encoder(std::istreambuf_iterator<char>(input_file.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(output_file));
  }

  return 0;
}
