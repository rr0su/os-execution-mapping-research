**“Stack, Heap, and Function Address Logging — Why These Addresses Matter in Modern Binary Exploitation”**

---

## 🔥 1. **Memory Layout Refresher (Process Virtual Address Space)**

When a Linux program runs, it is placed into a virtual memory layout designed by the OS + loader (`ld.so`). The important regions:

```scss
HIGH ADDRESSES
-----------------------  Stack (grows DOWN)
|                      |
|        stack         |
|                      |
-----------------------
|      Shared libs     |
-----------------------
|                      |
|        heap          |  <-- grows UP (malloc/brk)
|                      |
-----------------------
|      .data/.bss      |
-----------------------
|      .text (code)    |
-----------------------
LOW ADDRESSES
```

Each region has **different security properties**, permissions, and behavior:

- **.text (code) → RX**
    
- **heap → RW**, dynamic, used for `malloc`
    
- **stack → RW**, used for function frames
    
- **shared libs** mapped by loader
    
- **VSYSCALL/VVAR/VDSO pages**
    

---

## 🔥 2. **Stack — Grows Downward**

The **stack** stores:

- function local variables
    
- saved return addresses
    
- saved base pointers
    
- arguments (depending on ABI)
    

The stack **grows downward**, meaning:

- Higher address → caller
    
- Lower address → deeper nested functions
    

Example:

```css
0x7fffffffe000  ← top of stack
...
0x7fffffffd000  ← deeper stack frames
```

Why this matters:

- Buffer overflows overwrite **lower addresses**
    
- Understanding stack direction = understanding _how corruption spreads_
    
- ROP chains and return address control rely on this
    

---

## 🔥 3. **Heap — Grows Upward**

The **heap** is where memory allocated via:

- `malloc`
    
- `calloc`
    
- `realloc`
    
- `new` (C++)
    

Heap grows **upward**:

```
0x555555760000   ← heap start
0x555555770000   ← more allocations

```

Heap security is different:

- Metadata stored around chunks
    
- Use-after-free, double-free, heap overflows
    
- ASLR randomizes top
    

---

## 🔥 4. **Code Segment (Text Segment)**

Contains:

- machine instructions
    
- constant code
    
- immutable data in `.rodata`
    

Permissions: `r-x`

Important:

- Code segment location affects **function addresses**
    
- PIE enabled:
    
    - code segment is **randomized**
        
    - so function addresses change every run
        
- PIE disabled:
    
    - function addresses are **stable**, ideal for learning
        

---

## 🔥 5. **Why Logging These Addresses Matters**

This experiment teaches you the three pillars of memory exploitation:

### **1. Stack Address → ASLR on Stack**

- You learn how randomized the stack is
    
- You verify if stack offset is constant/variable
    
- Used when building stack-based exploits
    

### **2. Heap Address → Understanding Allocator Behavior**

- Heap base often reveals ASLR entropy
    
- Helps you understand glibc malloc chunk layout
    
- Necessary for heap exploitation (use-after-free, overwrite)
    

### **3. Function Addresses (main → text segment)**

- Shows if PIE is enabled
    
- Shows the randomized base of the binary
    
- Critical for ROP gadget finding
    
- Helps you understand the linker + loader
    

If:

- `main()` address changes → PIE enabled
    
- heap address changes → ASLR enabled
    
- stack address changes → ASLR enabled
    

This gives you a **complete understanding of the program’s runtime memory map**.

---

## 🔥 6. **Expected Observations**

You will observe:

- Stack address (`&x`) → almost always around **0x7fffffffdXXX**
    
- Heap address → around **0x55555577XXXX**
    
- Function address → small, near base of program
    

This visually connects:

```r
STACK > LIBS > HEAP > DATA > TEXT
```

Exactly what you need for:

- ret2text
    
- ret2plt
    
- ret2libc
    
- ROP
    
- heap exploit
    
- PIE/ASLR bypass thought process