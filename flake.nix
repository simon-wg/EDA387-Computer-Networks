{
  description = "A Nix-flake-based C/C++ development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {
          inherit system;
          config = {allowUnfree = true;};
        };
      in {
        devShells = {
          default = pkgs.mkShell {
            packages = with pkgs; [
              # Compilers and build tools
              gcc
              clang
              cmake
              ninja # Fast build system alternative to make
              meson # Modern build system

              # Development tools
              clang-tools # clangd, clang-format, clang-tidy
              gdb
              lldb # LLVM debugger (alternative to gdb)
              valgrind # Memory debugging
              rr # Record and replay debugger

              # Static analysis and linting
              cppcheck
              clang-analyzer

              # Documentation
              doxygen
              graphviz # For doxygen graphs
              man
              man-pages
              man-pages-posix

              # Package managers and libraries
              conan
              vcpkg
              vcpkg-tool
              pkg-config # Essential for finding libraries

              # Testing frameworks
              gtest
              catch2
              criterion # C testing framework

              # Code coverage and profiling
              lcov
              gcovr # Alternative coverage tool
              perf-tools # Performance profiling

              # Utilities
              codespell
              bear # Generate compile_commands.json for clangd
              ccls # Alternative language server

              # Libraries your project uses
              readline
              termcap

              uv
            ];

            # Set up environment variables
            shellHook = ''
              echo "🚀 C/C++ Development Environment"
              echo "Available compilers: gcc $(gcc --version | head -1 | cut -d' ' -f4), clang $(clang --version | head -1 | cut -d' ' -f4)"
              echo "Build systems: cmake, ninja, meson"
              echo "Debuggers: gdb, lldb"
              echo "Static analysis: cppcheck, clang-tidy"
              echo ""

              # Set up compile_commands.json for better IDE support
              export CMAKE_EXPORT_COMPILE_COMMANDS=1

              # Useful aliases
              alias cmake-debug='cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1'
              alias cmake-release='cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1'
              alias ninja-debug='cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1'
            '';
          };

          # Minimal shell for CI/automated builds
          ci = pkgs.mkShell {
            packages = with pkgs; [
              gcc
              cmake
              ninja
              gtest
            ];
          };

          # Clang-focused development shell
          clang = pkgs.mkShell.override {stdenv = pkgs.clangStdenv;} {
            packages = with pkgs; [
              clang
              clang-tools
              lldb
              cmake
              ninja
              libcxx
            ];

            shellHook = ''
              echo "🦙 Clang-based C/C++ Environment"
              export CC=clang
              export CXX=clang++
            '';
          };
        };

        # Optional: Define packages for your actual projects
        packages = {
          # Example of how you might build your lsh project
          # lsh = pkgs.stdenv.mkDerivation {
          #   pname = "lsh";
          #   version = "0.1.0";
          #   src = ./.;
          #   nativeBuildInputs = [ pkgs.cmake ];
          #   buildInputs = with pkgs; [ readline termcap ];
          # };
        };
      }
    );
}
