#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include "lexicalscanner.h"

//class to test FSM
//compares the tokens and lexemes produced by the FSM to the expected results. 
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
