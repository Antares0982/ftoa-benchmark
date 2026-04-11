#!/usr/bin/env bash
set -e
cmake --build build
./build/any_ftoa_benchmark \
	./build/libs/libxjb.so:xjb64:xjb32\
	./build/libs/libzmij_cpp.so:zmijcpp_detail_write_double:zmijcpp_detail_write_float\
	./build/libs/libzmij_c.so:zmij_detail_write_double:zmij_detail_write_float\
	./build/libs/libzmij_rust.so:zmijrust_detail_write_double:zmijrust_detail_write_float
