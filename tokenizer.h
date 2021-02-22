#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include "lexicalscanner.h"

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
  list<Record> parse_input();
};
