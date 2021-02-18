#include "token.h"

auto token::operator=(const token& other) -> token& {
  lexemme_ = other.lexemme_;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const token& t) {
  return t.write(out);
}
