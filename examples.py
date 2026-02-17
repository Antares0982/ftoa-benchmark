#!/usr/bin/env python3
"""
Example usage of the zmij playground library.

This demonstrates how to use the different implementations
and compare their results.
"""

import zmij_playground


def main():
    print("ZMIJ Playground - Usage Examples\n")
    print("=" * 60)
    
    # Example 1: Basic usage
    print("\n1. Basic Conversion Examples:")
    print("-" * 60)
    
    value = 3.141592653589793
    print(f"Converting {value}:")
    print(f"  Rust f64:  {zmij_playground.zmij_rust_double(value)}")
    print(f"  C f64:     {zmij_playground.zmij_c_double_py(value)}")
    print(f"  C++ f64:   {zmij_playground.zmij_cpp_double_py(value)}")
    
    # Example 2: Float conversions
    print("\n2. Float (f32) Conversions:")
    print("-" * 60)
    
    value_f = 2.71828
    print(f"Converting {value_f}:")
    print(f"  Rust f32:  {zmij_playground.zmij_rust_float(value_f)}")
    print(f"  C f32:     {zmij_playground.zmij_c_float_py(value_f)}")
    print(f"  C++ f32:   {zmij_playground.zmij_cpp_float_py(value_f)}")
    
    # Example 3: Special values
    print("\n3. Special Values:")
    print("-" * 60)
    
    import math
    special_values = [0.0, -0.0, float('inf'), float('-inf'), float('nan')]
    
    for val in special_values:
        try:
            rust_result = zmij_playground.zmij_rust_double(val)
            c_result = zmij_playground.zmij_c_double_py(val)
            print(f"  {val:10} -> Rust: {rust_result:10} C: {c_result}")
        except Exception as e:
            print(f"  {val:10} -> Error: {e}")
    
    # Example 4: Reading from files
    print("\n4. Reading Test Data from Files:")
    print("-" * 60)
    
    doubles = zmij_playground.read_doubles_from_file("testdata/doubles.txt")
    print(f"Loaded {len(doubles)} test doubles")
    print(f"First 5 values: {doubles[:5]}")
    
    # Example 5: Batch conversion
    print("\n5. Batch Conversion Performance:")
    print("-" * 60)
    
    test_values = [1.0, 2.0, 3.0, 4.0, 5.0]
    print("Converting batch of values with Rust:")
    for val in test_values:
        result = zmij_playground.zmij_rust_double(val)
        print(f"  {val} -> {result}")
    
    print("\n" + "=" * 60)
    print("Examples complete! Run 'python benchmark.py' for performance tests.")


if __name__ == '__main__':
    main()
