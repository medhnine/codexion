*This project has been created as part of the 42 curriculum by mohhnine.*

---

# Codexion

A concurrency simulation where coders compete for shared USB dongles to compile their code. Built with POSIX threads in C.

---

## Description

Imagine a co-working space where coders sit in a circle. In the middle sits a Quantum Compiler. Each coder needs **two USB dongles** plugged in at the same time to compile. The dongles are shared — each one sits between two neighboring coders.

The challenge: make sure every coder gets to compile regularly without burning out, without deadlocks, and without anyone starving.

Each coder follows this loop forever:
1. Pick up left and right dongle
2. Compile (hold both dongles)
3. Put both dongles back
4. Debug
5. Refactor
6. Repeat

If a coder goes too long without compiling, they **burn out** and the simulation stops.

---

## Instructions

### Compile

```bash
make
```

### Run

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All times are in milliseconds. Scheduler must be `fifo` or `edf`.

### Examples

```bash
# 5 coders, should complete without burnout
./codexion 5 800 100 100 100 3 1 fifo

# Same but use EDF scheduling
./codexion 5 800 100 100 100 3 1 edf

# Tight timing — will burn out
./codexion 5 200 100 100 100 5 1 fifo
```

### Clean

```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # full rebuild
```

---

## Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | How many coders (and dongles) in the simulation |
| `time_to_burnout` | Max ms a coder can go without starting to compile |
| `time_to_compile` | How long compiling takes (holds both dongles) |
| `time_to_debug` | How long debugging takes |
| `time_to_refactor` | How long refactoring takes |
| `number_of_compiles_required` | Simulation ends when every coder hits this count |
| `dongle_cooldown` | Ms a dongle must rest after being released |
| `scheduler` | `fifo` (arrival order) or `edf` (earliest deadline first) |

---

## Blocking Cases Handled

**Deadlock prevention**
Coders pick up dongles in opposite orders depending on their ID (odd picks left first, even picks right first). This breaks the circular wait condition — one of Coffman's four conditions for deadlock.

**Starvation prevention**
Two scheduling policies are available. FIFO serves requests in arrival order so no coder waits forever. EDF prioritizes the coder closest to burning out, which both prevents starvation and is optimal for keeping everyone alive.

**Dongle cooldown**
After a coder releases a dongle, it becomes unavailable for `dongle_cooldown` ms. Each dongle tracks its release timestamp and waiting coders check elapsed time before taking it.

**Burnout detection**
A dedicated monitor thread wakes every 1ms and checks each coder's `last_compile` timestamp. If the gap exceeds `time_to_burnout`, it immediately sets the stop flag, prints the burnout message, and wakes all sleeping threads so they exit cleanly. The burnout message appears within ~1ms of the actual deadline.

**Log serialization**
A single `pause_print` mutex protects all output. Crucially, the monitor locks `pause_print` before setting `simulation_running = 0` — so no coder can sneak a message in after the burnout line is printed.

---

## Thread Synchronization Mechanisms

**One thread per coder + one monitor thread**
Each coder runs `simulation()` in its own pthread. The monitor runs separately and is the only thread allowed to stop the simulation.

**`pause_dongle` mutex + polling**
Each dongle has its own mutex. A coder locks it, inserts itself into the dongle's priority queue (min-heap), then polls every 1ms waiting for three conditions to be true simultaneously: dongle is free, cooldown has passed, and the coder is at the front of the queue. This prevents any race on dongle state.

**`pause` mutex**
Protects shared simulation state: `simulation_running`, `last_compile`, `number_of_compilations`, and `done`. Any thread that reads or writes these fields must hold this mutex.

**`pause_print` mutex**
Protects stdout. All `fprintf` calls are wrapped in lock/unlock. The monitor also holds this mutex while setting `simulation_running = 0` to ensure the burnout message is always the last line printed.

**Lock ordering**
To prevent deadlock between mutexes, a strict order is always respected:
```
pause_dongle → pause_print → pause
```
No thread ever acquires a mutex earlier in this chain while holding one later in it.

**Priority queue (min-heap)**
Each dongle maintains a small heap of waiting coders. With FIFO, the key is the arrival timestamp. With EDF, the key is `last_compile + time_to_burnout`. The coder at index 0 (smallest key) gets the dongle next. Insert is O(log n), pop with sift-down is O(log n).

---

## Resources

- POSIX Threads documentation — `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`
- [The Little Book of Semaphores](https://greenteapress.com/wp/semaphores/) — Allen B. Downey, dining philosophers problem
- [POSIX.1-2017 standard](https://pubs.opengroup.org/onlinepubs/9699919799/) — threads and synchronization primitives
- Dijkstra's dining philosophers problem — original formulation of the resource contention problem

**How AI was used in this project**

AI (Claude) was used as a debugging and review tool throughout the project. Specifically:
- Identifying race conditions (messages printing after burnout, EDF causing burnout due to done-coder heap corruption)
- Explaining why certain mutex patterns were wrong (calling `pthread_cond_timedwait` with an unlocked mutex)
- Reviewing the monitor thread's lock/unlock balance
- Designing the test suite to catch edge cases

All code was written and understood by the author. AI suggestions were reviewed, tested, and validated before being applied.
