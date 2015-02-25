#include <iostream>
#include <cstdlib>
#include "parallel.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " input_file output_file\n";
    return 1;
  }

  int threads;
  if (argc >= 4) {
    threads = atoi(argv[3]);
  } else {
    threads = 8;
  }

  encode_in_parallel(argv[1], argv[2], threads, argc < 5);

  return 0;
}
