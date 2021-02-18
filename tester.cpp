#include <fstream>
#include <iostream>


#include "tokenizer.h"
#include "token.h"

using namespace std;

int main(int argc, char* argv[]) {

  cout << "Tester" << endl;
  if (argc < 3) {
    cerr << "Please provide an input file name and output test file. Exiting.\n";
    exit(1);
  }
  string input_file_stream = argv[1];
  string expected_output_file = argv[2];
  tokenizer tester(input_file_stream);
  string were_we_successful = " ";
  were_we_successful = tester.parse_input();
  cout << were_we_successful << endl;
  token test1;
  token test2("howdy");
  test1 = test2;
  cout << test1 << endl;
  cout << test2 << endl;

  // compare to the output test file
  ifstream expected_output_stream(expected_output_file, std::ios::in);
  if (!expected_output_stream.bad()) {
      stringstream buffer (were_we_successful);
      int line = 1;
      while(!expected_output_stream.eof()) {
          string expected_line;
          string actual_line;
          getline(expected_output_stream, expected_line);
          getline(buffer, actual_line);

          int different_char_pos = -1;
          for (int i = 0; i < expected_line.length(); ++i) {
              if (expected_line[i] != actual_line[i]) {
                  different_char_pos = i;
              }
          }

          if (different_char_pos != -1) {
            cout << "Difference found at line " << line << endl << " at position " << (different_char_pos + 1) << endl;
            cout << "Expected: " << expected_line << endl;
            cout << "Actual:   " << actual_line << endl; 
            return 1;
          }
          line++;
      }
  }
  cout << "The output of the lexer matches the expected output" << endl << endl;
  return 0;
}
