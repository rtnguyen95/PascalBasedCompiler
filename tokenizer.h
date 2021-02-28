#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include "lexicalscanner.h"

//The tokenizer class is used to parse input
class tokenizer
{
private:
  std::string filename_; //holds the name of the input file
  std::stringstream parser_; //holds the contents of the input file that will be parsed

public:
  ~tokenizer() { } //default destructor

  tokenizer() {filename_ = " ";} //default constructor - sets fileame to blank

//overloaded constructor - accepts a single argument string representing the input file name
  tokenizer(std::string filename) {
    filename_ = filename;
  }

  list<Record> parse_input(string output_file_name);// paramter = output location
   //function for parsing the input that takes no arguments and returns a list of Record objects
};
