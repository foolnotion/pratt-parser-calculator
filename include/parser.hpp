#ifndef PRATT_PARSER_HPP
#define PRATT_PARSER_HPP

#include <cmath>
#include <unordered_map>

#include "lexer.hpp"

namespace pratt {

std::array<int, token_count> token_precedence = {
     0, // lparen
     0, // rparen
    10, // add
    10, // sub
    20, // mul
    20, // div
    30, // log
    30, // exp
    30, // sin
    30, // cos
    30, // tan
    30, // sqrt
    30, // cbrt
    30, // square
    30, // pow
    40, // variable
    40, // constant
     0, // eof
};

template<typename T>
struct nud {
    using token_t = T;
    using value_t = typename T::value_t;

    template <typename Parser>
    value_t operator()(Parser& parser, token_kind tok, value_t val)
    {
        auto bp = token_precedence[tok]; // binding power

        switch (tok) {
        case token_kind::constant: {
            return val;
        }

        case token_kind::sub: {
            return -parser.parse_bp(bp, token_kind::eof).value;
        }

        case token_kind::exp: {
            return std::exp(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::log: {
            return std::log(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::sin: {
            return std::sin(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::cos: {
            return std::cos(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::tan: {
            return std::tan(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::lparen: {
            return parser.parse_bp(bp, token_kind::rparen).value;
        }

        case token_kind::sqrt: {
            return std::sqrt(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::cbrt: {
            return std::cbrt(parser.parse_bp(bp, token_kind::eof).value);
        }

        case token_kind::square: {
            auto v = parser.parse_bp(bp, token_kind::eof).value;
            return v * v;
        }

        default: {
            throw std::runtime_error("nud: unsupported token " + std::string(token_name[static_cast<int>(tok)]));
        };
        }
    }
};

template<typename T>
struct led {
    using token_t = T;
    using value_t = typename T::value_t;

    template <typename Parser>
    value_t operator()(Parser& parser, token_kind tok, value_t lhs, value_t rhs)
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

        case token_kind::pow:
            return std::pow(lhs, rhs);

        default:
            throw std::runtime_error("led: unsupported token " + std::string(token_name[static_cast<int>(tok)]));
        };
    }
};

struct identity {
    template<typename U>
    constexpr auto operator()(U&& v) const noexcept -> decltype(std::forward<U>(v))
    {
        return std::forward<U>(v);
    }
};

template <typename NUD, typename LED, typename CONV>
class parser {
public:
    using token_t = typename NUD::token_t;
    using value_t = typename token_t::value_t;

    parser(std::string const& infix, std::unordered_map<std::string, ulong> const& vars)
        : lexer_(infix)
        , vars_(vars)
    {
        static_assert(std::is_same_v<typename NUD::token_t, typename LED::token_t>);
    }

    value_t parse()
    {
        return parse_bp(0).value;
    }

    friend NUD;
    friend LED;

private:
    lexer<typename NUD::token_t, CONV> lexer_;
    std::unordered_map<std::string, ulong> vars_;

    token_t expr(value_t value) const { return token_t(token_kind::constant, value); }

    inline int precedence(token_kind tok) const {
        return token_precedence[tok];
    }

    token_t parse_bp(int rbp = 0, token_kind end = token_kind::eof)
    {
        NUD nud;
        LED led;

        auto left = lexer_.peek(); lexer_.consume();
        left.value = nud(*this, left.kind, left.value);

        while(true) {
            auto op = lexer_.peek();

            if (op.kind == end) {
                lexer_.consume();
                break;
            }

            auto lbp = token_precedence[op.kind];

            if (lbp <= rbp) {
                break;
            }

            auto bp = (op.kind >= token_kind::add && op.kind <= token_kind::div)
                ? lbp      // left-associative
                : lbp - 1; // right-associative

            lexer_.consume();

            auto right = parse_bp(bp, end);
            left = expr(led(*this, op.kind, left.value, right.value));
        }

        return left;
    };
};
} // namespace

#endif
