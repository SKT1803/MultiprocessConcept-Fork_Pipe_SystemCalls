# Multiprocess Communication in C – Fork & Pipe System Calls

This project demonstrates **inter-process communication (IPC)** between three processes in C using the `fork()` and `pipe()` system calls.

---


## Objective

- Create two child processes (`P2`, `P3`) from a **parent process (`P1`)**.
- Use **four pipes** to enable **bidirectional communication**:
  - P1 → P2
  - P1 → P3
  - P2 → P1
  - P3 → P1
- Each process has a specific task:
  - **P1 (Parent)** reads a file of random integers and distributes them to both children.
  - **P2** categorizes integers by the number of digits (1–5).
  - **P3** counts how many numbers are **prime** and **non-prime**.

---

## Structure Overview
MultiprocessConcept-Fork_Pipe_SystemCalls
<pre>
├── forkPipe.c # Main C program with fork() and pipe() logic
├── README.md # Project documentation (you’re here)
├── input files/ # Example input files for manual testing
│ ├── numbers.txt
│ ├── numbers2.txt
│ └── numbers3.txt
├── testIOs/
│ ├── correctoutputs.txt # Reference outputs
│ ├── numbers1.txt - numbers10.txt  
</pre>


---


All unused pipe ends are closed in each process to ensure proper communication and avoid blocking.


##  How to Compile & Run

### Compile
```bash
    gcc -o forkPipe forkPipe.c
```
Run
```bash
  ./forkPipe input\ files/numbers.txt
```

    
---

Write a C program, which demonstrates interprocess communicationIPC between three processes using pipes. Your program as the parent process should create two child
processes and utilize four pipes, establishing a multi-directional communication as shown in the figure below:

<div align="center">
<img src="https://github.com/user-attachments/assets/045a50cf-1407-488a-a68f-848bf781f231" width=400px/><br>
</div>

Note that unnecessary pipe ends must be closed to prevent improper pipe usage. Each pipe needs one write
one read end to remain open for two way communication.
The parent process should read a filename as a command line argument (so that different files can be used on
each execution). Each file should have an unspecified number of random integers. The parent process P1
should read integers and send to BOTH child processes one by one. 
<br>
<br>
<ul>
    <li>P2 receives the integers one by one, categorizes each integer according to number of digits. For
        readability, implement a function named “int nrDigits(int)” to do this. You may assume integer
        numbers in the test files be between 0 and 99999. Count the number of integers received according
        to the number of digits.
    </li>
    <li>P3 receives the integers one by one, computes the number of prime integers received. Implement a
        function named “int isPrime(int)” to do this.
    </li>
</ul>

When P2 and P3 have finished their jobs, they must submit back the results they have computed back
to the parent process and terminate.
<br>
<br>
Parent process P1, after receiving results from P2 and P3 should print a nicely formatted output as
given in the following example and should terminate. 

<b>Notes</b>
<ul>
    <li>You may (modify and) use the following shell script to generate input files with random numbers:<br><br>
        for i in {1..1000}; do echo $((RANDOM)) >> numbers.txt; done
    </li>
    <li>Both child processes receive unknown number of (say N) integers from the pipe in stage one (one
        integer per read operation), but write only once during stage two (all results in one write operation). 
    </li>
</ul>

<b>Sample Output for 3 input files</b>
<pre>
  $ ./numbers.txt                  $ ./numbers2.txt                   $ ./ numbers3.txt
  Input file: numbers.txt          Input file: numbers2.txt           Input file: numbers3.txt
  1 digits - 0                     1 digits - 6                       1 digits - 0
  2 digits - 4                     2 digits – 62                      2 digits - 0
  3 digits - 21                    3 digits – 529                     3 digits - 10
  4 digits - 283                   4 digits – 4875                    4 digits – 58
  5 digits - 692                   5 digits - 13026                   5 digits - 182
  Primes - 108                     Primes - 1986                      Primes - 34
  Nonprimes - 892                  Nonprimes – 16514                  Nonprimes – 216
</pre>
