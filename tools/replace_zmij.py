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

index1 = content.find("auto write(")
assert index1 > 0
assert content[index1 - 1] == "\n"
part1 = content[:index1]
content = content[index1:]
newcontent = part1 + "ZMIJ_INLINE "
index2 = content.find("template auto write(")
assert index2 > 0
assert content[index2 - 1] == "\n"
newcontent += content[:index2] + "// "
content = content[index2:]
index3 = content.find("template auto write(", 1)
assert index3 > 0
assert content[index3 - 1] == "\n"
newcontent += content[:index3] + "// " + content[index3:]
newcontent += """
// ---- Begin playground FFI exports ----
// These extern "C" wrappers are appended for the benchmark playground.
// They live in the same TU so the compiler inlines the actual algorithm.

extern "C" {

__attribute__((visibility("default"))) char* zmijcpp_detail_write_float(
    float value, char* buffer) {
  return zmij::detail::write(value, buffer);
}

__attribute__((visibility("default"))) char* zmijcpp_detail_write_double(
    double value, char* buffer) {
  return zmij::detail::write(value, buffer);
}

}  // extern "C"
"""
if args.output is not None:
    with open(args.output, "w") as f:
        f.write(newcontent)
    print(f"File written to {args.output}")
else:
    print(newcontent)
