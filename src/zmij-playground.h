/*
 * zmij-playground.h — shared types, utilities, and benchmark helpers
 *
 * Included by benchmark.c, any_ftoa_benchmark.c, and verifier.c.
 * All functions are static inline to avoid unused-function warnings
 * in translation units that don't call every helper.
 */

#ifndef ZMIJ_PLAYGROUND_H
#define ZMIJ_PLAYGROUND_H

#define _GNU_SOURCE
#include <dlfcn.h>
#include <libgen.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---- Constants ---------------------------------------------------------- */

#define WARMUP_ROUNDS 100
#define MAX_LIBS 64

#define DEFAULT_SYM_DOUBLE "zmijcpp_detail_write_double"
#define DEFAULT_SYM_FLOAT "zmijcpp_detail_write_float"
#define DEFAULT_INPUT_PATH "test_input.txt"

/* ---- Types -------------------------------------------------------------- */

typedef char* (*write_float_fn)(float value, char* buffer);
typedef char* (*write_double_fn)(double value, char* buffer);

typedef struct {
  const char* name;
  void* handle;
  write_float_fn wf;
  write_double_fn wd;
} dtoa_lib_t;

typedef struct {
  float* f;
  double* d;
  size_t count;
} values_t;

/* ---- Utility ------------------------------------------------------------ */

static inline double timespec_diff_ns(const struct timespec* end,
                                      const struct timespec* start) {
  return (double)(end->tv_sec - start->tv_sec) * 1e9 +
         (double)(end->tv_nsec - start->tv_nsec);
}

/* ---- Input reader ------------------------------------------------------- */

static inline values_t read_values(const char* path) {
  FILE* fp = fopen(path, "r");
  if (!fp) {
    perror(path);
    exit(1);
  }

  size_t cap = 1024;
  float* fv = (float*)malloc(cap * sizeof(float));
  double* dv = (double*)malloc(cap * sizeof(double));
  size_t n = 0;
  char line[256];

  while (fgets(line, sizeof(line), fp)) {
    if (line[0] == '\n' || line[0] == '\0') continue;
    if (n == cap) {
      cap *= 2;
      fv = (float*)realloc(fv, cap * sizeof(float));
      dv = (double*)realloc(dv, cap * sizeof(double));
    }
    dv[n] = strtod(line, NULL);
    fv[n] = strtof(line, NULL);
    n++;
  }
  fclose(fp);

  values_t v;
  v.f = fv;
  v.d = dv;
  v.count = n;
  return v;
}

/* ---- Benchmark ---------------------------------------------------------- */

static inline void bench_float(const char* name, write_float_fn fn,
                               const float* vals, size_t count, int rounds) {
  char buf[64];
  volatile size_t sink = 0;

  for (int r = 0; r < WARMUP_ROUNDS; r++)
    for (size_t i = 0; i < count; i++) {
      char* end = fn(vals[i], buf);
      sink += (size_t)(end - buf);
    }

  double* round_ns = (double*)malloc((size_t)rounds * sizeof(double));
  struct timespec t0, t1;

  for (int r = 0; r < rounds; r++) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    for (size_t i = 0; i < count; i++) {
      char* end = fn(vals[i], buf);
      sink += (size_t)(end - buf);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    round_ns[r] = timespec_diff_ns(&t1, &t0);
  }

  double total_ns = 0;
  for (int r = 0; r < rounds; r++) total_ns += round_ns[r];
  double mean = total_ns / ((double)rounds * (double)count);

  double var_sum = 0;
  for (int r = 0; r < rounds; r++) {
    double d = round_ns[r] / (double)count - mean;
    var_sum += d * d;
  }
  double stddev = sqrt(var_sum / (double)rounds);
  free(round_ns);

  printf(
      "  %-24s  %10zu calls  %12.2f ms total  %8.2f ± %.2f ns/call  "
      "(sink=%zu)\n",
      name, (size_t)rounds * count, total_ns / 1e6, mean, stddev, (size_t)sink);
}

static inline void bench_double(const char* name, write_double_fn fn,
                                const double* vals, size_t count, int rounds) {
  char buf[64];
  volatile size_t sink = 0;

  for (int r = 0; r < WARMUP_ROUNDS; r++)
    for (size_t i = 0; i < count; i++) {
      char* end = fn(vals[i], buf);
      sink += (size_t)(end - buf);
    }

  double* round_ns = (double*)malloc((size_t)rounds * sizeof(double));
  struct timespec t0, t1;

  for (int r = 0; r < rounds; r++) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    for (size_t i = 0; i < count; i++) {
      char* end = fn(vals[i], buf);
      sink += (size_t)(end - buf);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    round_ns[r] = timespec_diff_ns(&t1, &t0);
  }

  double total_ns = 0;
  for (int r = 0; r < rounds; r++) total_ns += round_ns[r];
  double mean = total_ns / ((double)rounds * (double)count);

  double var_sum = 0;
  for (int r = 0; r < rounds; r++) {
    double d = round_ns[r] / (double)count - mean;
    var_sum += d * d;
  }
  double stddev = sqrt(var_sum / (double)rounds);
  free(round_ns);

  printf(
      "  %-24s  %10zu calls  %12.2f ms total  %8.2f ± %.2f ns/call  "
      "(sink=%zu)\n",
      name, (size_t)rounds * count, total_ns / 1e6, mean, stddev, (size_t)sink);
}

/* ---- Lib spec parsing --------------------------------------------------- */
/*
 * Parse a spec string of the form:
 *   path
 *   path:sym_double
 *   path:sym_double:sym_float
 *
 * Defaults: sym_double = DEFAULT_SYM_DOUBLE, sym_float = DEFAULT_SYM_FLOAT.
 * Returns a dtoa_lib_t with handle==NULL on failure.
 * Float symbol lookup failure is non-fatal (wf set to NULL).
 */
static inline dtoa_lib_t parse_and_load(const char* spec) {
  dtoa_lib_t lib = {NULL, NULL, NULL, NULL};

  char* buf = strdup(spec);
  if (!buf) {
    perror("strdup");
    exit(1);
  }

  char* path = buf;
  char* sym_double = NULL;
  char* sym_float = NULL;

  char* colon1 = strchr(path, ':');
  if (colon1) {
    *colon1 = '\0';
    sym_double = colon1 + 1;
    char* colon2 = strchr(sym_double, ':');
    if (colon2) {
      *colon2 = '\0';
      sym_float = colon2 + 1;
    }
  }

  if (!sym_double || *sym_double == '\0') sym_double = DEFAULT_SYM_DOUBLE;
  if (!sym_float || *sym_float == '\0') sym_float = DEFAULT_SYM_FLOAT;

  /* Derive display name from basename */
  char* path_copy = strdup(path);
  const char* bname = basename(path_copy);
  const char* display = bname;
  if (strncmp(display, "lib", 3) == 0) display += 3;
  char* name = strdup(display);
  char* dot = strstr(name, ".so");
  if (dot) *dot = '\0';
  free(path_copy);

  void* h = dlopen(path, RTLD_NOW | RTLD_LOCAL);
  if (!h) {
    fprintf(stderr, "dlopen(%s): %s\n", path, dlerror());
    free(buf);
    free(name);
    return lib;
  }

  dlerror();
  write_double_fn wd = (write_double_fn)dlsym(h, sym_double);
  const char* err = dlerror();
  if (err) {
    fprintf(stderr, "dlsym(%s, %s): %s\n", path, sym_double, err);
    dlclose(h);
    free(buf);
    free(name);
    return lib;
  }

  dlerror();
  write_float_fn wf = (write_float_fn)dlsym(h, sym_float);
  err = dlerror();
  if (err) {
    /* float is allowed to fail — just set NULL */
    wf = NULL;
  }

  lib.name = name;
  lib.handle = h;
  lib.wd = wd;
  lib.wf = wf;

  free(buf);
  return lib;
}

#endif /* ZMIJ_PLAYGROUND_H */
