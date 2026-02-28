clang

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       3808.08 ms total      8.28 ± 0.22 ns/call  (sink=6036043800)
  C++            459660000 calls       3742.52 ms total      8.14 ± 0.31 ns/call  (sink=4160472900)
  Rust           459660000 calls       4018.60 ms total      8.74 ± 0.18 ns/call  (sink=4160850300)
  xjb            459660000 calls       3260.36 ms total      7.09 ± 0.21 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5451.17 ms total     11.86 ± 0.10 ns/call  (sink=7896585000)
  C++            459660000 calls       4666.85 ms total     10.15 ± 0.31 ns/call  (sink=7896585000)
  Rust           459660000 calls       4920.32 ms total     10.70 ± 0.23 ns/call  (sink=7896789000)
  xjb            459660000 calls       3993.61 ms total      8.69 ± 0.15 ns/call  (sink=7896789000)
```

clang+sse4.1

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       3817.69 ms total      8.31 ± 0.29 ns/call  (sink=6036043800)
  C++            459660000 calls       3742.88 ms total      8.14 ± 0.29 ns/call  (sink=4160472900)
  Rust           459660000 calls       4024.75 ms total      8.76 ± 0.19 ns/call  (sink=4160850300)
  xjb            459660000 calls       3283.62 ms total      7.14 ± 0.18 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5491.72 ms total     11.95 ± 0.12 ns/call  (sink=7896585000)
  C++            459660000 calls       4362.36 ms total      9.49 ± 0.26 ns/call  (sink=7896585000)
  Rust           459660000 calls       4361.03 ms total      9.49 ± 0.07 ns/call  (sink=7896789000)
  xjb            459660000 calls       4037.82 ms total      8.78 ± 0.19 ns/call  (sink=7896789000)
```

gcc

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5649.56 ms total     12.29 ± 0.58 ns/call  (sink=6036043800)
  C++            459660000 calls       3652.52 ms total      7.95 ± 0.14 ns/call  (sink=4160472900)
  Rust           459660000 calls       4007.28 ms total      8.72 ± 0.10 ns/call  (sink=4160850300)
  xjb            459660000 calls       3346.29 ms total      7.28 ± 0.31 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       6155.04 ms total     13.39 ± 0.14 ns/call  (sink=7896585000)
  C++            459660000 calls       4498.13 ms total      9.79 ± 0.19 ns/call  (sink=7896585000)
  Rust           459660000 calls       4923.03 ms total     10.71 ± 0.28 ns/call  (sink=7896789000)
  xjb            459660000 calls       3948.85 ms total      8.59 ± 0.11 ns/call  (sink=7896789000)
```

gcc+sse4.1

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5668.15 ms total     12.33 ± 0.51 ns/call  (sink=6036043800)
  C++            459660000 calls       3684.08 ms total      8.01 ± 0.29 ns/call  (sink=4160472900)
  Rust           459660000 calls       4042.30 ms total      8.79 ± 0.26 ns/call  (sink=4160850300)
  xjb            459660000 calls       3393.82 ms total      7.38 ± 0.40 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       6182.05 ms total     13.45 ± 0.54 ns/call  (sink=7896585000)
  C++            459660000 calls       4383.75 ms total      9.54 ± 0.51 ns/call  (sink=7896585000)
  Rust           459660000 calls       4394.98 ms total      9.56 ± 0.30 ns/call  (sink=7896789000)
  xjb            459660000 calls       4004.72 ms total      8.71 ± 0.79 ns/call  (sink=7896789000)
```

