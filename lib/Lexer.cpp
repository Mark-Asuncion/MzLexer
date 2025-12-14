#include "Lexer.hpp"
#include "Token.hpp"
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>

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
    token_end_col = col;


MzLexer::Lexer::Lexer(const std::string& s)
{
    MzLexer::U16Converter convert;
    source = convert.from_bytes(s.c_str());
    length = source.length();
    row = 1;
    col = 1;
}

char16_t MzLexer::Lexer::peek(uint offset)
{
    if (ptr+1+offset >= length) return u'\0';
    return source[ptr+1+offset];
}

void MzLexer::Lexer::advance(uint amount)
{
    if (is_eof()) return;
    ptr+=amount;
    col+=amount;
}


bool MzLexer::Lexer::is_whitespace(char16_t c)
{
    return c == u'\n' || c == u' ' || c == u'\t' || c == '\r';
}

void MzLexer::Lexer::skip_whitespace()
{
    while (!is_eof())
    {
        if (source[ptr] == u'\n')
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
        if (source[rev_ptr] == u'\n')
        {
            line_start = rev_ptr + 1;
            break;
        }
        rev_ptr--;
    }
    while (forw_ptr < (int)length)
    {
        if (source[forw_ptr] == u'\n')
        {
            line_end = forw_ptr;
            break;
        }
        forw_ptr++;
    }

    if (line_end == 0)
        line_end = length;

    u16stringstream ss;
    ss << u"Unknown Token\n";
    ss << source.substr(line_start, line_end - line_start) << u'\n';
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
            SET_ONE_CHAR(TokenType::Multiply);
            break;
        case u'+':
            SET_ONE_CHAR(TokenType::Plus);
            break;
        case u'-':
           SET_ONE_CHAR(TokenType::Minus);
            break;
        case u'/':
            if (peek() == u'/')
            {
                SET_TWO_CHAR(TokenType::Comment, u"//");
            }
            else {
                SET_ONE_CHAR(TokenType::Divide);
            }
            break;
        case u'=':
            SET_ONE_CHAR(TokenType::Equal);
            break;
        case u'>':
            SET_ONE_CHAR(TokenType::GreaterThan);
            break;
        case u'<':
            SET_ONE_CHAR(TokenType::LessThan);
            break;
        case u'|':
            if (peek() == u'|')
            {
                SET_TWO_CHAR(TokenType::LogicOr, u"||");
            }
            else
            {
                SET_ONE_CHAR(TokenType::BitwiseOr);
            }
            break;
        case u'&':
            if (peek() == u'&')
            {
                SET_TWO_CHAR(TokenType::LogicAnd, u"&&");
            }
            else
            {
                SET_ONE_CHAR(TokenType::BitwiseAnd);
            }
            break;
        case u'!':
            SET_ONE_CHAR(TokenType::LogicNot);
            break;
        default:
            if (is_number())
            {
                assert(false && "Handle number");
                return true;
                break;
            }
            set_error();
            return false;
            break;
    }

    return true;
}

void MzLexer::Lexer::print_current_token(std::ostream& fd)
{
    fd <<  "TokenType: " << std::setw(15) << std::left << tokentype_to_string(token) <<
        " Lexeme: "    << std::setw(15) << std::left << u16_to_string(lexeme) <<
        " Token Start Row: " << std::setw(15) << std::left << token_start_row <<
        " Token Start Col: " << std::setw(15) << std::left << token_start_col <<
        " Token End Row: " << std::setw(15) << std::left << token_end_row <<
        " Token End Col: " << std::setw(15) << std::left << token_end_col <<
        '\n';
}


bool MzLexer::Lexer::is_eof()
{
    return ptr >= length || source[ptr] == u'\0';
}

bool MzLexer::Lexer::is_alpha()
{
    assert(false && "NOT IMPLEMENTED");
}

bool MzLexer::Lexer::is_number()
{
    char16_t c = source[ptr];
    return c >= u'0' && c <= u'9';
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
        case BitwiseExAnd:          return "BitwiseExAnd";
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
        case Arrow:                 return "Arrow";
        case GreaterThan:           return "GreaterThan";
        case LessThan:              return "LessThan";
        default:                    return "";
    }
}
