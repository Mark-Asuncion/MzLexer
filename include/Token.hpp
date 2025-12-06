#ifndef __TOKENHPP__
#define __TOKENHPP__

enum TokenType
{
    Null = 0,

    OpPlus,
    OpMinus,
    OpDivide,
    OpMultiply,
    OpEqual,

    BinOr,
    BinExOr,
    BinAnd,
    BinExAnd,
    BinNot,
    BinShiftLeft,
    BinShiftRight,

    Equal,

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
    String
};

#endif // __TOKENHPP__
