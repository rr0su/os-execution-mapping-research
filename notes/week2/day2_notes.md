## **Understanding `/proc/self/maps` — The Real Runtime Memory Layout**

### **1. What is /proc/self/maps?**

Every Linux process has a virtual address space divided into regions.  
These regions include:

- Executable code (`.text`)
    
- Global variables (`.data`)
    
- Zero-initialized globals (`.bss`)
    
- Heap (`brk` and `mmap`)
    
- Stack
    
- Shared libraries (libc, ld-linux, libm, etc.)
    
- Anonymous `mmap()` regions
    
- VDSO (kernel helper page)
    
- Stack guard pages
    
- Heap guard pages
    

`/proc/self/maps` exposes the **true layout** that the kernel gives your process after ASLR, PIE, loader initialization, relocations, and library loading.

This is the **single most valuable file** in memory exploitation.

---

### **2. Why address space is NOT sequential**

Virtual memory is HUGE (128 TB on x86_64), and mappings appear scattered because:

- ASLR randomizes the start of each region
    
- PIE executables get randomized base addresses
    
- Shared libraries are mapped at random offsets
    
- Heap grows upward
    
- Stack grows downward
    
- mmap() allocations fill gaps dynamically
    

Nothing is linear anymore — everything is randomized.

This randomness is exactly why techniques like:

- ret2plt
    
- ret2libc
    
- GOT overwrite
    
- heap grooming
    
- stack leaks
    

are necessary.

You’re now learning the _origin_ of all these ideas.

---

### **3. What regions you will actually see in maps**

For a simple `hello.bin`, typical regions:

```css
0x555555554000-0x555555557000  r-xp  [text segment]
0x555555757000-0x555555758000  r--p  [data]
0x555555758000-0x555555759000  rw-p  [bss]
0x7ffff7dd7000-0x7ffff7fa7000  r-xp  libc.so.6
0x7ffff7ffc000-0x7ffff7ffe000  rw-p  ld-linux-x86-64.so.2
0x7ffffffde000-0x7ffffffff000  rw-p  [stack]
0xffffffffff600000-0xffffffffff601000  r-xp  [vdso]
```

we will NOT see section names like `.text` or `.data` here — maps shows **segments**, not ELF sections.

This is why we did Week-1 → program headers → segments first.

---

### **4. ASCII diagram (include in notes)**

```css
HIGH ADDRESSES
──────────────────────────────────────────────
|                Stack (grows down)           |
──────────────────────────────────────────────
|                Shared Libraries             |
|          (libc, ld-linux, etc.)            |
──────────────────────────────────────────────
|                  Heap (grows up)           |
──────────────────────────────────────────────
|      .data  |   .bss  |  .text (code)      |
|   [rw-p]    | [rw-p]  | [r-xp]             |
|   main exe mapping (PIE or no-PIE)         |
──────────────────────────────────────────────
LOW ADDRESSES
```



---

### **5. Why `/proc/self/maps` is GOD-level for exploit dev**

Because:

- It reveals exact location of libc
    
- Lets you calculate libc base → ret2libc exploit
    
- Shows stack base → sometimes stack pivoting
    
- Shows heap maps → heap exploitation
    
- Shows PIE base → ROP chain offsets
    
- Shows RWX pages → identify JIT or vulnerabilities
    
- Shows memory permissions (`rwx`) → identify attack surfaces
    

Every advanced CTF / exploit writeup uses maps somewhere.