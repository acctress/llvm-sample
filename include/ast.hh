#pragma once

#include <cstdint>
#include <memory>
#include <variant>

namespace ast
{
    //! expressions

    //! a forward reference of the binary expression node
    struct BinaryExpr;

    //! an integer constant, e.g. `42`
    struct IntegerLiteral
    {
        std::int64_t value;
    };

    struct FloatLiteral
    {
        double value;
    };

    struct BooleanLiteral
    {
        bool value;
    };

    //! every kind of expression.
    using Expr = std::variant< BinaryExpr, IntegerLiteral, FloatLiteral, BooleanLiteral >;

    struct BinaryExpr
    {
        enum class BinaryOp : std::uint8_t
        {
            Add,
            Sub,
            Mul,
            Div,
        };

        BinaryOp                op {};
        std::unique_ptr< Expr > lhs { nullptr }, rhs { nullptr };
    };

    //! statements

    //! `return <value>;`
    struct ReturnStmt
    {
        Expr value;
    };

    //! every kind of statement. add a struct above and list it here.
    using Stmt = std::variant< ReturnStmt >;
} // namespace ast
