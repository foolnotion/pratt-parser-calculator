#ifndef PRATT_PARSER_HPP
#define PRATT_PARSER_HPP

#include <cassert>
#include <cmath>
#include <unordered_map>
#include <optional>

#include "lexer.hpp"

namespace pratt {

template <typename NUD, typename LED, typename CONV,
         typename TokenMap = std::unordered_map<std::string_view, typename NUD::token_t>,
         typename VarMap = std::unordered_map<std::string_view, size_t>>
class parser {
public:
    using token_t = typename NUD::token_t;
    using value_t = typename token_t::value_t;

    parser(std::string const& infix, TokenMap const& token_map, VarMap const& var_map)
        : lexer_(infix, token_map)
        , vars_(var_map)
    {
        static_assert(std::is_same_v<typename NUD::token_t, typename LED::token_t>, "The NUD and LED operations must use the same token type.");
    }

    inline auto parse() -> value_t
    {
        return parse_bp(0).value();
    }

    friend NUD;
    friend LED;

private:
    lexer<typename NUD::token_t, CONV, TokenMap> lexer_;
    VarMap vars_;

    template<typename T = typename VarMap::mapped_type>
    inline auto get_desc(std::string const& name) const -> std::optional<T> {
        auto it = vars_.find(name);
        return it == vars_.end()
            ? std::nullopt
            : std::make_optional(it->second);
    }

    inline auto expr(value_t value) const -> token_t {
        return token_t(token_kind::constant) = value;
    }

    inline auto parse_bp(int rbp = 0, token_kind end = token_kind::eof) -> token_t
    {
        NUD nud;
        LED led;

        auto left = lexer_.peek(); lexer_.consume();
        left.value() = nud(*this, left, left);

        if (left.kind() == token_kind::lparen) {
            assert(lexer_.peek().kind() == token_kind::rparen);
            lexer_.consume(); // eat rparen
        }

        while(true) {
            auto next = lexer_.peek();

            if (next.kind() == end) {
                break;
            }

            auto lbp = next.precedence();

            if (lbp <= rbp) {
                break;
            }

            int bp{0};
            if (next.is_left_associative()) {
                bp = lbp;
            } else if (next.is_right_associative()) {
                bp = lbp - 1;
            }

            lexer_.consume();

            auto right = parse_bp(bp, end);
            auto op = next;
            left = expr(led(*this, op, left, right));
        }

        return left;
    };
};
} // namespace pratt

#endif
