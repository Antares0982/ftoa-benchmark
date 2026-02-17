# zmij-playground

A benchmark playground for comparing **three language implementations** (C, C++, Rust) of the [zmij](https://github.com/vitaut/zmij) dtoa (double-to-ASCII) algorithm.

Each implementation is compiled as a shared library (`.so`), loaded at runtime via `dlopen`, and benchmarked through a unified C ABI interface.

## Prerequisites

- **Nix** (recommended) — provides all build tools in a reproducible environment
- Or install manually: `clang`, `clang++`, `cargo` (Rust), `cmake`, `python3`

## Quick Start

```bash
# Enter development shell (provides clang, cargo, cmake, python3)
nix develop

# Build everything (libraries + benchmark executable)
cmake -B build
cmake --build build

# Run benchmark
./build/zmij_benchmark test_input.txt
```

## Preparing Test Input

The benchmark reads one floating-point number per line from a text file `test_input.txt`. You can also create a test file manually:

```
-62.136664999999937
65.851379000000009
3.14159
1.0e-10
```

## Building

### Full CMake Build

```bash
cmake -B build
cmake --build build
```

This will:
1. Invoke `build_libs.py` to compile three shared libraries (`libzmij_c.so`, `libzmij_cpp.so`, `libzmij_rust.so`)
2. Generate assembly files (`zmij_c.s`, `zmij_cpp.s`, `zmij_rust.s`) for inspection
3. Build the `zmij_benchmark` executable

### Libraries Only

```bash
python3 build_libs.py [--output-dir <dir>] [--sse41] [--cc clang]
```

### SSE4.1 Mode

Enable SSE4.1 instructions for all three implementations:

```bash
# Via CMake
cmake -B build -DCMAKE_C_COMPILER=clang -DSSE41=ON
cmake --build build

# Or directly
python3 build_libs.py --sse41
```

## Running Benchmarks

```bash
./build/zmij_benchmark [OPTIONS] <input.txt>
```

### Options

| Flag | Default | Description |
|------|---------|-------------|
| `--lib-dir <path>` | `build/libs/` | Directory containing `.so` files |
| `--rounds <N>` | `5000` | Number of benchmark rounds |
| `-h, --help` | | Show usage |

### Example

```bash
# Default: 5000 rounds
./build/zmij_benchmark test_input.txt

# Custom rounds
./build/zmij_benchmark --rounds 500 test_input.txt

# Use libraries from a different directory
./build/zmij_benchmark --lib-dir /path/to/libs test_input.txt
```

### Output Format

```
=== float benchmark (1000 rounds × 91932 values, 100 warmup) ===
  C               91932000 calls        769.49 ms total      8.37 ± 0.50 ns/call  (sink=1301891800)
  C++             91932000 calls        749.55 ms total      8.15 ± 0.08 ns/call  (sink=897356900)
  Rust            91932000 calls        798.53 ms total      8.69 ± 0.10 ns/call  (sink=897438300)
```

Each line reports: total calls, total time, **mean ± stddev** ns/call, and a sink value (to prevent dead-code elimination). A 100-round warmup runs before measurement begins.

## Assembly Analysis

After building, assembly files are placed in `build/libs/`.

## Uniform C ABI

All three libraries export the same two symbols:

```c
char *zmij_detail_write_float(float value, char *buffer);
char *zmij_detail_write_double(double value, char *buffer);
```

- `buffer` must be at least 16 bytes (float) or 25 bytes (double)
- Returns a pointer past the last written byte
- Libraries are loaded with `RTLD_LOCAL`, so identical symbol names don't conflict

## Design Notes

- **Same translation unit**: FFI exports are appended directly to the original source files (`zmij.cc` and `lib.rs`), not in separate wrapper files. This ensures the compiler naturally inlines the algorithm without requiring LTO.
- Default using clang as compiler since clang generates faster code.

## License

See [LICENSE](LICENSE). zmij licenses are placed in each source directory.
