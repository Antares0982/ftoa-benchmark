#!/usr/bin/env python3
"""Build zmij shared libraries (.so) and assembly files (.s) for C, C++, and Rust.

This script is fully portable — it assumes clang, clang++, and cargo are
available in PATH.
"""

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parent


def run(cmd: list[str], **kwargs) -> None:
    print(f"  → {' '.join(cmd)}", flush=True)
    subprocess.check_call(cmd, **kwargs)


# --------------------------------------------------------------------------- #
# C build
# --------------------------------------------------------------------------- #


def build_c(output_dir: Path, cc: str, extra_cflags: list[str]) -> None:
    print("\n=== Building zmij-c ===", flush=True)
    src = ROOT / "zmij-c" / "zmij.c"
    so_out = output_dir / "libzmij_c.so"
    asm_out = output_dir / "zmij_c.s"

    common = [
        cc,
        "-std=gnu17",
        "-O3",
        "-DNDEBUG",
        "-fPIC",
        "-fno-stack-protector",
        "-fomit-frame-pointer",
    ] + extra_cflags

    # shared library
    run(common + ["-shared", "-o", str(so_out), str(src)])
    # assembly
    run(common + ["-S", "-g", "-o", str(asm_out), str(src)])


# --------------------------------------------------------------------------- #
# C++ build
# --------------------------------------------------------------------------- #


def build_cpp(output_dir: Path, cxx: str, extra_cflags: list[str]) -> None:
    print("\n=== Building zmij-cpp ===", flush=True)
    cpp_dir = ROOT / "zmij-cpp"
    src = cpp_dir / "zmij.cc"
    so_out = output_dir / "libzmij_cpp.so"
    asm_out = output_dir / "zmij_cpp.s"

    # Single TU: zmij.cc contains both the algorithm and the appended
    # extern "C" FFI exports, so the compiler naturally inlines them.
    common = [
        cxx,
        "-std=c++17",
        "-O3",
        "-DNDEBUG",
        "-fPIC",
        "-fno-stack-protector",
        "-fomit-frame-pointer",
        f"-I{cpp_dir}",
    ] + extra_cflags

    # shared library
    run(common + ["-shared", "-o", str(so_out), str(src)])
    # assembly
    run(common + ["-S", "-g", "-o", str(asm_out), str(src)])


# --------------------------------------------------------------------------- #
# Rust build
# --------------------------------------------------------------------------- #


def build_rust(output_dir: Path, extra_rustflags: str) -> None:
    print("\n=== Building zmij-rust ===", flush=True)
    rust_dir = ROOT / "zmij-rust"

    env = dict(os.environ)
    if extra_rustflags:
        existing = env.get("RUSTFLAGS", "")
        env["RUSTFLAGS"] = f"{existing} {extra_rustflags}".strip()

    # Build cdylib (release) — the zmij crate itself now has crate-type cdylib
    # and FFI exports appended to lib.rs
    run(
        [
            "cargo",
            "build",
            "--release",
            "--manifest-path",
            str(rust_dir / "Cargo.toml"),
        ],
        env=env,
    )

    # Locate the built .so (cdylib on Linux is libzmij.so)
    target_dir = rust_dir / "target" / "release"
    candidates = list(target_dir.glob("libzmij.so"))
    if not candidates:
        candidates = list(target_dir.glob("libzmij.dylib"))
    if not candidates:
        print("ERROR: Could not find built Rust cdylib", file=sys.stderr)
        sys.exit(1)

    so_src = candidates[0]
    so_dst = output_dir / "libzmij_rust.so"
    shutil.copy2(so_src, so_dst)
    print(f"  → copied {so_src} → {so_dst}")

    # Generate assembly (with debug info for source annotations)
    run(
        [
            "cargo",
            "rustc",
            "--release",
            "--manifest-path",
            str(rust_dir / "Cargo.toml"),
            "--",
            "--emit=asm",
            "-C",
            "debuginfo=2",
        ],
        env=env,
    )

    # Locate the assembly file
    deps_dir = target_dir / "deps"
    asm_candidates = sorted(deps_dir.glob("zmij-*.s"))
    if not asm_candidates:
        asm_candidates = sorted(deps_dir.glob("zmij*.s"))
    if asm_candidates:
        asm_src = asm_candidates[-1]
        asm_dst = output_dir / "zmij_rust.s"
        shutil.copy2(asm_src, asm_dst)
        print(f"  → copied {asm_src} → {asm_dst}")
    else:
        print("  ⚠ Could not locate Rust assembly output", file=sys.stderr)


# --------------------------------------------------------------------------- #
# Main
# --------------------------------------------------------------------------- #


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Build zmij shared libraries for C, C++, and Rust"
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=ROOT / "build" / "libs",
        help="Directory for output .so and .s files (default: build/libs)",
    )
    parser.add_argument(
        "--sse41", action="store_true", help="Enable SSE4.1 instructions for all builds"
    )
    parser.add_argument(
        "--cc",
        choices=["clang", "gcc"],
        default="clang",
        help="C/C++ compiler family (default: clang)",
    )
    args = parser.parse_args()

    output_dir: Path = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    print(f"Output directory: {output_dir}")

    cc = args.cc
    cxx = "g++" if cc == "gcc" else "clang++"
    print(f"Compiler: {cc} / {cxx}")

    extra_cflags: list[str] = []
    extra_rustflags = ""

    if args.sse41:
        print("SSE4.1 mode: ENABLED")
        extra_cflags.append("-msse4.1")
        extra_cflags.append("-DZMIJ_USE_SSE4_1=1")
        extra_rustflags = "-C target-feature=+sse4.1"
    else:
        print("SSE4.1 mode: disabled (baseline)")

    build_c(output_dir, cc, extra_cflags)
    build_cpp(output_dir, cxx, extra_cflags)
    build_rust(output_dir, extra_rustflags)

    print("\n=== Done ===")
    print(f"Libraries and assembly files in: {output_dir}")
    for f in sorted(output_dir.iterdir()):
        print(f"  {f.name}")


if __name__ == "__main__":
    main()
