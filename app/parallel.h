#ifndef PARALLEL_H
#define PARALLEL_H

#include <fstream>
#include <ios>
#include <iterator>

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

void encode_in_parallel(const char* input, const char* output, size_t threads);

void decode_in_parallel(const char* input, const char* output, size_t threads);

#endif /* PARALLEL_H */
