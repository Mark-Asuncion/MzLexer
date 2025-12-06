#ifndef __LEXERHPP__
#define __LEXERHPP__

#include <cstdio>
#include <string>
#include "Token.hpp"

class Lexer
{
public:
    std::string source;
    uint ptr = 0;

    TokenType token;
    std::string lexeme;

    uint length = 0;
    uint row = 0;
    uint col = 0;

    Lexer(const std::string& s) : source(s) { }

    bool next_token();
    bool next_token(TokenType expect_token);
    void print_current_token(std::FILE fd);

private:
    char peek();
    void advance(int amount = 0);
    void skip_whitespace();
    bool is_whitespace();
    bool isEof();
};

#endif // __LEXERHPP__
