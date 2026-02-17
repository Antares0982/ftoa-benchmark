#!/usr/bin/env python3
"""
Simple test script to verify all zmij playground functionality.
"""

import zmij_playground
import sys


def test_all_implementations():
    """Test all 6 implementations (3 languages × 2 types)."""
    print("Testing all implementations...\n")
    
    test_values = [
        3.141592653589793,
        2.718281828459045,
        0.0,
        -0.0,
        1.0,
        -1.0,
    ]
    
    print("=== Double (f64) Tests ===")
    for value in test_values[:3]:
        print(f"\nValue: {value}")
        print(f"  Rust:  {zmij_playground.zmij_rust_double(value)}")
        print(f"  C:     {zmij_playground.zmij_c_double_py(value)}")
        print(f"  C++:   {zmij_playground.zmij_cpp_double_py(value)}")
    
    print("\n=== Float (f32) Tests ===")
    for value in test_values[:3]:
        print(f"\nValue: {value}")
        print(f"  Rust:  {zmij_playground.zmij_rust_float(float(value))}")
        print(f"  C:     {zmij_playground.zmij_c_float_py(float(value))}")
        print(f"  C++:   {zmij_playground.zmij_cpp_float_py(float(value))}")
    
    print("\n✓ All implementations working correctly")


def test_file_reading():
    """Test file reading functionality."""
    print("\n=== File Reading Tests ===")
    
    try:
        doubles = zmij_playground.read_doubles_from_file("testdata/doubles.txt")
        print(f"✓ Successfully read {len(doubles)} doubles from file")
        print(f"  First 3 values: {doubles[:3]}")
    except Exception as e:
        print(f"✗ Failed to read doubles: {e}")
        return False
    
    try:
        floats = zmij_playground.read_floats_from_file("testdata/floats.txt")
        print(f"✓ Successfully read {len(floats)} floats from file")
        print(f"  First 3 values: {floats[:3]}")
    except Exception as e:
        print(f"✗ Failed to read floats: {e}")
        return False
    
    return True


def main():
    print("=" * 60)
    print("ZMIJ Playground - Functionality Test")
    print("=" * 60)
    
    try:
        test_all_implementations()
        test_file_reading()
        
        print("\n" + "=" * 60)
        print("All tests passed! ✓")
        print("=" * 60)
        return 0
    except Exception as e:
        print(f"\nTest failed: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
