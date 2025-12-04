# 🧠 Week 2 — Day 2  
# Understanding `/proc/self/maps` and Linux Virtual Memory Layout

## 📌 Goal of Day 2
Learn how Linux represents a running program’s memory layout and how to read `/proc/self/maps`, the most important file for understanding real runtime memory behavior.

---

# 1. 📘 Virtual Memory Theory (Short & Precise)

Every process in Linux gets its own **virtual address space**.  
The kernel maps different memory regions into this space, each with a role:

### **Text segment**
Contains executable machine code.  
Usually mapped as:  

r-xp

   
### **Data & BSS**
- `.data` → initialized global variables  
- `.bss` → uninitialized global variables  

Memory flags:  


rw-p
  
### **Heap**
Grows upward using `sbrk()` or `mmap()` for allocations (`malloc`, `new`).

### **Shared Libraries**
Dynamic loader maps libraries such as:

- `libc.so.6`
- `ld-linux-x86-64.so.2`

Usually around the **0x7f******** region**.

### **Stack**
Grows downward from high memory → low memory.  
Contains:

- local variables  
- saved registers  
- return addresses  

### **Anonymous mappings**
Used for `malloc`, thread stacks, JIT regions, file-backed maps, etc.

---

# 2. 📌 Why `/proc/self/maps` Matters
This file shows:

- Real base addresses after ASLR  
- Exact library load positions  
- Stack, heap, text locations  
- Page permissions (rwx)  
- Shared object mappings  
- Gaps in the virtual address layout  
- PIE vs non-PIE differences  

This is **fundamental for exploit development**, binary analysis, and OS research.

---

# 3. 🧪 Experiments Performed

## (A) Mapping for our test program `hello.bin`

You ran:

```bash
./hello.bin &
pid=$(pgrep -f hello.bin)
cat /proc/$pid/maps > experiments/memory-mapping/proc-maps/hello_proc_maps.txt

