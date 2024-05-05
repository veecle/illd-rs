//! Configure the paths for the build system.

use std::path::PathBuf;
use path_macro::path;

fn main() {
    let src = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap());
    
    let illd_base = path!(src / "src/iLLD_1_0_1_16_0__TC37A/Src/BaseSw");
    let illd_lib = path!(src / "src/iLLD_1_0_1_16_0__TC37A");

    println!("cargo:rustc-env=ILLD_BASE={}", illd_base.to_str().unwrap());
    println!("cargo:rustc-env=ILLD_LIB={}", illd_lib.to_str().unwrap());
}