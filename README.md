# Rigorous Two Phase Locking
In computer science, rigorous two-phase locking (Rigorous 2PL) is a locking method used in concurrent systems.

The two rules of Rigorous 2PL are:
1. If a transaction T wants to read/write an object, it must request a shared/exclusive lock on the object.
2. All locks (both exclusive and shared) held by transaction T are released when T commits (and not before).

Under rigorous 2PL, transactions can be serialized by the order in which they commit. Rigorous 2PL is like strict 2PL, with the additional constraint that all locks (shared and exclusive) must be held until a transaction commits. 

This implementation of Rigorous 2-PL ensures that our schedule is:
* Recoverable
* Cascadeless

Hence it gives us freedom from Cascading Abort which was still there in Basic 2-PL and moreover guarantee Strict Schedules but still Deadlocks are possible!

**Language**: C++
**Libraries**: bits/stdc++, threads, unistd, mutex, condition_variable

## Steps

1. Clone this repositry with `git clone https://github.com/the-utkarshjain/Rigorous-2-Phase-Locking.git`.
2. Inside the folder run `make` on the terminal.
3. Now run `./out`.