//! Stub library to package Infineon's iLLD drivers via Cargo.
#![no_std]

/// Path to the iLLD BaseSw directory.
pub const ILLD_BASE: &str = env!("ILLD_BASE");
/// Path to the iLLD pre-compiled library directory.
pub const ILLD_LIB: &str = env!("ILLD_LIB");