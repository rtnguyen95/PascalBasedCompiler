#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include "lexicalscanner.h"
#include "errorhandler.h"
//The tokenizer class is used to parse input
class tokenizer
{
private:
  std::string filename_; //holds the name of the input file
  std::stringstream parser_; //holds the contents of the input file that will be parsed
  std::string parser_type;
  ErrorHandler errorHandler;

public:
  ~tokenizer() { } //default destructor

  tokenizer() {filename_ = " ";} //default constructor - sets fileame to blank

//overloaded constructor - accepts a single argument string representing the input file name
  tokenizer(std::string filename, std::string parser) {
    filename_ = filename;
    parser_type = parser;
  }

  vector<Record> parse_input(string output_file_name);// paramter = output location
   //function for parsing the input that takes no arguments and returns a list of Record objects

   const ErrorHandler & getErrorHandler() {
     return errorHandler;
   }

protected:
  string get_output_file(const string & type);

  void output_string(const string & type, const string & output);
  
  template <class T>
  void output_string(const string & type, T & o);

  void output_string(const string & type, void (*fn)(ostream &));

};
