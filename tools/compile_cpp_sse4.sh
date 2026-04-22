#!/usr/bin/env bash
# Quick-compile a C++ source file with SSE4.1 into a shared library (.so) and assembly (.s).
# Usage: ./compile_cpp_sse4.sh <source.cpp> [output_name]
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/compile_core.sh" --lang cpp --label "C++, SSE4.1" "$@" -- -msse4.1 -DZMIJ_USE_SSE4_1=1
