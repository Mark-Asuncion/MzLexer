#include "Lexer.hpp"
#include "Token.hpp"
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

MzLexer::Lexer::Lexer(const std::string& s)
{
    source = s;
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


bool MzLexer::Lexer::is_whitespace(char c)
{
    return c == '\n' || c == ' ' || c == '\t' || c == '\r';
}

void MzLexer::Lexer::skip_whitespace()
{
    while (!is_eof())
    {
        if (source[ptr] == '\n')
        {
            row++;
            col=0;
        }
        if (is_whitespace(source[ptr]))
        {
            advance();
        }
        else {
            return;
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

    while (rev_ptr >= 0) {
        if (source[rev_ptr] == '\n')
        {
            line_start = rev_ptr + 1;
            break;
        }
        rev_ptr--;
    }
    while (forw_ptr < (int)length)
    {
        if (source[forw_ptr] == '\n')
        {
            line_end = forw_ptr;
            break;
        }
        forw_ptr++;
    }

    if (line_end == 0)
        line_end = length;

    std::stringstream ss;
    ss << "Unknown Token\n";
    ss << source.substr(line_start, line_end - line_start) << '\n';
    ss << std::string(col-1, ' ') << "|\n";

    error = ss.str();

}

bool MzLexer::Lexer::next_token()
{
#define SET_ONE_CHAR(type)          \
    token = type;                   \
    lexeme = source[ptr];           \
    token_start_row = row;          \
    token_start_col = col;          \
    token_end_row = row;            \
    token_end_col = col;            \
    advance()

#define SET_TWO_CHAR(type, v)       \
    token = type;                   \
    lexeme = v;                     \
    token_start_row = row;          \
    token_start_col = col;          \
    advance();                      \
    token_end_row = row;            \
    token_end_col = col;            \
    advance();

    skip_whitespace();
    if (is_eof()) return false;
    switch (source[ptr])
    {
        case '+':
            SET_ONE_CHAR(TokenType::Plus);
            break;
        case '-':
           if (peek() == '>')
           {
               SET_TWO_CHAR(TokenType::Arrow, "->");
           }
           else
           {
               SET_ONE_CHAR(TokenType::Minus);
           }
            break;
        case '/':
            if (peek() == '/')
            {
                SET_TWO_CHAR(TokenType::Comment, "//");
            }
            else {
                SET_ONE_CHAR(TokenType::Divide);
            }
            break;
        case '*':
            SET_ONE_CHAR(TokenType::Multiply);
            break;
        case '=':
            SET_ONE_CHAR(TokenType::Equal);
            break;
        case '>':
            SET_ONE_CHAR(TokenType::GreaterThan);
            break;
        case '<':
            SET_ONE_CHAR(TokenType::LessThan);
            break;
        case '|':
            if (peek() == '|')
            {
                SET_TWO_CHAR(TokenType::LogicOr, "||");
            }
            else
            {
                SET_ONE_CHAR(TokenType::BitwiseOr);
            }
            break;
        case '&':
            if (peek() == '&')
            {
                SET_TWO_CHAR(TokenType::LogicAnd, "&&");
            }
            else
            {
                SET_ONE_CHAR(TokenType::BitwiseAnd);
            }
            break;
        case '^':
            SET_ONE_CHAR(TokenType::BitwiseExOr);
            break;
        case '~':
            SET_ONE_CHAR(TokenType::BitwiseNot);
            break;
        case '!':
            SET_ONE_CHAR(TokenType::LogicNot);
            break;
        case '{':
            SET_ONE_CHAR(TokenType::GrCurlyStart);
            break;
        case '}':
            SET_ONE_CHAR(TokenType::GrCurlyEnd);
            break;
        case '(':
            SET_ONE_CHAR(TokenType::GrParenthesisStart);
            break;
        case ')':
            SET_ONE_CHAR(TokenType::GrParenthesisEnd);
            break;
        case '[':
            SET_ONE_CHAR(TokenType::GrSquareBracketStart);
            break;
        case ']':
            SET_ONE_CHAR(TokenType::GrSquareBracketEnd);
            break;
        case ';':
            SET_ONE_CHAR(TokenType::StatementEnd);
            break;
        case '.':
            if (peek() == '.' && peek(1) == '.')
            {
                token = TokenType::Spread;
                lexeme = "...";
                token_start_row = row;
                token_start_col = col;
                advance(2);
                token_end_row = row;
                token_end_col = col;
                advance();
            }
            else if (peek() == '.')
            {
                SET_TWO_CHAR(TokenType::Range, "..");
            }
            else
            {
                SET_ONE_CHAR(TokenType::Dot);
            }
            break;
        default:
            if (is_number())
            {
                handle_number();
                break;
            }
            else if (is_alpha() || source[ptr] == '"' || source[ptr] == '\'') {
                assert(false && "Handle alpha");
                return true;
                break;
            }
            else
            {
                SET_ONE_CHAR(TokenType::SpecialChar);
                break;
            }
            // set_error();
            // return false;
            // break;
    }

    return true;
}

void MzLexer::Lexer::handle_number() {
    uint start_ptr = ptr;
    token_start_row = row;
    token_start_col = col;
    token = TokenType::WholeNumber;
    advance();

    while(!is_eof() && is_number()) {
        if (peek() == '.') {
            token = TokenType::Decimal;
            advance();
        }
        advance();
    }

    lexeme = source.substr(start_ptr, ptr-start_ptr);
    token_end_row = row;
    token_end_col = col;
}

void MzLexer::Lexer::handle_string() {
}

bool MzLexer::Lexer::is_token_string() {
    return token == TokenType::String || token == TokenType::MultiLineString ||
        token == TokenType::RString || token == TokenType::FString;
}

void MzLexer::Lexer::print_current_token(std::ostream& fd)
{
    int w = 10;
    std::stringstream ss;

    std::string gr = "";
    if (is_token_string())
    {
        gr = "\"";
    }
    else if (token == TokenType::Char)
    {
        gr = "'";
    }

    ss << tokentype_to_string(token) << "(" << gr << lexeme << gr << ")";

    fd <<  "TokenType: " << std::setw(w*2) << std::left << ss.str() <<
        " Token Start Row: " << std::setw(w) << std::left << token_start_row <<
        " Token Start Col: " << std::setw(w) << std::left << token_start_col <<
        " Token End Row: " << std::setw(w) << std::left << token_end_row <<
        " Token End Col: " << std::setw(w) << std::left << token_end_col <<
        '\n';
}


bool MzLexer::Lexer::is_eof()
{
    return ptr >= length || source[ptr] == '\0';
}

bool MzLexer::Lexer::is_alpha()
{
    uint a = 'a';
    uint z = 'z';
    uint A = 'A';
    uint Z = 'Z';
    uint cur_char = source[ptr];
    return (cur_char >= a && cur_char <= z) || (cur_char >= A && cur_char <= Z);
}

bool MzLexer::Lexer::is_number()
{
    char c = source[ptr];
    return c >= '0' && c <= '9';
}

std::string MzLexer::Lexer::get_error()
{
    return error;
}

std::string MzLexer::tokentype_to_string(TokenType type)
{
    switch (type)
    {
        case Null:                  return "Null";
        case Plus:                  return "Plus";
        case Minus:                 return "Minus";
        case Divide:                return "Divide";
        case Multiply:              return "Multiply";
        case Equal:                 return "Equal";
        case BitwiseOr:             return "BitwiseOr";
        case BitwiseExOr:           return "BitwiseExOr";
        case BitwiseAnd:            return "BitwiseAnd";
        case BitwiseNot:            return "BitwiseNot";
        case LogicOr:               return "LogicOr";
        case LogicAnd:              return "LogicAnd";
        case LogicNot:              return "LogicNot";
        case GrCurlyStart:          return "GrCurlyStart";
        case GrCurlyEnd:            return "GrCurlyEnd";
        case GrParenthesisStart:    return "GrParenthesisStart";
        case GrParenthesisEnd:      return "GrParenthesisEnd";
        case GrSquareBracketStart:  return "GrSquareBracketStart";
        case GrSquareBracketEnd:    return "GrSquareBracketEnd";
        case StatementEnd:          return "StatementEnd";
        case WholeNumber:           return "WholeNumber";
        case Decimal:               return "Decimal";
        case Identifier:            return "Identifier";
        case Spread:                return "Spread";
        case Char:                  return "Char";
        case MultiLineString:       return "MultiLineString";
        case RString:               return "RString";
        case FString:               return "FString";
        case String:                return "String";
        case Comment:               return "Comment";
        case Dot:                   return "Dot";
        case Range:                 return "Range";
        case Arrow:                 return "Arrow";
        case GreaterThan:           return "GreaterThan";
        case LessThan:              return "LessThan";
        case SpecialChar:           return "SpecialChar";
        default:                    return "";
    }
}
