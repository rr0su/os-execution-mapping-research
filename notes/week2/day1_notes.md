📘 WEEK 2 — DAY 1 — THEORETICAL NOTES
Topic: ELF File Deep Dive + Loader Responsibilities
1. What Happens When You Compile a C Program

Preprocessing → .i

Compilation → .s

Assembling → .o

Linking → ELF executable

2. ELF Header (Important Fields)

e_type → executable / relocatable / shared object

e_entry → entry point

e_phoff → program header table offset

e_shoff → section header table offset

3. Program Headers (Loader Uses These)

Essential for execve:

Segment	Meaning
PT_LOAD	OS should map this into memory
PT_DYNAMIC	For dynamic linking
PT_INTERP	Path to /lib/ld-linux-x86-64.so.2
PT_PHDR	Program header itself
PT_NOTE	Metadata

The loader never cares about section headers.

4. ELF Memory Layout

.text → RX

.data → RW

.bss → RW (zeroed)

.rodata → R

Mapped according to PT_LOAD segments.

5. Static vs Dynamic ELF

Static: no interpreter, no runtime linking

Dynamic: loader must map libc + resolve symbols

6. execve() → Kernel → Loader

Flow:

execve(path, argv, envp)

Kernel validates ELF

Kernel loads ELF program headers

If dynamic → load interpreter

Map all segments

Stack setup

Jump to entry point

7. Your Experiments That Prove Theory

my performed:

readelf -h hello.bin
readelf -l hello.bin
readelf -S hello.bin


These confirm:

Entry point

Program headers

Section headers

Dynamic tags
