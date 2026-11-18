# llvm-sample

A small program which lowers a hand-built AST into LLVM IR, this sample can be built ontop of.

```cpp
const std::array< ast::Stmt, 1 > body = { ast::ReturnStmt { ast::BinaryExpr {
    .op  = ast::BinaryExpr::BinaryOp::Add,
    .lhs = std::make_unique< ast::Expr >( ast::IntegerLiteral { 69 } ),
    .rhs = std::make_unique< ast::Expr >( ast::IntegerLiteral { 92 } ),
} } };

...

auto result = lowerer.lower_function( "foo", body ); 
```

```llvmir
; ModuleID = 'llvm-sample'
source_filename = "llvm-sample"

define i64 @foo() {
entry:
  ret i64 161
}
```

## Requirements

- CMake 3.21+
- A C++23 compiler (tested with clang++ on Windows, with Ninja)
- LLVM, built as a library

## Setup (Windows)

The regular `LLVM-x.y.z-win64.exe` installer only contains the compiler, not the
libraries, so it can't be used to build this project.

1. Download `clang+llvm-<version>-x86_64-pc-windows-msvc.tar.xz` from the
   [LLVM releases page](https://github.com/llvm/llvm-project/releases) and extract it:

   ```bash
   tar -xf clang+llvm-<version>-x86_64-pc-windows-msvc.tar.xz -C C:\llvm
   ```

2. That archive needs zlib and zstd but doesn't include them. Install them with
   [vcpkg](https://github.com/microsoft/vcpkg):

   ```bash
   vcpkg install zlib zstd --triplet x64-windows-static
   ```

3. Configure and build, pointing CMake at LLVM and vcpkg:

   ```bash
   cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DLLVM_DIR=C:/llvm/clang+llvm-<version>-x86_64-pc-windows-msvc/lib/cmake/llvm -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static
   ```

   ```bash
   cmake --build build
   ```

   In CLion, put the same `-D...` options under
   *Settings -> Build, Execution, Deployment -> CMake -> CMake options*.

If it worked, the CMake output includes a line like `Found LLVM 23.1.5`.

## Setup (Linux / macOS)

Install your package manager's LLVM dev package (e.g. `llvm-dev` for debian/ubuntu
or `brew install llvm`), then:

```bash
cmake -S . -B build -DLLVM_DIR=<llvm>/lib/cmake/llvm
```

```bash
cmake --build build
```
