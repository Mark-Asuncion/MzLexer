#include <iostream>
#include <sstream>
#include "Lexer.hpp"

using namespace MzLexer;
int main () {
    std::stringstream source_stream;
    // source_stream << "é  //";
    while (!std::cin.eof())
    {
        std::string source;
        std::getline(std::cin, source);
        source_stream << source;
    }
    // source_stream << "\n";

    std::cout << "Source:\n\"" << source_stream.str() << "\"\n";

    Lexer lexer(source_stream.str());
    while (!lexer.is_error && !lexer.is_eof())
    {
        lexer.next_token();
        lexer.print_current_token(std::cout);
    }
    if (lexer.is_error)
    {
        std::cout << lexer.get_error() << '\n';
    }

    return 0;
}
