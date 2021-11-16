#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <string>
#include <functional>

#include "../example/calculator.hpp"

namespace pratt::test {

using T = pratt::token<double>;
using NUD = pratt::calculator::nud;
using LED = pratt::calculator::led;
using CONV = pratt::calculator::identity;

double eval(std::string const& infix) {
    return pratt::parser<NUD, LED, CONV>(infix, {}).parse();
}

TEST_CASE("Tokenizer")
{
    SUBCASE("1 + 2")
    {
        std::string infix("1 + 2");

        pratt::lexer<T, CONV> lex(infix);
        auto tokens = lex.tokenize();
        CHECK(tokens.size() == 4);
    }

    SUBCASE("(1 + 2)")
    {
        std::string infix("(1 + 2)");

        pratt::lexer<T, CONV> lex(infix);
        auto tokens = lex.tokenize();
        std::cout << "tokens:\n";
        for (auto const& t : tokens) {
            std::cout << t << "\n";
        }
        CHECK(tokens.size() == 6);
    }

    SUBCASE("1 + exp(2) + log(3)")
    {
        std::string infix("1 + exp(2) + log(3)");
        pratt::lexer<T, CONV> lex(infix);
        auto tokens = lex.tokenize();
        std::cout << "tokens:\n";
        for (auto const& t : tokens) {
            std::cout << t << "\n";
        }
        CHECK(tokens.size() == 12);
    }

    SUBCASE("exp(tan(5))")
    {
        std::string infix("exp(tan(5))");
        pratt::lexer<T, CONV> lex(infix);
        auto tokens = lex.tokenize();
        std::cout << "tokens:\n";
        for (auto const& t : tokens) {
            std::cout << t << "\n";
        }
        CHECK(tokens.size() == 8);
    }
}

#define CHECK_SUBCASE(x, y) SUBCASE(x) { CHECK_EQ(eval(x), y); }

TEST_CASE("Parser")
{
    CHECK_SUBCASE("1 + 2",               3);
    CHECK_SUBCASE("-1 + 2",              1);
    CHECK_SUBCASE("1 + 2 * 3",           7);
    CHECK_SUBCASE("(1 + 2)",             3);
    CHECK_SUBCASE("-(1 + 2)",            -3);
    CHECK_SUBCASE("(1 + 2) - 1",         2);
    CHECK_SUBCASE("(1 + 2) * (3 + 4)",   21);
    CHECK_SUBCASE("(1 + 2) * 3",         9);
    CHECK_SUBCASE("(1 + 3) * 3",         12);
    CHECK_SUBCASE("3 * 1 + 2",           5);
    CHECK_SUBCASE("3 * (1 + 2)",         9);
    CHECK_SUBCASE("2 * -(1 + 2)",        -6);
    CHECK_SUBCASE("-(2 + 1) / (1 + 2)",  -1);
    CHECK_SUBCASE("3 - 2 - 1",           0);
    CHECK_SUBCASE("3 - 2 - 1 - 1",       -1);
    CHECK_SUBCASE("2 ^ 3 ^ 2",           512);
    CHECK_SUBCASE("(2 ^ 3) ^ 2",         64);
    CHECK_SUBCASE("exp(2)",              std::exp(2));
    CHECK_SUBCASE("log(3)",              std::log(3));
    CHECK_SUBCASE("sin(4)",              std::sin(4));
    CHECK_SUBCASE("cos(5)",              std::cos(5));
    CHECK_SUBCASE("2 * cos(5)",          2 * std::cos(5));
    CHECK_SUBCASE("exp(tan(5))",         std::exp(std::tan(5)));
    CHECK_SUBCASE("square(exp(tan(5)))", std::pow(std::exp(std::tan(5)), 2));
    CHECK_SUBCASE("cos(5) * sin(6)",     std::cos(5) * std::sin(6));
}

} // namespace
