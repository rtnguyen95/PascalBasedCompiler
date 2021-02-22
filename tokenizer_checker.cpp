#include "tokenizer_checker.h"

int tokenizer_checker::check_output(list<Record> records) {

  stringstream actual_output;

  actual_output << "TOKENS        Lexemes" << endl << endl;
  for (list<Record>::iterator it = records.begin(); it != records.end(); ++it) {
    if (it->lexeme.length()) {
        actual_output << *it << endl;
    }
  }

  if (!expected_output_stream.bad()) {
      int line = 1;
      actual_output.seekg(ios_base::beg);
      while(!expected_output_stream.eof()) {
          string expected_line;
          string actual_line;
          getline(expected_output_stream, expected_line);
          getline(actual_output, actual_line);

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
            return false;
          }
          line++;
      }
      return true;
  }
  return false;
}