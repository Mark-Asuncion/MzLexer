#include <iostream>
#include <sstream>
#include "Lexer.hpp"

int main ()
{
    std::stringstream source_stream;
    while (!std::cin.eof())
    {
        std::string source;
        std::getline(std::cin, source);
        source_stream << source << '\n';
    }

    std::cout << source_stream.str() << '\n';
    Lexer lexer(source_stream.str());

    return 0;
}
