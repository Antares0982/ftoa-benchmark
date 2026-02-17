#!/usr/bin/env python3
"""
ZMIJ Algorithm Performance Benchmark

This script benchmarks the performance of different zmij (dtoa) implementations:
- Rust implementation (using dtoa crate)
- C implementation
- C++ implementation

Each implementation is tested for both float and double types.
"""

import time
import zmij_playground
import sys
import os


def benchmark_implementation(name, func, values, iterations=1000):
    """Benchmark a specific implementation."""
    start_time = time.perf_counter()
    
    for _ in range(iterations):
        for value in values:
            result = func(value)
    
    end_time = time.perf_counter()
    elapsed = end_time - start_time
    
    ops_per_sec = (len(values) * iterations) / elapsed
    
    return {
        'name': name,
        'elapsed': elapsed,
        'ops_per_sec': ops_per_sec,
        'avg_time_ns': (elapsed / (len(values) * iterations)) * 1_000_000_000
    }


def test_correctness(implementations, test_values):
    """Test that all implementations produce reasonable output."""
    print("\n=== Correctness Test ===\n")
    
    for value in test_values[:5]:  # Test first 5 values
        print(f"Value: {value}")
        for name, func in implementations:
            result = func(value)
            print(f"  {name:20s}: {result}")
        print()


def run_benchmarks():
    """Run all benchmarks."""
    print("=" * 70)
    print("ZMIJ Algorithm Performance Benchmark")
    print("=" * 70)
    
    # Test double implementations
    print("\n=== Testing Double (f64) Implementations ===\n")
    
    # Try to load test data from file
    script_dir = os.path.dirname(os.path.abspath(__file__))
    testdata_file = os.path.join(script_dir, 'testdata', 'doubles.txt')
    
    try:
        test_doubles = zmij_playground.read_doubles_from_file(testdata_file)
        print(f"Loaded {len(test_doubles)} test values from {testdata_file}")
    except Exception as e:
        print(f"Warning: Could not load test data from file: {e}")
        print("Using default test values instead")
        test_doubles = [
            3.141592653589793,
            2.718281828459045,
            1.4142135623730951,
            0.0,
            1.0,
            -1.0,
            123.456,
            -789.012,
            1e10,
            1e-10
        ]
    
    double_impls = [
        ("Rust f64", zmij_playground.zmij_rust_double),
        ("C f64", zmij_playground.zmij_c_double_py),
        ("C++ f64", zmij_playground.zmij_cpp_double_py),
    ]
    
    # Correctness test
    test_correctness(double_impls, test_doubles)
    
    # Performance benchmark
    print("Running performance benchmarks (1000 iterations per value)...")
    results = []
    for name, func in double_impls:
        print(f"Benchmarking {name}...", end=" ", flush=True)
        result = benchmark_implementation(name, func, test_doubles, iterations=1000)
        results.append(result)
        print(f"Done!")
    
    print("\n=== Double (f64) Results ===\n")
    print(f"{'Implementation':<20} {'Total Time (s)':<15} {'Ops/sec':<15} {'Avg Time (ns)':<15}")
    print("-" * 70)
    
    for result in results:
        print(f"{result['name']:<20} {result['elapsed']:<15.4f} "
              f"{result['ops_per_sec']:<15,.0f} {result['avg_time_ns']:<15.2f}")
    
    # Test float implementations
    print("\n\n=== Testing Float (f32) Implementations ===\n")
    
    # Try to load test data from file
    testdata_file = os.path.join(script_dir, 'testdata', 'floats.txt')
    
    try:
        test_floats = zmij_playground.read_floats_from_file(testdata_file)
        print(f"Loaded {len(test_floats)} test values from {testdata_file}")
    except Exception as e:
        print(f"Warning: Could not load test data from file: {e}")
        print("Using default test values instead")
        test_floats = [float(v) for v in test_doubles]
    
    float_impls = [
        ("Rust f32", zmij_playground.zmij_rust_float),
        ("C f32", zmij_playground.zmij_c_float_py),
        ("C++ f32", zmij_playground.zmij_cpp_float_py),
    ]
    
    # Correctness test
    test_correctness(float_impls, test_floats)
    
    # Performance benchmark
    print("Running performance benchmarks (1000 iterations per value)...")
    results = []
    for name, func in float_impls:
        print(f"Benchmarking {name}...", end=" ", flush=True)
        result = benchmark_implementation(name, func, test_floats, iterations=1000)
        results.append(result)
        print(f"Done!")
    
    print("\n=== Float (f32) Results ===\n")
    print(f"{'Implementation':<20} {'Total Time (s)':<15} {'Ops/sec':<15} {'Avg Time (ns)':<15}")
    print("-" * 70)
    
    for result in results:
        print(f"{result['name']:<20} {result['elapsed']:<15.4f} "
              f"{result['ops_per_sec']:<15,.0f} {result['avg_time_ns']:<15.2f}")
    
    print("\n" + "=" * 70)
    print("Benchmark complete!")
    print("=" * 70)


if __name__ == '__main__':
    try:
        run_benchmarks()
    except Exception as e:
        print(f"\nError: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        sys.exit(1)
