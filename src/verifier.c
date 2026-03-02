/*
 * dtoa verifier
 *
 * Loads shared libraries via dlopen, reads doubles from an input file,
 * converts each via the library's dtoa, and prints any that differ from
 * the original text representation.
 *
 * Usage:
 *   verifier [--test-input <path>] <lib_spec> [<lib_spec> ...]
 *
 * Each <lib_spec> is: path[:entrance_for_double]
 * Default entrance: zmijcpp_detail_write_double
 * Default input:    test_input.txt
 */

#include "zmij-playground.h"

/* ---- Main --------------------------------------------------------------- */

static void usage(const char* prog) {
  fprintf(stderr,
          "Usage: %s [--test-input <path>] <lib_spec> [<lib_spec> ...]\n"
          "\n"
          "Each <lib_spec> is: path[:sym_double]\n"
          "  Default: sym_double=%s\n"
          "  --test-input <path>  Input file (default: %s)\n",
          prog, DEFAULT_SYM_DOUBLE, DEFAULT_INPUT_PATH);
}

int main(int argc, char** argv) {
  const char* input_path = DEFAULT_INPUT_PATH;
  const char* lib_specs[MAX_LIBS];
  int n_specs = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--test-input") == 0 && i + 1 < argc) {
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

  /* Load libraries */
  dtoa_lib_t libs[MAX_LIBS];
  int nlibs = 0;

  for (int i = 0; i < n_specs; i++) {
    dtoa_lib_t lib = parse_and_load(lib_specs[i]);
    if (lib.handle) {
      libs[nlibs++] = lib;
      printf("Loaded: %s\n", lib.name);
    } else {
      fprintf(stderr, "Failed to load: %s\n", lib_specs[i]);
    }
  }

  if (nlibs == 0) {
    fprintf(stderr, "Error: no libraries could be loaded.\n");
    return 1;
  }

  /* Read input and verify line by line */
  FILE* fp = fopen(input_path, "r");
  if (!fp) {
    perror(input_path);
    return 1;
  }

  char line[256];
  size_t lineno = 0;
  int total_mismatches = 0;
  char dtoa_buf[64];

  printf("\nVerifying dtoa output against input text (%s)...\n\n", input_path);

  while (fgets(line, sizeof(line), fp)) {
    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
      line[--len] = '\0';
    if (len == 0) continue;
    lineno++;

    double val = strtod(line, NULL);

    for (int k = 0; k < nlibs; k++) {
      char* end = libs[k].wd(val, dtoa_buf);
      *end = '\0';

      if (strcmp(line, dtoa_buf) != 0) {
        printf("  [%zu] %-16s  input=%-24s  dtoa=%-24s\n", lineno, libs[k].name,
               line, dtoa_buf);
        total_mismatches++;
      }
    }
  }
  fclose(fp);

  printf("\n%zu lines checked, %d mismatches total.\n", lineno,
         total_mismatches);

  for (int i = 0; i < nlibs; i++) {
    dlclose(libs[i].handle);
    free((void*)libs[i].name);
  }

  return total_mismatches > 0 ? 1 : 0;
}
