#ifndef ZMIJ_CPP_H
#define ZMIJ_CPP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Convert double to string using C++ implementation
int zmij_cpp_double(double value, char* buffer, size_t buffer_size);

// Convert float to string using C++ implementation
int zmij_cpp_float(float value, char* buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif // ZMIJ_CPP_H
