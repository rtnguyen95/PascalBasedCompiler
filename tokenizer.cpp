#include "tokenizer.h"
#include "lexicalscanner.h"
#include <filesystem>

//function definition for parsing the input that takes no arguments and returns a list of Record objects
list<Record> tokenizer::parse_input()
{
  std::ifstream input_file_stream(filename_, std::ios::in); //declare an ifstream object for reading the input file
    
    //if the input file does not exist, display an error message to the user and return the empty list
  if (!input_file_stream.is_open())
  {
    std::cerr << "No such file.\n";
    return list<Record>();
  }
    
  parser_ << input_file_stream.rdbuf(); //write the contents of the input file stream to the stringstream parser
    
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

  LexicalScanner scanner(parser_);  //pass the input file stream to the lexical scanner
  ostringstream output;             //create a output string stream for the lexemes and tokens
  ofstream result_code;
  result_code.open ("output.txt");
  result_code <<  "TOKENS        Lexemes" << endl << endl;   //write the header to the output file
  //create a record object and initialize token to blank,  lexeme to blank, final state/acceptance to true, and the error message to blank. 
  //this variable is used to temporarily hold the data of the current lexeme being processed  
  Record record = {"", "", true, ""}; 
  list<Record> records;
  output << "TOKENS        Lexemes" << endl << endl;
    
  //loop that iterates until we reach the end of the file or we come across an invalid token that cannot reach a final state at the end of processing
  while (!scanner.isFinished() && record.accepted) {
          record = scanner.lexer();       //scan the next string in the input with the lexer and store the result in record
          if (record.lexeme.length())     //?
          {   
            cout << record << endl;  //output the record of the lexeme scanned to the console
            result_code << record << endl; //add the record of the lexeme scanned to the end of recordsList
            records.push_back(record);
          }
  }
  result_code.close();
  stringstream states;  //create an output string stream for the state transitions for each lexeme processed
  states << endl << " State Transitions: " << endl;
    //write all the state transitions processed by the lexer to states
  for (list<State>::iterator s = scanner.stateTransitions.begin(); s != scanner.stateTransitions.end(); ++s) {
      states << s->toString();

      if (s->next_input == 0) //include a linebreak between the state transitions of new lexemes
        states << endl;
    }
    
    //create an output file called state.txt and write the contents of states to it
  ofstream states_file(std::filesystem::path(filename_).stem().string() + "-states.txt");
  states_file << states.str();

  cout << endl;

  return recordsList; //return the token and lexeme list to the caller 
}
