#pragma once
class token {
private:
  std::string lexemme_;
public:
  ~token() { }
  token() { }
  token(std::string lexemme) {
    lexemme_ = lexemme;
  }
  std::string operator=(std::string lexemme);
}
