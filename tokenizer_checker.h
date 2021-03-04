#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include "lexicalscanner.h"

//This class is used to check the results of the lexical analyzer against a file of the expected output. It is used to test that the lexical analyzer is working as expected.

class tokenizer_checker
{
private:
  std::ifstream expected_output_stream;
    
public:
  ~tokenizer_checker() { }
  tokenizer_checker()  { }
  tokenizer_checker(std::string filename) : expected_output_stream(filename, std::ios::in)
  {
    if (!expected_output_stream.is_open()) {
        std::cerr << "No such file " << filename;
    }
  }

  int check_output(list<Record> records);
};
