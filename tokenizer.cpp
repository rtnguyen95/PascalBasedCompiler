#include "tokenizer.h"
#include "lexicalscanner.h"
#include <filesystem>

list<Record> tokenizer::parse_input() {
  std::ifstream input_file_stream(filename_, std::ios::in);
  if (!input_file_stream.is_open()) {
    std::cerr << "No such file.\n";
    return list<Record>();
  }
  parser_ << input_file_stream.rdbuf();
  // TODO: here is where we need to parse this stringstream "parser"
  // states:
  // scanner must ignore whitespace except for end of token.  transition from
  // our starting state back to what the starting state was when we last had whitespace
  //
  // cannot defer a decision until the end of the string. (end of source code)
  //   looking for a multiplicity of possible tokens.  One accepting state for each token
  // ^ so that each state tells us what token was found
  //
  // fourth, some strings are identifiers while others are keywords. we can have either
  // =seperate state for each keywrod
  // =accept the keyword as an identifier and make a final decision based on whether that
  //   string is in a list of keywords
  //
  //- a problem from the book - rate*time we need to realize * ends rate but also re-read *
  //   as multiply

  LexicalScanner scanner(parser_);
  ostringstream output;

  Record record = {"", "", true, ""};
  list<Record> records;
  output << "TOKENS        Lexemes" << endl << endl;
  while (!scanner.isFinished() && record.accepted) {
          record = scanner.lexer();
          if (record.lexeme.length()) {
            cout << record << endl;  
            records.push_back(record);
          }
  }

  stringstream states;
  states << endl << " State Transitions: " << endl;
  for (list<State>::iterator s = scanner.stateTransitions.begin(); s != scanner.stateTransitions.end(); ++s) {
      states << s->toString();

      if (s->next_input == 0)
        states << endl;
  }
  ofstream states_file(std::filesystem::path(filename_).stem().string() + "-states.txt");
  states_file << states.str();

  cout << endl;

  return records;
}
