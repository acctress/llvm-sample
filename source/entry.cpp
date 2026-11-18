#include <ast.hh>
#include <cstdio>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include <lower.hh>
#include <print>

std::int32_t main( )
{
    //! return 69 + 92;
    const std::array< ast::Stmt, 1 > body = { ast::ReturnStmt { ast::BinaryExpr {
        .op  = ast::BinaryExpr::BinaryOp::Add,
        .lhs = std::make_unique< ast::Expr >( ast::IntegerLiteral { 69 } ),
        .rhs = std::make_unique< ast::Expr >( ast::IntegerLiteral { 92 } ),
    } } };

    llvm::LLVMContext context;
    lower::ASTLowerer lowerer( context, "llvm-sample" );

    if ( auto result = lowerer.lower_function( "foo", body ); !result )
    {
        std::println( stderr, "ast lowering failed:\n{}", result.error( ) );
        return 1;
    }

    lowerer.module( ).print( llvm::outs( ), nullptr );
}
