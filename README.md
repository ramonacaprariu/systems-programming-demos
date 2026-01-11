# systems-programming-demos

Small C/Unix systems programming demos: bit manipulation and thread synchronization (mutexes, semaphores)

## Contents

- `bit_operations_heartrate.c` — Packs/decodes a user-provided heartrate reading into 16-bits using custom bitwise functions (bitset, bitclear, bitread); prints hex + grouped binary number and interprets status/flag bits.
- `sync_mutex_pattern.c` — Thread synchronization demo using `pthread_mutex_t` to control access, for a pattern of 1 number and 2 letters, alternating.
- `sync_semaphore_limiter_mac.c` — macOS-geared semaphore limiter using `dispatch_semaphore_t` with pthreads.
- `sync_semaphore_limiter.c` — POSIX semaphore (`sem_t`) limiter.
- `run_sync_demos.sh` — Builds the synchronization demos and runs them via a small menu (with `expect` automation when running the two demos successively).

## Requirements

- macOS or Linux
- `clang` or `gcc`
-  `expect` (for automating prompt interactivity)
    - macOS: `brew install expect`
    - Ubuntu Linux: `sudo apt-get install expect`

## Build

```bash
clang -g sync_mutex_pattern.c -o mutex_demo -pthread
clang -g sync_semaphore_limiter_mac.c -o sem_demo -pthread
clang -g bit_operations_heartrate.c -o heartrate
