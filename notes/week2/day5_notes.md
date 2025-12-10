
### 🔹 What is ASLR?

ASLR (Address Space Layout Randomization) is a memory protection that randomizes memory regions on every execution.

Goal:
Make exploitation harder by preventing attackers from predicting addresses.

----------
----------

🔹 What regions does ASLR randomize?

1. Stack

       Created per process

       High memory region

       Randomized base address every execution

       Affects:

          .Local variables (&x)

          .Saved RIP/RBP



2. Heap

       Grows upward

       Base is randomized

       Affects:

           .malloc() return addresses

           .Dynamic allocations



3. Shared Libraries

       libc, ld-linux, libpthread, etc

       Loaded into random virtual addresses

       Crucial for return-to-libc and ROP



4. PIE Text Segment (if PIE is enabled)

        For Position Independent Executables (-fPIE -pie)
   
        The entire code region relocates randomly
   
        So main() changes address every run



-----

## If PIE is disabled (-no-pie):

    .text remains fixed

    Only stack + heap + libs randomized


-------
------

### 🔹 Why ASLR matters for exploitation?

     You cannot hardcode addresses.

     ROP chains break unless you leak base addresses.

     Heap exploits change behavior per run.

     Ret2libc requires actual libc base.



``ASLR requires:

     information leak

     OR memory disclosure bug


This is the first step toward understanding:

    ROP

    ret2libc

    full memory layout attacks


---------
-----------
