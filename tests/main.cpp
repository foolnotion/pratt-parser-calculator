#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "parser.hpp"
#include <string>

namespace pratt::test {

pratt::nud::T eval(std::string const& infix) {
    return pratt::parser<pratt::nud, pratt::led>(infix, {}).parse();
}

TEST_CASE("Tokenizer")
{
    SUBCASE("1 + 2")
    {
        std::string infix("1 + 2");

        pratt::lexer lex(infix);
        auto tokens = lex.tokenize();
        CHECK(tokens.size() == 4);
    }

    SUBCASE("(1 + 2)")
    {
        std::string infix("(1 + 2)");

        pratt::lexer lex(infix);
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
        pratt::lexer lex(infix);
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
        pratt::lexer lex(infix);
        auto tokens = lex.tokenize();
        std::cout << "tokens:\n";
        for (auto const& t : tokens) {
            std::cout << t << "\n";
        }
        CHECK(tokens.size() == 8);
    }
}

#define SUBCASE_(x, y) SUBCASE(x) { CHECK_EQ(eval(x), y); }

TEST_CASE("Parser")
{
    SUBCASE_("1 + 2",               3);
    SUBCASE_("-1 + 2",              1);
    SUBCASE_("1 + 2 * 3",           7);
    SUBCASE_("(1 + 2)",             3);
    SUBCASE_("-(1 + 2)",            -3);
    SUBCASE_("(1 + 2) - 1",         2);
    SUBCASE_("(1 + 2) * (3 + 4)",   21);
    SUBCASE_("(1 + 2) * 3",         9);
    SUBCASE_("(1 + 3) * 3",         12);
    SUBCASE_("3 * 1 + 2",           5);
    SUBCASE_("3 * (1 + 2)",         9);
    SUBCASE_("2 * -(1 + 2)",        -6);
    SUBCASE_("-(2 + 1) / (1 + 2)",  -1);
    SUBCASE_("3 - 2 - 1",           0);
    SUBCASE_("3 - 2 - 1 - 1",       -1);
    SUBCASE_("2 ^ 3 ^ 2",           512);
    SUBCASE_("(2 ^ 3) ^ 2",         64);
    SUBCASE_("exp(2)",              std::exp(2));
    SUBCASE_("log(3)",              std::log(3));
    SUBCASE_("sin(4)",              std::sin(4));
    SUBCASE_("cos(5)",              std::cos(5));
    SUBCASE_("2 * cos(5)",          2 * std::cos(5));
    SUBCASE_("exp(tan(5))",         std::exp(std::tan(5)));
    SUBCASE_("square(exp(tan(5)))", std::pow(std::exp(std::tan(5)), 2));
    SUBCASE_("cos(5) * sin(6)",     std::cos(5) * std::sin(6));
}

} // namespace
