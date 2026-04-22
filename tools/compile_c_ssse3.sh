#!/usr/bin/env bash
# Quick-compile a C source file with SSSE3 into a shared library (.so) and assembly (.s).
# Usage: ./compile_c_ssse3.sh <source.c> [output_name]
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/compile_core.sh" --lang c --label "C, SSSE3" "$@" -- -mssse3
