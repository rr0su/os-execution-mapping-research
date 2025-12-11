✅ DAY 6 — PIE vs Non-PIE Memory Mapping (Notes)
🔵 1. What is PIE (Position Independent Executable)?

PIE = a binary whose code segment can be relocated anywhere in memory at runtime.

The compiler generates code using relative addressing

The loader can place the executable at any randomized base address

ASLR becomes effective for the main executable

PIE makes the text/code segment unpredictable, adding entropy.

🔵 2. What is Non-PIE?

Non-PIE = traditional executable where:

Text segment loads at a fixed base address

Common base on 64-bit Linux:
0x555555554000 (varies, but predictable)

Even with ASLR enabled:

Stack → randomized

Heap → randomized

mmap region → randomized

Executable text base → NOT randomized ❌

This predictability weakens exploit mitigation.

🔵 3. Why PIE Matters (Security Impact)
Without PIE:

ROP gadgets reside at known offsets

Return-to-text attacks become easy

Single memory leak gives full binary control

With PIE:

The entire executable shifts randomly each run

Gadgets move

Function addresses move

Attackers must defeat ASLR before exploitation

PIE = significantly harder exploitation.

🔵 4. How to Check PIE Status
Using readelf:

```
readelf -h hello | grep Type
```

Type: EXEC → Non-PIE

Type: DYN → PIE
