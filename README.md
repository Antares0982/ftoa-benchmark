# ZMIJ Algorithm Performance Playground

A performance testing playground for comparing zmij (dtoa - double to ASCII) algorithm implementations across Rust, C, and C++ with Python bindings.

## Overview

This project benchmarks different implementations of the zmij/dtoa algorithm:

- **Rust implementation**: Using the `dtoa` crate
- **C implementation**: Custom implementation using `snprintf`
- **C++ implementation**: Using `std::stringstream` with max precision

Each implementation supports both `float` (f32) and `double` (f64) types, providing 6 different conversion variants for comparison.

## Features

- ✅ Export Rust, C, and C++ implementations to Python
- ✅ Support for both float and double types (6 variants total)
- ✅ C interface for reading test data from text files
- ✅ Performance benchmarking framework
- ✅ Correctness testing for all implementations

## Prerequisites

- Rust toolchain (rustc, cargo)
- Python 3.7+
- C/C++ compiler (gcc/clang)
- pip

## Installation

1. Clone the repository:
```bash
git clone https://github.com/Antares0982/zmij-playground.git
cd zmij-playground
```

2. Install Python dependencies:
```bash
pip install setuptools-rust
```

3. Build and install the Python module:
```bash
pip install -e .
```

This will:
- Compile the C and C++ implementations
- Build the Rust library with PyO3 bindings
- Install the `zmij_playground` Python module

## Usage

### Running Benchmarks

Run the benchmark script to compare all implementations:

```bash
python benchmark.py
```

The benchmark will:
1. Load test values from `testdata/doubles.txt` and `testdata/floats.txt`
2. Test correctness by showing conversion results
3. Run performance benchmarks (1000 iterations per value)
4. Display results showing operations per second and average time

### Using in Python

```python
import zmij_playground

# Rust implementations
result = zmij_playground.zmij_rust_double(3.141592653589793)
result = zmij_playground.zmij_rust_float(3.14159)

# C implementations
result = zmij_playground.zmij_c_double_py(3.141592653589793)
result = zmij_playground.zmij_c_float_py(3.14159)

# C++ implementations
result = zmij_playground.zmij_cpp_double_py(3.141592653589793)
result = zmij_playground.zmij_cpp_float_py(3.14159)

# Load test data from files
doubles = zmij_playground.read_doubles_from_file("testdata/doubles.txt")
floats = zmij_playground.read_floats_from_file("testdata/floats.txt")
```

## Test Data Format

The test data files (`testdata/doubles.txt` and `testdata/floats.txt`) contain one numeric value per line:

```
3.141592653589793
2.718281828459045
1.4142135623730951
0.0
1.0
...
```

You can add your own test values to these files.

## API Reference

### Conversion Functions

All conversion functions take a numeric value and return a string representation:

- `zmij_rust_double(value: float) -> str` - Rust f64 conversion
- `zmij_rust_float(value: float) -> str` - Rust f32 conversion
- `zmij_c_double_py(value: float) -> str` - C f64 conversion
- `zmij_c_float_py(value: float) -> str` - C f32 conversion
- `zmij_cpp_double_py(value: float) -> str` - C++ f64 conversion
- `zmij_cpp_float_py(value: float) -> str` - C++ f32 conversion

### File I/O Functions

- `read_doubles_from_file(filename: str) -> List[float]` - Read doubles from file
- `read_floats_from_file(filename: str) -> List[float]` - Read floats from file

## Project Structure

```
zmij-playground/
├── src/
│   └── lib.rs              # Rust library with PyO3 bindings
├── csrc/
│   ├── zmij_c.h           # C implementation header
│   ├── zmij_c.c           # C implementation
│   ├── zmij_cpp.h         # C++ implementation header
│   └── zmij_cpp.cpp       # C++ implementation
├── testdata/
│   ├── doubles.txt        # Test data for doubles
│   └── floats.txt         # Test data for floats
├── build.rs               # Rust build script for C/C++
├── Cargo.toml             # Rust dependencies
├── pyproject.toml         # Python build configuration
├── setup.py               # Python package setup
└── benchmark.py           # Performance benchmark script
```

## Performance Notes

- The Rust implementation (`dtoa` crate) typically offers the best performance
- C implementation uses `snprintf` with precision specifiers
- C++ implementation uses `std::stringstream` with max precision
- All implementations handle special values (NaN, Infinity) correctly

## Development

To rebuild after making changes:

```bash
pip install -e . --force-reinstall --no-cache-dir
```

To run only the Rust tests:

```bash
cargo test
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## References

- Rust dtoa crate: https://github.com/dtolnay/dtoa
- C++ fmt library: https://github.com/fmtlib/fmt
- PyO3 (Rust-Python bindings): https://pyo3.rs/
