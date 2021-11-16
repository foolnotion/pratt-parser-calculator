#ifndef PRATT_PARSER_HPP
#define PRATT_PARSER_HPP

#include <cassert>
#include <cmath>
#include <unordered_map>
#include <optional>

#include "lexer.hpp"

namespace pratt {

static constexpr std::array<int, token_count> token_precedence = {
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

template <typename NUD, typename LED, typename CONV, typename Map = std::unordered_map<std::string, uint64_t>>
class parser {
public:
    using token_t = typename NUD::token_t;
    using value_t = typename token_t::value_t;

    parser(std::string const& infix, Map const& vars)
        : lexer_(infix)
        , vars_(vars)
    {
        static_assert(std::is_same_v<typename NUD::token_t, typename LED::token_t>);
    }

    inline value_t parse()
    {
        return parse_bp(0).value;
    }

    friend NUD;
    friend LED;

private:
    lexer<typename NUD::token_t, CONV> lexer_;
    Map vars_;

    template<typename T = typename Map::mapped_type>
    inline auto get_desc(std::string const& name) const -> std::optional<T> {
        auto it = vars_.find(name);
        return it == vars_.end()
            ? std::nullopt
            : std::make_optional(it->second);
    }

    inline token_t expr(value_t value) const { return token_t(token_kind::constant, value); }

    inline int precedence(token_kind tok) const {
        return token_precedence[tok];
    }

    inline token_t parse_bp(int rbp = 0, token_kind end = token_kind::eof)
    {
        NUD nud;
        LED led;

        auto left = lexer_.peek(); lexer_.consume();
        left.value = nud(*this, left.kind, left);

        if (left.kind == token_kind::lparen) {
            assert(lexer_.peek().kind == token_kind::rparen);
            lexer_.consume(); // eat rparen
        }

        while(true) {
            auto next = lexer_.peek();

            if (next.kind == end) {
                break;
            }

            auto lbp = token_precedence[next.kind];

            if (lbp <= rbp) {
                break;
            }

            auto bp = (next.kind >= token_kind::add && next.kind <= token_kind::div)
                ? lbp      // left-associative
                : lbp - 1; // right-associative

            lexer_.consume();

            auto right = parse_bp(bp, end);
            auto op = next.kind;
            left = expr(led(*this, op, left, right));
        }

        return left;
    };
};
} // namespace

#endif
