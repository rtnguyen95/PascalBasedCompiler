#!/bin/sh

g++ -c -o tester.o tester.cpp -std=c++17 -g
g++ -c -o token.o token.cpp -std=c++17 -g
g++ -c -o tokenizer.o tokenizer.cpp -std=c++17 -g
#g++ -c -o lexicalscanner.o identifiers.cpp -std=c++17
g++ -o tester.out token.o tokenizer.o tester.o  -std=c++17 -g