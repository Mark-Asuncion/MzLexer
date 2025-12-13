#include <iostream>
#include <sstream>
#include "Lexer.hpp"

using namespace MzLexer;
int main () {
    std::stringstream source_stream;
    while (!std::cin.eof())
    {
        std::string source;
        std::getline(std::cin, source);
        source_stream << source << '\n';
    }

    std::cout << "Source:\n\"" << source_stream.str() << "\"\n";

    Lexer lexer(source_stream.str());
    while (!lexer.is_error)
    {
        lexer.next_token();
        if (lexer.is_eof() || lexer.is_error) break;
        lexer.print_current_token(std::cout);
    }
    if (lexer.is_error)
    {
        std::cout << lexer.get_error() << '\n';
    }

    return 0;
}
