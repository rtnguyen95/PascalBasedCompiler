#pragma once
#include <string>
#include <list>
#include "record.h"
using namespace std;

enum error_type {
    lexical_error,
    syntax_error
};

struct Error {
    string file;
    int line;
    int position;
    string message;
    error_type type;

    Error(const string & file, int line, int position, const string & message, error_type type)
    : file(file), line(line), position(position), message(message), type(type) {

    }


    Error(Record & token, const string & message, error_type type): message(message), type(type) {
        file = *token.filename;
        line = token.line;
        position = token.linePosition;
    }

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
    list<Error> errors;

public:
    void addError(const Error & error) {
        errors.push_back(error);
    }

    bool hasErrors() const {
        return !errors.empty();
    }

    string toString() const {
        string result = "Errors:\n";
        for(list<Error>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
            result.append(it->toString()).append("\n");
        }
        return result;
    }
};