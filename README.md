# Embedded Foundations

> A hands-on engineering roadmap documenting my journey toward becoming a **Senior Embedded Software Engineer**.

This repository is not a collection of programming exercises.

It is a long-term engineering project where every topic is studied through:

- 📖 Theory
- 🧪 Experiments
- 💻 Hands-on Laboratories
- 🔍 Binary Analysis
- 📄 Technical Documentation
- ✅ Knowledge Checks

The goal is to understand **how things work internally**, not just how to use them.

---

# Learning Philosophy

The objective of this repository is to understand embedded software from the ground up.

Every concept is approached using the following methodology:

1. Learn the theory.
2. Build a small laboratory.
3. Analyze the generated binaries.
4. Document the findings.
5. Validate the understanding through experiments.
6. Commit the results.

Nothing is accepted as true until it has been verified experimentally.

---

# Repository Structure

```text
embedded-foundations/
│
├── README.md
├── LICENSE
├── CMakeLists.txt
├── Makefile
│
├── docs/
│   ├── A01_Compilation.md
│   ├── A02_MemoryLayout.md
│   ├── Glossary.md
│   └── ...
│
├── examples/
│   ├── A01_Compilation/
│   ├── A02_MemoryLayout/
│   └── ...
│
├── include/
├── src/
├── tests/
├── images/
└── tools/
```

---

# Roadmap

## Phase 0 - Development Environment

- [x] Git
- [x] GitHub
- [x] VS Code
- [x] WSL Ubuntu
- [x] GCC
- [x] GNU Binutils

---

## Level 1 - C Language Mastery

### Phase A - Compilation & Memory Fundamentals

| Module | Status |
|----------|:------:|
| A01 - Compilation Process | ✅ |
| A02 - Memory Layout | ✅ |
| A03 - Stack | ⏳ |
| A04 - Heap | ⏳ |
| A05 - Sections (.text, .data, .bss, .rodata) | ⏳ |
| A06 - Linker | ⏳ |
| A07 - Startup Code | ⏳ |
| A08 - ABI | ⏳ |
| A09 - Calling Convention | ⏳ |

> **Scope:** A02 covers where each variable lives and the memory map of a running process.
> The detailed study of ELF sections (`readelf -S`, `size`, why `.bss` takes no file space) belongs to A05.

---

### Phase B - Advanced C

- Pointer Arithmetic
- Const Correctness
- Volatile
- Inline
- Restrict
- Macros
- Preprocessor
- Header Files
- Modular Design

---

### Phase C - Memory Representation

- Alignment
- Padding
- Packing
- Endianness
- Bit Fields
- Unions

---

### Phase D - Advanced Language Concepts

- Undefined Behavior
- Integer Promotions
- Strict Aliasing
- Sequence Points

---

### Phase E - Software Architecture

- Function Pointers
- Callback Mechanisms
- Interfaces in C
- Polymorphism in C
- Design Patterns

---

# Current Progress

| Level | Progress |
|--------|----------|
| Phase 0 | ✅ Complete |
| A01 | ✅ Complete |
| A02 | ✅ Complete |
| A03 | ⏳ Next |

---

# Tools Used

Development Environment

- Visual Studio Code
- Ubuntu (WSL2)
- Git
- GitHub

Compiler

- GCC

GNU Binutils

- file
- nm
- readelf
- objdump
- size

Build System

- Make
- CMake (`examples/A01_Compilation/CMakeLists.txt`)

---

# Engineering Workflow

Every module follows the same engineering process.

```text
Theory
    │
    ▼
Laboratory
    │
    ▼
Experiments
    │
    ▼
Binary Analysis
    │
    ▼
Documentation
    │
    ▼
Knowledge Check
    │
    ▼
Git Commit
```

---

# Why This Repository Exists

Many programming courses teach how to write C code.

This repository focuses on understanding what happens **behind the scenes**.

Topics include:

- How the compiler works.
- What object files contain.
- How the linker resolves symbols.
- How memory is organized.
- What happens before `main()`.
- How embedded firmware is actually built.

The long-term objective is to acquire the knowledge required for professional embedded software development.

---

# Completed Modules

| Module | Documentation |
|----------|---------------|
| A01 - Compilation Process | docs/A01_Compilation.md |
| A02 - Memory Layout | docs/A02_MemoryLayout.md |

---

# Future Topics

The repository will eventually include:

- Embedded Memory Management
- STM32 Development
- HAL Architecture
- RTOS
- FreeRTOS
- Embedded Drivers
- Unit Testing
- MISRA C
- Bootloaders
- OpenGL for Embedded Systems
- Embedded Linux
- Design Patterns
- CI/CD
- Static Analysis

---

# Author

**José Torres**

Embedded Software Engineer

Engineering roadmap toward Senior Embedded Software Engineering.

---

# License

This project is licensed under the MIT License.