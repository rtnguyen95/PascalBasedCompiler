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

The output to the screen will be the tokens and productions as they are being processed followed by a represenation of the parse tree that shows a token, followed by the productions that were used.

```
-------Parse Tree Productions -----------
KEYWORD     = float
     <Program>
     <StatementList> -> <Statement> <MoreStatements>
     <Statement> -> <Assign> | <Declaration>
     <Declaration> -> <Type><ID>
     <Type> -> float | int | bool
IDENTIFIER  = fahr
     <ID> -> identifier
SEPARATOR   = ;
     <MoreStatements> -> ; <Statement> <MoreStatements> | epsilon
```

Several files are generated in the same folder as the test file that will be named: `<testfile>-<type>.txt` where type is one of the following.

1. states - All of the state transitions from the lexical scanner
2. symbols - A list of all the defined symbols (identifiers in the code) with type and name with the location in the file where they were defined.
3. code - A view of the parse tree that only shows the terminal nodes (leaves) that will look like the source code with comments removed and different spacing.
4. productions - The view of the parse tree that lists tokens and then the productions used to verify the syntax
5. parse tree - A view of the parse tree that will show indented nodes for children.  This shows terminal and non-terminal nodes.

# Demonstration of Features

## Assignments and Declaratives
This shows that the syntax analyzer can handle statements that involve declarations, assignment and arthmetic expressions including identifers and numbers.
```
./tokenizer tests/declarations.txt
```

## While Loop
This example has a while loop inside of a while loop.
```
./tokenizer tests/while.txt
```

## Error handling:
The syntax analyzer stops at the first error.

### Invalid Lexeme:
```
./tokenizer tests/lexicalerror.txt
```
Output includes: 
```
Lexical: tests/lexicalerror.txt:2:5 - _ is only allowed inside of an identifier: _invalid_name
```
### Undefined Symbols:
This will show what happens when an indentifier is used that was not previously defined:
```
./tokenizer tests/undefined.txt 
```
The output includes:
```
Syntactic:  tests/undefined.txt:5:4 - sum was not previously defined
```
Syntax Error:
In this example a while loop is missing the do keyword:
```
./tokenizer tests/syntaxerror.txt
```
In this case the error refers to `while` and a production is not satisfied, though it doesn't give the exact cause.
```
Syntactic:  tests/syntaxerror.txt:4:6 - this rule <TermPrime> -> *<Factor><TermPrime> | /<Factor><TermPrime> | epsilon could not be met with 'while'
```


The console output will be show the processing of the input file with tokens and 
rules as they are found to match.  After each statement it will print:
1. processing statement with success - if there are no errors in the statement
2. processing statement with failure - if there is an error or the end of the source file is reached.

Then it prints to the console the parse tree in three different ways
1. Prints the only the terminals (leaves), which will look like the source code
2. Prints a list of lexemes (in order they are encountered), followed by the productions used
3. Prints the tree starting with the root node, where children are indented under their parents
