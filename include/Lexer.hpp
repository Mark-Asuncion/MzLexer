#ifndef __LEXERHPP__
#define __LEXERHPP__

#include <cstdio>
#include <ostream>
#include <string>
#include "Token.hpp"

namespace MzLexer
{

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

    uint token_start_row = 0;
    uint token_start_col = 0;
    uint token_end_row   = 0;
    uint token_end_col   = 0;

    bool is_error = false;

    Lexer(const std::string& s);

    bool next_token();
    void print_current_token(std::ostream& fd);
    bool is_eof();
    std::string get_error();
private:
    std::string error;

    bool is_alpha();
    bool is_number();
    bool is_whitespace(char);
    bool is_token_string();

    char peek(uint offset = 0);
    void advance(uint amount = 1);
    void skip_whitespace();
    inline void set_error();

    void handle_number();
    void handle_string();
};
}
#endif // __LEXERHPP__
