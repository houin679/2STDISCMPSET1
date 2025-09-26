# Multithreaded Prime Finder

This project demonstrates different approaches to finding prime numbers using **C++ multithreading**.  
It compares two strategies (range splitting vs. divisibility checking) and two output styles (immediate vs. buffered).

---

## Build & Run (Visual Studio 2022)

1. Open the provided `.sln` file in **Visual Studio 2022**.  
2. Press **Run** (▶️) to build and execute the program.
3. Once running, the program will display a menu

    Enter the number (1–4) corresponding to the mode you want to run.

      1 → Range division, immediate output
      2 → Range division, buffered output
      3 → Divisibility-threading, immediate output
      4 → Divisibility-threading, buffered output


---

## Program Overview

When you run the program, you will be prompted to select one of four modes:

1. **Range Division : Immediate Printing**  
   - The number space (up to `MAX_N`) is divided among threads.  
   - Each thread checks its range and prints primes **as soon as they are found**.  
   - Output is interleaved between threads.

2. **Range Division : Buffered Printing**  
   - Similar to (1), but each thread first **stores results locally**.  
   - After all threads finish, results are merged, sorted, and printed in order.  
   - Ensures clean, ordered output.

3. **Divisibility Threading : Immediate Printing**  
   - Each number is tested for primality by dividing work across multiple threads.  
   - Threads check different divisors in parallel.  
   - If prime, it is **printed immediately**.

4. **Divisibility Threading : Buffered Printing**  
   - Same as (3), but primes are collected into a buffer.  
   - After all checks, results are sorted and printed.  
   - Produces ordered output with reduced console contention.

---

## Key Concepts Demonstrated

- **Thread-based workload distribution**  
  - Range partitioning across threads.  
  - Divisor partitioning across threads for primality testing.

- **Synchronization**  
  - `mutex` used to protect shared result vectors.  
  - `atomic<bool>` used to stop divisor checking early if a factor is found.

- **Buffered vs. Immediate Output**  
  - Immediate: concurrent printing, faster but unordered.  
  - Buffered: results gathered and sorted before printing, slower but clean.

---
