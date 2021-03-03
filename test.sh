#!/bin/sh

./tokenizer tests/simple.txt tests/simple_expected.txt
./tokenizer tests/sample.txt tests/sample_expected.txt
./tokenizer tests/quadratic.txt tests/quadratic_expected.txt
./tokenizer tests/countEights.txt tests/countEights_expected.txt
./tokenizer tests/floats.txt tests/floats_expected.txt