#ifndef PRATT_TOKEN_HPP
#define PRATT_TOKEN_HPP

#include <cstdint>
#include <string>
#include <numeric>
#include <sstream>

namespace pratt {

enum class associativity : uint8_t {
    none,
    left,
    right
};

enum class token_kind : uint8_t {
    lparen,
    rparen,
    dynamic,
    variable,
    constant,
    eof
};

constexpr int token_count = static_cast<uint8_t>(token_kind::eof) + 1;

template <char... Args>
inline auto is(char c) -> bool { return ((c == Args) || ...); }

constexpr char lp = '(';
constexpr char rp = ')';
constexpr char sp = ' ';

template<typename T>
class token {
    token_kind kind_;        // token kind
    T value_{};              // value for terminals
    std::string name_;       // name (for variables)
    size_t opcode_;          // unique identifier
    int precedence_;         // token precedence
    associativity associativity_;

    static constexpr size_t noop = std::numeric_limits<size_t>::max();

public:
    using value_t = T;

    explicit token(token_kind kind = token_kind::eof, std::string name = "", size_t opcode = noop, int precedence = 0, associativity assoc = associativity::none)
        : kind_(kind)
        , name_(std::move(name))
        , opcode_(opcode)
        , precedence_(precedence)
        , associativity_(assoc) 
    {
    }

    [[nodiscard]] auto name() const -> std::string const& { return name_; }
    [[nodiscard]] auto kind() const -> token_kind { return kind_; }
    auto value() const -> T const& { return value_; }
    auto value() -> T& { return value_; }
    [[nodiscard]] auto opcode() const -> size_t { return opcode_; }
    [[nodiscard]] auto precedence() const -> int { return precedence_; }
    [[nodiscard]] auto is_left_associative() const -> bool { return associativity_ == associativity::left; }
    [[nodiscard]] auto is_right_associative() const -> bool { return associativity_ == associativity::right; }

    auto operator=(T const& value) -> token& { value_ = value; return *this; }

    template <token_kind... Args>
    [[nodiscard]] inline auto is() const noexcept -> bool { return ((kind_ == Args) || ...); }

    [[nodiscard]] auto to_string() const -> std::string
    {
      std::ostringstream ss;
      ss << *this;
      return ss.str();
    }

    friend auto operator<<(std::ostream& os, token const& tok) -> std::ostream&
    {
        switch (tok.kind_) {
        case token_kind::constant: {
            os << tok.value_;
            break;
        }
        default: {
            os << tok.name_;
        }
        }
        return os;
    }
};

} // namespace pratt

#endif
