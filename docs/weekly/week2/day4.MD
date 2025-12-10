📘 DAY 4 — Stack, Heap, and Function Address Logging
🎯 Goal

Understand how the stack, heap, and code segment appear in memory at runtime — and verify their behavior using a small C program.

Today you learn:

Why stack grows downward

Why heap grows upward

Why function addresses (text segment) stay fixed unless PIE

How ASLR affects stack + heap

How to record these addresses for research



🧠 THEORY
🔹 1. Stack

The stack is a LIFO memory region used for:

local variables

function call frames

return addresses

Memory layout:

High addresses
     ↓ (stack grows downward)
Low addresses


Meaning:

Every new local variable → lower memory address

Function calls push new frames downward

Stack is close to kernel memory, so ASLR randomizes the base each run

-----------


🔹 2. Heap

The heap is used for dynamic allocations (malloc, calloc, realloc).

Memory layout:
```
Low addresses
     ↑ (heap grows upward)
High addresses
```

Meaning:

Every malloc() tends to allocate higher addresses

ASLR randomizes the heap base as well


---------

🔹 3. Code Segment (Text Segment)

Contains:

machine instructions

function entry points (main, other functions)

Properties:

usually read + execute (RX)

does not change across allocations

stays fixed unless PIE is enabled

If PIE is enabled, the entire binary is position-independent, so function addresses will also move.



---------

🔹 4. Why These Addresses Matter

This is core binary exploitation foundation:

Stack addresses → for stack-based buffer overflows

Heap addresses → for heap exploitation, UAF, tcache attacks

Function addresses → for ROP, GOT overwrite, ret2text, ASLR bypass

Knowing how to observe them is mandatory to understand how a process lives in memory.
