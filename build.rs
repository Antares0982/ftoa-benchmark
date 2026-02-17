fn main() {
    // Compile C implementation
    cc::Build::new()
        .file("csrc/zmij_c.c")
        .warnings(false)
        .compile("zmij_c");
    
    // Compile C++ implementation
    cc::Build::new()
        .file("csrc/zmij_cpp.cpp")
        .cpp(true)
        .warnings(false)
        .compile("zmij_cpp");
    
    println!("cargo:rerun-if-changed=csrc/zmij_c.c");
    println!("cargo:rerun-if-changed=csrc/zmij_c.h");
    println!("cargo:rerun-if-changed=csrc/zmij_cpp.cpp");
    println!("cargo:rerun-if-changed=csrc/zmij_cpp.h");
}
