#pragma once
#include <string>
#include <list>
using namespace std;

enum error_type {
    lexical_error,
    syntax_error
};

struct error {
    string file;
    int line;
    int position;
    string message;
    error_type type;

    string toString() const {
        string typeString;
        switch (type) {
            case lexical_error: typeString = "Lexical: "; break;
            case syntax_error:  typeString = "Syntax:  "; break;
            default:            typeString = "Unknown: "; break;
        }
        return typeString + file + ":" + to_string(line) + ":" + to_string(position) + " - " + message;
    }
};

class ErrorHandler {
    list<error> errors;

public:
    void addError(const error & error) {
        errors.push_back(error);
    }

    bool hasErrors() const {
        return !errors.empty();
    }

    string toString() const {
        string result = "Errors:\n";
        for(list<error>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
            result.append(it->toString()).append("\n");
        }
        return result;
    }
};