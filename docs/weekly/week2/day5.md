✍️ DAY 5 — ASLR Behavior Experiment
Goal

Observe ASLR randomness in stack, heap, and function addresses.


--------

Commands Used
Run program 10 times

```bash
for i in {1..10}; do ./addr >> experiments/memory-mapping/aslr_runs.txt; done
```

----

Results Table

Run	Stack addr	Heap addr	main() addr
1	0x7ff…	          0x55f…	 0x555…
2	0x7ff…	          0x564…	 0x555..


----------

Patterns you should see:

    Stack: changes every run

    Heap: changes every run

    main(): fixed because you compiled with -no-pie

If you compile with PIE later:

main() also becomes random.

---------
--------
