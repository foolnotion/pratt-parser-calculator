#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN 
#include "doctest.h"
#include "parser.hpp"
#include <string>

TEST_CASE("Tokenizer") {
    SUBCASE("1 + 2") {
        std::string infix = "1 + 2";

        pratt::lexer lex(infix);
        auto tokens = lex.tokenize();
        CHECK(tokens.size() == 4);
    }

    SUBCASE("(1 + 2)") {
        std::string infix = "(1 + 2)";

        pratt::lexer lex(infix);
        auto tokens = lex.tokenize();
        std::cout << "tokens:\n";
        for(auto const& t : tokens) {
            std::cout << t << "\n";
        }
        CHECK(tokens.size() == 6);
    }
}

TEST_CASE("Parser") {
    std::string infix;

    SUBCASE("1 + 2") {
        infix = "1 + 2";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 3);
    }

    SUBCASE("-1 + 2") {
        infix = "-1 + 2";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 1);
    }

    SUBCASE("1 + 2 * 3") {
        infix = "1 + 2 * 3";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 7);
    }

    SUBCASE("(1 + 2)") {
        infix = "(1 + 2)";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 3);
    }

    SUBCASE("(1 + 3) * 3") {
        infix = "(1 + 3) * 3";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 12);
    }

    SUBCASE("1 + 3 * 3") {
        infix = "1 + 3 * 3";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 10);
    }

    SUBCASE("3 * 1 + 2") {
        infix = "3 * 1 + 2";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == 5);
    }

    SUBCASE("-(1 + 2)") {
        infix = "-(1 + 2)";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == -3);
    }

    SUBCASE("2 * -(1 + 2)") {
        infix = "2 * -(1 + 2)";
        pratt::parser<pratt::nud, pratt::led> p(infix, {});
        auto result = p.parse();
        CHECK(result == -6);
    }
}
