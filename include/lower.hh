#pragma once

#include <ast.hh>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <expected>
#include <memory>
#include <span>
#include <string>
#include <string_view>

namespace lower
{
    class ASTLowerer
    {
    public:
        ASTLowerer( llvm::LLVMContext &context, std::string_view module_name );

        //! creates `i64 name()` and lowers `body` into it. returns llvm's verifier
        //! message if the result isn't valid ir
        //! temporary until the ast has a function node
        std::expected< llvm::Function *, std::string > lower_function( std::string_view name, std::span< const ast::Stmt > body );

        const llvm::Module &module( ) const { return *m_module; }

        //! moves the finished LLVM module out of the lowerer
        std::unique_ptr< llvm::Module > take( ) { return std::move( m_module ); }

    private:
        //! visit the variants
        llvm::Value *lower_expr( const ast::Expr &expr );
        void         lower_stmt( const ast::Stmt &stmt );

        //! overload per node
        llvm::Value *lower( const ast::IntegerLiteral &literal );
        void         lower( const ast::ReturnStmt &stmt );

        llvm::LLVMContext              &m_context;
        std::unique_ptr< llvm::Module > m_module;
        llvm::IRBuilder<>               m_builder;
    };
} // namespace lowering
