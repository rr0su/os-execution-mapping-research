## 🎯 **Goal of Day 3**

Today i will map the connection between:

- **ELF file segments** (from program headers)
    
- **Linux virtual memory pages** (`/proc/<pid>/maps`)
    
- **What GDB sees as loaded regions** (`info proc mappings`)
    

This gives  the _real_ execution picture:

> _“What the loader actually mapped into memory.”_

This is a core foundation for later:

- ASLR entropy research
    
- PIE vs non-PIE mapping
    
- Memory corruption exploitation
    
- Kernel-side page faults understanding
    
- Deep RE and loader internals
    

---

# 📘 1. THEORY (What You Learned Today)

## 1.1 ELF Segments vs Sections

- **Sections** = compiler-level grouping (.text, .data, .rodata…).
    
- **Segments** = loader-level mapping units (PT_LOAD).
    
- Loader does **not** care about sections.
    
- Loader maps _segments_, aligns them to page boundaries.
    

Example:

|ELF Section|Belongs to Segment|Final Page Permissions|
|---|---|---|
|.text|LOAD #1|RX (read/execute)|
|.rodata|LOAD #1|R (read)|
|.data|LOAD #2|RW (read/write)|
|.bss|LOAD #2|RW, zero-filled|

---

## 1.2 Virtual Memory Pages (what the process sees)

Linux memory pages are always in multiples of **4 KB**.

During loading:

- Code pages → **RX**
    
- Data pages → **RW**
    
- Heap → **RW**
    
- Shared libraries → mapped as needed
    
- Stack → **RW**
    

This is observable via:

- `/proc/<pid>/maps`
    
- `gdb → info proc mappings`
    

---

## 1.3 GDB `info proc mappings`

This GDB command is a wrapper around `/proc/<pid>/maps`.

It shows:

- VMA start/end
    
- Permissions
    
- Backing file
    
- Offset in file
    
- Which part is anonymous memory
    

This tells you:

> _“Which part of my ELF is in memory, with exact addresses and permissions.”_

---

## 1.4 Why this is important

Understanding segments→pages mapping is required for:

- Writing reliable ROP chains
    
- Creating custom loaders
    
- Understanding how glibc is mapped
    
- ASLR bypass research
    
- Memory forensics
    
- Detecting memory corruption
    
- Kernel exploit research
    

This is one of your **core long-term foundations**.

---

# 🛠️ 2. HANDS-ON (Your Exact Commands Done Today)

## 2.1 Enter gdb

```bash
gdb -q ./hello.bin
set pagination off
```

---

## 2.2 Run program

```
run
```

---

## 2.3 Capture vmmap (GDB variant)

```gdb
set logging file /mnt/c/.../vmmap_hello.txt
set logging on
info proc mappings
set logging off
```

I then moved it into:

```bash
experiments/memory-mapping/gdb-outputs/
```

---

## 2.4 Capture ELF load info (`info files`)

```gdb
set logging file /mnt/c/.../info_files_hello.txt
set logging on
info files
set logging off
```

---

## 2.5 Capture backtrace

Backtrace mainly confirms:

- where execution currently is
    
- that mapping is correct
    
- glibc stack location
    

Commands:

```
set logging file /mnt/c/.../backtrace_hello.txt
set logging on
backtrace
set logging off
```

---

# 📂 3. OUTPUT FILES (Stored Today)

These 3 files were generated and moved into:

```
experiments/memory-mapping/gdb-outputs/
```

Files:

```
vmmap_hello.txt
info_files_hello.txt
backtrace_hello.txt
```

These act as the **ground truth evidence** of your memory mapping.

---

# 🧠 4. WHAT I SHOULD “SEE” INSIDE VMMap

You will observe:

### ✔ The main binary text segment

Permissions: `r-xp`

### ✔ The data segment

Permissions: `rw-p`

### ✔ The heap

Permissions: `rw-p` (grows up)

### ✔ The stack

Permissions: `rw-p` (grows down)

### ✔ Shared libraries (.so files)

Like:

```
libc.so.6
ld-linux-x86-64.so.2
```

### ✔ vvar, vdso, vsyscall

Special kernel pages.

---

# 🧩 5. Cross-Verify Segment → Page Mapping

Use:

```
readelf -l hello.bin
```

Check LOAD segments:

- Filesize vs Memsize
    
- Offset
    
- Permissions
    

Compare to GDB output:

```
info proc mappings
```

You will see perfect alignment (page-aligned).

This shows:

> _“This part of ELF → this exact runtime memory region.”_

---

# 🏁 6. End of Day Summary

Today you learned:

### ✔ How ELF segments become real runtime memory pages

### ✔ How to inspect them with GDB

### ✔ How loaders assign permissions

### ✔ How ASLR affects mapping

### ✔ How libc and ld.so appear in memory

### ✔ How to capture VM maps for research-grade documentation

This day is **critical**, forming the foundation for:

- Week 3 loader internals
    
- Week 4 ASLR entropy research
    
- Your PhD-level OS execution mapping project