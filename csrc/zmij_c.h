#ifndef ZMIJ_C_H
#define ZMIJ_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// Convert double to string, returns length written
int zmij_c_double(double value, char* buffer, size_t buffer_size);

// Convert float to string, returns length written
int zmij_c_float(float value, char* buffer, size_t buffer_size);

// Read doubles from file, one per line
// Returns number of values read
int zmij_read_doubles(const char* filename, double* values, size_t max_count);

// Read floats from file, one per line
// Returns number of values read
int zmij_read_floats(const char* filename, float* values, size_t max_count);

#ifdef __cplusplus
}
#endif

#endif // ZMIJ_C_H
