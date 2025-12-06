#include "Lexer.hpp"
#include <iostream>

Lexer::Lexer(const std::string& s) : source(s)
{
    std::cout << "Lexer" << ' ' << s << '\n';
}
