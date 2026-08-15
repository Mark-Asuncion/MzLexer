#include "Lexer.hpp"
#include "Token.hpp"
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

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
        case u'+':
            SET_ONE_CHAR(TokenType::Plus);
            break;
        case u'-':
           if (peek() == u'>')
           {
               SET_TWO_CHAR(TokenType::Arrow, u"->");
           }
           else
           {
               SET_ONE_CHAR(TokenType::Minus);
           }
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
        case u'*':
            SET_ONE_CHAR(TokenType::Multiply);
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
        case u'^':
            SET_ONE_CHAR(TokenType::BitwiseExOr);
            break;
        case u'~':
            SET_ONE_CHAR(TokenType::BitwiseNot);
            break;
        case u'!':
            SET_ONE_CHAR(TokenType::LogicNot);
            break;
        case u'{':
            SET_ONE_CHAR(TokenType::GrCurlyStart);
            break;
        case u'}':
            SET_ONE_CHAR(TokenType::GrCurlyEnd);
            break;
        case u'(':
            SET_ONE_CHAR(TokenType::GrParenthesisStart);
            break;
        case u')':
            SET_ONE_CHAR(TokenType::GrParenthesisEnd);
            break;
        case u'[':
            SET_ONE_CHAR(TokenType::GrSquareBracketStart);
            break;
        case u']':
            SET_ONE_CHAR(TokenType::GrSquareBracketEnd);
            break;
        case u';':
            SET_ONE_CHAR(TokenType::StatementEnd);
            break;
        case u'.':
            if (peek() == u'.' && peek(1) == u'.')
            {
                token = TokenType::Spread;
                lexeme = u"...";
                token_start_row = row;
                token_start_col = col;
                advance(2);
                token_end_row = row;
                token_end_col = col;
                advance();
            }
            else if (peek() == u'.')
            {
                SET_TWO_CHAR(TokenType::Range, u"..");
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
            else if (is_alpha() || source[ptr] == u'"' || source[ptr] == u'\'') {
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
        if (peek() == u'.') {
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

void MzLexer::Lexer::print_current_token(std::wostream& fd)
{
    int w = 10;
    u16stringstream ss;

    std::u16string gr = u"";
    if (is_token_string())
    {
        gr = u"\"";
    }
    else if (token == TokenType::Char)
    {
        gr = u"'";
    }

    ss << tokentype_to_string(token) << u"(" << gr << lexeme << gr << u")";
    std::wstring wst;
#ifdef _WIN32
    std::u16string utf16 = ss.str();
    wst.reserve(utf16.size());
    std::copy(utf16.begin(), utf16.end(), std::back_inserter(wst));
#else
    U16Converter u16conv;
    std::string utf8 = u16conv.to_bytes(ss.str());
    WideConverter wconv;
    wst = wconv.from_bytes(utf8);
#endif

    fd <<  "TokenType: " << std::setw(w*2) << std::left << wst <<
        " Token Start Row: " << std::setw(w) << std::left << token_start_row <<
        " Token Start Col: " << std::setw(w) << std::left << token_start_col <<
        " Token End Row: " << std::setw(w) << std::left << token_end_row <<
        " Token End Col: " << std::setw(w) << std::left << token_end_col <<
        '\n';
}


bool MzLexer::Lexer::is_eof()
{
    return ptr >= length || source[ptr] == u'\0';
}

bool MzLexer::Lexer::is_alpha()
{
    uint a = u'a';
    uint z = u'z';
    uint A = u'A';
    uint Z = u'Z';
    uint cur_char = source[ptr];
    return (cur_char >= a && cur_char <= z) || (cur_char >= A && cur_char <= Z);
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

std::u16string MzLexer::tokentype_to_string(TokenType type)
{
    switch (type)
    {
        case Null:                  return u"Null";
        case Plus:                  return u"Plus";
        case Minus:                 return u"Minus";
        case Divide:                return u"Divide";
        case Multiply:              return u"Multiply";
        case Equal:                 return u"Equal";
        case BitwiseOr:             return u"BitwiseOr";
        case BitwiseExOr:           return u"BitwiseExOr";
        case BitwiseAnd:            return u"BitwiseAnd";
        case BitwiseNot:            return u"BitwiseNot";
        case LogicOr:               return u"LogicOr";
        case LogicAnd:              return u"LogicAnd";
        case LogicNot:              return u"LogicNot";
        case GrCurlyStart:          return u"GrCurlyStart";
        case GrCurlyEnd:            return u"GrCurlyEnd";
        case GrParenthesisStart:    return u"GrParenthesisStart";
        case GrParenthesisEnd:      return u"GrParenthesisEnd";
        case GrSquareBracketStart:  return u"GrSquareBracketStart";
        case GrSquareBracketEnd:    return u"GrSquareBracketEnd";
        case StatementEnd:          return u"StatementEnd";
        case WholeNumber:           return u"WholeNumber";
        case Decimal:               return u"Decimal";
        case Identifier:            return u"Identifier";
        case Spread:                return u"Spread";
        case Char:                  return u"Char";
        case MultiLineString:       return u"MultiLineString";
        case RString:               return u"RString";
        case FString:               return u"FString";
        case String:                return u"String";
        case Comment:               return u"Comment";
        case Dot:                   return u"Dot";
        case Range:                 return u"Range";
        case Arrow:                 return u"Arrow";
        case GreaterThan:           return u"GreaterThan";
        case LessThan:              return u"LessThan";
        case SpecialChar:           return u"SpecialChar";
        default:                    return u"";
    }
}
