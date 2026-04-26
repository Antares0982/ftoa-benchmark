---
name: asm-diff
description: Use this skill when the user asks to compare assembly output between two or more source files or library versions, generate an asm diff or assembly patch, or phrases like "diff the assembly", "compare asm", "show assembly differences", "compile and diff". Handles compiling with compile_core.sh and producing a git diff patch of the resulting .s files.
version: 1.0.0
---

# Assembly Diff Skill

Compile two or more source targets with `./tools/compile_core.sh`, then produce a `git diff --no-index` patch of their assembly outputs.

## Argument Parsing

Parse the user's request into a structured list of **targets** plus shared options.

### Per-target fields

Each target has:
- `source` — path to the source file (required)
- `name` — output label / base name, used as both the `compile_core.sh` name arg and the `.s` filename stem (default: basename without extension)
- `flags` — extra compiler flags **specific to this target** (e.g. `-DXJB_NO_MEMMOVE=1`)

### Shared options

- `lang` — `c` or `cpp` (default: `cpp`; infer from file extension when unambiguous)
- `shared_flags` — extra flags applied to **all** targets (e.g. `-O2`, `-march=native`)
- `patch_name` — output patch filename (default: `<name_a>-vs-<name_b>.patch`; for >2 targets use the first and last name)

## Step 1 — Compile Each Target

For each target run:

```
./tools/compile_core.sh --lang <lang> <source> <name> [-- <shared_flags...> <target_flags...>]
```

- Always pass `--lang`.
- Combine `shared_flags` and per-target `flags` after `--`.
- Suppress stdout with `1>/dev/null` so the diff output is clean; let stderr through.
- After the command succeeds, confirm `./build/libs/<name>.s` exists.

Example for two targets, no extra flags:
```bash
./tools/compile_core.sh --lang cpp xjb/ftoa.cpp xjb-old 1>/dev/null
./tools/compile_core.sh --lang cpp xjb-new/ftoa.cpp xjb-new 1>/dev/null
```

Example with per-target macro:
```bash
./tools/compile_core.sh --lang cpp xjb/ftoa.cpp xjb-old 1>/dev/null
./tools/compile_core.sh --lang cpp xjb/ftoa.cpp xjb-old-nomove -- -DXJB_NO_MEMMOVE=1 1>/dev/null
```

## Step 2 — Generate the Diff Patch

Compare the first two targets (or whichever pair the user specified) with:

```bash
git diff --no-index -- ./build/libs/<name_a>.s ./build/libs/<name_b>.s > ./<patch_name> || true
```

The `|| true` is required because `git diff --no-index` exits 1 when differences exist.

For more than two targets, generate one patch per consecutive pair:
```
<name1>-vs-<name2>.patch
<name2>-vs-<name3>.patch
```

## Step 3 — Report

After generating, tell the user:
- Which `.s` files were compared.
- The patch filename(s) written to the project root.
- The line count of each patch (`wc -l <patch_name>`).

If the patch is empty (no differences), say so explicitly.

## Edge Cases

- If `source` does not exist, abort and tell the user before running any compile commands.
- If `compile_core.sh` fails (non-zero exit), stop and show the compiler error; do not generate a partial patch.
- If the user does not specify `name` for a target and two targets share the same basename, append a numeric suffix (`_1`, `_2`) to avoid overwriting.
- If the user asks for a specific output path for the patch (e.g. `./patches/foo.patch`), create the parent directory first with `mkdir -p`.

## Example Invocations

**Minimal — two source files, auto names:**
> "diff the asm between xjb/ftoa.cpp and xjb-new/ftoa.cpp"
```bash
./tools/compile_core.sh --lang cpp xjb/ftoa.cpp xjb 1>/dev/null
./tools/compile_core.sh --lang cpp xjb-new/ftoa.cpp xjb-new 1>/dev/null
git diff --no-index -- ./build/libs/xjb.s ./build/libs/xjb-new.s > ./xjb-vs-xjb-new.patch || true
```

**With a shared flag and per-target macro:**
> "compare xjb/ftoa.cpp (label old) vs xjb/ftoa.cpp with -DXJB_NO_MEMMOVE=1 (label old-nomove), both with -O2"
```bash
./tools/compile_core.sh --lang cpp xjb/ftoa.cpp old -- -O2 1>/dev/null
./tools/compile_core.sh --lang cpp xjb/ftoa.cpp old-nomove -- -O2 -DXJB_NO_MEMMOVE=1 1>/dev/null
git diff --no-index -- ./build/libs/old.s ./build/libs/old-nomove.s > ./old-vs-old-nomove.patch || true
```
