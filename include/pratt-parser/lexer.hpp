#ifndef PRATT_LEXER_HPP
#define PRATT_LEXER_HPP

#include <algorithm>
#include <cmath>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fast_float/fast_float.h"
#include "token.hpp"

namespace pratt {

template<typename TOKEN, typename CONV, typename MAP>
class lexer {
public:
    explicit lexer(std::string infix, MAP const& map)
        : token_map_(map)
        , expr_(std::move(infix))
        , pos_(0)
    {
    }

    inline auto peek() const -> TOKEN
    {
        auto [t, _] = next();
        return t;
    }

    inline void consume()
    {
        auto [_, i] = next();
        pos_ = i;
    }

    [[nodiscard]] inline auto eof() const -> bool { return pos_ >= expr_.size(); }

    inline void expect(token_kind k) const { assert(peek().kind == k); }

    inline auto tokenize() -> std::vector<TOKEN>
    {
        std::vector<TOKEN> tokens;
        do {
            tokens.push_back(peek());
            consume();
        } while (tokens.back().kind() != token_kind::eof);
        return tokens;
    }

    inline void reset()
    {
        pos_ = 0;
    }

private:
    // returns a new token and index
    inline auto next() const -> std::tuple<TOKEN, size_t>
    {
        if (pos_ >= expr_.size()) {
            return { TOKEN(token_kind::eof), expr_.size() };
        }

        auto i = pos_;

        while (i < expr_.size() && std::isspace(expr_[i])) {
            ++i;
        }

        if (is<lp, rp>(expr_[i])) {
            return { parse(std::string_view(expr_.data() + i, 1)), i + 1 };
        }

        auto j = i + 1;

        while (j < expr_.size() && !(std::isspace(expr_[j]) || is<lp, rp>(expr_[j]))) {
            ++j;
        }

        return { parse(std::string_view(expr_.data() + i, j - i)), j };
    }

    inline auto parse(std::string_view sv) const -> TOKEN
    {
        // check if we can match a known token name
        if (auto it = token_map_.find(sv); it != token_map_.end()) {
            return it->second;
        }

        // check if we can match a double value
        double result{0};
        auto answer = fast_float::from_chars(sv.data(), sv.data() + sv.size(), result);
        if(answer.ec == std::errc()) {
            return TOKEN(token_kind::constant) = conv_(result);
        }

        // check if we can match a variable name (all chars are alphanumeric, the first char is a letter)
        if (std::isalpha(sv.front()) && std::all_of(sv.begin(), sv.end(), [](auto c) { return std::isalnum(c) || is<'_'>(c); })) {
            TOKEN t(token_kind::variable, std::string(sv.begin(), sv.end()));
            return t;
        }

        return TOKEN(token_kind::eof);
    }

    MAP token_map_;
    CONV conv_;
    std::string expr_;
    size_t pos_;
};
} // namespace pratt
#endif
