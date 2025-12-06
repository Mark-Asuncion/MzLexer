#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"

int main ()
{
    std::ifstream file;
    file.open("./sample.txt");
    if (!file.is_open())
    {
        std::cerr << "Error: File does not exist\n";
        return 1;
    }

    std::stringstream source_stream;
    while (!file.eof())
    {
        std::string source;
        std::getline(file, source);
        source_stream << source;
    }

    Lexer lexer(source_stream.str());

    return 0;
}
