#include <iostream>
#include <string>

#include "sexpr.hpp"

auto main(int /*unused*/, char** /*unused*/) -> int {
    using nud  = pratt::sexpr::nud;
    using led  = pratt::sexpr::led;
    using conv = pratt::sexpr::conv;
    using token = nud::token_t;

    using pratt::sexpr::operations;
    using pratt::associativity;

    std::unordered_map<std::string_view, token> tokens {
        { "+", token(pratt::token_kind::dynamic, "+", operations::add, 10, associativity::left) },
        { "-", token(pratt::token_kind::dynamic, "-", operations::sub, 10, associativity::left) },
        { "*", token(pratt::token_kind::dynamic, "*", operations::mul, 20, associativity::left) },
        { "/", token(pratt::token_kind::dynamic, "/", operations::div, 20, associativity::left) },
        { "^", token(pratt::token_kind::dynamic, "^", operations::pow, 30, associativity::right) },
        { "exp", token(pratt::token_kind::dynamic, "exp", operations::exp, 30, associativity::none) },
        { "log", token(pratt::token_kind::dynamic, "log", operations::log, 30, associativity::none) },
        { "sin", token(pratt::token_kind::dynamic, "sin", operations::sin, 30, associativity::none) },
        { "cos", token(pratt::token_kind::dynamic, "cos", operations::cos, 30, associativity::none) },
        { "tan", token(pratt::token_kind::dynamic, "tan", operations::tan, 30, associativity::none) },
        { "sqrt", token(pratt::token_kind::dynamic, "sqrt", operations::sqrt, 30, associativity::none) },
        { "square", token(pratt::token_kind::dynamic, "square", operations::square, 30, associativity::none) },
        { "(", token(pratt::token_kind::lparen, "(", operations::noop, 0, associativity::none) },
        { ")", token(pratt::token_kind::rparen, "(", operations::noop, 0, associativity::none) },
        { "eof", token(pratt::token_kind::eof, "eof", operations::noop, 0, associativity::none) }
    };

    std::string input;
    while(std::getline(std::cin, input)) {
        try {
            pratt::parser<nud, led, conv> p(input, tokens, {});
            auto result = p.parse();
            std::cout << input << " = " << result << "\n";
            input.clear();
        }
        catch(std::exception& e) {
            std::cout << "error parsing input string.\n";
            std::cout << e.what() << "\n";
        }
    }
    return 0;
}
