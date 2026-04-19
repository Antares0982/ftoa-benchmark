/*
 * any_ftoa_benchmark — generic dtoa benchmark runner
 *
 * Loads arbitrary shared libraries via dlopen and benchmarks their
 * float/double conversion performance.
 *
 * Usage:
 *   any_ftoa_benchmark [--test-input <path>] [--rounds <N>] <lib_spec> ...
 *
 * Each <lib_spec> has the format:
 *   path
 *   path:entrance_for_double
 *   path:entrance_for_double:entrance_for_float
 *
 * Defaults: --test-input test_input.txt
 *           sym_double = zmijcpp_detail_write_double
 *           sym_float  = zmijcpp_detail_write_float
 */

#include "benchmark.h"

/* ---- Main --------------------------------------------------------------- */

static void usage(const char* prog) {
  fprintf(stderr,
          "Usage: %s [--test-input <path>] [--rounds <N>] <lib_spec> "
          "[<lib_spec> ...]\n"
          "\n"
          "Each <lib_spec> is: path[:sym_double[:sym_float]]\n"
          "  Defaults: sym_double=%s  sym_float=%s\n"
          "  --test-input <path>  Input file (default: %s)\n"
          "  --rounds <N>         Benchmark rounds (default: 5000)\n",
          prog, DEFAULT_SYM_DOUBLE, DEFAULT_SYM_FLOAT, DEFAULT_INPUT_PATH);
}

int main(int argc, char** argv) {
  int rounds = 5000;
  const char* input_path = DEFAULT_INPUT_PATH;
  const char* lib_specs[MAX_LIBS];
  int n_specs = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--rounds") == 0 && i + 1 < argc) {
      rounds = atoi(argv[++i]);
      if (rounds <= 0) {
        fprintf(stderr, "Invalid rounds: %s\n", argv[i]);
        return 1;
      }
    } else if (strcmp(argv[i], "--test-input") == 0 && i + 1 < argc) {
      input_path = argv[++i];
    } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      usage(argv[0]);
      return 0;
    } else {
      if (n_specs >= MAX_LIBS) {
        fprintf(stderr, "Too many libraries (max %d)\n", MAX_LIBS);
        return 1;
      }
      lib_specs[n_specs++] = argv[i];
    }
  }

  if (n_specs == 0) {
    fprintf(stderr, "Error: need at least one lib spec.\n");
    usage(argv[0]);
    return 1;
  }

  printf("Reading values from: %s\n", input_path);
  values_t vals = read_values(input_path);
  printf("Loaded %zu values.\n\n", vals.count);
  if (vals.count == 0) {
    fprintf(stderr, "Error: input file is empty.\n");
    return 1;
  }

  /* Load libraries */
  dtoa_lib_t libs[MAX_LIBS];
  int nlibs = 0;

  for (int i = 0; i < n_specs; i++) {
    dtoa_lib_t lib = parse_and_load(lib_specs[i]);
    if (lib.handle) {
      libs[nlibs++] = lib;
      printf("  Loaded: %s\n", lib.name);
    } else {
      fprintf(stderr, "  Failed to load: %s\n", lib_specs[i]);
    }
  }

  if (nlibs == 0) {
    fprintf(stderr, "Error: no libraries could be loaded.\n");
    return 1;
  }
  printf("\n");

  /* Pin to a single core to reduce scheduling noise */
  pin_to_core(1);

  /* Check if any lib has float support */
  int has_float = 0;
  for (int i = 0; i < nlibs; i++) {
    if (libs[i].wf) {
      has_float = 1;
      break;
    }
  }

  /* Benchmark: float */
  if (has_float) {
    printf("=== float benchmark (%d rounds × %zu values, %d warmup) ===\n",
           rounds, vals.count, WARMUP_ROUNDS);
    for (int i = 0; i < nlibs; i++) {
      if (libs[i].wf)
        bench_float(libs[i].name, libs[i].wf, vals.f, vals.count, rounds);
    }
    printf("\n");
  }

  /* Benchmark: double */
  printf("=== double benchmark (%d rounds × %zu values, %d warmup) ===\n",
         rounds, vals.count, WARMUP_ROUNDS);
  for (int i = 0; i < nlibs; i++)
    bench_double(libs[i].name, libs[i].wd, vals.d, vals.count, rounds);
  printf("\n");

  /* Cleanup */
  for (int i = 0; i < nlibs; i++) {
    dlclose(libs[i].handle);
    free((void*)libs[i].name);
  }
  free(vals.f);
  free(vals.d);
  return 0;
}
