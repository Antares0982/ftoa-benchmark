cmake_minimum_required(VERSION 3.16)
project(zmij-benchmark C)

option(SSE41 "Enable SSE4.1 for all zmij builds" OFF)
option(USE_GCC "Use gcc instead of clang" OFF)

# --------------------------------------------------------------------------- #
# Build shared libraries via build_libs.py
# --------------------------------------------------------------------------- #

set(LIB_OUTPUT_DIR "${CMAKE_BINARY_DIR}/libs")
set(PYTHON_SCRIPT "${CMAKE_SOURCE_DIR}/build_libs.py")

set(BUILD_LIBS_ARGS "${PYTHON_SCRIPT}" "--output-dir" "${LIB_OUTPUT_DIR}")

if(SSE41)
  list(APPEND BUILD_LIBS_ARGS "--sse41")
endif()
if(USE_GCC)
  list(APPEND BUILD_LIBS_ARGS "--cc" "gcc")
endif()

add_custom_command(
  OUTPUT "${LIB_OUTPUT_DIR}/libzmij_c.so" "${LIB_OUTPUT_DIR}/libzmij_cpp.so"
         "${LIB_OUTPUT_DIR}/libzmij_rust.so" "${LIB_OUTPUT_DIR}/libxjb.so"
  COMMAND python3 ${BUILD_LIBS_ARGS}
  DEPENDS "${PYTHON_SCRIPT}"
          "${CMAKE_SOURCE_DIR}/zmij-c/zmij.c"
          "${CMAKE_SOURCE_DIR}/zmij-c/zmij-c.h"
          "${CMAKE_SOURCE_DIR}/zmij-cpp/zmij.cc"
          "${CMAKE_SOURCE_DIR}/zmij-cpp/zmij.h"
          "${CMAKE_SOURCE_DIR}/zmij-rust/src/lib.rs"
          "${CMAKE_SOURCE_DIR}/zmij-rust/Cargo.toml"
          "${CMAKE_SOURCE_DIR}/xjb/ftoa.cpp"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMENT "Building zmij shared libraries via build_libs.py"
  VERBATIM)

add_custom_target(
  zmij_libs ALL
  DEPENDS "${LIB_OUTPUT_DIR}/libzmij_c.so" "${LIB_OUTPUT_DIR}/libzmij_cpp.so"
          "${LIB_OUTPUT_DIR}/libzmij_rust.so" "${LIB_OUTPUT_DIR}/libxjb.so")

# --------------------------------------------------------------------------- #
# Benchmark executable
# --------------------------------------------------------------------------- #

add_executable(zmij_benchmark src/benchmark.c)
target_link_libraries(zmij_benchmark PRIVATE dl m)
target_compile_options(zmij_benchmark PRIVATE -O2 -Wall -Wextra)
add_dependencies(zmij_benchmark zmij_libs)

# Make the lib output dir available as a compile definition so the benchmark can
# locate .so files at runtime by default.
target_compile_definitions(zmij_benchmark
                           PRIVATE DEFAULT_LIB_DIR="${LIB_OUTPUT_DIR}")
