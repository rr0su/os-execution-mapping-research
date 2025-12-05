## **1. What is “Process Memory Mapping”?**

When a program runs, Linux loads it into memory by mapping **ELF segments** into **virtual memory pages**.

we saw `/proc/<pid>/maps` in Day 2.  
Today we use **GDB** + **pwndbg** to visualize this more clearly.

### A process has memory regions like:

|Region|Meaning|
|---|---|
|**Text (Code)**|Executable instructions (.text)|
|**Data**|Initialized global vars (.data)|
|**BSS**|Zero-initialized globals (.bss)|
|**Heap**|Memory from malloc/brk|
|**Stack**|Per-thread stacks|
|**Shared libs**|libc, ld, etc.|
|**VDSO / VVAR**|Kernel helper pages|

These are **not random** — they come directly from the **ELF program headers**.

---

## **2. How ELF Segments Become Memory Pages**

The ELF binary contains **segments**, not pages.

Examples:

|Segment|Flags|Typical Pages|
|---|---|---|
|`PT_LOAD` (code)|`R-X`|Maps into RX pages|
|`PT_LOAD` (data)|`RW-`|Maps into RW pages|
|`PT_DYNAMIC`|`RW-`|Dynamic linker structures|
|`PT_INTERP`|`R--`|Path to `/lib64/ld-linux-x86-64.so.2`|

The kernel reads these segments and maps them into **4 KB virtual pages**.

### Example of mapping:

ELF segment `.text` from file offset `0x1000 → 0x3fff`  
is loaded into memory pages like:

```r
0x555555554000  RX  
0x555555555000  RX  
0x555555556000  RX  
```

ELF `.data` segment is loaded like:

```r
0x555555758000  RW
0x555555759000  RW
```

📌 **IMPORTANT RULE:**  
**Segments → Memory regions → Each region is aligned to page boundaries.**

---

## **3. Why GDB is Needed**

`/proc/<pid>/maps` shows everything _after the program is running_.  
GDB shows **how ELF maps to memory** and also shows **symbol → address mappings**.

We use:

- `info proc mappings` → raw memory map
    
- `info files` → ELF section → memory
    
- `pwndbg vmmap` → pretty printed regions
    
- `backtrace` → verify stack → return addresses
    

---

## **4. GDB Commands (Deep Explanation)**

### **4.1 `info proc mappings`**

Shows exactly the same information as `/proc/<pid>/maps`, but inside GDB.

It reveals:

- base address of binary
    
- each mapped region
    
- permissions
    
- file association (ld, libc, stack, heap)
    

Example output (simplified):

```r
0x555555554000-0x555555555000 r-xp hello.bin
0x555555755000-0x555555756000 r--p hello.bin
0x555555756000-0x555555757000 rw-p hello.bin
0x7ffff7ddc000-0x7ffff7fa3000 r-xp libc.so.6
...
```

---

### **4.2 `info files`**

This is the most IMPORTANT command today.

It tells you:

- where `.text` is mapped
    
- where `.data` is mapped
    
- where the entry point is
    
- how ELF sections land in memory
    

Example:

```r
Entry point: 0x555555554000
0x555555554000 - 0x555555554020 is .interp
0x555555554020 - 0x555555556000 is .text
0x555555756000 - 0x555555757000 is .data
```

This is THE connection:

```r
ELF sections → Segment → Virtual memory pages
```
---

### **4.3 `pwndbg vmmap`**

This is a beautiful visualization:

```
pwndbg> vmmap
```

Output example:

```r
0x555555554000 0x555555556000 r-xp /hello.bin     ← CODE (RX)
0x555555756000 0x555555757000 rw-p /hello.bin     ← DATA (RW)
0x7ffff7ddc000 0x7ffff7fa3000 r-xp /lib/x86_64-linux-gnu/libc.so.6
0x7fffffffde000 0x7ffffffffff000 rw-p [stack]

```

Color-coded and grouped.

---

### **4.4 `backtrace`**

Why?

Because it proves:

- stack memory region is correct
    
- call frames are working
    
- return addresses → belong to **text (RX) pages**
    

Example:

```css
#0  main at hello.c:5
#1  0x7ffff7c2d082 in __libc_start_main
```

---

## **5. PIPELINE: ELF → Segments → Pages → GDB Mappings**

Here is the **full process**, extremely clear:

1. ELF has **sections** (.text, .data, .rodata)
    
2. Linker groups sections into **segments**
    
3. Kernel loads segments into **pages**
    
4. GDB displays those pages
    

```css
ELF (.text, .data)
       ↓
PT_LOAD segments
       ↓
Kernel maps them into memory pages
       ↓
GDB shows them via:
- info files
- info proc mappings
- pwndbg vmmap
```

This is the ENTIRE today’s teaching.

---

## **6. What You Will Do in Experiments**

You will extract **three datasets**:

### `vmmap_hello.txt`

Using:

```
vmmap
```

Shows live memory layout.

### `info_files.txt`

Using:

```
info files
```

Shows mapping between ELF + memory.

### `backtrace.txt`

Using:

```
backtrace
```

Shows call stack → stack frame → return address locations.

You will save them in:

```
experiments/memory-mapping/gdb-outputs/
```