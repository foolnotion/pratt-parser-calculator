#ifndef PRATT_SEXPR_HPP
#define PRATT_SEXPR_HPP

#include "pratt-parser/parser.hpp"

#include <sstream>

namespace pratt::sexpr {

enum operations { add,
    sub,
    mul,
    div,
    pow,
    exp,
    log,
    sin,
    cos,
    tan,
    sqrt,
    square,
    noop };

struct nud {
    using token_t = token<std::string>;
    using value_t = typename token_t::value_t;

    template <typename Parser>
    auto operator()(Parser& parser, token_t const& tok, token_t const& left) -> value_t
    {
        auto bp = tok.precedence(); // binding power

        switch (tok.kind()) {
        case token_kind::constant: {
            return left.value();
        }

        case token_kind::variable: {
            return left.name();
        }

        case token_kind::dynamic: {
            switch (tok.opcode()) {
            case operations::sub:
            case operations::exp:
            case operations::log:
            case operations::sin:
            case operations::cos:
            case operations::tan:
            case operations::sqrt:
                return "(" + tok.name() + " " + parser.parse_bp(bp, token_kind::eof).value() + ")";
            default: {
                throw std::runtime_error("led: unknown dynamic node opcode " + std::to_string(tok.opcode()));
            }
            }
            break;
        }

        case token_kind::lparen: {
            return parser.parse_bp(bp, token_kind::rparen).value();
        }

        default: {
            throw std::runtime_error("nud: unsupported token " + tok.to_string());
        };
            // unreachable
        }
    }
};

struct led {
    using token_t = token<std::string>;
    using value_t = token_t::value_t;

    template <typename Parser>
    auto operator()(Parser& /*unused*/, token_t const& tok, token_t const& left, token_t const& right) -> value_t
    {
        auto const& lhs = left.value();
        auto const& rhs = right.value();

        switch (tok.kind()) {

        case token_kind::dynamic:
            switch (tok.opcode()) {
            case operations::add:
            case operations::sub:
            case operations::mul:
            case operations::div:
            case operations::pow:
                return "(" + tok.name() + " " + lhs + " " + rhs + ")";
            }

        default:
            throw std::runtime_error("led: unsupported token " + tok.to_string());
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

} // namespace pratt::sexpr

#endif
