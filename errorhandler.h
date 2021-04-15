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
    string file; //string that holds the name of the file being written to
    int line;   //int that holds the line number where the error was found
    int position; //int that holds the position number where the error was found
    string message; //string to output an error message to the user
    error_type type; //variable that holds whether the error is a lexical error or a syntax error

    Error(const string & file, int line, int position, const string & message, error_type type)
    : file(file), line(line), position(position), message(message), type(type) {

    }


    Error(Record & token, const string & message, error_type type): message(message), type(type) {
        file = *token.filename; //set file to the filename of the Record token where the rror was found
        line = token.line; //set line to the line number of the token where the error was found
        position = token.linePosition; //set position to the position of the token where the error was found
    }

    //toString function converts the error information to a string
    string toString() const {
        string typeString; //holds the type of error that has been found, lexical or syntactic
        
        //assign typeString to "Lexical" or "Syntactic based on the error_type
        switch (type) {
            case lexical_error: typeString = "Lexical: "; break;
            case syntax_error:  typeString = "Syntactic:  "; break;
            default:            typeString = "Unknown: "; break;
        }
        
        //return the error type, file name, line number, position number, and error message to the user
        return string("Error: ") + typeString + file + ":" + to_string(line) + ":" + to_string(position) + " - " + message;
    }
};

class ErrorHandler {
    list<Error> errors; //list of Error objects

public:
    //add an error to the list of errors
    void addError(const Error & error) {
        errors.push_back(error);
    }

    //check to see if a file has errors by checking to see if the errors list is empty
    //return false if the errors list is empty and the file has no errors
    //return true if the file has errors
    bool hasErrors() const {
        return !errors.empty();
    }

    //function toString returns the entire errors list as a string 
    string toString() const {
        string result = "Errors:\n";
        for(list<Error>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
            result.append(it->toString()).append("\n");
        }
        return result;
    }
};
