#pragma once
#include <iostream>

class token
{
private:
  std::string lexemme_;
    
public:
  ~token() { }
  token() {
    lexemme_ = " ";
  }
  token(std::string lexemme){
    lexemme_ = lexemme;
  }
  auto write(std::ostream& out) const -> std::ostream& {
    out << lexemme_;
    return out;
  }
  auto operator=(const token& other) -> token&;
};
std::ostream& operator<<(std::ostream& out, const token& t);
