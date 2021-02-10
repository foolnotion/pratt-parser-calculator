#ifndef PARSER_HPP
#define PARSER_HPP

#include <charconv>
#include <limits>
#include <optional>
#include <ostream>
#include <type_traits>
#include <unordered_map>

#include "lexer.hpp"

namespace pratt {

struct nud {
    using T = double;

    template <typename Parser>
    T operator()(Parser* parser, token_kind tok, T val)
    {
        switch (tok) {
        case token_kind::constant: {
            return val;
        }

        case token_kind::sub: {
            auto bp = token_precedence[token_kind::sub];
            return -parser->parse_bp(bp, token_kind::eof).value;
        }

        case token_kind::lparen: {
            auto bp = token_precedence[token_kind::lparen];
            return parser->parse_bp(bp, token_kind::rparen).value;
        }

        default: {
            throw std::runtime_error("nud: unsupported token " + std::string(token_name[static_cast<int>(tok)]));
        };
        }
    }
};

struct led {
    using T = double;

    template <typename Parser>
    T operator()(Parser* parser, token_kind tok, T lhs, T rhs)
    {
        switch (tok) {
        case token_kind::add:
            return lhs + rhs;

        case token_kind::sub:
            return lhs - rhs;

        case token_kind::mul:
            return lhs * rhs;

        case token_kind::div:
            return lhs / rhs;

        default:
            throw std::runtime_error("led: unsupported token " + std::string(token_name[static_cast<int>(tok)]));
        };
    }
};

template <typename NUD, typename LED>
class parser {
public:
    using T = typename NUD::T;

    parser(std::string const& infix, std::unordered_map<std::string, ulong> const& vars)
        : lexer_(infix)
        , vars_(vars)
    {
        static_assert(std::is_same_v<typename NUD::T, typename LED::T>);
    }

    T parse()
    {
        return parse_bp(0).value;
    }

    friend NUD;
    friend LED;

private:
    lexer lexer_;
    std::unordered_map<std::string, ulong> vars_;

    token expr(T value, int bp = 0) const { return token(token_kind::constant, bp, value); }

    token parse_bp(int rbp = 0, token_kind end = token_kind::eof)
    {
        NUD nud;
        LED led;

        auto left = lexer_.peek(); 
        lexer_.consume();

        if (left.is<token_kind::lparen>()) {
             left = parse_bp(token_precedence[token_kind::lparen], token_kind::rparen);
             lexer_.consume(); // eat rparen
        }

        left.value = nud(this, left.kind, left.value);

        while(true) {
            auto op = lexer_.peek();

            if (op.kind == end || op.kind == token_kind::eof || op.lbp <= rbp) {
                break;
            }

            lexer_.consume();
            auto right = parse_bp(op.lbp, end);
            std::cout << left.value << " " << op << " " << right.value << " = ";
            left = expr(led(this, op.kind, left.value, right.value));
            std::cout << left << "\n";
        }

        return left;
    };

    int x = -1;
};
} // namespace

#endif
