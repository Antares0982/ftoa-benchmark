/*
 * zmij benchmark runner
 *
 * Loads three zmij implementations (C, C++, Rust) via dlopen and benchmarks
 * their float/double conversion performance.
 *
 * Usage:
 *   zmij_benchmark [--lib-dir <path>] [--rounds <N>] <input.txt>
 */

#define _GNU_SOURCE
#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WARMUP_ROUNDS 100

#ifndef DEFAULT_LIB_DIR
#    define DEFAULT_LIB_DIR "build/libs"
#endif

/* ---- Types -------------------------------------------------------------- */

typedef char *(*write_float_fn)(float value, char *buffer);
typedef char *(*write_double_fn)(double value, char *buffer);

typedef struct {
    const char *name;
    void *handle;
    write_float_fn wf;
    write_double_fn wd;
} zmij_lib_t;

/* ---- Helpers ------------------------------------------------------------ */

static double timespec_diff_ns(const struct timespec *end,
                               const struct timespec *start) {
    return (double)(end->tv_sec - start->tv_sec) * 1e9 +
           (double)(end->tv_nsec - start->tv_nsec);
}

static void *must_dlsym(void *handle, const char *sym, const char *lib_name) {
    dlerror(); /* clear */
    void *p = dlsym(handle, sym);
    const char *err = dlerror();
    if (err) {
        fprintf(stderr, "dlsym(%s, %s): %s\n", lib_name, sym, err);
        exit(1);
    }
    return p;
}

static zmij_lib_t load_lib(const char *dir, const char *filename,
                           const char *display_name) {
    char path[4096];
    snprintf(path, sizeof(path), "%s/%s", dir, filename);

    void *h = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!h) {
        fprintf(stderr, "dlopen(%s): %s\n", path, dlerror());
        exit(1);
    }

    zmij_lib_t lib;
    lib.name = display_name;
    lib.handle = h;
    lib.wf = (write_float_fn)must_dlsym(h, "zmij_detail_write_float", path);
    lib.wd = (write_double_fn)must_dlsym(h, "zmij_detail_write_double", path);
    return lib;
}

int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

/* ---- Input reader ------------------------------------------------------- */

typedef struct {
    float *f;
    double *d;
    size_t count;
} values_t;

static values_t read_values(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror(path);
        exit(1);
    }

    size_t cap = 1024;
    float *fv = (float *)malloc(cap * sizeof(float));
    double *dv = (double *)malloc(cap * sizeof(double));
    size_t n = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        /* skip empty lines */
        if (line[0] == '\n' || line[0] == '\0')
            continue;
        if (n == cap) {
            cap *= 2;
            fv = (float *)realloc(fv, cap * sizeof(float));
            dv = (double *)realloc(dv, cap * sizeof(double));
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

static void bench_float(const char *name, write_float_fn fn, const float *vals,
                        size_t count, int rounds) {
    char buf[64];
    volatile size_t sink = 0; /* prevent dead-code elimination */

    /* Warm up */
    for (int r = 0; r < WARMUP_ROUNDS; r++) {
        for (size_t i = 0; i < count; i++) {
            char *end = fn(vals[i], buf);
            sink += (size_t)(end - buf);
        }
    }

    /* Per-round timing */
    double *round_ns = (double *)malloc((size_t)rounds * sizeof(double));
    struct timespec t0, t1;

    for (int r = 0; r < rounds; r++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
        for (size_t i = 0; i < count; i++) {
            char *end = fn(vals[i], buf);
            sink += (size_t)(end - buf);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        round_ns[r] = timespec_diff_ns(&t1, &t0);
    }

    /* Statistics */
    double total_ns = 0;
    for (int r = 0; r < rounds; r++)
        total_ns += round_ns[r];

    double mean_per_call = total_ns / ((double)rounds * (double)count);

    double var_sum = 0;
    for (int r = 0; r < rounds; r++) {
        double per_call = round_ns[r] / (double)count;
        double diff = per_call - mean_per_call;
        var_sum += diff * diff;
    }
    double stddev = sqrt(var_sum / (double)rounds);

    free(round_ns);

    size_t total_calls = (size_t)rounds * count;
    printf("  %-12s  %10zu calls  %12.2f ms total  %8.2f ± %.2f ns/call  (sink=%zu)\n",
           name, total_calls, total_ns / 1e6, mean_per_call, stddev, (size_t)sink);
}

static void bench_double(const char *name, write_double_fn fn,
                         const double *vals, size_t count, int rounds) {
    char buf[64];
    volatile size_t sink = 0;

    /* Warm up */
    for (int r = 0; r < WARMUP_ROUNDS; r++) {
        for (size_t i = 0; i < count; i++) {
            char *end = fn(vals[i], buf);
            sink += (size_t)(end - buf);
        }
    }

    /* Per-round timing */
    double *round_ns = (double *)malloc((size_t)rounds * sizeof(double));
    struct timespec t0, t1;

    for (int r = 0; r < rounds; r++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
        for (size_t i = 0; i < count; i++) {
            char *end = fn(vals[i], buf);
            sink += (size_t)(end - buf);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        round_ns[r] = timespec_diff_ns(&t1, &t0);
    }

    /* Statistics */
    double total_ns = 0;
    for (int r = 0; r < rounds; r++)
        total_ns += round_ns[r];

    double mean_per_call = total_ns / ((double)rounds * (double)count);

    double var_sum = 0;
    for (int r = 0; r < rounds; r++) {
        double per_call = round_ns[r] / (double)count;
        double diff = per_call - mean_per_call;
        var_sum += diff * diff;
    }
    double stddev = sqrt(var_sum / (double)rounds);

    free(round_ns);

    size_t total_calls = (size_t)rounds * count;
    printf("  %-12s  %10zu calls  %12.2f ms total  %8.2f ± %.2f ns/call  (sink=%zu)\n",
           name, total_calls, total_ns / 1e6, mean_per_call, stddev, (size_t)sink);
}

/* ---- Verification ------------------------------------------------------ */

static int verify_float(const zmij_lib_t libs[3], const float *vals,
                        size_t count) {
    char bufs[3][64];
    int mismatches = 0;
    size_t skip_index = 3;

    for (size_t i = 0; i < 3; ++i) {
        if (strcmp("C", libs[i].name) == 0) {
            skip_index = i;
            printf("  *** skipping verification for C ***\n");
            break;
        }
    }

    for (size_t i = 0; i < count; i++) {
        for (int k = 0; k < 3; k++) {
            char *end = libs[k].wf(vals[i], bufs[k]);
            *end = '\0';
        }

        if ((skip_index == 2 && strcmp(bufs[0], bufs[1]) != 0) ||
            (skip_index == 1 && strcmp(bufs[0], bufs[2]) != 0) ||
            (skip_index == 0 && strcmp(bufs[1], bufs[2]) != 0)) {
            if (mismatches == 0)
                printf("  *** float mismatches detected ***\n");
            printf("  [%zu] C=%-20s  C++=%-20s  Rust=%-20s\n",
                   i, bufs[0], bufs[1], bufs[2]);
            mismatches++;
        }
    }
    return mismatches;
}

static int verify_double(const zmij_lib_t libs[3], const double *vals,
                         size_t count) {
    char bufs[3][64];
    int mismatches = 0;

    for (size_t i = 0; i < count; i++) {
        for (int k = 0; k < 3; k++) {
            char *end = libs[k].wd(vals[i], bufs[k]);
            *end = '\0';
        }

        if (strcmp(bufs[0], bufs[1]) != 0 ||
            strcmp(bufs[0], bufs[2]) != 0) {
            if (mismatches == 0)
                printf("  *** double mismatches detected ***\n");
            char std_buf[64];
            snprintf(std_buf, sizeof(std_buf), "%.*g", 17, vals[i]);
            printf("  [%zu] stdlib=%-24s  C=%-24s  C++=%-24s  Rust=%-24s\n",
                   i, std_buf, bufs[0], bufs[1], bufs[2]);
            mismatches++;
        }
    }
    return mismatches;
}

/* ---- Main --------------------------------------------------------------- */

static void usage(const char *prog) {
    fprintf(stderr,
            "Usage: %s [--lib-dir <path>] [--rounds <N>] <input.txt>\n"
            "\n"
            "  --lib-dir <path>  Directory containing .so files (default: "
            "compile-time DEFAULT_LIB_DIR)\n"
            "  --rounds <N>      Number of benchmark rounds (default: 5000)\n",
            prog);
}

int main(int argc, char **argv) {
    const char *lib_dir = DEFAULT_LIB_DIR;
    const char *input_path = NULL;
    int rounds = 5000;

    /* simple arg parsing */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--lib-dir") == 0 && i + 1 < argc) {
            lib_dir = argv[++i];
        } else if (strcmp(argv[i], "--rounds") == 0 && i + 1 < argc) {
            rounds = atoi(argv[++i]);
            if (rounds <= 0) {
                fprintf(stderr, "Invalid rounds: %s\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            usage(argv[0]);
            return 0;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        } else {
            input_path = argv[i];
        }
    }

    if (!input_path) {
        fprintf(stderr, "Error: no input file specified.\n");
        usage(argv[0]);
        return 1;
    }

    /* Read values */
    printf("Reading values from: %s\n", input_path);
    values_t vals = read_values(input_path);
    printf("Loaded %zu values.\n\n", vals.count);

    if (vals.count == 0) {
        fprintf(stderr, "Error: input file is empty.\n");
        return 1;
    }

    /* Load libraries */
    printf("Loading libraries from: %s\n\n", lib_dir);
    zmij_lib_t libs[3];
    libs[0] = load_lib(lib_dir, "libzmij_c.so", "C");
    libs[1] = load_lib(lib_dir, "libzmij_cpp.so", "C++");
    libs[2] = load_lib(lib_dir, "libzmij_rust.so", "Rust");

    /* Benchmark: float group */
    printf("=== float benchmark (%d rounds × %zu values, %d warmup) ===\n",
           rounds, vals.count, WARMUP_ROUNDS);
    for (int i = 0; i < 3; i++) {
        bench_float(libs[i].name, libs[i].wf, vals.f, vals.count, rounds);
    }

    printf("\n");

    /* Benchmark: double group */
    printf("=== double benchmark (%d rounds × %zu values, %d warmup) ===\n",
           rounds, vals.count, WARMUP_ROUNDS);
    for (int i = 0; i < 3; i++) {
        bench_double(libs[i].name, libs[i].wd, vals.d, vals.count, rounds);
    }

    /* Verify consistency across implementations */
    printf("=== Verifying output consistency ===\n");
    int fmm = verify_float(libs, vals.f, vals.count);
    int dmm = verify_double(libs, vals.d, vals.count);
    if (fmm == 0 && dmm == 0)
        printf("  All %zu values: C, C++, Rust outputs are identical.\n",
               vals.count);
    else
        printf("  float mismatches: %d, double mismatches: %d\n", fmm, dmm);
    printf("\n");

    /* Cleanup */
    for (int i = 0; i < 3; i++)
        dlclose(libs[i].handle);
    free(vals.f);
    free(vals.d);

    return 0;
}
