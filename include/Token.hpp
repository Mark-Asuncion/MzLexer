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
    Arrow,
    GreaterThan,
    LessThan
};

std::string tokentype_to_string(TokenType);
}

#endif // __TOKENHPP__
