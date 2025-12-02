# Week 1 — Day 6 Log

## Goal
Understand ELF sections, segments, and how the loader maps the binary into memory.

## ELF Header Summary
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Position-Independent Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x1060
  Start of program headers:          64 (bytes into file)
  Start of section headers:          13976 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         13
  Size of section headers:           64 (bytes)
  Number of section headers:         31
  Section header string table index: 30


## Memory Mapping Diagram (to convert into PNG later)

```
[ELF File]
├── Sections
│   ├── .text
│   ├── .rodata
│   ├── .data
│   └── .bss
│
└── Segments (runtime)
    ├── PT_LOAD (RX) → .text + part of .rodata
    ├── PT_LOAD (RW) → .data + .bss
    ├── PT_DYNAMIC
    └── PT_INTERP

[Memory Layout]
0x400000 → RX (text segment)
0x600000 → RW (data segment)
```
