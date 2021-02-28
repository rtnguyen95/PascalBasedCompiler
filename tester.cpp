#include <fstream>
#include <iostream>

#include "lexicalscanner.h"
#include "tokenizer.h"
#include "token.h"
#include "tokenizer_checker.h"

using namespace std;

int main(int argc, char* argv[]) {

  cout << "Tester" << endl << endl;
  if (argc < 2) {
    cerr << "Please provide an input file name and output test file. Exiting.\n";
    exit(1);
  }
  string input_file_stream = argv[1];
  tokenizer tester(input_file_stream);
  string dot = ".";
  string file_handle = input_file_stream.substr(0, input_file_stream.find(dot));
  string extension = "_output.txt";
  file_handle = file_handle + extension;
  list<Record> records = tester.parse_input(file_handle);
  // output locatin is passed to parse_input
  if (argc > 2) {
    string expected_output_file = argv[2];

    tokenizer_checker checker(expected_output_file);

    int compare = checker.check_output(records);
    //compare to the output test file
    if (compare)
      cout << "The output of the lexer matches the expected output" << endl << endl;
    else cout << "The output of the lexer does not match the expected output" << endl << endl;
  }
  return 0;
}
