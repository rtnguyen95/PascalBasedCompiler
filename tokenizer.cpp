#include "tokenizer.h"
#include "lexicalscanner.h"
#include "topdownsyntaxanalyzer.h"
#include "tabletopdownsyntaxanalyzer.h"
#include "OperatorPrecedenceParser.h"
#include "LRParser.h"
#include <filesystem>

//function definition for parsing the input
//takes a single parameter of a string of the name of the output file and returns a list of Record objects
vector<Record> tokenizer::parse_input(string output_file_name)
{
    std::ifstream input_file_stream(filename_, std::ios::in); //declare an ifstream object for reading the input file

    //if the input file does not exist, display an error message to the user and return the empty list
    if (!input_file_stream.is_open())
    {
        std::cerr << "No such file.\n";
        return vector<Record>();
    }

  parser_ << input_file_stream.rdbuf(); //write the contents of the input file stream to the stringstream parser

  LexicalScanner scanner(parser_, filename_, errorHandler);  //pass the input file stream to the lexical scanner

  //=====syntax analysis phase ======
  SymbolTable symbolTable;
  SyntaxAnalyzer * syntaxAnalyzer = nullptr;
  if (parser_type == "lr") {
      syntaxAnalyzer = new LRParser(scanner, symbolTable, errorHandler);
  } else if (parser_type == "op") {
      syntaxAnalyzer = new OperatorPrecedenceParser(scanner, symbolTable, errorHandler);
  } else if (parser_type == "table") {
      syntaxAnalyzer = new TableTopDownSyntaxAnalyzer(scanner, symbolTable, errorHandler);
  } else {
      syntaxAnalyzer = new TopDownSyntaxAnalyzer(scanner, symbolTable, errorHandler);
  }

  ParseTree* parseTree = syntaxAnalyzer->createParseTree();

  ostringstream code_output;           
  parseTree->printNodes(code_output, false);
  output_string("code", code_output.str());

  ostringstream production_output;
  parseTree->printRules(production_output);           
  output_string("productions", production_output.str());

  ostringstream tree_output;
  parseTree->printTree(tree_output);  
  output_string("parse-tree", tree_output.str());

  delete parseTree;

  ostringstream output;             //create a output string stream for the lexemes and tokens
  ofstream result_code;
  result_code.open (output_file_name);
  result_code <<  "TOKENS        Lexemes" << endl << endl;   //write the header to the output file
  //create a record object and initialize token to blank,  lexeme to blank, final state/acceptance to true, and the error message to blank.
  //this variable is used to temporarily hold the data of the current lexeme being processed
  Record record = {"", "", true, filename_, 1, 0, ""};
  vector<Record> records = syntaxAnalyzer->getTokenList();
  output << "TOKENS        Lexemes" << endl << endl;
  for (vector<Record>::iterator it = records.begin();
       it != records.end(); ++it) {
    result_code << *it << endl; //add the record of the lexeme scanned to the end of recordsList
  }

  // print the symbol table to a file
  ofstream symbol_file(get_output_file("symbols"));
  symbol_file << symbolTable;
  symbol_file.close();

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
  //ofstream states_file(get_output_file("states"));
  //states_file << states.str();
  //states_file.close();

  output_string("states", states.str());

  cout << endl;

  delete syntaxAnalyzer;
  return records; //return the token and lexeme list to the caller
}

string tokenizer::get_output_file(const string & type) {
  string type_filename;

  type_filename = std::filesystem::path(filename_).parent_path().string() + "/" +
                  std::filesystem::path(filename_).stem().string() + 
                  "-" + type + ".txt";
  return type_filename; 
}

void tokenizer::output_string(const string & type, const string & output) {
  ofstream file(get_output_file(type));
  file << output;
  file.close();
}

template <class T>
void tokenizer::output_string(const string & type, T & o) {
  ofstream file(get_output_file(type));
  file << o;
  file.close();
}

//template <class T>
void tokenizer::output_string(const string & type, void (*fn)(ostream &)) {
  ofstream file(get_output_file(type));
  (*fn)(file);
  file.close();
}
