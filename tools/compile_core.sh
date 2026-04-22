#!/usr/bin/env bash
# Core compile helper — called by language/ISA-specific wrapper scripts.
# Usage: compile_core.sh --lang <c|cpp> [--label <label>] <source> [output_name] [-- <extra_flags...>]
#
# --lang   c   : compile as C   (uses $CC,  defaults to clang,   -std=gnu17)
# --lang   cpp : compile as C++ (uses $CXX, defaults to clang++, -std=c++17)
# --label      : descriptive string shown in the "=== Compiling ===" banner
# --           : everything after this is appended to the compiler flags
#
# Output goes to build/libs/ relative to this script's parent directory.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUTPUT_DIR="${SCRIPT_DIR}/../build/libs"

LANG=""
LABEL=""
EXTRA_FLAGS=()
POSITIONAL=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --lang)  LANG="$2";  shift 2 ;;
        --label) LABEL="$2"; shift 2 ;;
        --)      shift; EXTRA_FLAGS=("$@"); break ;;
        *)       POSITIONAL+=("$1"); shift ;;
    esac
done

if [[ -z "$LANG" ]]; then
    echo "Error: --lang <c|cpp> is required" >&2
    exit 1
fi

if [[ ${#POSITIONAL[@]} -lt 1 ]]; then
    echo "Usage: $0 --lang <c|cpp> [--label <label>] <source> [output_name] [-- extra_flags...]" >&2
    exit 1
fi

SRC="${POSITIONAL[0]}"
if [[ ! -f "$SRC" ]]; then
    echo "Error: source file not found: $SRC" >&2
    exit 1
fi

if [[ "$LANG" == "c" ]]; then
    COMPILER="${CC:-clang}"
    BASE_FLAGS=(
        -std=gnu17
        -O3
        -DNDEBUG
        -fPIC
        -fno-stack-protector
        -fomit-frame-pointer
    )
    if [[ ${#POSITIONAL[@]} -ge 2 ]]; then
        NAME="${POSITIONAL[1]}"
    else
        NAME="$(basename "$SRC" .c)"
    fi
    : "${LABEL:=C}"
elif [[ "$LANG" == "cpp" ]]; then
    COMPILER="${CXX:-clang++}"
    INCLUDE_DIR="$(dirname "$SRC")"
    BASE_FLAGS=(
        -std=c++17
        -O3
        -DNDEBUG
        -fPIC
        -fvisibility=hidden
        -fno-stack-protector
        -fomit-frame-pointer
        "-I${INCLUDE_DIR}"
    )
    if [[ ${#POSITIONAL[@]} -ge 2 ]]; then
        NAME="${POSITIONAL[1]}"
    else
        BASE="$(basename "$SRC")"
        NAME="${BASE%.cpp}"
        NAME="${NAME%.cc}"
        NAME="${NAME%.cxx}"
    fi
    : "${LABEL:=C++}"
else
    echo "Error: --lang must be 'c' or 'cpp', got: $LANG" >&2
    exit 1
fi

ALL_FLAGS=("${BASE_FLAGS[@]}" "${EXTRA_FLAGS[@]}")

mkdir -p "$OUTPUT_DIR"

echo "=== Compiling $SRC ($LABEL) ==="
echo "  Compiler: $COMPILER"

# Shared library
set -x
"$COMPILER" "${ALL_FLAGS[@]}" -shared -o "${OUTPUT_DIR}/lib${NAME}.so" "$SRC"
{ set +x; } 2>/dev/null

# Assembly
set -x
"$COMPILER" "${ALL_FLAGS[@]}" -S -g -o "${OUTPUT_DIR}/${NAME}.s" "$SRC"
{ set +x; } 2>/dev/null

echo "  -> ${OUTPUT_DIR}/lib${NAME}.so"
echo "  -> ${OUTPUT_DIR}/${NAME}.s"
