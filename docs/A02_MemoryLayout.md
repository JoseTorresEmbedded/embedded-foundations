# A02 - Memory Layout

> **Roadmap:** Level 1 - C Language Mastery  
> **Module:** A02 - Memory Layout  
> **Status:** ✅ Completed  
> **Last Updated:** November 2026

---

# Objective

Understand where every kind of C variable lives, both in the executable file and in the memory of a running process.

By the end of this module I should understand:

- How storage duration and initialization decide the section of a variable.
- Why some variables have a symbol and others do not.
- How linkage (`static` vs. global) shows up in the binary.
- What the compiler is allowed to do with a variable when optimizing.
- How file addresses (`nm`) relate to runtime addresses.
- The memory map of a running Linux process: program, heap, stack, shared libraries.
- How memory permissions protect `const` data.

---

# Why This Module Matters

Before this module I knew the names `.text`, `.data` and `.bss`, but I had never verified where my own variables end up.

On a microcontroller this is not an academic detail:

- `.rodata` stays in Flash.
- `.bss` uses RAM only (it is zeroed at startup).
- `.data` uses **both**: its initial values live in Flash and the startup code copies them to RAM.
- The stack and the heap share the remaining RAM and grow towards each other.

Knowing where each variable lives is a prerequisite for linker scripts (A06), startup code (A07), and for reasoning about RAM/Flash budgets in real firmware.

---

# Scope Note

The detailed study of ELF sections (`readelf -S`, `size`, why `.bss` takes no space in the file, compiler-generated sections) belongs to **A05 - Sections**.

This module focuses on the **memory layout**: where each variable is placed, and what the process looks like at runtime.

---

# Table of Contents

1. Theory
2. Laboratory
3. Experiments
4. Process Memory Map
5. Commands Used
6. Key Concepts Learned
7. Mental Model
8. Engineering Notes
9. Knowledge Check
10. References
11. Questions for Further Study
12. Summary

---

# Theory

The section of a variable is decided by two questions:

1. **How long does it live?** (*storage duration*)
    - **Static:** for the whole program. Globals and anything declared `static`, even inside a function.
    - **Automatic:** only while its function is executing. Normal local variables.
2. **Does it have a non-zero initial value? Can it be modified?**

| Section | Contents | Space in the file? |
|---|---|---|
| `.text` | machine code | yes |
| `.rodata` | read-only constant data | yes |
| `.data` | static-duration variables with a non-zero initial value | yes (the value must be stored) |
| `.bss` | static-duration variables without initializer, or initialized to 0 | no (only its size is recorded) |
| stack | automatic variables | no (created at runtime) |
| heap | memory returned by `malloc` | no (created at runtime) |

---

# Laboratory

## Repository Structure

```text
examples/
└── A02_MemoryLayout
    ├── Makefile
    └── main.c
```

The `Makefile` is the generic one developed in the A01 Make laboratory (pattern rules, automatic dependencies, out-of-tree `build/`, validated `debug`/`release` modes).

Compiler flags:

```text
debug:   -std=c11 -Wall -Wextra -Wcast-qual -Werror -MMD -MP -O0 -g3
release: -std=c11 -Wall -Wextra -Wcast-qual -Werror -MMD -MP -O2 -DNDEBUG
```

## Variables Under Study

```c
int global_initialized = 10;
int global_uninitialized;

static int static_initialized = 20;
static int static_uninitialized;

const char message[] = "Embedded";

int main(void)
{
    int local = 30;

    static int local_static = 40;
    ...
}
```

---

# Experiment 1 - Warnings Reveal Linkage

## Command

```bash
make
```

With the first version of `main.c`, where the variables were declared but not used:

## Output

```text
main.c:15:16: error: unused variable 'local_static' [-Werror=unused-variable]
main.c:7:12: error: 'static_uninitialized' defined but not used [-Werror=unused-variable]
main.c:6:12: error: 'static_initialized' defined but not used [-Werror=unused-variable]
```

## Observation

None of the six variables was used, but GCC only complained about the `static` ones.

| Declaration | Linkage | Visible to | Can GCC prove it is unused? |
|---|---|---|---|
| `static int static_initialized` (file scope) | internal | this `.c` only | yes |
| `static int local_static` (inside `main`) | none | `main` only | yes |
| `int global_initialized` | external | any `.c` of the program | no |

The compiler works on **one translation unit at a time**. Another file could declare `extern int global_initialized;` and use it, so GCC cannot prove a global is unused. Only the linker sees all object files, and the linker does not warn.

`static` does not only hide a variable: it gives the compiler information. This is one reason MISRA C requires objects not used outside a file to be `static`.

The fix was to **use** every variable (print it), not to silence the warning. If an unused `static` variable is removed by the optimizer, there is nothing left to analyze.

---

# Experiment 2 - Predict, Then Verify with `nm`

Before running any tool, I wrote down my prediction:

| Variable | Prediction |
|---|---|
| `global_initialized = 10` | `.data` |
| `global_uninitialized` | `.bss` |
| `static_initialized = 20` | `.data` |
| `static_uninitialized` | `.bss` |
| `message[]` (`const`) | `.rodata` |
| `local = 30` | stack |
| `local_static = 40` | `.data` |

## Command

```bash
nm build/debug/program | grep -E 'global|static|message|local|main'
```

## Output

```text
0000000000001220 t __do_global_dtors_aux
0000000000003d78 d __do_global_dtors_aux_fini_array_entry
                 U __libc_start_main@GLIBC_2.34
0000000000004010 D global_initialized
000000000000402c B global_uninitialized
0000000000004018 d local_static.0
0000000000001269 T main
0000000000002008 R message
0000000000004014 d static_initialized
0000000000004030 b static_uninitialized
```

## Observation

All six variables with static storage duration match the prediction:

| Variable | `nm` | Section |
|---|---|---|
| `global_initialized` | `D` | `.data` |
| `global_uninitialized` | `B` | `.bss` |
| `static_initialized` | `d` | `.data` |
| `static_uninitialized` | `b` | `.bss` |
| `message` | `R` | `.rodata` |
| `local_static` | `d` | `.data` |

- Uppercase letter = external linkage; lowercase = internal linkage (`static`). This is the same distinction seen in Experiment 1.
- `local_static` is a local variable, but `static` gives it static storage duration, so it lives in `.data` next to the globals. Only its **visibility** is local.
- GCC renamed it `local_static.0` so that another function can have its own `local_static` without a symbol clash.
- `__do_global_dtors_aux` and `__libc_start_main` are not my code: they belong to the C runtime that runs before and after `main()` (A07).

`local` does not appear at all. It has automatic storage duration: it does not exist in the file, only while `main` runs.

---

# Experiment 3 - Finding `local` in the Machine Code

If `local` is not in any data section, the value `30` (`0x1e`) must be inside the code.

This experiment was run on the first version of `main.c` (commit `ccce1de`), before the address-printing code was added.

## Command

```bash
objdump -d --no-show-raw-insn build/debug/program | grep -A15 '<main>:'
```

## Output (relevant line)

```text
    1175:	movl   $0x1e,-0x4(%rbp)
```

## Observation

```text
movl   $0x1e,      -0x4(%rbp)
 │       │              │
 │       │              └─ destination: memory at (rbp - 4)
 │       └─ source: the immediate value 30
 └─ move 32 bits (one int)
```

- `%rbp` is the frame pointer: the base of the stack frame of this call to `main`.
- `local` has no fixed address. It is a **position relative to the current stack frame**.
- The value `30` is stored inside the instruction itself, in `.text`.

Compare with `global_initialized`: its value `10` lives in `.data` and the code reads it from a fixed address.

---

# Experiment 4 - What the Optimizer Does with `local`

## Command

```bash
make BUILD=release
objdump -d --no-show-raw-insn build/release/program | grep -A15 '<main>:'
```

## Output (relevant lines, first version of `main.c`)

```text
    106c:	mov    $0x1e,%edx
    107b:	call   1050 <__printf_chk@plt>
```

## Observation

With `-O2`, `local` no longer exists in memory. There is no stack slot: the compiler loaded `30` directly into the register used to pass the argument to `printf`.

This is the **as-if rule**: the compiler may transform the program in any way, as long as the observable behavior (what `printf` prints) is the same. A C variable is an abstraction, not a promise of a memory location.

Consequences:

- In a release build, GDB reports such variables as `<optimized out>`.
- In embedded code, if the compiler believes a value cannot change, it may remove the read. This is why `volatile` exists (Phase B).

### Taking the address forces memory

After adding `printf("%p", (void *)&local)`, the release build changed:

```text
    116b:	movl   $0x1e,0xc(%rsp)
    118d:	mov    $0x1e,%edx
```

Once the program asks for the **address** of `local`, the variable must exist in memory (`0xc(%rsp)`). The compiler still passes `30` to `printf` directly in a register, because it knows the value.

---

# Experiment 5 - The Toolchain Adds Things I Did Not Write

In Experiment 4 the value `30` was in `%edx`, the **third** argument register, not `%esi`, the second. That meant the called function was not exactly `printf`.

## Command

```bash
objdump -d --no-show-raw-insn build/debug/program   | grep -A40 '<main>:' | grep call
objdump -d --no-show-raw-insn build/release/program | grep -A40 '<main>:' | grep call
```

## Output (first version of `main.c`)

```text
debug:    call   1070 <printf@plt>
release:  call   1050 <__printf_chk@plt>
```

## Observation

Ubuntu's GCC defines `_FORTIFY_SOURCE=2` by default when optimizing:

```bash
echo | gcc -O2 -dM -E - | grep -i fortify
```

```text
#define _FORTIFY_SOURCE 2
```

This replaces `printf` with `__printf_chk(flag, format, ...)`. The extra `flag` argument shifts every argument one register to the right, which is why `30` ended up in `%edx`.

**Same source code, same Makefile, different function in the binary.** The binary depends on the code, the flags, and also on the exact compiler and its distribution defaults. This is why safety-critical projects identify the toolchain by exact version, and why reproducible builds use a fixed compiler (for example inside a container).

---

# Experiment 6 - Runtime Addresses and ASLR

`main.c` was extended to print the address of every variable and of a block returned by `malloc`.

## Command

```bash
./build/debug/program
```

## Output

```text
message              0x563f2ede4008
global_initialized   0x563f2ede6010
static_initialized   0x563f2ede6014
local_static         0x563f2ede6018
global_uninitialized 0x563f2ede602c
static_uninitialized 0x563f2ede6030
heap_value           0x563f301042a0
local                0x7fff7d7e080c
```

## Observation

- `.rodata`, `.data` and `.bss` are a few KB apart.
- The heap is roughly 20 MB above them.
- The stack (`local`) is at the top of the user address space, `0x7fff...`.

Running the program twice gives **different addresses every time**. Linux loads the program at a random base address on each run: **ASLR** (Address Space Layout Randomization), a security feature.

The last three hex digits do not change (except for the stack): the program is moved in whole 4 KB pages (`0x1000`), so the offset inside the page is preserved.

## Relation Between `nm` and Runtime Addresses

```bash
printf '%x\n' $((0x563f2ede4008 - 0x2008))
printf '%x\n' $((0x563f2ede6010 - 0x4010))
```

```text
563f2ede2000
563f2ede2000
```

Both subtractions give the same value:

```text
runtime address = load base address + nm address
```

The executable is **PIE** (Position Independent Executable). `nm` shows addresses relative to the start of the program; at runtime the whole program is placed, as one block, at a random base. Distances between variables never change.

On a microcontroller there is no random base: the linker script fixes the addresses, and the `nm` address **is** the real address.

---

# Experiment 7 - The Process Memory Map

The program prints its own `/proc/self/maps`.

## Output (relevant lines, same run as Experiment 6)

```text
563f2ede2000-563f2ede3000 r--p 00000000 08:20 53102    build/debug/program
563f2ede3000-563f2ede4000 r-xp 00001000 08:20 53102    build/debug/program
563f2ede4000-563f2ede5000 r--p 00002000 08:20 53102    build/debug/program
563f2ede5000-563f2ede6000 r--p 00002000 08:20 53102    build/debug/program
563f2ede6000-563f2ede7000 rw-p 00003000 08:20 53102    build/debug/program
563f30104000-563f30125000 rw-p 00000000 00:00 0        [heap]
7fff7d7c1000-7fff7d7e2000 rw-p 00000000 00:00 0        [stack]
```

## Observation

| Region | Permissions | Contains |
|---|---|---|
| `563f2ede2000` | `r--` | ELF headers — this is the **load base** computed in Experiment 6 |
| `563f2ede3000` | `r-x` | `.text` |
| `563f2ede4000` | `r--` | `.rodata` → `message` |
| `563f2ede5000` | `r--` | relocation tables, made read-only after loading |
| `563f2ede6000` | `rw-` | `.data` + `.bss` → all static-duration variables |
| `[heap]` | `rw-` | `heap_value` |
| `[stack]` | `rw-` | `local` |

Every printed address falls inside the expected region.

The permissions are enforced by the **hardware** (the MMU):

- Code can be executed but not written.
- `.rodata` is read-only.
- Data, heap and stack can be written but not executed.

### A side effect: `stdout` moved my variables

`global_uninitialized` moved from `0x4020` to `0x402c` after adding `fputs(line, stdout)`:

```text
0000000000004020 B stdout@GLIBC_2.2.5
0000000000004028 b completed.0
000000000000402c B global_uninitialized
```

The linker reserved a copy of the C library's `stdout` inside **my** `.bss`. Adding one line of code changed the address of variables I did not touch. Addresses must never be assumed; they must be checked in the linker map.

---

# Experiment 8 - Writing to `const` Data

My prediction: this would **not compile**.

```c
const char message[] = "Embedded";
...
((char *)message)[0] = 'X';
```

## Commands and Output

```bash
gcc -std=c11 -Wall -Wextra -Werror -O0 -g3 main.c -o program
./program
```

```text
antes:   Embedded
Segmentation fault
exit code: 139
```

```bash
gcc -std=c11 -Wall -Wextra -Wcast-qual -Werror -c main.c
```

```text
main.c:10:6: error: cast discards 'const' qualifier from pointer target type [-Werror=cast-qual]
```

## Observation

My prediction was wrong:

1. **It compiles**, even with `-Wall -Wextra -Werror`. The explicit cast tells the compiler "trust me". Modifying an object defined as `const` is undefined behavior, and no diagnostic is required.
2. **At runtime the MMU stops it.** The write hits the `r--` page of `.rodata` and Linux kills the process with `SIGSEGV` (exit code 139 = 128 + signal 11).
3. **`-Wcast-qual` catches it at compile time.** `-Wall` does not include this warning.

On a microcontroller there may be no MMU to save you: writing to Flash may be ignored or cause a HardFault, and a `const` object placed in RAM would be silently modified. MISRA C (Rule 11.8) forbids casts that remove `const` or `volatile`.

As a result, `-Wcast-qual` was added to the flags of every laboratory. It immediately caught a real case in this module's own code (`(void *)message` passed to `%p`), fixed as `(const void *)message`.

---

# Process Memory Map

```text
high address   ┌─────────────┐
0x7fff...      │    stack    │  grows DOWN with each function call
               │      ↓      │
               │             │
               │ (huge gap)  │
               │      ↑      │
0x563f3010...  │    heap     │  grows UP with each malloc
               ├─────────────┤
0x563f2ede6... │ .data .bss  │  rw-
0x563f2ede4... │   .rodata   │  r--
0x563f2ede3... │    .text    │  r-x
low address    └─────────────┘
```

On a microcontroller the design is the same, inside a few KB of RAM. If the stack grows too much it collides with the heap or `.bss` and silently overwrites variables.

---

# Commands Used

```bash
make
make BUILD=release

nm build/debug/program | grep -E 'global|static|message|local|main'

objdump -d --no-show-raw-insn build/debug/program | grep -A15 '<main>:'
objdump -d --no-show-raw-insn build/release/program | grep -A15 '<main>:'
objdump -d --no-show-raw-insn build/release/program | grep -A40 '<main>:' | grep call

echo | gcc -O2 -dM -E - | grep -i fortify

./build/debug/program

printf '%x\n' $((0x563f2ede4008 - 0x2008))

gcc -std=c11 -Wall -Wextra -Wcast-qual -Werror -c main.c
```

---

# Tools Used

- GCC
- GNU Make
- GNU Binutils
    - nm
    - objdump
- `/proc/self/maps` (Linux)
- Git
- GitHub

---

# Key Concepts Learned

- The section of a variable depends on its storage duration and its initializer.
- `static` inside a function changes the lifetime, not the visibility.
- Automatic variables have no symbol: they are offsets in the stack frame.
- Linkage is visible in `nm`: uppercase for external, lowercase for internal.
- The compiler can only prove that internal-linkage objects are unused.
- The optimizer may remove a variable from memory entirely (as-if rule).
- Taking the address of a variable forces it to exist in memory.
- The toolchain can add behavior that is not in the build files (`_FORTIFY_SOURCE`).
- runtime address = load base + `nm` address (PIE + ASLR).
- Memory permissions are enforced by hardware; `const` data is read-only at runtime.
- `-Wall -Wextra` is not "all warnings": `-Wcast-qual` must be enabled explicitly.

---

# My Mental Model

The executable file is a **blueprint**. `nm` shows where each piece goes, relative to the start of the blueprint.

When the program runs, the operating system **builds the house at a random location** (ASLR). Every room keeps the same relative position, so one subtraction converts a blueprint address into a real address.

Automatic variables are not in the blueprint at all. They are furniture brought in when a function is called and removed when it returns.

---

# What Surprised Me

- I predicted all seven placements correctly, but I could not have proven it without `nm` and `objdump`.
- In release mode, `local` disappeared from memory completely.
- The release binary calls `__printf_chk`, a function I never wrote, because of a default in Ubuntu's compiler.
- Writing to a `const` array **compiles** without any warning under `-Wall -Wextra -Werror`. I expected a compile error; it was a segmentation fault.
- Adding `fputs(..., stdout)` moved the addresses of my global variables.

---

# Engineering Notes

- A warning is information about the code. Fixing the warnings in Experiment 1 also kept the variables in the binary for analysis.
- Predicting the result **before** running a tool is the same discipline as writing the expected result of a test before executing it.
- Results depend on the toolchain: the same Makefile produced `printf` in debug and `__printf_chk` in release.
- The warning set is an engineering decision. `-Wcast-qual` was added after this module and is now part of every laboratory build.

---

# Knowledge Check

Can I answer the following questions?

- [x] In which section does each kind of variable live, and why?
- [x] Why does `local` not appear in the output of `nm`?
- [x] Why does GCC warn about unused `static` variables but not about unused globals?
- [x] What does uppercase vs. lowercase mean in `nm` output?
- [x] Why can the runtime address of a variable change between runs?
- [x] How do I convert an `nm` address into a runtime address?
- [x] What happens when code writes to a `const` object after casting away `const`?
- [ ] Why does `.bss` take no space in the file but `.data` does? *(A05)*

---

# References

- ISO/IEC 9899:2011 (C11), §6.2.2 Linkages of identifiers, §6.2.4 Storage durations of objects
- GNU GCC Documentation — Warning Options (`-Wcast-qual`)
- GNU Binutils Documentation — `nm`, `objdump`
- `proc(5)` Linux manual page — `/proc/[pid]/maps`
- MISRA C:2012, Rule 11.8
- System V ABI, AMD64 Architecture Processor Supplement (argument registers)

---

# Questions for Further Study

- How does the startup code copy `.data` from Flash to RAM and zero `.bss`? *(A07)*
- How does a linker script decide the address of each section? *(A06)*
- How large are the stack and the heap, and who decides it?
- How can stack usage be measured to prevent a collision with the heap?
- What exactly is a copy relocation, like the one created for `stdout`?
- What is the role of `volatile` when the optimizer removes memory accesses? *(Phase B)*

---

# Summary

This module mapped every kind of C variable to its place in memory, first in the executable file and then in a running process.

Every placement was predicted first and then verified with binary evidence. The experiments showed that variables are abstractions the optimizer can remove, that the toolchain can change the binary without changes to the build files, that runtime addresses are the file addresses shifted by a random base, and that the hardware enforces `const` at runtime.

---

## Next Module

➡ **A03 - Stack**
