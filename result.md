clang

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       3817.71 ms total      8.31 ± 0.22 ns/call  (sink=6036043800)
  C++            459660000 calls       3759.76 ms total      8.18 ± 0.42 ns/call  (sink=4160472900)
  Rust           459660000 calls       4010.30 ms total      8.72 ± 0.11 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5448.13 ms total     11.85 ± 0.05 ns/call  (sink=7896585000)
  C++            459660000 calls       4642.58 ms total     10.10 ± 0.10 ns/call  (sink=7896585000)
  Rust           459660000 calls       4904.46 ms total     10.67 ± 0.05 ns/call  (sink=7896789000)
```

clang+sse4.1

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       3835.74 ms total      8.34 ± 0.38 ns/call  (sink=6036043800)
  C++            459660000 calls       3753.93 ms total      8.17 ± 0.24 ns/call  (sink=4160472900)
  Rust           459660000 calls       4034.08 ms total      8.78 ± 0.31 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5473.46 ms total     11.91 ± 0.27 ns/call  (sink=7896585000)
  C++            459660000 calls       4593.83 ms total      9.99 ± 0.26 ns/call  (sink=7896585000)
  Rust           459660000 calls       4360.97 ms total      9.49 ± 0.09 ns/call  (sink=7896789000)
```

gcc

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5677.37 ms total     12.35 ± 0.35 ns/call  (sink=6036043800)
  C++            459660000 calls       3698.70 ms total      8.05 ± 0.31 ns/call  (sink=4160472900)
  Rust           459660000 calls       4017.38 ms total      8.74 ± 0.19 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       6184.52 ms total     13.45 ± 0.33 ns/call  (sink=7896585000)
  C++            459660000 calls       4558.73 ms total      9.92 ± 0.45 ns/call  (sink=7896585000)
  Rust           459660000 calls       4897.78 ms total     10.66 ± 0.17 ns/call  (sink=7896789000)
```

gcc+sse4.1

```
=== float benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       5656.03 ms total     12.30 ± 0.22 ns/call  (sink=6036043800)
  C++            459660000 calls       3704.23 ms total      8.06 ± 0.33 ns/call  (sink=4160472900)
  Rust           459660000 calls       4023.75 ms total      8.75 ± 0.23 ns/call  (sink=4160850300)

=== double benchmark (5000 rounds × 91932 values, 100 warmup) ===
  C              459660000 calls       6175.12 ms total     13.43 ± 0.50 ns/call  (sink=7896585000)
  C++            459660000 calls       4550.73 ms total      9.90 ± 0.40 ns/call  (sink=7896585000)
  Rust           459660000 calls       4379.98 ms total      9.53 ± 0.30 ns/call  (sink=7896789000)
```

