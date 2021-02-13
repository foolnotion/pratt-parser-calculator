#ifndef PRATT_LEXER_HPP
#define PRATT_LEXER_HPP

#include <algorithm>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pratt {

enum token_kind : int {
    lparen,
    rparen,
    add,
    sub,
    mul,
    div,
    log,
    exp,
    sin,
    cos,
    tan,
    sqrt,
    cbrt,
    square,
    pow,
    variable,
    constant,
    eof
};

constexpr int token_count = token_kind::eof + 1;

std::array<const char*, token_count> token_name = {
    "(",
    ")",
    "+",
    "-",
    "*",
    "/",
    "log",
    "exp",
    "sin",
    "cos",
    "tan",
    "sqrt",
    "cbrt",
    "square",
    "^",
    "var",
    "C",
    "EOF"
};

const std::unordered_map<std::string_view, token_kind> token_map = {
    { "(", token_kind::lparen },
    { ")", token_kind::rparen },
    { "+", token_kind::add },
    { "-", token_kind::sub },
    { "*", token_kind::mul },
    { "/", token_kind::div },
    { "log", token_kind::log },
    { "exp", token_kind::exp },
    { "sin", token_kind::sin },
    { "cos", token_kind::cos },
    { "tan", token_kind::tan },
    { "sqrt", token_kind::sqrt },
    { "cbrt", token_kind::cbrt },
    { "square", token_kind::square },
    { "^", token_kind::pow }
};

template <char... args>
inline bool is(char c) { return ((c == args) || ...); }

constexpr char lp = '(';
constexpr char rp = ')';
constexpr char sp = ' ';

template<typename T>
struct token {
    using value_t = T;

    token_kind kind;  // token kind
    T value;     // value for terminals
    std::string name; // name (for variables)

    token(token_kind kind_ = token_kind::eof, T val_ = T(), std::string const& name_ = "")
        : kind(kind_)
        , value(val_)
        , name(name_)
    {
    }

    template <token_kind... args>
    inline bool is() const noexcept { return ((kind == args) || ...); }

    friend std::ostream& operator<<(std::ostream& os, token const& tok)
    {
        switch (tok.kind) {
        case token_kind::constant: {
            os << tok.value;
            break;
        }
        case token_kind::variable: {
            os << tok.name;
            break;
        }
        default: {
            os << token_name[static_cast<int>(tok.kind)];
        }
        }
        return os;
    }
};

template<typename T, typename V>
class lexer {
public:
    lexer(std::string const& infix)
        : expr_(infix)
        , pos_(0)
    {
    }

    T peek() const
    {
        auto [t, _] = next();
        return t;
    }

    void consume()
    {
        auto [_, i] = next();
        pos_ = i;
    }

    bool eof() const { return pos_ >= expr_.size(); }

    void expect(token_kind k) const { assert(peek().kind == k); }

    std::vector<T> tokenize()
    {
        std::vector<T> tokens;
        do {
            tokens.push_back(peek());
            consume();
        } while (tokens.back().kind != token_kind::eof);
        return tokens;
    }

    void reset()
    {
        pos_ = 0;
    }

private:
    // returns a new token and index
    std::tuple<T, size_t> next() const
    {
        if (pos_ >= expr_.size()) {
            return { T(token_kind::eof), expr_.size() };
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

    T parse(std::string_view sv) const
    {
        // check if we can match a known token name
        if (auto it = token_map.find(sv); it != token_map.end()) {
            return T(it->second);
        }

        // check if we can match a number
        char* end;
        double val = std::strtod(sv.data(), &end);
        auto ok = std::isspace(*end) || end == sv.data() + sv.size();
        if (ok) {
            T t(token_kind::constant);
            t.value = V{}(val);
            return t;
        }

        // check if we can match a variable name (all chars are alphanumeric, the first char is a letter)
        if (std::isalpha(sv.front()) && std::all_of(sv.begin(), sv.end(), [](auto c) { return std::isalnum(c); })) {
            T t(token_kind::variable);
            t.name = std::string(sv.begin(), sv.end());
            return t;
        }

        return T(token_kind::eof);
    }

    std::string expr_;
    size_t pos_;
};

} // namespace
#endif
