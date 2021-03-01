# changes 2/28 (Sean)

parseinput() now takes a parameter parseinput(string) the string is the output destination

tester.cpp reads the input as a command line argument, but now adds "_output.txt" to the end of that.  if you gave  ./tester.out file.txt
it opens file.txt, runs, then the output is in file_output.txt

the else statement was removed in the beginning of our FSM loop (before the switch case)

'#' was added in char_to_col

# changes 2/28 (Eric)

* Allow any other character to be processed (besides #).  A new column was added to the end table.
* Ignore _output.txt files from this repo

# changes 3/1
* Make the switch/case modular
* create LexicalScanner.cpp which will contain all of the processing functions
* Add documentation comments to each processing function
* Rename some test files and expected output files
* Add expected output files to all tests
* create a not expected file to demonstrate the comparions feature
* rename "Tester" to "Tokenizer" in the console output
* Add other console output to list what files are being used or created
* Move the change log from README.md to this file.
* Delete unused token.h/c files