#ifndef PRATT_SEXPR_HPP
#define PRATT_SEXPR_HPP

#include "parser.hpp"

#include <sstream>

namespace pratt::sexpr {

struct nud {
    using token_t = token<std::string>;
    using value_t = typename token_t::value_t;

    template <typename Parser>
    value_t operator()(Parser& parser, token_kind tok, token_t const& left)
    {
        auto bp = token_precedence[tok]; // binding power

        switch (tok) {
        case token_kind::constant: {
            return left.value;
        }

        case token_kind::variable: {
            return left.name;
        }

        case token_kind::sub:
        case token_kind::exp:
        case token_kind::log:
        case token_kind::sin:
        case token_kind::cos:
        case token_kind::tan:
        case token_kind::tanh:
        case token_kind::sqrt:
        case token_kind::cbrt:
        case token_kind::square: {
            return "(" + std::string(pratt::token_name[tok]) + " " + parser.parse_bp(bp, token_kind::eof).value + ")";
        }

        case token_kind::lparen: {
            return parser.parse_bp(bp, token_kind::rparen).value;
        }

        default: {
            throw std::runtime_error("nud: unsupported token " + std::string(token_name[static_cast<int>(tok)]));
        };
        }
    }
};

struct led {
    using token_t = token<std::string>;
    using value_t = token_t::value_t;

    template <typename Parser>
    value_t operator()(Parser& parser, token_kind tok, token_t const& left, token_t const& right)
    {
        auto const& lhs = left.value;
        auto const& rhs = right.value;

        switch (tok) {
        case token_kind::add:
        case token_kind::sub:
        case token_kind::mul:
        case token_kind::div:
        case token_kind::pow:
            return "(" + std::string(pratt::token_name[tok]) + " " + lhs + " " + rhs + ")";

        default:
            throw std::runtime_error("led: unsupported token " + std::string(token_name[static_cast<int>(tok)]));
        };
    }
};

struct conv {
    auto operator()(double v) const noexcept -> std::string 
    {
        std::ostringstream buf;
        buf << v;
        return buf.str();
    }
};
}

#endif
