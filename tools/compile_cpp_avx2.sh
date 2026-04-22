#!/usr/bin/env bash
# Quick-compile a C++ source file with AVX2 into a shared library (.so) and assembly (.s).
# Usage: ./compile_cpp_avx2.sh <source.cpp> [output_name]
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/compile_core.sh" --lang cpp --label "C++, AVX2" "$@" -- -mavx2 -DZMIJ_USE_SSE4_1=1
