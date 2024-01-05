#include <iostream>
#include "parser.h"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "enter two arguments: input, output\n";
        return -1;
    }
    Parser parser(argv[1], argv[2]);

    parser.parse();

    return 0;
}
