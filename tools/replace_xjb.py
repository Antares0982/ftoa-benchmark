#!/usr/bin/env python
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("file", help="Path to the file to edit.")
parser.add_argument(
    "--output",
    "-o",
    help="Path to the output file. If not provided, the input file will be overwritten.",
    required=False,
    type=str,
    default=None,
)
args = parser.parse_args()

with open(args.file, "r") as f:
    content = f.read()

# 1. Replace "// static inline\n<ws>char *xjb64(" with "static inline\n<ws>char *xjb64("
old1 = "// static inline\n\tchar *xjb64("
assert old1 in content, "Could not find commented-out static inline before xjb64"
content = content.replace(old1, "static inline\n\tchar *xjb64(", 1)

# 2. Replace "// static inline\n<ws>char *xjb32(" with "static inline\n<ws>char *xjb32("
old2 = "// static inline\n\tchar *xjb32("
assert old2 in content, "Could not find commented-out static inline before xjb32"
content = content.replace(old2, "static inline\n\tchar *xjb32(", 1)

# 3. Append FFI exports after the end of namespace xjb
content += """

// ---- Begin playground FFI exports ----
// These extern "C" wrappers are appended for the benchmark playground.
// They live in the same TU so the compiler inlines the actual algorithm.

extern "C" {

__attribute__((visibility("default"))) char* xjb32(
    float value, char* buffer) {
  return xjb::xjb32(value, buffer);
}

__attribute__((visibility("default"))) char* xjb64(
    double value, char* buffer) {
  return xjb::xjb64(value, buffer);
}

}  // extern "C"
"""

if args.output is not None:
    with open(args.output, "w") as f:
        f.write(content)
    print(f"File written to {args.output}")
else:
    print(content)
