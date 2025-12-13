#include "Lexer.hpp"
#include "Token.hpp"
#include <cassert>
#include <string>
#include <iostream>

#define SET_ONE_CHAR(type)   \
    token = type;            \
    lexeme = source[ptr];    \
    token_row = row;         \
    token_col = col;         \
    token_end = col;         \
    advance()


MzLexer::Lexer::Lexer(const std::string& s)
{
    MzLexer::U16Converter convert;
    source = convert.from_bytes(s.c_str());
    length = source.length();
    row = 1;
    col = 1;
}

char MzLexer::Lexer::peek(uint offset)
{
    if (ptr+1+offset >= length) return '\0';
    return source[ptr+1+offset];
}

void MzLexer::Lexer::advance(uint amount)
{
    if (is_eof()) return;
    ptr+=amount;
    col+=amount;
}

void MzLexer::Lexer::skip_whitespace()
{
    while (!is_eof())
    {
        switch (source[ptr])
        {
            case u'\n':
                row++;
                col=0;
                advance();
                break;
            case u' ':
            case u'\t':
            case u'\r':
                advance();
                break;
            default:
                return;
                break;
        }
    }
}

inline void MzLexer::Lexer::set_error()
{
    is_error = true;
    int rev_ptr = ptr;
    int forw_ptr = ptr;
    uint line_start = 0;
    uint line_end = 0;

    while (rev_ptr-- && rev_ptr >= 0) {
        if (source[rev_ptr] == u'\n')
        {
            line_start = rev_ptr + 1;
            break;
        }
    }
    while (forw_ptr++ && forw_ptr < (int)length)
    {
        if (forw_ptr >= (int)length || source[forw_ptr] == u'\n')
        {
            line_end = forw_ptr;
            break;
        }
    }

    u16stringstream ss;
    ss << u"Unknown Token\n";
    ss << source.substr(line_start, line_end) << u'\n';
    ss << std::u16string(col-1, ' ') << u"|\n";

    error = ss.str();

}

bool MzLexer::Lexer::next_token()
{
    skip_whitespace();
    if (is_eof()) return false;
    switch (source[ptr])
    {
        case u'*':
            SET_ONE_CHAR(TokenType::OpMultiply);
            break;
        case u'+':
            SET_ONE_CHAR(TokenType::OpPlus);
            break;
        case u'-':
            SET_ONE_CHAR(TokenType::OpMinus);
            break;
        default:
            set_error();
            return false;
            break;
    }

    return true;
}

void MzLexer::Lexer::print_current_token(std::ostream& fd)
{
    fd << "TokenType: " << token <<
        " Lexeme: " << u16_to_string(lexeme) <<
        " Token Row: " << token_row <<
        " Token Col: " << token_col <<
        " Token End: " << token_end <<
        '\n';
}


bool MzLexer::Lexer::is_eof()
{
    return ptr >= length || source[ptr] == '\0';
}

bool MzLexer::Lexer::is_alpha()
{
    assert(false && "NOT IMPLEMENTED");
}

bool MzLexer::Lexer::is_number()
{
    assert(false && "NOT IMPLEMENTED");
}

std::string MzLexer::Lexer::get_error()
{
    return u16_to_string(error);
}

std::string MzLexer::u16_to_string(const std::u16string& v)
{
    MzLexer::U16Converter convert;
    return convert.to_bytes(v);
}
