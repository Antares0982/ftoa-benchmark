#!/usr/bin/env bash
set -e
./tools/compile_cpp.sh zmij-cpp/zmij.cc zmij
./tools/compile_cpp.sh xjb/ftoa.cpp xjb
./build/any_dtoa_benchmark ./build/libs/libzmij.so:zmijcpp_detail_write_double:zmijcpp_detail_write_float ./build/libs/libxjb.so:xjb64:xjb32
