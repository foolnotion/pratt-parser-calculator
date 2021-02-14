# Pratt parser and calculator

[![Build Status](https://travis-ci.com/foolnotion/pratt-parser-calculator.svg?branch=main)](https://travis-ci.com/foolnotion/pratt-parser-calculator)

This is a simple operator precedence parser following the algorithm described by Pratt [[wikipedia](https://en.wikipedia.org/wiki/Operator-precedence_parser#cite_note-3)].

It supports a rather limited mathematical grammar but should be easily extensible. Its main distinctive feature is a modular design, which allows the user to supply the basic blocks for the parser to build the desired data structure (e.g., AST-like structures, S-expressions). This is done with the help of three functors:

- NUD (null denotation): define the operation applied to a token when the _left_ expression is null (e.g. negation) returning a `value_t`
- LED( left denotation): define the operation applied to a token when the _left_ expression exists (e.g. addition) returning a `value_t`
- CONV (convert): define the conversion from a basic value (e.g. a number parsed from the input string) into a `value_t`

The `value_t` type represents the token payload and is defined using a template parameter.

For example, to convert an infix string into an S-expression, one would implement:

```cpp
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
        auto lhs = left.value;
        auto rhs = right.value;

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
```

and then use the parser like this:
```cpp
using NUD  = pratt::sexpr::nud;
using LED  = pratt::sexpr::led;
using CONV = pratt::sexpr::conv;
pratt::parser<NUD, LED, CONV> p(input, {});
auto result = p.parse();
```

Examples of an expression calculator and an infix to prefix converter are found in the [src](https://github.com/foolnotion/pratt-parser-calculator/tree/main/src) folder. Note that the lexer is quite basic at the moment, so all symbols must be separate by spaces.
