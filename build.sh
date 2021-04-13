#!/bin/sh

g++ -c -o tester.o tester.cpp -std=c++17 -g
g++ -c -o tokenizer.o tokenizer.cpp -std=c++17 -g
g++ -c -o tokenizer_checker.o tokenizer_checker.cpp -std=c++17 -g

g++ -c -o lexicalscanner.o lexicalscanner.cpp -std=c++17 -g
g++ -c -o syntaxanalyzer.o syntaxanalyzer.cpp -std=c++17 -g
g++ -c -o topdownsyntaxanalyzer.o topdownsyntaxanalyzer.cpp -std=c++17 -g
g++ -c -o symboltable.o symboltable.cpp -std=c++17 -g
g++ -o tokenizer tokenizer.o tester.o tokenizer_checker.o syntaxanalyzer.o lexicalscanner.o topdownsyntaxanalyzer.o symboltable.o -std=c++17 -g