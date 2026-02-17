#include "zmij_cpp.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <limits>

extern "C" {

// C++ implementation using stringstream with max precision
int zmij_cpp_double(double value, char* buffer, size_t buffer_size) {
    if (buffer_size == 0) return 0;
    
    // Handle special cases
    if (std::isnan(value)) {
        strncpy(buffer, "NaN", buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
        return 3;
    }
    if (std::isinf(value)) {
        if (value < 0) {
            strncpy(buffer, "-Infinity", buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            return 9;
        } else {
            strncpy(buffer, "Infinity", buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            return 8;
        }
    }
    
    // Use stringstream with maximum precision
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
    std::string result = oss.str();
    
    size_t len = result.length();
    if (len >= buffer_size) {
        len = buffer_size - 1;
    }
    
    memcpy(buffer, result.c_str(), len);
    buffer[len] = '\0';
    
    return static_cast<int>(len);
}

int zmij_cpp_float(float value, char* buffer, size_t buffer_size) {
    if (buffer_size == 0) return 0;
    
    // Handle special cases
    if (std::isnan(value)) {
        strncpy(buffer, "NaN", buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
        return 3;
    }
    if (std::isinf(value)) {
        if (value < 0) {
            strncpy(buffer, "-Infinity", buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            return 9;
        } else {
            strncpy(buffer, "Infinity", buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            return 8;
        }
    }
    
    // Use stringstream with maximum precision for float
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<float>::max_digits10) << value;
    std::string result = oss.str();
    
    size_t len = result.length();
    if (len >= buffer_size) {
        len = buffer_size - 1;
    }
    
    memcpy(buffer, result.c_str(), len);
    buffer[len] = '\0';
    
    return static_cast<int>(len);
}

} // extern "C"
