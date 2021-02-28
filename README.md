# Assignment 1 Extra Credit
Robert Nguyen
Sean Javiya
Allison Atienza
Eric Britten

Follow the instructions below on building and running tests

# Build
sh ./build.sh

# Run with a test file as the first argument
./tester.out tests/sample_input.txt

# For debugging, run with two tests and compare to expected output
sh ./test.sh

# changes 2/28

parseinput() now takes a parameter parseinput(string) the string is the output destination

tester.cpp reads the input as a command line argument, but now adds "_output.txt" to the end of that.  if you gave  ./tester.out file.txt
it opens file.txt, runs, then the output is in file_output.txt

the else statement was removed in the beginning of our FSM loop (before the switch case)

'#' was added in char_to_col
