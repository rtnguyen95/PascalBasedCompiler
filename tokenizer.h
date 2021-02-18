#pragma once
#include <sstream>

class tokenizer {
private:
  std::string filename_;
  std::stringstream parser_;
public:
  ~tokenizer() { }
  tokenizer() {filename_ = " ";}
  tokenizer(std::string filename) {
    filename_ = filename;
  }
  std::string parse_input();
};
