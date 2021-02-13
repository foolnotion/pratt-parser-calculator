#ifndef PRATT_CALCULATOR_HPP
#define PRATT_CALCULATOR_HPP

#include "parser.hpp"

namespace pratt::calculator {

struct nud {
    using token_t = token<double>;
    using value_t = typename token_t::value_t;

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

struct led {
    using token_t = token<double>;
    using value_t = typename token_t::value_t;

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
    template <typename U>
    constexpr auto operator()(U&& v) const noexcept -> decltype(std::forward<U>(v))
    {
        return std::forward<U>(v);
    }
};

}

#endif
