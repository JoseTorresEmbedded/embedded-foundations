# A01 - C Compilation Process

> **Roadmap:** Level 1 - C Language Mastery  
> **Module:** A01 - Compilation Process  
> **Status:** ✅ Completed  
> **Last Updated:** July 2026

---

# Objective

Understand the complete C compilation pipeline, from a source file (`.c`) to an executable program.

By the end of this module I should understand:

- The compilation stages.
- The purpose of object files.
- The role of symbols.
- The responsibilities of the linker.
- The basic structure of an ELF object file.

---

# Why This Module Matters

Before starting this module I believed that:

> A compiler translates C code directly into an executable program.

After completing this module I now understand that the process is divided into several independent stages.

Each stage produces a different artifact that will later be consumed by another tool.

Understanding this pipeline is essential before studying:

- Memory layout
- Linker Scripts
- Startup Code
- Static variables
- Embedded systems

---

# Table of Contents

1. Compilation Pipeline
2. Laboratory
3. Experiments
4. ELF Object File
5. Symbols
6. Commands Used
7. Key Concepts Learned
8. Mental Model
9. Engineering Notes
10. Knowledge Check
11. References
12. Questions for Further Study
13. Summary

---

# Compilation Pipeline

```text
                 Source Code
                      │
                      ▼
              Preprocessor
                      │
                      ▼
                 Compiler
                      │
                      ▼
                 Assembler
                      │
                      ▼
              Object File (.o)
                      │
                      ▼
                   Linker
                      │
                      ▼
            Executable (ELF)
```

Each stage has a specific responsibility.

The compiler does **not** produce the final executable.

---

# Laboratory

## Repository Structure

```text
embedded-foundations/

└── examples
    └── A01_Compilation
        ├── main.c
        ├── math.c
        └── math.h
```

---

# Experiment 1 - Compile Source Files Independently

## Commands

```bash
gcc -c main.c
gcc -c math.c
```

Generated files

```text
main.o
math.o
```

## Observation

The `-c` option tells GCC to stop after generating the object file.

No executable is created.

Each source file becomes an independent object file.

---

# Experiment 2 - Inspect the Object File

Command

```bash
file main.o
```

Output

```text
ELF 64-bit LSB relocatable
```

## Observation

The object file is:

- ELF format
- Relocatable
- Not executable

This means another step is still required before the program can run.

---

# Experiment 3 - Symbol Inspection

Command

```bash
nm main.o
```

Output

```text
U add
T main
U printf
```

## Interpretation

| Symbol | Meaning |
|----------|------------------------------------------------|
| U | Undefined symbol. Referenced but not defined inside this object file. |
| T | Function located in the `.text` section. |
| D | Initialized global variable (`.data`). |
| d | Initialized static variable (`.data`). |
| B | Uninitialized global variable (`.bss`). |
| b | Uninitialized static variable (`.bss`). |
| R | Read-only data (`.rodata`). |

---

# Experiment 4 - Inspect ELF Sections

Command

```bash
readelf -S main.o
```

Relevant Sections

| Section | Description |
|-----------|---------------------------------------------|
| .text | Executable machine code |
| .data | Initialized global/static variables |
| .bss | Uninitialized global/static variables |
| .rodata | Read-only data |
| .symtab | Symbol table |
| .strtab | String table |
| .rela.text | Relocation information used by the linker |

---

# Experiment 5 - Variable Placement

Variables added:

```c
int global_initialized = 100;

int global_uninitialized;

static int static_initialized = 50;

static int static_uninitialized;

const char message[] = "Hello World";
```

Result

```text
D global_initialized
C global_uninitialized
d static_initialized
b static_uninitialized
R message
```

## Observation

Different storage classes generate different symbol types and are placed into different ELF sections.

This experiment demonstrates that memory placement depends on the storage class of each variable.

---

# Object File Anatomy

An object file is not just machine code.

Conceptually it looks like this:

```text
+----------------------------+
| ELF Header                 |
+----------------------------+
| .text                      |
+----------------------------+
| .data                      |
+----------------------------+
| .bss                       |
+----------------------------+
| .rodata                    |
+----------------------------+
| Relocation Information     |
+----------------------------+
| Symbol Table               |
+----------------------------+
| String Table               |
+----------------------------+
```

Each section serves a different purpose.

---

# Commands Used

```bash
gcc -c main.c

gcc -c math.c

gcc main.o math.o -o program

file main.o

nm main.o

readelf -h main.o

readelf -S main.o

readelf -s main.o

size main.o
```

---

# Tools Used

- GCC
- GNU Binutils
    - file
    - nm
    - readelf
    - size
- Git
- GitHub
- WSL Ubuntu
- Visual Studio Code

---

# Key Concepts Learned

- The compiler processes one source file at a time.
- Every source file generates an independent object file.
- Object files are not executable.
- Undefined symbols are expected during compilation.
- The linker resolves symbol references.
- ELF files contain multiple sections.
- Different storage classes produce different symbol types.
- Machine code is stored inside the `.text` section.

---

# My Mental Model

I imagine every object file as an individual LEGO block.

Each block contains:

- machine code
- symbols
- relocation information
- metadata

Some pieces are still disconnected because they reference symbols that are not yet defined.

The linker connects all LEGO blocks together to build the final executable.

---

# What Surprised Me

Before this module I believed the compiler generated the executable directly.

Now I understand that:

- Every source file becomes an object file.
- Object files may contain unresolved symbols.
- The linker is responsible for connecting all object files into a complete program.

---

# Engineering Notes

One of the biggest takeaways from this module is that object files are not black boxes.

Using tools such as:

- nm
- readelf
- file
- size

allows me to inspect the internal structure generated by the compiler.

This will become extremely useful when studying:

- Linker Scripts
- Startup Code
- Memory Layout
- Embedded Firmware

---

# Knowledge Check

Can I answer the following questions?

- [x] Why does `gcc -c` generate an object file instead of an executable?
- [x] Why can the compiler generate `main.o` even though `add()` is not implemented?
- [x] What does an undefined symbol (`U`) mean?
- [x] What does `T` mean?
- [x] What is an ELF object file?
- [x] What is the role of the linker?
- [x] How can I inspect an object file?

---

# References

- The C Programming Language (Kernighan & Ritchie)
- GNU GCC Documentation
- GNU Binutils Documentation
- ELF Specification
- System V ABI

---

# Questions for Further Study

- Why is `.data` copied from Flash to RAM during startup?
- Why does `.bss` occupy RAM but not Flash?
- Why are string literals stored in `.rodata`?
- What information is stored inside `.rela.text`?
- How does the linker resolve symbols?
- What happens before `main()` is executed?

---

# Summary

This module introduced the complete C compilation pipeline.

Instead of treating the compiler as a black box, I learned how source files become object files and how the linker combines them into an executable.

This module provides the foundation required to understand memory layout, linker scripts, startup code, and embedded firmware architecture.

---

## Next Module

➡ **A02 - Memory Layout**