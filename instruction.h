#pragma once
#include <string>
using namespace std;

struct Instruction
{
    string op;
    string oprnd;
    
    Instruction(string temp_op, string temp_oprnd)
    {
        op = temp_op;
        oprnd = temp_oprnd;
    }

}

