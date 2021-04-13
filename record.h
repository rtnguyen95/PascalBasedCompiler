#pragma once
#include <string>
using namespace std;

//struct Record to hold data for each input (lexeme) that is read
struct Record
{
    string token; //string variable to hold what token the input is (keyword, identifier, etc.)
    string lexeme;  //string variable to hold the raw input
    bool accepted; //boolean variable that returns true if the input has been accepted, false if otherwise
    const string * filename;  //string variable to hold the name of the file being written to
    int line;
    int linePosition;
    string errorMessage; //string for outputting an error message if the lexeme is not accepted

    Record() : token(""), lexeme(""), accepted(false), filename(NULL), line(1), linePosition(0), errorMessage("") {
        
    }

    Record(const string & token, const string & lexeme, bool accepted, const string & filename, int line, int linePosition, string errorMessage = "") 
    : token(token), lexeme(lexeme), accepted(accepted), filename(&filename), line(line), linePosition(linePosition), errorMessage(errorMessage) {

    }

    void set(const string & token, const string & lexeme, bool accepted, const string & filename, int line, int linePosition, string errorMessage = "") {
        this->token = token;
        this->lexeme = lexeme;
        this->accepted = accepted;
        this->filename = &filename;
        this->line = line;
        this->linePosition = linePosition;
        this->errorMessage = errorMessage;
    }
};

//
// outputs the string representation of a record to an output stream
//
inline ostream & operator << (ostream & stream, Record record){
    stream.width(11);
    stream << left << record.token << " = " << record.lexeme;
    if (!record.accepted)
        stream << record.errorMessage;
    return stream;
}
