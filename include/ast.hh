#pragma once

#include <cstdint>
#include <variant>

namespace ast
{
    //! expressions

    //! an integer constant, e.g. `42`
    struct IntegerLiteral
    {
        std::int64_t value;
    };

    //! every kind of expression.
    using Expr = std::variant< IntegerLiteral >;

    //! statements

    //! `return <value>;`
    struct ReturnStmt
    {
        Expr value;
    };

    //! every kind of statement. add a struct above and list it here.
    using Stmt = std::variant< ReturnStmt >;
} //! namespace ast
