cmake_minimum_required(VERSION 3.10)

# set the project name
project(_323)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(tokenizer tokenizer.cpp tester.cpp tokenizer_checker.cpp syntaxanalyzer.cpp lexicalscanner.cpp topdownsyntaxanalyzer.cpp symboltable.cpp tabletopdownsyntaxanalyzer.cpp OperatorPrecedenceParser.cpp LRParser.cpp)