#!/usr/bin/env bash
# Quick-compile a C++ source file with x86-64-v3 into a shared library (.so) and assembly (.s).
# Usage: ./compile_cpp_x64v3.sh <source.cpp> [output_name]
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/compile_core.sh" --lang cpp --label "C++, x86-64-v3" "$@" -- -march=x86-64-v3 -DZMIJ_USE_SSE4_1=1
