# Week 1 — Day 5 Log

## Goal
Understand the full ELF layout: header → program headers → sections → segments → raw bytes.

## Commands Used
```bash
readelf -h experiments/hello.bin
readelf -l experiments/hello.bin
readelf -S experiments/hello.bin
readelf -a experiments/hello.bin
xxd experiments/hello.bin
