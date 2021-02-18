#include <fstream>
#include <iostream>


#include "tokenizer.cpp"
#include "token.cpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Please provide an input file name. Exiting.\n";
    exit(1);
  }
  string input_file_stream = argv[1];
  tokenizer tester(input_file_stream);
  string were_we_successful = " ";
  were_we_successful = tester.parse_input();
  cout << were_we_successful << endl;
  token test1;
  token test2("howdy");
  test1 = test2;
  cout << test1 << endl;
  cout << test2 << endl;
  return 0;
}
