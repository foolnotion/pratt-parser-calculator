#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <string>
#include <functional>

#include "../example/calculator.hpp"

namespace pratt::test {

using token = pratt::token<double>;
using nud = pratt::calculator::nud;
using led = pratt::calculator::led;
using conv = pratt::calculator::identity;

using pratt::calculator::operations;
using pratt::associativity;

const std::unordered_map<std::string_view, token> tokens {
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
    { "square", token(pratt::token_kind::dynamic, "square", operations::square, 30, associativity::right) },
    { "(", token(pratt::token_kind::lparen, "(", operations::noop, 0, associativity::none) },
    { ")", token(pratt::token_kind::rparen, "(", operations::noop, 0, associativity::none) },
    { "eof", token(pratt::token_kind::eof, "eof", operations::noop, 0, associativity::none) }
};

auto eval(std::string const& infix) -> double {
    return pratt::parser<nud, led, conv>(infix, tokens, {}).parse();
}

TEST_CASE("Tokenizer")
{
    SUBCASE("1 + 2")
    {
        std::string infix("1 + 2");

        pratt::lexer<token, conv, decltype(tokens)> lex(infix, tokens);
        auto tokens = lex.tokenize();
        CHECK(tokens.size() == 4);
    }

    SUBCASE("(1 + 2)")
    {
        std::string infix("(1 + 2)");
        pratt::lexer<token, conv, decltype(tokens)> lex(infix, tokens);
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
        pratt::lexer<token, conv, decltype(tokens)> lex(infix, tokens);
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
        pratt::lexer<token, conv, decltype(tokens)> lex(infix, tokens);
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
