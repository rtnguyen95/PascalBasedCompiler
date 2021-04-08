# Assignment 2 Extra Credit
Robert Nguyen
Sean Javiya
Allison Atienza
Eric Britten

Follow the instructions below on building and running tests

# Build
sh ./build.sh

# Run with a test file as the first argument
./tokenizer tests/declarations.txt

The console output will be show the processing of the input file with tokens and 
rules as they are found to match.  After each statement it will print:
1. processing statement with success - if there are no errors in the statement
2. processing statement with failure - if there is an error or the end of the source file is reached.

Then it prints to the console the parse tree in three different ways
1. Prints the only the terminals (leaves), which will look like the source code
2. Prints a list of lexemes (in order they are encountered), followed by the productions used
3. Prints the tree starting with the root node, where children are indented under their parents
