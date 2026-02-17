use pyo3::prelude::*;
use pyo3::wrap_pyfunction;
use std::ffi::CString;
use std::os::raw::c_char;

// External C function declarations
extern "C" {
    fn zmij_c_double(value: f64, buffer: *mut c_char, buffer_size: usize) -> i32;
    fn zmij_c_float(value: f32, buffer: *mut c_char, buffer_size: usize) -> i32;
    fn zmij_read_doubles(filename: *const c_char, values: *mut f64, max_count: usize) -> i32;
    fn zmij_read_floats(filename: *const c_char, values: *mut f32, max_count: usize) -> i32;
    fn zmij_cpp_double(value: f64, buffer: *mut c_char, buffer_size: usize) -> i32;
    fn zmij_cpp_float(value: f32, buffer: *mut c_char, buffer_size: usize) -> i32;
}

// Rust implementations using dtoa crate
fn rust_dtoa_double(value: f64) -> String {
    let mut buffer = dtoa::Buffer::new();
    buffer.format(value).to_string()
}

fn rust_dtoa_float(value: f32) -> String {
    let mut buffer = dtoa::Buffer::new();
    buffer.format(value).to_string()
}

// Python-exposed functions for Rust implementation
#[pyfunction]
fn zmij_rust_double(value: f64) -> PyResult<String> {
    Ok(rust_dtoa_double(value))
}

#[pyfunction]
fn zmij_rust_float(value: f32) -> PyResult<String> {
    Ok(rust_dtoa_float(value))
}

// Python-exposed functions for C implementation
#[pyfunction]
fn zmij_c_double_py(value: f64) -> PyResult<String> {
    let mut buffer = vec![0u8; 128];
    unsafe {
        let len = zmij_c_double(value, buffer.as_mut_ptr() as *mut c_char, buffer.len());
        if len > 0 && (len as usize) < buffer.len() {
            buffer.truncate(len as usize);
            Ok(String::from_utf8_lossy(&buffer).to_string())
        } else {
            Err(pyo3::exceptions::PyRuntimeError::new_err(
                "C conversion failed",
            ))
        }
    }
}

#[pyfunction]
fn zmij_c_float_py(value: f32) -> PyResult<String> {
    let mut buffer = vec![0u8; 128];
    unsafe {
        let len = zmij_c_float(value, buffer.as_mut_ptr() as *mut c_char, buffer.len());
        if len > 0 && (len as usize) < buffer.len() {
            buffer.truncate(len as usize);
            Ok(String::from_utf8_lossy(&buffer).to_string())
        } else {
            Err(pyo3::exceptions::PyRuntimeError::new_err(
                "C conversion failed",
            ))
        }
    }
}

// Python-exposed functions for C++ implementation
#[pyfunction]
fn zmij_cpp_double_py(value: f64) -> PyResult<String> {
    let mut buffer = vec![0u8; 128];
    unsafe {
        let len = zmij_cpp_double(value, buffer.as_mut_ptr() as *mut c_char, buffer.len());
        if len > 0 && (len as usize) < buffer.len() {
            buffer.truncate(len as usize);
            Ok(String::from_utf8_lossy(&buffer).to_string())
        } else {
            Err(pyo3::exceptions::PyRuntimeError::new_err(
                "C++ conversion failed",
            ))
        }
    }
}

#[pyfunction]
fn zmij_cpp_float_py(value: f32) -> PyResult<String> {
    let mut buffer = vec![0u8; 128];
    unsafe {
        let len = zmij_cpp_float(value, buffer.as_mut_ptr() as *mut c_char, buffer.len());
        if len > 0 && (len as usize) < buffer.len() {
            buffer.truncate(len as usize);
            Ok(String::from_utf8_lossy(&buffer).to_string())
        } else {
            Err(pyo3::exceptions::PyRuntimeError::new_err(
                "C++ conversion failed",
            ))
        }
    }
}

// File reading functions
#[pyfunction]
fn read_doubles_from_file(filename: String) -> PyResult<Vec<f64>> {
    let c_filename = CString::new(filename)?;
    let mut values = vec![0.0f64; 10000];
    
    unsafe {
        let count = zmij_read_doubles(
            c_filename.as_ptr(),
            values.as_mut_ptr(),
            values.len(),
        );
        
        if count < 0 {
            return Err(pyo3::exceptions::PyIOError::new_err(
                "Failed to read file",
            ));
        }
        
        values.truncate(count as usize);
        Ok(values)
    }
}

#[pyfunction]
fn read_floats_from_file(filename: String) -> PyResult<Vec<f32>> {
    let c_filename = CString::new(filename)?;
    let mut values = vec![0.0f32; 10000];
    
    unsafe {
        let count = zmij_read_floats(
            c_filename.as_ptr(),
            values.as_mut_ptr(),
            values.len(),
        );
        
        if count < 0 {
            return Err(pyo3::exceptions::PyIOError::new_err(
                "Failed to read file",
            ));
        }
        
        values.truncate(count as usize);
        Ok(values)
    }
}

// Python module definition
#[pymodule]
fn zmij_playground(m: &Bound<'_, PyModule>) -> PyResult<()> {
    // Rust implementations
    m.add_function(wrap_pyfunction!(zmij_rust_double, m)?)?;
    m.add_function(wrap_pyfunction!(zmij_rust_float, m)?)?;
    
    // C implementations
    m.add_function(wrap_pyfunction!(zmij_c_double_py, m)?)?;
    m.add_function(wrap_pyfunction!(zmij_c_float_py, m)?)?;
    
    // C++ implementations
    m.add_function(wrap_pyfunction!(zmij_cpp_double_py, m)?)?;
    m.add_function(wrap_pyfunction!(zmij_cpp_float_py, m)?)?;
    
    // File I/O functions
    m.add_function(wrap_pyfunction!(read_doubles_from_file, m)?)?;
    m.add_function(wrap_pyfunction!(read_floats_from_file, m)?)?;
    
    Ok(())
}
