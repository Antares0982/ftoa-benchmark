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

assert "static inline char* xjb64(" in content, "Could not find static inline xjb64"
assert "static inline char* xjb32(" in content, "Could not find static inline xjb32"

# Comment out the existing xjb_ftoa export functions
old_exports = """char* xjb_ftoa(float v, char* buf) {
    return xjb::xjb32(v, buf);
}
char* xjb_ftoa(double v, char* buf) {
    return xjb::xjb64(v, buf);
}"""
assert old_exports in content, "Could not find xjb_ftoa export functions"
commented = "\n".join("// " + line for line in old_exports.splitlines())
content = content.replace(old_exports, commented, 1)

# Append FFI exports after the end of namespace xjb
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
