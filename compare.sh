#!/usr/bin/env bash
./tools/compile_cpp_sse4.sh ./zmij-cpp/zmij.cc zmijcpp
./tools/compile_cpp_sse4.sh ./sse4-improve/zmij.cc sse4_even_faster
./tools/compile_cpp_sse4.sh ./sse4-improve-ctz/zmij.cc sse4_even_faster_ctz
./build/any_dtoa_benchmark ./build/libs/libzmijcpp.so:zmijcpp_detail_write_double:skip ./build/libs/libsse4_even_faster.so:zmijcpp_detail_write_double:skip ./build/libs/libsse4_even_faster_ctz.so:zmijcpp_detail_write_double:skip
