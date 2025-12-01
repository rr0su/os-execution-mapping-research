
📘 OS Execution Mapping & Micro-RE Analysis Suite

<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/34fea467-d666-4b0a-8b48-b69f2057bc53" />


A research project that documents the complete lifecycle of a Linux program — from source code to running process.
This project reveals what actually happens inside the OS during:

Compilation

ELF binary structure

Program loading

Dynamic linking (PLT/GOT)

Virtual memory mapping

Calling conventions & stack frames

Basic static/dynamic reverse engineering

Safe stack behavior demonstrations

All experiments are reproducible and supported by code samples, logs, and diagrams.

-------

📂 Repository Structure

```text
os-execution-mapping-research/
│
├── docs/          # Research chapters and explanations
├── code/          # Small C programs used for experiments
├── experiments/   # Outputs (assembly, objdump, readelf, gdb, strace)
├── diagrams/      # Visual diagrams of internal OS mechanisms
└── README.md
```

----------

🧩 Research Coverage
1. Compiler Pipeline

How a C program becomes a binary:

Preprocessing

Compilation

Assembly output

Object file generation

Linking


2. ELF Internals

Deep breakdown of:

ELF headers

Sections & segments

Symbol tables

Relocations


3. Program Loading

What happens when you run ./a.out:

execve()

_start before main

Kernel → loader → user program


4. Dynamic Linking

How functions like printf resolve at runtime:

PLT / GOT

Lazy binding

Runtime relocations


5. Virtual Memory Mapping

Based on /proc/self/maps:

Stack

Heap

Text / data

Shared libraries

VDSO / VVAR

ASLR behavior


6. Calling Conventions

System V AMD64:

Register arguments

Stack frames

Function prologue/epilogue

Return address handling


7. Micro Reverse-Engineering Suite

Small, safe RE tasks:

Disassembly reading

Identifying functions

Optimized vs non-optimized binaries

Using gdb / objdump / strace / ltrace


8. Safe Stack Behavior Demonstrations

Conceptual examples:

Stack bounds

Stack canaries

NX protection

PIE basics

No harmful exploitation — only safe demonstrations for academic understanding.


-------------

🧪 Experiment Tools
```text
The project uses:
gcc
readelf, objdump
gdb
strace, ltrace
/proc/self/maps
Markdown + diagrams
```

----------

🎯 Goal

Produce a complete “execution map” of how Linux compiles, loads, links, and executes a program.
This project forms a strong foundation for future work in:

Reverse engineering

Binary exploitation research

Systems programming

OS internals

Compiler & toolchain research

----

📅 Status

Active — content added daily as experiments and documentation progress.
