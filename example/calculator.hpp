#ifndef PRATT_CALCULATOR_HPP
#define PRATT_CALCULATOR_HPP

#include "pratt-parser/parser.hpp"

namespace pratt::calculator {

enum operations { add, sub, mul, div, pow, exp, log, sin, cos, tan, sqrt, noop, square };

struct identity {
    template <typename U>
    constexpr auto operator()(U&& v) const noexcept -> decltype(std::forward<U>(v))
    {
        return std::forward<U>(v);
    }
};
struct nud {
    using token_t = token<double>;
    using value_t = typename token_t::value_t;

    template <typename Parser>
    auto operator()(Parser& parser, token_t const& tok, token_t const& left) -> value_t
    {
        auto bp = tok.precedence(); // binding power

        switch (tok.kind()) {
        case token_kind::constant: {
            return left.value();
        }

        case token_kind::dynamic: {
            switch (tok.opcode()) {
            case operations::sub: {
                return -parser.parse_bp(bp, token_kind::eof).value();
            }
            case operations::exp: {
                return std::exp(parser.parse_bp(bp, token_kind::eof).value());
            }
            case operations::log: {
                return std::log(parser.parse_bp(bp, token_kind::eof).value());
            }
            case operations::sin: {
                return std::sin(parser.parse_bp(bp, token_kind::eof).value());
            }
            case operations::cos: {
                return std::cos(parser.parse_bp(bp, token_kind::eof).value());
            }
            case operations::tan: {
                return std::tan(parser.parse_bp(bp, token_kind::eof).value());
            }
            case operations::sqrt: {
                return std::sqrt(parser.parse_bp(bp, token_kind::eof).value());
            }
            case operations::square: {
                 auto v = parser.parse_bp(bp, token_kind::eof).value();
                 return v * v;
            }
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
            throw std::runtime_error("nud: unsupported token " + std::string(tok.name()) + " " + tok.to_string());
        };
        }
        // unreachable
    }
};

struct led {
    using token_t = token<double>;
    using value_t = typename token_t::value_t;

    template <typename Parser>
    auto operator()(Parser& /*unused*/, token_t const& tok, token_t const& left, token_t const& right) -> value_t
    {
        auto lhs = left.value();
        auto rhs = right.value();

        switch (tok.kind()) {
        case token_kind::dynamic:
            switch (tok.opcode()) {
            case operations::add: {
                return lhs + rhs;
            }
            case operations::sub: {
                return lhs - rhs;
            }
            case operations::mul: {
                return lhs * rhs;
            }
            case operations::div: {
                return lhs / rhs;
            }
            case operations::pow: {
                return std::pow(lhs, rhs);
            }
            default: {
                throw std::runtime_error("led: unknown dynamic node opcode " + std::to_string(tok.opcode()));
            }
            }
            break;

        default:
            throw std::runtime_error("led: unsupported token " + std::string(tok.name()));
        };
    }
};

} // namespace pratt::calculator

#endif
