#!/usr/bin/env bash
# Quick-compile a C source file into a shared library (.so) and assembly (.s).
# Usage: ./compile_c.sh <source.c> [output_name]
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/compile_core.sh" --lang c "$@"
