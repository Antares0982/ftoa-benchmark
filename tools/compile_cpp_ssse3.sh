#!/usr/bin/env bash
# Quick-compile a C++ source file with SSSE3 into a shared library (.so) and assembly (.s).
# Usage: ./compile_cpp_ssse3.sh <source.cpp> [output_name]
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/compile_core.sh" --lang cpp --label "C++, SSSE3" "$@" -- -mssse3
