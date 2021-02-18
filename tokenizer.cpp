#include "tokenizer.h"

std::string tokenizer::parse_input() {
  std::ifstream input_file_stream(filename_, std::ios::in);
  if (!input_file_stream.is_open()) {
    std::cerr << "No such file.\n";
    return "NaN";
  }
  parser_ << input_file_stream.rdbuf();
  return parser_.str();
}
