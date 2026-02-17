#include "zmij_c.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// Simple but functional dtoa implementation for C
int zmij_c_double(double value, char* buffer, size_t buffer_size) {
    if (buffer_size == 0) return 0;
    
    // Handle special cases
    if (isnan(value)) {
        snprintf(buffer, buffer_size, "NaN");
        return 3;
    }
    if (isinf(value)) {
        if (value < 0) {
            snprintf(buffer, buffer_size, "-Infinity");
            return 9;
        } else {
            snprintf(buffer, buffer_size, "Infinity");
            return 8;
        }
    }
    
    // Use snprintf with %.17g for shortest representation
    int len = snprintf(buffer, buffer_size, "%.17g", value);
    return len < 0 ? 0 : len;
}

int zmij_c_float(float value, char* buffer, size_t buffer_size) {
    if (buffer_size == 0) return 0;
    
    // Handle special cases
    if (isnan(value)) {
        snprintf(buffer, buffer_size, "NaN");
        return 3;
    }
    if (isinf(value)) {
        if (value < 0) {
            snprintf(buffer, buffer_size, "-Infinity");
            return 9;
        } else {
            snprintf(buffer, buffer_size, "Infinity");
            return 8;
        }
    }
    
    // Use snprintf with %.9g for float (shorter precision)
    int len = snprintf(buffer, buffer_size, "%.9g", value);
    return len < 0 ? 0 : len;
}

int zmij_read_doubles(const char* filename, double* values, size_t max_count) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    size_t count = 0;
    char line[256];
    while (count < max_count && fgets(line, sizeof(line), file)) {
        double val;
        if (sscanf(line, "%lf", &val) == 1) {
            values[count++] = val;
        }
    }
    
    fclose(file);
    return (int)count;
}

int zmij_read_floats(const char* filename, float* values, size_t max_count) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    size_t count = 0;
    char line[256];
    while (count < max_count && fgets(line, sizeof(line), file)) {
        float val;
        if (sscanf(line, "%f", &val) == 1) {
            values[count++] = val;
        }
    }
    
    fclose(file);
    return (int)count;
}
