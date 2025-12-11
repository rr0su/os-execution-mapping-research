# Week 2 — OS Execution Mapping Research Summary

This week focused on understanding how a Linux ELF binary becomes a running process, how its memory layout forms, and how security mechanisms (ASLR + PIE) influence entropy and predictability. This is foundational for exploit development, loader analysis, and kernel-level research.

---

## 1. How an ELF Becomes a Process (Loader → Memory Layout)

When I execute:

./hello.bin


the Linux kernel:

1. Reads the ELF header  
2. Parses the **Program Header Table (PHT)** to identify loadable segments  
3. Invokes the ELF dynamic loader (`ld-linux-x86-64.so.2`)  
4. Loader maps segments into memory and relocates symbols  
5. Control is transferred to `_start()`

The kernel + loader convert **file segments → memory regions**, which appear in:

/proc/<pid>/maps


This matches MY Program Header Table outputs.

---

## 2. Segments → Pages: Runtime Memory Layout

ELF files contain **segments**, not “sections”.  
Segments are mapped to process memory pages.

| Segment     | Purpose                    | Flags | Page Type |
|-------------|-----------------------------|--------|-----------|
| `.text`     | Machine code               | r-x    | RX pages  |
| `.rodata`   | Constants                  | r--    | RO pages  |
| `.data`     | Initialized globals        | rw-    | RW pages  |
| `.bss`      | Zero-initialized globals   | rw-    | RW pages  |
| `PT_DYNAMIC`| Relocation info            | rw-    | Metadata  |

This mapping was visible in your `hello_proc_maps.txt` and GDB outputs.

---

## 3. Stack, Heap, Data, and BSS Behavior

From  Day-4 experiments:

### Stack
- Lives high in memory  
- Grows **downward**  
- Contains function locals (`int x`)

### Heap
- Allocated by `malloc()`  
- Grows **upward**  
- Managed by glibc via `brk()` and `mmap()`

### Data / BSS
- `.data` → globals with initial values  
- `.bss` → globals initialized to zero  

These appear as RW segments in `/proc/<pid>/maps`.

---

## 4. ASLR — Address Space Layout Randomization

Checked via:

/proc/sys/kernel/randomize_va_space


ASLR randomizes:
- Stack base  
- Heap base  
- mmap regions  
- Shared library bases  

But **non-PIE text base remains fixed**, so code addresses (like `main`) do **not** change in non-PIE binaries
I repeated runs proved:

- Stack address changes  
- Heap address changes  
- Function address in non-PIE stays constant  

---

## 5. PIE — Position Independent Executable

### Non-PIE:

Type: EXEC
Fixed text base (e.g., 0x555555554000)
Function addresses do not change

### PIE:
Type: DYN
Text base is randomized every run
All functions shift by the PIE slide


MY Day-6 captures:

- `maps_nopie.txt` → constant base  
- `maps_pie.txt` → randomized base  

PIE greatly increases entropy needed for ROP or code-reuse attacks.

---

## 6. ld-linux — The Dynamic Loader

Every dynamically-linked ELF is started by:

/lib64/ld-linux-x86-64.so.2

It performs:
- Segment mapping  
- Relocations / symbol binding  
- PLT/GOT patching  
- Shared library loading  
- Transfer to `_start()`  

In your MAPS files you always saw an entry for the loader.

---

## 7. Week-2 Achievements

By the end of Week 2, you have:

- Complete mapping of ELF file → memory layout  
- Stack, heap, and segment behavior documented  
- ASLR randomness captured  
- PIE vs non-PIE behavior experimentally verified  
- GDB + loader internal outputs  
- A reproducible OS-level RE research environment  

This structure matches real RE lab methodology.

---

## 8. Final Folder Structure (Relevant to Week-2)

```
docs/weekly/week2/
├── day1.md
├── day2.md
├── day3.md
├── day4.md
├── day6.md
└── week-summary.md
```


Experiments:

```
experiments/memory-mapping/
├── proc-maps/
├── gdb-outputs/
├── addr_raw_output.txt
├── stack.txt
├── heap.txt
├── function_addrs.txt
├── maps_nopie.txt
├── maps_pie.txt
```


---

## 9. Final Insight

ELF execution is **a pipeline**:

ELF → Kernel → Loader → Segments → Pages → Process


Understanding this pipeline is essential for:
- Exploit development  
- Binary hardening  
- Loader/VM analysis  
- Kernel-level research  

This week completed MY foundation for deeper virtual memory and loader internals in Week-3.

---


