#include <iostream>
#include <string>

#include "calculator.hpp"

int main(int, char**) {
    using NUD  = pratt::calculator::nud;
    using LED  = pratt::calculator::led;
    using CONV = pratt::calculator::identity;

    std::string input;
    while(std::getline(std::cin, input)) {
        try {
            pratt::parser<NUD, LED, CONV> p(input, {});
            auto result = p.parse();
            std::cout << input << " = " << result << "\n";
            input.clear();
        }
        catch(std::exception& e) {
            std::cout << "error parsing input string.\n";
        }
    }
    return 0;
}
