PIE vs Non-PIE Mapping
🎯 Goal

Show the difference between building an executable as PIE (position-independent) vs Non-PIE and record how that affects where the program’s text (.text/code) segment is mapped at runtime. Capture /proc/<pid>/maps for both versions and save the results for later analysis.

1. Short theory (2–4 paragraphs)

Non-PIE executables are linked to a fixed preferred load address. When run, their code segment (text) typically loads at a predictable base (on x86_64 often near 0x555555554000 in simple examples). Because the text segment does not get relocated, function addresses inside the binary remain constant across runs — making gadget addresses and offsets predictable.

PIE executables are linked as position-independent code (like shared libraries). At runtime the loader may choose a randomized base for the entire binary, enabling the main program’s text segment to be placed at different addresses each run. PIE + ASLR increases entropy for the binary itself and significantly raises the difficulty for attackers trying to use hardcoded addresses (ROP gadgets, direct function calls).

In short: Non-PIE = predictable code base (weaker), PIE = relocatable/randomized code base (stronger). For exploit development and ASLR research you must always check whether a binary is PIE or not; that determines whether leaking one address is sufficient to compute other addresses.



-------------
---------

# . Commands
```bash
# prepare day6 output folder
mkdir -p experiments/memory-mapping/day6

# from src/ (where hello.c lives)
cd src

# compile non-PIE and PIE versions
gcc hello.c -no-pie -o ../experiments/memory-mapping/day6/hello_nopie
gcc hello.c -pie   -o ../experiments/memory-mapping/day6/hello_pie
cd ..

# run non-PIE and capture maps
./experiments/memory-mapping/day6/hello_nopie &
echo $! > experiments/memory-mapping/day6/pid_nopie.txt
cat /proc/$(cat experiments/memory-mapping/day6/pid_nopie.txt)/maps \
    > experiments/memory-mapping/day6/maps_nopie.txt
kill $(cat experiments/memory-mapping/day6/pid_nopie.txt)

# run PIE and capture maps
./experiments/memory-mapping/day6/hello_pie &
echo $! > experiments/memory-mapping/day6/pid_pie.txt
cat /proc/$(cat experiments/memory-mapping/day6/pid_pie.txt)/maps \
    > experiments/memory-mapping/day6/maps_pie.txt
kill $(cat experiments/memory-mapping/day6/pid_pie.txt)

# capture ELF headers (optional but useful)
readelf -h experiments/memory-mapping/day6/hello_nopie \
    > experiments/memory-mapping/day6/readelf_hello_nopie.txt
readelf -h experiments/memory-mapping/day6/hello_pie \
    > experiments/memory-mapping/day6/readelf_hello_pie.txt

# quick compare (text segments)
grep ' r-xp ' experiments/memory-mapping/day6/maps_nopie.txt \
    > experiments/memory-mapping/day6/compare_text_segments.txt
echo '---' >> experiments/memory-mapping/day6/compare_text_segments.txt
grep ' r-xp ' experiments/memory-mapping/day6/maps_pie.txt \
    >> experiments/memory-mapping/day6/compare_text_segments.txt

```

-------
--------

# What to expect (how to read the outputs)
```
experiments/memory-mapping/day6/maps_nopie.txt — you should see the binary’s r-xp mapping at a stable/predictable address (often 0x5555... pattern on x86_64).

experiments/memory-mapping/day6/maps_pie.txt — binary’s r-xp mapping will be at a different, randomized base (often 0x55ab... or other).

readelf_hello_nopie.txt vs readelf_hello_pie.txt — Type: field shows EXEC for non-PIE and DYN for PIE.

compare_text_segments.txt shows the two r-xp lines side by side for quick visual comparison.
```

