#![no_main]

use libfuzzer_sys::fuzz_target;
use std::mem;

macro_rules! zmij_test {
    ($val:expr, $method:ident) => {
        match $val {
            val => {
                let mut buffer = zmij::Buffer::new();
                let string = buffer.$method(val);
                assert!(string.len() <= mem::size_of::<zmij::Buffer>());
                if val.is_finite() {
                    assert_eq!(val, string.parse().unwrap());
                }
            }
        }
    };
}

fuzz_target!(|inputs: (f64, bool)| {
    let (input, finite) = inputs;
    match (input, finite) {
        (val, false) => zmij_test!(val, format),
        (val, true) => zmij_test!(val, format_finite),
    }
});
