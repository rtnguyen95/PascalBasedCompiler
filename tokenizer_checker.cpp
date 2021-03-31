#include "tokenizer_checker.h"


/*
 check_output accepts a list of Record objects representing the lexemes and tokens found by the lexical analyzer. It compares records to a given file containing the expected output of the lexer.
 
 If differences are found then the line number and character of the difference is reported to the console.
 
 This feature allows for quick verification that changes to the code didn’t unintentionally modify the lexer. If there was a breaking change, then the state transition output file can be inspected to see which state transition was incorrect.
 */
int tokenizer_checker::check_output(vector<Record> records) {

  stringstream actual_output;

  actual_output << "TOKENS        Lexemes" << endl << endl;
  for (vector<Record>::iterator it = records.begin(); it != records.end(); ++it) {
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
