#!/bin/sh

./tester.out tests/simple_test.txt tests/simple_output.txt
./tester.out tests/sample_input.txt tests/sample_output.txt
./tester.out tests/complicated_input.txt
./tester.out tests/some_tests.txt
./tester.out tests/floats.txt