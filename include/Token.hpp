#ifndef __TOKENHPP__
#define __TOKENHPP__
#include <string>

namespace MzLexer
{

enum TokenType
{
    Null = 0,

    Plus,
    Minus,
    Divide,
    Multiply,
    Equal,

    LogicOr,
    LogicAnd,
    LogicNot,

    BitwiseOr,
    BitwiseExOr,
    BitwiseAnd,
    BitwiseExAnd,
    BitwiseNot,

    GrCurlyStart,
    GrCurlyEnd,
    GrParenthesisStart,
    GrParenthesisEnd,
    GrSquareBracketStart,
    GrSquareBracketEnd,

    StatementEnd,

    WholeNumber,
    Decimal,

    Identifier,
    Char,
    RawString,
    String,

    Comment,
    Dot,
    Spread,
    Range,
    Arrow,
    GreaterThan,
    LessThan,
    SpecialChar
};

std::string tokentype_to_string(TokenType type);
}

#endif // __TOKENHPP__
