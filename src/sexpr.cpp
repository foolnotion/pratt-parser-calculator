#include <iostream>
#include <string>

#include "sexpr.hpp"

int main(int argc, char** argv) {

    using NUD  = pratt::sexpr::nud;
    using LED  = pratt::sexpr::led;
    using CONV = pratt::sexpr::conv;

    std::string input;
    while(std::getline(std::cin, input)) {
        try {
            pratt::parser<NUD, LED, CONV> p(input, {});
            auto result = p.parse();
            std::cout << input << " = " << result << "\n";
            input.clear();
        }
        catch(std::exception e) {
            std::cout << "error parsing input string.\n";
            std::cout << e.what() << "\n";
        }
    }
    return 0;
}
