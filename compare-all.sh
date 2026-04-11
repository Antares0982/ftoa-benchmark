#!/usr/bin/env bash
set -e
cmake --build build
./tools/compile_cpp.sh ./zmij-cpp-old/zmij.cc zmij-7afa3c
./tools/compile_cpp.sh ./zmij-cpp-old2/zmij.cc zmij-4baf80
./tools/compile_cpp.sh ./zmij-cpp-pr/zmij.cc zmij-pr120
./build/any_dtoa_benchmark \
	./build/libs/libxjb.so:xjb64:xjb32\
	./build/libs/libzmij_cpp.so:zmijcpp_detail_write_double:zmijcpp_detail_write_float\
	./build/libs/libzmij-pr120.so:zmijcpp_detail_write_double:zmijcpp_detail_write_float\
	./build/libs/libzmij-7afa3c.so:zmijcpp_detail_write_double:zmijcpp_detail_write_float\
	./build/libs/libzmij-4baf80.so:zmijcpp_detail_write_double:zmijcpp_detail_write_float\
	./build/libs/libzmij_c.so:zmij_detail_write_double:zmij_detail_write_float\
	./build/libs/libzmij_rust.so:zmijrust_detail_write_double:zmijrust_detail_write_float
