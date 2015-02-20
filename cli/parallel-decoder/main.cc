#include <iostream>
#include "parallel.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " input_file output_file\n";
    return 1;
  }

  decode_in_parallel(argv[1], argv[2], 4);

  return 0;
}
