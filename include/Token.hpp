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
    Spread,
    Char,
    MultiLineString,
    RString,
    FString,
    String,

    Comment,
    Dot,
    Range,
    Arrow,
    GreaterThan,
    LessThan,
    SpecialChar
};

std::u16string tokentype_to_string(TokenType type);
}

#endif // __TOKENHPP__
