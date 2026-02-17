clang

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       3787.57 ms total      8.24 ± 0.15 ns/call  (sink=6036043800)
  C++            459660000 calls       3744.35 ms total      8.15 ± 0.34 ns/call  (sink=4160472900)
  Rust           459660000 calls       4030.78 ms total      8.77 ± 0.36 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5483.99 ms total     11.93 ± 0.34 ns/call  (sink=7896585000)
  C++            459660000 calls       5112.92 ms total     11.12 ± 0.19 ns/call  (sink=7896585000)
  Rust           459660000 calls       4912.29 ms total     10.69 ± 0.23 ns/call  (sink=7896789000)
```

clang+sse4.1

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       3792.52 ms total      8.25 ± 0.16 ns/call  (sink=6036043800)
  C++            459660000 calls       3809.91 ms total      8.29 ± 0.51 ns/call  (sink=4160472900)
  Rust           459660000 calls       4045.94 ms total      8.80 ± 0.46 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5522.36 ms total     12.01 ± 0.33 ns/call  (sink=7896585000)
  C++            459660000 calls       5172.26 ms total     11.25 ± 0.34 ns/call  (sink=7896585000)
  Rust           459660000 calls       4357.54 ms total      9.48 ± 0.21 ns/call  (sink=7896789000)
```

gcc

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5614.11 ms total     12.21 ± 0.22 ns/call  (sink=6036043800)
  C++            459660000 calls       3689.67 ms total      8.03 ± 0.20 ns/call  (sink=4160472900)
  Rust           459660000 calls       4014.05 ms total      8.73 ± 0.15 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       6163.17 ms total     13.41 ± 0.30 ns/call  (sink=7896585000)
  C++            459660000 calls       5352.04 ms total     11.64 ± 0.12 ns/call  (sink=7896585000)
  Rust           459660000 calls       4921.54 ms total     10.71 ± 0.30 ns/call  (sink=7896789000)
```

gcc+sse4.1

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5632.71 ms total     12.25 ± 0.49 ns/call  (sink=6036043800)
  C++            459660000 calls       3655.90 ms total      7.95 ± 0.14 ns/call  (sink=4160472900)
  Rust           459660000 calls       4010.80 ms total      8.73 ± 0.07 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       6129.39 ms total     13.33 ± 0.12 ns/call  (sink=7896585000)
  C++            459660000 calls       5307.93 ms total     11.55 ± 0.13 ns/call  (sink=7896585000)
  Rust           459660000 calls       4347.84 ms total      9.46 ± 0.29 ns/call  (sink=7896789000)
```

