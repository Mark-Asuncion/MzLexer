#ifndef __LEXERHPP__
#define __LEXERHPP__

#include <cstdio>
#include <ostream>
#include <sstream>
#include <string>
#include "Token.hpp"
#include <codecvt>
#include <locale>

namespace MzLexer
{

std::string u16_to_string(const std::u16string&);

typedef std::basic_stringstream<char16_t> u16stringstream;
typedef std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> U16Converter;

class Lexer
{
public:
    std::u16string source;
    uint ptr = 0;

    TokenType token;
    std::u16string lexeme;

    uint length = 0;
    uint row = 0;
    uint col = 0;

    uint token_row = 0;
    uint token_col = 0;
    uint token_end = 0;

    bool is_error = false;

    Lexer(const std::string& s);

    bool next_token();
    void print_current_token(std::ostream& fd);
    bool is_eof();
    std::string get_error();
private:
    std::u16string error;

    bool is_alpha();
    bool is_number();
    char peek(uint offset = 0);
    void advance(uint amount = 1);
    void skip_whitespace();
    inline void set_error();
};
}
#endif // __LEXERHPP__
