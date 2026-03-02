#!/usr/bin/env bash
# Quick-compile a C source file with SSE4.1 into a shared library (.so) and assembly (.s).
# Usage: ./compile_c_sse4.sh <source.c> [output_name]
#
# If output_name is omitted, it is derived from the source filename:
#   foo.c -> libfoo.so + foo.s
#
# Respects $CC (defaults to clang). Output goes to build/libs/.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUTPUT_DIR="${SCRIPT_DIR}/../build/libs"

CC="${CC:-clang}"

if [ $# -lt 1 ]; then
    echo "Usage: $0 <source.c> [output_name]" >&2
    exit 1
fi

SRC="$1"
if [ ! -f "$SRC" ]; then
    echo "Error: source file not found: $SRC" >&2
    exit 1
fi

# Derive output name
if [ $# -ge 2 ]; then
    NAME="$2"
else
    NAME="$(basename "$SRC" .c)"
fi

mkdir -p "$OUTPUT_DIR"

COMMON_FLAGS=(
    -std=gnu17
    -O3
    -DNDEBUG
    -fPIC
    -fno-stack-protector
    -fomit-frame-pointer
    -msse4.1
    -DZMIJ_USE_SSE4_1=1
)

echo "=== Compiling $SRC (C, SSE4.1) ==="
echo "  Compiler: $CC"

# Shared library
set -x
"$CC" "${COMMON_FLAGS[@]}" -shared -o "${OUTPUT_DIR}/lib${NAME}.so" "$SRC"
{ set +x; } 2>/dev/null

# Assembly
set -x
"$CC" "${COMMON_FLAGS[@]}" -S -g -o "${OUTPUT_DIR}/${NAME}.s" "$SRC"
{ set +x; } 2>/dev/null

echo "  -> ${OUTPUT_DIR}/lib${NAME}.so"
echo "  -> ${OUTPUT_DIR}/${NAME}.s"
