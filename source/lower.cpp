#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <lower.hh>

namespace lower
{
    ASTLowerer::ASTLowerer( llvm::LLVMContext &context, std::string_view module_name ) :
        m_context( context ),
        m_module( std::make_unique< llvm::Module >( module_name, context ) ),
        m_builder( context )
    {
    }

    std::expected< llvm::Function *, std::string > ASTLowerer::lower_function( const std::string_view             name,
                                                                               const std::span< const ast::Stmt > body )
    {
        auto *type     = llvm::FunctionType::get( m_builder.getInt64Ty( ), false );
        auto *function = llvm::Function::Create( type, llvm::Function::ExternalLinkage, name, *m_module );

        //! every function starts in an entry block; the builder appends instructions to it
        m_builder.SetInsertPoint( llvm::BasicBlock::Create( m_context, "entry", function ) );

        for ( const auto &stmt : body )
            lower_stmt( stmt );

        //! verifyFunction returns true when the function is broken
        std::string              error;
        llvm::raw_string_ostream stream( error );
        if ( llvm::verifyFunction( *function, &stream ) )
        {
            function->eraseFromParent( );
            return std::unexpected( std::move( error ) );
        }

        return function;
    }

    llvm::Value *ASTLowerer::lower_expr( const ast::Expr &expr )
    {
        return std::visit( [ this ]( const auto &node )
        {
            //! visit the expression node, this could be IntegerLiteral
            return lower( node );
        }, expr );
    }

    void ASTLowerer::lower_stmt( const ast::Stmt &stmt )
    {
        std::visit( [ this ]( const auto &node )
        {
            //! visit the statement node, this could be ReturnStmt
            lower( node );
        }, stmt );
    }

    llvm::Value *ASTLowerer::lower( const ast::BinaryExpr &bin )
    {
        auto *lhs = lower_expr( *bin.lhs );
        auto *rhs = lower_expr( *bin.rhs );
        switch ( bin.op )
        {
            case ast::BinaryExpr::BinaryOp::Add : return m_builder.CreateAdd( lhs, rhs );
            case ast::BinaryExpr::BinaryOp::Sub : return m_builder.CreateSub( lhs, rhs );
            case ast::BinaryExpr::BinaryOp::Mul : return m_builder.CreateMul( lhs, rhs );
            case ast::BinaryExpr::BinaryOp::Div : return m_builder.CreateSDiv( lhs, rhs );
        }

        std::unreachable( );
    }

    llvm::Value *ASTLowerer::lower( const ast::BooleanLiteral &literal )
    {
        //! for BooleanLiteral, we can simply create a 1 bit boolean
        return llvm::ConstantInt::getSigned( m_builder.getInt1Ty( ), literal.value );
    }

    llvm::Value *ASTLowerer::lower( const ast::IntegerLiteral &literal )
    {
        //! for IntegerLiteral expression node,
        //! create a constant int of type int64
        return llvm::ConstantInt::getSigned( m_builder.getInt64Ty( ), literal.value );
    }

    void ASTLowerer::lower( const ast::ReturnStmt &stmt )
    {
        //! on ReturnStmt, simply call "CreateRet"
        //! and lower ReturnStmt.value for the return value
        m_builder.CreateRet( lower_expr( stmt.value ) );
    }
} // namespace lower
